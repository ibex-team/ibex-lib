import os, sys, re, math, shutil, collections, logging, math
import ibexutils
from waflib import TaskGen, Task, Utils, Configure, Build, Logs, Errors
benchlock = Utils.threading.Lock()

BENCHS_DEFAULT_ARGS = {"time_limit": "5", "prec_ndigits_max": "6",
                       "prec_ndigits_min": "1", "iter": "3"}
BENCHS_ARGS_NAME = BENCHS_DEFAULT_ARGS.keys()
BENCHS_ARGS_PATTERN = " ; ".join("%s = (?P<%s>.+?)" % (k, k) for k in BENCHS_ARGS_NAME)
BENCHS_ARGS_FORMAT = " ; ".join("%s = {%s}" % (k, k) for k in BENCHS_ARGS_NAME)

BENCHS_INSTABLE_FACTOR = 1.2
BENCHS_CMP_REGRESSION_FACTOR = 2.
BENCHS_CMP_IMPROVMENT_FACTOR = 2.

# return min, max, average and stantard deviation
def set_stats (S):
	if S:
		m = min (S)
		M = max (S)
		av = sum (S)/len(S)
		std = (sum((v-av)**2 for v in S)/len(S))**0.5
		return m, M, av, std
	else:
		nan = float ('nan')
		return nan, nan, nan, nan

class BenchRef (object):
	def __init__ (self, string, hash_salt):
		self.string = string
		self.hash_salt = hash_salt

	def __str__ (self):
		return self.string

	def slugify (self):
		f = lambda s: "".join (x if (x.isalnum() or x in "._-") else "_" for x in s)
		if self.hash_salt:
			return "%s_%s" % (f (self.hash_salt), f (self.string))
		else:
			return f (self.string)

	def __eq__ (self, other):
		return self.string == other.string and self.hash_salt == other.hash_salt

	def __hash__ (self):
		return hash ((self.string, self.hash_salt))

class BenchFileRef (BenchRef):
	def __init__ (self, filename):
		super(BenchFileRef, self).__init__ (filename, "file")

class BenchCurrentRef (BenchRef):
	__instance = None
	def __new__ (cls):
		if BenchCurrentRef.__instance is None:
			BenchCurrentRef.__instance = BenchRef.__new__ (cls)
		return BenchCurrentRef.__instance

	def __init__ (self):
		super(BenchCurrentRef, self).__init__ ("current benchmarks", None)

