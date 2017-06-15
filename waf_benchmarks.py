import os, sys, re, math, shutil
import ibexutils
from waflib import TaskGen, Task, Logs, Utils, Errors
benchlock = Utils.threading.Lock()

BENCHS_DEFAULT_ARGS = {"time_limit": 5, "prec_ndigits_max": "8",
                       "prec_ndigits_min": "1"}
BENCHS_ARGS_NAME = BENCHS_DEFAULT_ARGS.keys()

class Bench (Task.Task):
	"""
	Execute a benchmark
	"""

	color = 'CYAN'
	vars = []

	RE_FP = r"[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?"
	CAP_FP = "(%s)" % RE_FP
	REGEXES = {"cap_fp" : CAP_FP}
	RESULTS_PATTERN = "^BENCH: eps = {cap_fp} ; time = {cap_fp}\n$"
	RESULTS_RE = re.compile (RESULTS_PATTERN.format(**REGEXES))

	def runnable_status (self):
		# for the moment always return true.
		# TODO: distinguish between short and long bench (and use option)
		for t in self.run_after:
			if not t.hasrun:
				return Task.ASK_LATER
		return Task.RUN_ME

	@property
	def bench_env (self):
		return self.generator.bench_env

	@property
	def bench_bin (self):
		return self.generator.bintask.outputs[0].abspath()

	@property
	def bch_node (self):
		return self.inputs[0]

	@property
	def bch_file (self):
		return self.bch_node.abspath()

	@property
	def results_node (self):
		return self.outputs[0]

	@property
	def results_file (self):
		return self.results_node.abspath()

	@property
	def data_node (self):
		return self.outputs[1]

	@property
	def data_file (self):
		return self.data_node.abspath()

	@property
	def graph_node (self):
		try:
			return self.outputs[2]
		except IndexError:
			return None

	@property
	def graph_file (self):
		node = self.graph_node
		if node is None:
			return None
		else:
			return node.abspath()

	def cmd_bench (self):
		cmd = self.generator.bld.options.BENCHS_PRECMD.split()
		cmd += [ self.bench_bin, "--bench-file", self.bch_file ]
		for k in BENCHS_ARGS_NAME:
			_k = k.replace("_", "-")
			v = getattr (self.generator.bld.options, "BENCHS_" + k.upper(), None)
			if v is None:
				v = getattr(self, k)
			cmd += [ "--%s" % _k, str(v) ]
		return cmd

	def run_bench (self):
		# run the bench using bench_bin on bch_file
		cmd = self.cmd_bench()
		Logs.debug('run bench: %r', cmd)
		with open (self.results_file, "w") as outfile:
			proc = Utils.subprocess.Popen (cmd, env = self.bench_env,
			                               stdout = outfile, stderr = outfile)
			rc = proc.wait ()

		if rc != 0: # set error message in case of failure
			err_msg = None
			self.err_msg = "Benchmark of file '%s' failed with " % self.bch_node.name
			for l in ibexutils.to_unicode(self.results_node.read()).splitlines(True):
				if l.startswith ("error: "):
					err_msg = l[7:-1]
			if err_msg is None:
				self.err_msg += "unknown error, see %s for details." % self.results_node
			else:
				self.err_msg += "error: " + err_msg
		
		return rc

	def run_data (self):
		# Get the data and write the data file from the results_file
		data = []
		for l in ibexutils.to_unicode(self.results_node.read()).splitlines(True):
			match = self.RESULTS_RE.match (l)
			if match:
				eps = float (match.group(1))
				time = float (match.group(2))
				data.append((eps, time))
		data.sort (key=lambda x:-x[0])

		datastr = "# eps time" + os.linesep
		datastr += os.linesep.join([ "%s %s" % (eps, time) for eps, time in data])
		self.data_node.write (datastr + os.linesep)
		return data

	def run_graph (self):
		eargs_list = [ ("datafile", self.data_file),
		               ("outputfile", self.graph_file),
		               ("title", self.bch_node.relpath().replace ('_', '\_'))
		             ]
		for k in BENCHS_ARGS_NAME:
			eargs_list.append((k, str(getattr(self, k))))
		eargs = " ; ".join([ "%s = '%s'" % a for a in eargs_list])
		cmd = [ self.env.GNUPLOT[0], "-e", eargs, self.generator.graph_scriptfile ]
		proc = Utils.subprocess.Popen (cmd, env = self.bench_env,
		                               stdout = Utils.subprocess.PIPE,
		                               stderr = Utils.subprocess.PIPE)
		(_, stderr) = proc.communicate ()
		rc = proc.wait ()

		if rc != 0: # set error message in case of failure
			self.err_msg = "Generating file '%s' failed: " % self.graph_node.name
			self.err_msg += os.linesep + "  cmd: " + " ".join(cmd) 
			self.err_msg += os.linesep + stderr

		return rc

	def run (self):
		ret = self.run_bench()
		if ret != 0:
			return ret

		data = self.run_data()

		# Generate the graph if required
		if not self.graph_node is None:
			ret = self.run_graph ()
			if ret != 0:
				return ret

		res = (self.bch_node, data, self.outputs)
		benchlock.acquire()
		try:
			return self.generator.register_bench_results (res)
		finally:
			benchlock.release()

		return 0

	def keyword (self):
		return "Benchmarking"

