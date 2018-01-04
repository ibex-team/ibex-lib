import os, sys, re, math, shutil, collections, logging, math
import ibexutils
from waflib import TaskGen, Task, Utils, Configure, Build, Logs
benchlock = Utils.threading.Lock()

BENCHS_DEFAULT_ARGS = {"time_limit": "5", "prec_ndigits_max": "6",
                       "prec_ndigits_min": "1", "iter": "3"}
BENCHS_ARGS_NAME = BENCHS_DEFAULT_ARGS.keys()
BENCHS_ARGS_PATTERN = " ; ".join("%s = (?P<%s>.+?)" % (k, k) for k in BENCHS_ARGS_NAME)
BENCHS_ARGS_FORMAT = " ; ".join("%s = {BCH_%s}" % (k,k.upper()) for k in BENCHS_ARGS_NAME)

BENCHS_MAX_REGRESSION = 10

# Base class for all bench classes
class Bench (Task.Task):
	color = 'CYAN'
	vars = []
	always_run = True

# Class for the task that run the benchmark
class BenchRun (Bench):
	run_str = "${BCH_PRECMD} ${SRC[0]} %s --bench-file ${SRC[1]} > ${TGT[0]} 2>&1" % (" ".join("--%s ${BCH_%s}" % (k.replace("_", "-"), k.upper()) for k in BENCHS_ARGS_NAME))

	def __str__ (self):
		return self.inputs[1].path_from (self.inputs[1].ctx.launch_node())

	def keyword (self):
		return "Benchmarking"

# Class for the task that parses the output of the benchmark and produces a
# .data file
class BenchData (Bench):
	KEYS_TYPE = collections.OrderedDict ()
	KEYS_TYPE["eps"] = float
	KEYS_TYPE["status"] = int
	KEYS_TYPE["time"] = float
	KEYS_TYPE["nb_cells"] = int
	KEYS_TYPE["uplo"] = float
	KEYS_TYPE["loup"] = float
	KEYS_TYPE["random_seed"] = float
	PREFIX = "BENCH: "
	RESULTS_PATTERN = "(%s) = (.*)" % "|".join(KEYS_TYPE.keys())
	RESULTS_RE = re.compile (RESULTS_PATTERN)
	RESULTS_FORMAT = PREFIX + " ; ".join("%s = {%s}" % (k,k) for k in KEYS_TYPE.keys())

	@classmethod
	def parse_bench_line (cls, line):
		if line.startswith (cls.PREFIX):
			D = {}
			line = line[len(cls.PREFIX):]
			for part in line.split (" ; "):
				m = cls.RESULTS_RE.match (part)
				if m:
					k = str(m.group(1))
					D[k] = cls.KEYS_TYPE[k](m.group(2))
			return D
		else:
			return None

	def run (self):
		# Get the data and write the data file from the results_file
		data = []
		for l in ibexutils.to_unicode(self.inputs[0].read()).splitlines():
			D = self.parse_bench_line (l)
			if not D is None:
				data.append (D)
		data.sort (key=lambda x:-x["eps"])

		keys = self.KEYS_TYPE.keys()
		datastr = " ".join(keys) + os.linesep
		datalines = (" ".join ("%s" % d.get (k, "NaN") for k in keys) for d in data)
		datastr += os.linesep.join(datalines)
		self.outputs[0].write (datastr + os.linesep)

		res = (self.inputs[0].change_ext(''), data)
		benchlock.acquire()
		try:
			bld = self.generator.bld
			k1 = self.generator.name
			k2 = self.inputs[0].change_ext('').relpath()

			if not hasattr (bld, "bench_results"):
				bld.bench_results = {}
			if not k1 in bld.bench_results:
				bld.bench_results[k1] = {}
			bld.bench_results[k1][k2] = data
		finally:
			benchlock.release()

	def keyword (self):
		return "Parsing results from"