#
class Eps (object):
	NDECIMAL = 1

	@staticmethod
	def exp_to_prec (d):
		return - int (round (10**(Eps.NDECIMAL) * d))

	def __init__ (self, value):
		if value.startswith ("10^"):
			self._p = self.exp_to_prec (float (value[3:]))
		else:
			self._p = self.exp_to_prec (math.log10(float(value)))

	def to_double (self):
		return math.pow (10.0, - float (self._p) / (10**Eps.NDECIMAL))

	def __eq__ (self, other):
		return self._p == other._p

	def __lt__ (self, other):
		return self._p > other._p

	def __hash__ (self):
		return hash (self._p)

	def __str__ (self):
		return "%e" % self.to_double()

	def __repr__ (self):
		b = 10**Eps.NDECIMAL
		return "10^-%u.%u" % (self._p / b, self._p % b)

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
	KEYS_TYPE["eps"] = Eps
	KEYS_TYPE["status"] = int
	KEYS_TYPE["time"] = float
	KEYS_TYPE["nb_cells"] = int
	KEYS_TYPE["uplo"] = float
	KEYS_TYPE["loup"] = float
	KEYS_TYPE["random_seed"] = float
	PREFIX = "BENCH: "
	RESULTS_PATTERN = "(%s) = (.*)" % "|".join(KEYS_TYPE.keys())
	RESULTS_RE = re.compile (RESULTS_PATTERN)
	RESULTS_FORMAT = PREFIX + " ; ".join("%s = {%s!r}" % (k,k) for k in KEYS_TYPE.keys())

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
		data.sort (key=lambda x:x["eps"], reverse=True)

		keys = self.KEYS_TYPE.keys()
		datastr = " ".join(keys) + os.linesep
		datalines = (" ".join ("%s" % d.get (k, "NaN") for k in keys) for d in data)
		datastr += os.linesep.join(datalines)
		self.outputs[0].write (datastr + os.linesep)

		benchlock.acquire()
		try:
			# Add the result of the current bench file
			cur_bench_results = self.generator.bld.bench_results[BenchCurrentRef()]
			k = self.inputs[0].change_ext('').relpath()
			cur_bench_results[self.generator.name]["data"][k] = data
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
		groupname = self.generator.name
		try:
			cur_bench_res = self.generator.bld.bench_results[BenchCurrentRef()]
		except KeyError:
			self.err_msg = "In" + repr (self) + "\nNo '%s' " % BenchCurrentRef()
			self.err_msg += "key in bench_results dictionary, no results to write"
			return 1

		try:
			results = cur_bench_res[groupname]
		except KeyError:
			self.err_msg = "In" + repr (self) + "\nNo '" + groupname + "' key in "
			self.err_msg += "bench_results[\"%s\"] dictionary, " % BenchCurrentRef()
			self.err_msg += "no results to write"
			return 1

		strargs = BENCHS_ARGS_FORMAT.format (**results["args"])
		lst = [ "##### Group: %s [ %s ]" % (groupname, strargs) ]
		for k, d in results["data"].items():
			lst.append("### File: %s" % k)
			for m in d:
				lst.append (self.RESULTS_FORMAT.format (**m))
			# check [uplo, loup] interval
			uplo = max (d["uplo"] for d in d)
			loup = min (d["loup"] for d in d)
			if uplo > loup:
				err_fmt = "empty [uplo, loup] interval for %s:" + os.linesep
				err_fmt += "    * group '%s'" + os.linesep
				err_fmt += "    * bench '%s'" + os.linesep
				err_fmt += "    * [uplo, loup] = [%s, %s]" + os.linesep
				err_data = (BenchCurrentRef(), groupname, k, uplo, loup)
				self.generator.bld.bench_errors.append (err_fmt % err_data)

		outstr = os.linesep.join(lst) + os.linesep

		self.outputs[0].write (outstr)
		if self.generator.bld.savefile:
			with open (self.generator.bld.savefile, "a") as f:
				f.write (outstr)

	def keyword (self):
		return "Writing summary of '%s' into" % self.generator.name