# This function register the results from the bench. All successful 'bench' task
# call this function to add the results in the list bench_results of self.bld.
# This list will be used by benchmarks_format_output.
@TaskGen.taskgen_method
def register_bench_results (self, res):
	Logs.debug("register bench result: %r", res[0].abspath())
	
	try:
		self.bld.bench_results.append (res)
	except AttributeError:
		self.bld.bench_results = [res]

# This function create the task (from the class bench) that handle one .bch file
# The decorator TaskGen.extension(".bch") is here so that this function is
# called every time a .bch file is seen in a task generator. 
@TaskGen.extension(".bch")
def add_bch (self, node):
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

	# Set the environment of the task from default environment
	self.bench_env = dict (os.environ)

	# Get (optional) attribute: time_limit, min_prec, max_prec
	kw = {}
	for k in BENCHS_ARGS_NAME:
		kw[k] = getattr (self, k, BENCHS_DEFAULT_ARGS[k])

	# Set output nodes
	resnode = node.change_ext('.bench_result', '.bch')
	datanode = node.change_ext('.data', '.bch')
	outputs = [ resnode, datanode ]
	if self.bld.options.BENCHS_WITH_GRAPHS:
		if getattr(self, "graph_scriptfile", None):
			fignode = node.change_ext('.pdf', '.bch')
			outputs += [ fignode ]
	
	# Create the task
	tsk = self.create_task ('Bench', node, outputs, **kw)
	tsk.set_run_after (self.bintask)

# Format the output of benchmarks, using the list bench_results
# Assume that for each result, data are already sorted
def benchmarks_format_output (bch):
	# Create the logger to store the output of this function
	logfile = os.path.join (bch.bldnode.abspath(), "benchmarks.log")
	bch.logger = Logs.make_logger (logfile, "benchmarks")

	def compute_score (data):
		S = 0.0
		prev_elt = None
		for eps, time in data:
			if not prev_elt is None:
				prev_eps, prev_time = prev_elt
				log10_eps = -math.log10(eps)
				prev_log10_eps = -math.log10(prev_eps)
				S += (log10_eps-prev_log10_eps)*(time+prev_time)/2
			prev_elt = (eps, time)
		return S

	def write_results (data):
		for d in data:
			bch.to_log ("BENCH: eps = %.12g ; time = %f" % d)

	def parse_results_file (filename):
		measures_pattern = "^([0-9]+) measure[s]?, Score ="
		measures_re = re.compile (measures_pattern)
		data = []
		R = {}
		current_bchfile = None
		with open (filename, "r") as f:
			for l in ibexutils.to_unicode(f.read()).splitlines(True):
				match_measures = measures_re.match (l)
				match_bench = Bench.RESULTS_RE.match (l)
				if l.startswith ("FILE: "):
					current_bchfile = str(l[6:-1])
				elif match_measures:
					assert (current_bchfile)
					data.sort (key=lambda x:-x[0])
					S = compute_score (data)
					R[current_bchfile] = (S, data)
					data = []
				elif match_bench:
					eps = float (match_bench.group(1))
					time = float (match_bench.group(2))
					data.append((eps, time))
		return R

	if bch.options.BENCHS_CMP_TO:
		cmpdata = parse_results_file (bch.options.BENCHS_CMP_TO)
	else:
		cmpdata = {}

	lst = sorted (getattr (bch, 'bench_results', []), key = lambda x:x[0])
	for (f, data, outputs) in lst:
		bch.start_msg (f.relpath ())
		n = len (data)
		bch.to_log ("FILE: %s" % f.relpath())
		write_results (data)
		S = compute_score (data)
		msg = "%d measure%s, Score = %g" % (n, "s" if n > 1 else "", S)
		color = "CYAN"
		cmpS, _ = cmpdata.get (f.relpath(), (None, None))
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

	if bch.options.BENCHS_SAVE:
		shutil.copyfile (logfile, bch.options.BENCHS_SAVE)