# Class for the task that generates the graph from the .data file
class BenchGraph (Bench):
	run_str = "${BCH_PRECMD} ${GNUPLOT} -e ${tsk.eargs()} ${BCH_GRAPHFILE}"

	def eargs (self):
		L = [
		  "datafile='%s'" % self.inputs[0],
			"outputfile='%s'" % self.outputs[0],
			"title='%s'" % self.outputs[0].change_ext('').relpath().replace('_', '\_')
		]
		L.extend("%s='%s'"%(k,self.env["BCH_"+k.upper()]) for k in BENCHS_ARGS_NAME)
		return ";".join (L)

	def keyword (self):
		return "Generating graph from"

class BenchSummary (BenchData):
	def run (self):
		if not hasattr (self.generator.bld, "bench_results"):
			self.err_msg = "In" + repr (self) + "\nself.generator.bld does not have a"
			self.err_msg += " bench_results attribute, no results to write"
			return 1
		if not self.generator.name in self.generator.bld.bench_results:
			self.err_msg = "In" + repr (self) + "\nself.generator.bld.bench_results "
			self.err_msg += "does not have a " + self.generator.name + " key, "
			self.err_msg += "no results to write"
			return 1

		results = self.generator.bld.bench_results[self.generator.name]

		strargs = BENCHS_ARGS_FORMAT.format (**self.env)
		lst = [ "##### Serie: %s [ %s ]" % (self.generator.name, strargs) ]
		for k, d in results.items():
			lst.append("### File: %s" % k)
			for m in d:
				lst.append (self.RESULTS_FORMAT.format (**m))
		outstr = os.linesep.join(lst) + os.linesep

		self.outputs[0].write (outstr)
		if self.generator.bld.savefile:
			with open (self.generator.bld.savefile, "a") as f:
				f.write (outstr)

	def keyword (self):
		return "Writing summary of '%s' into" % self.generator.name

# Class for the task that generates the scatter plot for comparison
class BenchScatterPlotData (Bench):
	def get_time (self, L):
		T = [ l for l in L if abs(l["eps"]-self.eps) <= 1e-6 ]
		if len(T) == 0:
			return self.env.BCH_TIME_LIMIT
		else:
			return max(t["time"] for t in T)

	def run (self):
		serie = self.generator.name
		outstr = "benchfile current previous" + os.linesep
		for f, fdata in self.generator.bld.bench_results[serie].items():
			if f in self.data:
				tc = self.get_time (fdata) # timing of current bench
				tp = self.get_time (self.data[f]) # timing from previous bench
				outstr += "%s %s %s" % (f, tc, tp) + os.linesep

				if tc > tp * BENCHS_MAX_REGRESSION: # check for non regression
					err_fmt = "bench %s from %s is too long (%f > %d * %f)"
					err_data = (f, serie, tc, BENCHS_MAX_REGRESSION, tp)
					if not hasattr (self.generator.bld, "bench_errors"):
						self.generator.bld.bench_errors = []
					self.generator.bld.bench_errors.append (err_fmt % err_data)

		# Write data in output file
		self.outputs[0].write (outstr)

		# check intersection of [uplo, loup]
		for f, fdata in self.generator.bld.bench_results[serie].items():
			uplo = float("-inf")
			loup = float("+inf")
			for d in fdata:
				uplo = max (uplo, d["uplo"])
				loup = min (loup, d["loup"])
			print (uplo, loup)
			if uplo > loup:
				err_fmt = "bench %s from %s: intersection of [uplo, loup] is empty"
				err_data = (f, serie)
				if not hasattr (self.generator.bld, "bench_errors"):
					self.generator.bld.bench_errors = []
				self.generator.bld.bench_errors.append (err_fmt % err_data)

			if f in self.data:
				for d in self.data[f]:
					if uplo > d["loup"] or d["uplo"] > loup:
						err_fmt = "bench %s from %s: [uplo, loup] does not intersect with results from '%s'"
						err_data = (f, serie, self.cmp_ref)
						if not hasattr (self.generator.bld, "bench_errors"):
							self.generator.bld.bench_errors = []
						self.generator.bld.bench_errors.append (err_fmt % err_data)

	def keyword (self):
		return "Writing scatter plot data into"