# Class for the task that does the comparison between benchmarks
class BenchCmp (Bench):
	KEYS = [ "eps", "std1/std0", "#cells1/#cells0" ]
	CMP_FORMAT = "CMP: " + " ; ".join("%s = {%s!r}" % (k,k) for k in KEYS)

	def run (self):
		if not (self.k0, self.k1) in self.generator.bld.bench_cmp:
			self.generator.bld.bench_cmp[(self.k0, self.k1)] = {}
		bench_cmp = self.generator.bld.bench_cmp[(self.k0, self.k1)]

		groupname = self.generator.name
		if not groupname in bench_cmp:
			bench_cmp[groupname] = {}

		outstr = "####### data0 from %s" % self.k0 + os.linesep
		outstr += "####### data1 from %s" % self.k1 + os.linesep
		outstr += "##### Group: %s" % self.generator.name + os.linesep
		for f in set(self.data0.keys()) & set(self.data1.keys()):
			data = []
			outstr += "### File: %s" % f + os.linesep
			fdata0 = self.data0[f]
			fdata1 = self.data1[f]
			eps0 = set(d["eps"] for d in fdata0)
			eps1 = set(d["eps"] for d in fdata1)
			for eps in reversed(sorted(eps0 | eps1)):
				time_data0 = set (d["time"] for d in fdata0 if d["eps"] == eps)
				time_data1 = set (d["time"] for d in fdata1 if d["eps"] == eps)
				_, _, av0, std0 = set_stats (time_data0)
				_, _, av1, std1 = set_stats (time_data1)
				cells_data0 = set (d["nb_cells"] for d in fdata0 if d["eps"] == eps)
				cells_data1 = set (d["nb_cells"] for d in fdata1 if d["eps"] == eps)
				_, _, avcells0, _ = set_stats ({float(v) for v in cells_data0})
				_, _, avcells1, _ = set_stats ({float(v) for v in cells_data1})
				data.append ( { "eps": eps, "std1/std0": std1/std0,
																		"(av1-av0)/std0": (av1-av0)/std0,
																		"#cells1/#cells0": avcells1/avcells0 } )

			outstr += os.linesep.join (self.CMP_FORMAT.format (**d) for d in data)
			outstr += os.linesep
			bench_cmp[groupname][f] = data

			# check intersection of [uplo, loup]
			uplo0 = max (d["uplo"] for d in fdata0)
			loup0 = min (d["loup"] for d in fdata0)
			uplo1 = max (d["uplo"] for d in fdata1)
			loup1 = min (d["loup"] for d in fdata1)
			if uplo1 > loup0 or uplo0 > loup1:
				err_fmt = "[uplo, loup] intervals do not intersect:" + os.linesep
				err_fmt += "    * group '%s'" + os.linesep
				err_fmt += "    * bench '%s'" + os.linesep
				err_fmt += "    * [uplo, loup] = [%s, %s] in %s" + os.linesep
				err_fmt += "    * [uplo, loup] = [%s, %s] in %s" + os.linesep
				err_data = (f, groupname, uplo0, loup0, self.k0, uplo1, loup1, self.k1)
				self.generator.bld.bench_errors.append (err_fmt % err_data)

		# Write data in output file
		self.outputs[0].write (outstr)

	def keyword (self):
		return "Writing comparison data into"

# Class for the task that generates the scatter plot for comparison
class BenchScatterPlotData (Bench):
	def get_time (self, L):
		T = [ l["time"] for l in L if l["eps"] == self.eps ]
		return max(T) if T else self.env.BCH_TIME_LIMIT

	def run (self):
		groupname = self.generator.name
		outstr = "benchfile %s %s" % (self.k0, self.k1) + os.linesep
		for f in set(self.data0.keys()) & set(self.data1.keys()):
			fdata0 = self.data0[f]
			fdata1 = self.data1[f]
			t0 = self.get_time (fdata0)
			t1 = self.get_time (fdata1)
			outstr += "%s %s %s" % (f, t0, t1) + os.linesep
		self.outputs[0].write (outstr)

	def keyword (self):
		return "Writing scatter plot data into"

# Class for the task that generates the scatter plot for comparison
class BenchScatterPlotGraph (BenchGraph):
	run_str = "${BCH_PRECMD} ${GNUPLOT} -e ${tsk.eargs()} ${BCH_SCATTERPLOT_GRAPHFILE}"

	def eargs (self):
		L = [
		  "datafile='%s'" % self.inputs[0],
			"outputfile='%s'" % self.outputs[0],
			"ref0='%s'" % self.k0,
			"ref1='%s'" % self.k1,
		]
		L.extend("%s='%s'"%(k,self.env["BCH_"+k.upper()]) for k in BENCHS_ARGS_NAME)
		return (";".join (L)).replace (" ", "\_") # spaces break the command line