######################
###### options #######
######################
def options (opt):
	"""
	Options for the benchmarks
	"""

	categories = [ "easy", "medium", "hard" ]
	default_cat = "medium"
	cat_help = "Possible values: " + ", ".join(categories)
	cat_help += " [default: %s]" % default_cat

	grp = opt.add_option_group ("Options for benchmarks")
	for n, v in BENCHS_DEFAULT_ARGS.items():
		optname = "--benchs-" + n.replace("_", "-")
		helpstr = "Override default %s (default is %s)" % (n, v)
		grp.add_option (optname, action="store", default=None,
		                help = helpstr, dest = "BENCHS_" + n.upper())
	grp.add_option ("--benchs-category", action = "store", default = default_cat,
	                choices = categories, help = cat_help, dest = "BENCHS_CAT")
	grp.add_option ("--benchs-save", action = "store", default = "",
	                help = "Save the results of the benchmarks in the given file",
	                dest = "BENCHS_SAVE")
	grp.add_option ("--benchs-cmp-to", action = "store", default = "",
	                help = "Compare to a previously saved benchmarks",
	                dest = "BENCHS_CMP_TO")
	grp.add_option ("--benchs-with-graphs", action = "store_true", default=False,
	                help = "Generate graphics from benchs (when available)",
	                dest = "BENCHS_WITH_GRAPHS")
	grp.add_option ("--benchs-precmd", action = "store", default = "",
	                help = "Prefix the benchmarks command with this string",
	                dest = 'BENCHS_PRECMD')

######################
##### configure ######
######################
def configure (conf):
	conf.find_program ("gnuplot", var = "GNUPLOT", mandatory = False)

######################
##### benchmarks #####
######################
def benchmarks (bch):
	bch.add_post_fun (benchmarks_format_output)

	# For benchmarks, set the default number of parallel tasks is 1
	p = [ "-j", "--jobs", "-j%d" % bch.jobs, "--jobs=%d" % bch.jobs ]
	if all ([ not a in sys.argv for a in p]): # jobs is not explicitly set
		bch.jobs = bch.options.jobs = 1

	# Do not overwrite file with --save-results option
	if bch.options.BENCHS_SAVE and os.path.exists (bch.options.BENCHS_SAVE):
		f = bch.options.BENCHS_SAVE
		bch.fatal ("Benchmarks: file %s already exists, will not overwrite it." % f)

	# Check that option --benchs-cmp-to (if given) is a file
	if bch.options.BENCHS_CMP_TO:
		if not os.path.isfile (bch.options.BENCHS_CMP_TO):
			f = bch.options.BENCHS_CMP_TO
			bch.fatal ("Benchmarks: cannot compare to %s: not a file." % f)

	# We need GNUPLOT to generate graphs
	if bch.options.BENCHS_WITH_GRAPHS and not bch.env.GNUPLOT:
		bch.fatal ("gnuplot is required for the option '--benchs-with-graphs'")