# Class for the task that generates the scatter plot for comparison
class BenchScatterPlotGraph (BenchGraph):
	run_str = "${BCH_PRECMD} ${GNUPLOT} -e ${tsk.eargs()} ${BCH_SCATTERPLOT_GRAPHFILE}"

	def eargs (self):
		L = [
		  "datafile='%s'" % self.inputs[0],
			"outputfile='%s'" % self.outputs[0],
			"cmp_ref='%s'" % self.cmp_ref
		]
		L.extend("%s='%s'"%(k,self.env["BCH_"+k.upper()]) for k in BENCHS_ARGS_NAME)
		return ";".join (L)

@Configure.conf
def parse_summary_file (bch, filename):
	# deactivate logger for this function
	bch.logger = logging.getLogger ("devnull")
	bch.logger.addHandler (logging.NullHandler)

	bch.start_msg ("Parsing results from '%s' for comparison" % filename)

	data = {}
	seriematch = re.compile("^##### Serie: (.+?) \[ %s \]$" % BENCHS_ARGS_PATTERN)
	filematch = re.compile("^### File: (.+)$")
	try:
		with open (filename, "r") as f:
			for l in ibexutils.to_unicode(f.read()).splitlines():
				ms = seriematch.match (l)
				mf = filematch.match (l)
				if ms:
					curserie = str(ms.group(1))
					data[curserie] = {"args": {}, "data": {}}
					for k, v in ms.groupdict().items():
						data[curserie]["args"][str(k)] = str(v)
				elif mf:
					curfile = str(mf.group(1))
					data[curserie]["data"][curfile] = []
				else:
					data[curserie]["data"][curfile].append (BenchData.parse_bench_line(l))
	except UnboundLocalError:
		bch.end_msg ("error, the file is not correctly formatted", color="RED")
		return 1
	bch.cmp_to_data[filename] = data
	bch.end_msg ("done")
	bch.logger = None
	return 0

# Alias for creation benchmarks by looking at the file extensions
@Configure.conf
def benchmarks (bld, *k, **kw):
	kw["features"] = "benchmarks"
	return bld(*k, **kw)

@TaskGen.feature("benchmarks")
@TaskGen.before_method ("process_rule")
def benchmarks_process_options (self):
	#
	if hasattr (self, "graph_scriptfile"):
		self.env.BCH_GRAPHFILE = self.graph_scriptfile

	# Get attribute: time_limit, min_prec, max_prec, ...
	# First try options, then task attributes, else use default values
	for k in BENCHS_ARGS_NAME:
		v = getattr (self.bld.options, "BENCHS_" + k.upper(), None) # cmdline
		if v is None:
			v = getattr (self, k, BENCHS_DEFAULT_ARGS[k]) # task or default
		setattr (self.env, "BCH_" + k.upper(), v)

	# Get the name of the binary used for benchmarking: this is given by the
	# 'bench_bin' attribute.
	binname = None
	bins = self.to_list (getattr (self, 'bench_bin', []))
	if len (bins) == 1:
		binname = bins[0]
	else:
		self.bld.fatal ("Need a unique 'bench_bin' attribute to build a bench task")
	try:
		self.bintask = self.bld.get_tgen_by_name (binname).link_task
	except Errors.WafError:
		self.bld.fatal ("Could not find task '%s' to build bench task" % binname)

@TaskGen.feature("benchmarks")
@TaskGen.after_method ("process_source")
def benchmarks_gather_data (self):
	filenameformat = "benchmarks.%s.%s.%s"

	# Create output node
	logfiledata = (self.name, "summary", "log")
	lognode = self.bld.bldnode.make_node (filenameformat % logfiledata)

	# Create summary task
	prev_tasks = self.tasks[:]
	tsk = self.create_task ('BenchSummary', [], lognode)
	for t in prev_tasks:
		tsk.set_run_after (t)
	
	for cmp_ref, Data in self.bld.cmp_to_data.items():
		if self.name in Data:
			rn = cmp_ref.replace ("/", "_")
			args = Data[self.name]["args"]
			data = Data[self.name]["data"]
			if all (v == self.env["BCH_" + k.upper()] for k, v in args.items()):
				dataname = filenameformat % (self.name, "scatter_plot_%s" % rn, "data")
				graphname = filenameformat % (self.name, "scatter_plot_%s" % rn, "pdf")
				datanode = self.bld.bldnode.make_node (dataname)
				graphnode = self.bld.bldnode.make_node (graphname)

				dm = float(self.env["BCH_PREC_NDIGITS_MIN"])
				dM = float(self.env["BCH_PREC_NDIGITS_MAX"])
				eps = math.pow (10.0, -math.floor((dm+dM)/2.0))
				kw = {"data": data, "eps": eps, "cmp_ref": cmp_ref}
				tsk = self.create_task ('BenchScatterPlotData', [], datanode, **kw)
				for t in self.tasks:
					if type(t) is BenchData:
						tsk.set_run_after (t)

				if self.bld.with_graphs:
					self.create_task ('BenchScatterPlotGraph', datanode, graphnode, **kw)
			else:
				info = "Could not compare current serie '%s' " % self.name
				info += "to data from %s, some arguments differ:" % cmp_ref
				for k, v in args.items():
					vf = self.env["BCH_" + k.upper()]
					if v != vf:
						info += "\n  - for '%s' got '%s', expected '%s'" % (k, vf ,v)
				Logs.info (info)
		else:
			Logs.info ("No data for serie '%s' in file '%s'" % (self.name, cmp_ref))