@Configure.conf
def parse_summary_file (bch, filename):
	# deactivate logger for this function
	bch.logger = logging.getLogger ("devnull")
	bch.logger.addHandler (logging.NullHandler)

	bch.start_msg ("Parsing results from '%s' for comparison" % filename)

	data = {}
	groupmatch = re.compile("^##### Group: (.+?) \[ %s \]$" % BENCHS_ARGS_PATTERN)
	filematch = re.compile("^### File: (.+)$")
	try:
		with open (filename, "r") as f:
			for l in ibexutils.to_unicode(f.read()).splitlines():
				ms = groupmatch.match (l)
				mf = filematch.match (l)
				if ms:
					curgroup = str(ms.group(1))
					data[curgroup] = {"args": {}, "data": {}}
					for k, v in ms.groupdict().items():
						data[curgroup]["args"][str(k)] = str(v)
				elif mf:
					curfile = str(mf.group(1))
					data[curgroup]["data"][curfile] = []
				else:
					D = BenchData.parse_bench_line(l)
					if not D is None:
						data[curgroup]["data"][curfile].append (D)
	except UnboundLocalError:
		bch.end_msg ("error, the file is not correctly formatted", color="RED")
		return 1
	bch.bench_results[BenchFileRef(filename)] = data
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
	args = {}
	for k in BENCHS_ARGS_NAME:
		v = getattr (self.bld.options, "BENCHS_" + k.upper(), None) # cmdline
		if v is None:
			v = getattr (self, k, BENCHS_DEFAULT_ARGS[k]) # task or default
		setattr (self.env, "BCH_" + k.upper(), v)
		args[k] = v

	if not self.bld.cmp_only:
		# First group of benchmarks => create BenchCurrentRef entry in the dict
		if not BenchCurrentRef() in self.bld.bench_results:
			self.bld.bench_results[BenchCurrentRef()] = {}

		# Create the dict for the current group
		group_dict = { "args": args, "data": {} }
		self.bld.bench_results[BenchCurrentRef()][self.name] = group_dict

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

	# Summary
	if not self.bld.cmp_only:
		# Create output node
		logfiledata = (self.name, "summary", "log")
		lognode = self.bld.bldnode.make_node (filenameformat % logfiledata)

		# Create summary task
		prev_tasks = self.tasks[:]
		tsk = self.create_task ('BenchSummary', [], lognode)
		for t in prev_tasks:
			tsk.set_run_after (t)

	# Comparison
	cmp_key_set = set()
	for cmp_key, cmp_data in self.bld.bench_results.items():
		if self.name in cmp_data:
			cmp_key_set.add (cmp_key)

	if cmp_key_set:
		for cmp_key in set(self.bld.bench_results.keys()) - cmp_key_set:
			Logs.info ("No data for group '%s' in file '%s'" % (self.name, cmp_key))

	for k0, k1 in ((k0,k1) for k0 in cmp_key_set for k1 in cmp_key_set if k0<k1):
		if k0 == BenchCurrentRef():
			k0, k1 = k1, k0 # swap in order to have BenchCurrentRef as k1 if exists
		args0 = self.bld.bench_results[k0][self.name]["args"]
		args1 = self.bld.bench_results[k1][self.name]["args"]
		if all (args0[k] == args1[k] for k in BENCHS_ARGS_NAME):
			vs = "%s_VS_%s" % (k0.slugify(), k1.slugify())
			cmpname = filenameformat % (self.name, "cmp.%s" % vs, "summary.log")
			spdataname = filenameformat % (self.name, "scatter_plot.%s" % vs, "data")
			graphname = filenameformat % (self.name, "scatter_plot.%s" % vs, "pdf")
			cmpnode = self.bld.bldnode.make_node (cmpname)
			spdatanode = self.bld.bldnode.make_node (spdataname)
			graphnode = self.bld.bldnode.make_node (graphname)

			data0 = self.bld.bench_results[k0][self.name]["data"]
			data1 = self.bld.bench_results[k1][self.name]["data"]

			kw = { "k0": k0, "data0": data0, "k1": k1, "data1": data1 }
			tsk = self.create_task ('BenchCmp', [], cmpnode, **kw)

			dm = float(args0["prec_ndigits_min"]) # same value in args1
			dM = float(args0["prec_ndigits_max"]) # same value in args1
			kw["eps"] = Eps ("10^-%u" % math.floor((dm+dM)/2.0))
			tsk2 = self.create_task ('BenchScatterPlotData', [], spdatanode, **kw)

			if self.bld.with_graphs:
				self.create_task ('BenchScatterPlotGraph', spdatanode, graphnode, **kw)

			for t in self.tasks:
				if type(t) is BenchData:
					tsk.set_run_after (t)
					tsk2.set_run_after (t)
		else:
			info = "For group '%s', could not compare " % self.name
			info += "'%s' and '%s', some arguments differ:" % (k0, k1)
			for k in BENCHS_ARGS_NAME:
				v0 = args0[k]
				v1 = args1[k]
				if v0 != v1:
					info += "\n  - for '%s' got '%s' and '%s'" % (k, v0 ,v1)
			Logs.info (info)

# This function create the task (from the class bench) that handle one .bch file
# The decorator TaskGen.extension(".bch") is here so that this function is
# called every time a .bch file is seen in a task generator.
@TaskGen.extension(".bch")
def add_bch (self, node):
	if not "benchmarks" in self.features:
		self.bld.fatal ("The feature 'benchmarks' is needed to process .bch files")

	if not self.bld.cmp_only:
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

# Format the output of benchmarks, using the dict bench_results and bench_cmp
def benchmarks_format_output (bch):
	from waflib import Logs
	logfile = os.path.join (bch.bldnode.abspath(), "benchmarks.log")
	bch.logger = Logs.make_logger (logfile, "benchmarks")

	if not bch.cmp_only:
		bch.msg ("", "", color="NORMAL")
		bch.msg ("##### %s #####" % BenchCurrentRef(), "##########", color="NORMAL")
		fmt = "{:^9{fmt}} {:^9{fmt}} {:^9{fmt}} {:^9{fmt}}"
		D = bch.bench_results[BenchCurrentRef()]
		for groupname, groupdict in sorted(D.items(), key = lambda x:x[0]):
			bch.msg ("===== %s =====" % groupname, "==========", color = "NORMAL")
			for k,v in groupdict["args"].items():
				bch.msg ("args: %s" % k, v, color = "NORMAL")
			for f, data in sorted(groupdict["data"].items(), key = lambda x:x[0]):
				head = fmt.format ("min", "av", "max", "std", fmt="s")
				bch.msg (f, head, color = "CYAN")
				for eps in reversed(sorted(set(d["eps"] for d in data))):
					eps_time = set (d["time"] for d in data if d["eps"] == eps)
					m, M, av, std = set_stats (eps_time)
					c = "YELLOW" if M/m > 2 else "NORMAL"
					row = fmt.format (m, av, M, std, fmt=".2e")
					bch.msg ("  eps = %r" % eps, row, color=c)

	for k, D in bch.bench_cmp.items():
		bch.msg (" ", " ", color="NORMAL")
		bch.msg ("##### Comparison #####", "##########", color = "NORMAL")
		bch.msg ("reference", str(k[0]), color = "NORMAL")
		bch.msg ("compare with", str(k[1]), color = "NORMAL")
		for groupname, groupdict in sorted(D.items(), key = lambda x:x[0]):
			bch.msg ("===== %s =====" % groupname, "==========", color = "NORMAL")
			for f, data in sorted(groupdict.items(), key = lambda x:x[0]):
				bch.msg (f, "std1/std0 (av1-av0)/std0 #cells1/#cells0", color = "CYAN")
				for eps_data in data:
					msg_s = "  eps = %r" % eps_data["eps"]
					rstd = eps_data["std1/std0"]
					r = eps_data["(av1-av0)/std0"]
					rc = eps_data["#cells1/#cells0"]
					fmt = "  %s%.2f%s         %s%.2f%s           %.2f"

					if rstd >= BENCHS_INSTABLE_FACTOR:
						c1 = Logs.colors.RED
					elif rstd <= 1./BENCHS_INSTABLE_FACTOR:
						c1 = Logs.colors.GREEN
					else:
						c1 = Logs.colors.NORMAL
					args = [ c1, rstd, Logs.colors.NORMAL ]

					if r >= BENCHS_CMP_REGRESSION_FACTOR:
						c2 = Logs.colors.RED
					elif -r >= BENCHS_CMP_IMPROVMENT_FACTOR:
						c2 = Logs.colors.GREEN
					else:
						c2 = Logs.colors.NORMAL
					args += [ c2, r, Logs.colors.NORMAL, rc ]

					bch.msg (msg_s, fmt % tuple(args))

	if bch.bench_errors:
		sep = os.linesep + "  - "
		bch.fatal (sep.join (["Benchmarks errors:"] + bch.bench_errors))