# This function create the task (from the class bench) that handle one .bch file
# The decorator TaskGen.extension(".bch") is here so that this function is
# called every time a .bch file is seen in a task generator.
@TaskGen.extension(".bch")
def add_bch (self, node):
	if not "benchmarks" in self.features:
		self.bld.fatal ("The feature 'benchmarks' is needed to process .bch files")

	# Create the task that run the bench
	resnode = node.change_ext ('.bench_result', '.bch')
	self.create_task ('BenchRun', [self.bintask.outputs[0], node], resnode)

	# Create the task that parse the result
	datanode = node.change_ext ('.data', '.bch')
	self.create_task ('BenchData', resnode, datanode)

	# Set output nodes
	if self.bld.with_graphs:
		fignode = node.change_ext ('.pdf', '.bch')
		self.create_task ('BenchGraph', datanode, fignode)

# Format the output of benchmarks, using the list bench_results
# Assume that for each result, data are already sorted
def benchmarks_format_output (bch):
	from waflib import Logs
	logfile = os.path.join (bch.bldnode.abspath(), "benchmarks.log")
	bch.logger = Logs.make_logger (logfile, "benchmarks")

	def compute_score (data):
		S = 0.0
		prev_elt = None
		for d in data:
			eps = d["eps"]
			time = d["time"]
			if not prev_elt is None:
				prev_eps, prev_time = prev_elt
				log10_eps = -math.log10(eps)
				prev_log10_eps = -math.log10(prev_eps)
				S += (log10_eps-prev_log10_eps)*(time+prev_time)/2
			prev_elt = (eps, time)
		return S

	#if bch.options.BENCHS_CMP_TO:
	#	cmpdata = parse_results_file (bch.options.BENCHS_CMP_TO)

	cmpdata = {}

	D = getattr (bch, 'bench_results', {})
	for benchs_name, benchs_data_dict in sorted(D.items(), key = lambda x:x[0]):
		bch.msg ("===== %s =====" % benchs_name, "==========", color = "NORMAL")
		for f, data in sorted(benchs_data_dict.items(), key = lambda x:x[0]):
			bch.start_msg (f)
			n = len (data)
			S = compute_score (data)
			msg = "%d measure%s, Score = %g" % (n, "s" if n > 1 else "", S)
			color = "CYAN"
			cmpS, _ = cmpdata.get (f, (None, None))
			if cmpS:
				percent = 100.*(S/cmpS-1.)
				msg += " (%0.2f%%)" % percent
				if percent <= 0:
					color = "GREEN"
				elif percent >= 2.5:
					color = "RED"
				else:
					color = "YELLOW"
			bch.end_msg (msg, color = color)

	if hasattr (bch, "bench_errors"):
		sep = os.linesep + "  - "
		bch.fatal (sep.join (["Benchmarks errors:"] + bch.bench_errors))