#
class Cat (object):
	_choices = [ "dev", "easy", "medium", "hard", "blowup", "others", "unsolved" ]
	_default = "medium"

	@classmethod
	def help (cls):
		s = ", ".join (cls._choices)
		return "Possible values: %s [ default: %s ]" % (s, cls._default)

	@classmethod
	def default (cls):
		return [ cls._default ]

	@classmethod
	def list (cls):
		return cls._choices

	@staticmethod
	def opt_callback (option, opt_str, value, parser, *args, **kwargs):
		L = value.replace ("+", ",").split(",")
		for cat in L:
			if not cat in Cat.list():
				import optparse
				fmt = "option %s: invalid choice: '%s' (choose from %s)"
				h = ", ".join ("'%s'" % c for c in Cat.list())
				raise optparse.OptionValueError(fmt % (option, cat, h))
		setattr(parser.values, option.dest, L)


######################
###### options #######
######################
def options (opt):
	"""
	Options for the benchmarks
	"""


	grp = opt.add_option_group ("Options for benchmarks")
	for n, v in BENCHS_DEFAULT_ARGS.items():
		optname = "--benchs-" + n.replace("_", "-")
		grp.add_option (optname, action="store", dest = "BENCHS_" + n.upper(),
		                help = "Override default %s (default is %s)" % (n, v))
	grp.add_option ("--benchs-categories", help = Cat.help(), action = "callback",
	                callback = Cat.opt_callback, type = str,
	                dest = "BENCHS_CATEGORIES")
	grp.add_option ("--benchs-save", action = "store", dest = "BENCHS_SAVE",
	                help = "Save the results of the benchmarks in the given file")
	grp.add_option ("--benchs-cmp-to", action = "append", dest = "BENCHS_CMP_TO",
	                help = "Compare to previously saved benchmarks")
	grp.add_option ("--benchs-cmp-only", action = "store_true",
	                help = "No benchmarks run, only comparisons are performed",
	                dest = "BENCHS_CMP_ONLY")
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

	# Do not overwrite file with --benchs-save option
	if bch.options.BENCHS_SAVE:
		f = bch.options.BENCHS_SAVE
		if os.path.exists (f):
			bch.fatal ("Benchmarks: '%s' already exists, will not overwrite it." % f)
		bch.savefile = f
	else:
		bch.savefile = None

	# Check that option --benchs-cmp-to (if given) is a list of existing files
	bch.env.BCH_SCATTERPLOT_GRAPHFILE = "../benchs/scatter_plot.gnuplot"
	if bch.options.BENCHS_CMP_TO:
		for f in bch.options.BENCHS_CMP_TO:
			if not os.path.isfile (f):
				bch.fatal ("Benchmarks: cannot compare to %s: not a file." % f)
			elif bch.parse_summary_file (f) != 0:
				bch.fatal ("Error while parsing %s" % f)

	# Handle --benchs-precmd option
	if bch.options.BENCHS_PRECMD:
		bch.env.BCH_PRECMD = bch.options.BENCHS_PRECMD

	# Handle --benchs-cmp-only option
	if bch.options.BENCHS_CMP_ONLY:
		bch.cmp_only = bch.options.BENCHS_CMP_ONLY
	else:
		bch.cmp_only = False

	# Read list of categories from command line arguments
	if bch.options.BENCHS_CATEGORIES:
		bch.categories = bch.options.BENCHS_CATEGORIES
	else:
		if bch.cmp_only:
			bch.categories = Cat.list()
		else:
			bch.categories = Cat.default()

	# Handle --benchs-with-graphs option
	if bch.options.BENCHS_WITH_GRAPHS:
		bch.with_graphs = bch.options.BENCHS_WITH_GRAPHS
	else:
		bch.with_graphs = False

	# We need GNUPLOT to generate graphs
	if bch.with_graphs and not bch.env.GNUPLOT:
		bch.fatal ("gnuplot is required for the option '--benchs-with-graphs'")