######################
###### options #######
######################
def options (opt):
	"""
	Options for the benchmarks
	"""

	categories = [ "easy", "medium", "hard", "blowup", "others", "unsolved" ]
	cat_default = "medium"
	cat_help = "Possible values: " + ", ".join(categories)
	cat_help += " [ default: %s ]" % cat_default

	def parse_cat_callback (option, opt_str, value, parser, *args, **kwargs):
		choices = categories,
		L = value.replace ("+", ",").split(",")
		for cat in L:
			if not cat in categories:
				import optparse
				fmt = "option %s: invalid choice: '%s' (choose from %s)"
				h = ", ".join ("'%s'" % c for c in categories)
				raise optparse.OptionValueError(fmt % (option, cat, h))
		setattr(parser.values, option.dest, L)


	grp = opt.add_option_group ("Options for benchmarks")
	for n, v in BENCHS_DEFAULT_ARGS.items():
		optname = "--benchs-" + n.replace("_", "-")
		grp.add_option (optname, action="store", dest = "BENCHS_" + n.upper(),
		                help = "Override default %s (default is %s)" % (n, v))
	grp.add_option ("--benchs-categories", help = cat_help, action = "callback",
	                callback = parse_cat_callback, type = str,
	                default = [cat_default], dest = "BENCHS_CATEGORIES")
	grp.add_option ("--benchs-save", action = "store", dest = "BENCHS_SAVE",
	                help = "Save the results of the benchmarks in the given file")
	grp.add_option ("--benchs-cmp-to", action = "append", dest = "BENCHS_CMP_TO",
	                help = "Compare to previously saved benchmarks")
	grp.add_option ("--benchs-with-graphs", action = "store_true",
	                help = "Generate graphics from benchs (when available)",
	                dest = "BENCHS_WITH_GRAPHS")
	grp.add_option ("--benchs-precmd", action = "store", dest = 'BENCHS_PRECMD',
	                help = "Prefix the benchmarks command with this string")

######################
##### configure ######
######################
def configure (conf):
	conf.find_program ("gnuplot", var = "GNUPLOT", mandatory = False)

######################
##### benchmarks #####
######################
def benchmarks (bch):
	# Add a group so that task are run after the tasks from build
	bch.add_group()
	bch.post_mode = Build.POST_AT_ONCE # all groups are posted simultaneously

	# Add a function to be executed once all the tasks run
	bch.add_post_fun (benchmarks_format_output)

	# For benchmarks, set the default number of parallel tasks is 1
	p = [ "-j", "--jobs", "-j%d" % bch.jobs, "--jobs=%d" % bch.jobs ]
	if all ([ not a in sys.argv for a in p]): # jobs is not explicitly set
		bch.jobs = bch.options.jobs = 1

	# Read list of categories from command line arguments
	if bch.options.BENCHS_CATEGORIES:
		bch.categories = bch.options.BENCHS_CATEGORIES

	# Do not overwrite file with --benchs-save option
	if bch.options.BENCHS_SAVE:
		f = bch.options.BENCHS_SAVE
		if os.path.exists (f):
			bch.fatal ("Benchmarks: '%s' already exists, will not overwrite it." % f)
		bch.savefile = f
	else:
		bch.savefile = None

	# Check that option --benchs-cmp-to (if given) is a list of existing files
	bch.cmp_to_data = {}
	bch.env.BCH_SCATTERPLOT_GRAPHFILE = "../benchs/scatter_plot.gnuplot"
	if bch.options.BENCHS_CMP_TO:
		for f in bch.options.BENCHS_CMP_TO:
			if not os.path.isfile (f):
				bch.fatal ("Benchmarks: cannot compare to %s: not a file." % f)
			else:
				ret = bch.parse_summary_file (f)
				if ret != 0:
					bch.fatal ("Error while parsing %s" % f)

	# Handle --benchs-precmd option
	if bch.options.BENCHS_PRECMD:
		bch.env.BCH_PRECMD = bch.options.BENCHS_PRECMD

	# Handle --benchs-with-graphs option
	if bch.options.BENCHS_WITH_GRAPHS:
		bch.with_graphs = bch.options.BENCHS_WITH_GRAPHS
	else:
		bch.with_graphs = False

	# We need GNUPLOT to generate graphs
	if bch.with_graphs and not bch.env.GNUPLOT:
		bch.fatal ("gnuplot is required for the option '--benchs-with-graphs'")
