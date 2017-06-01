import os, sys, re
import ibexutils
from waflib import TaskGen, Task, Logs, Utils, Errors
benchlock = Utils.threading.Lock()

BENCHS_DEFAULT_ARGS = {"time_limit": 5, "prec_ndigits_max": "8",
                       "prec_ndigits_min": "1"}
BENCHS_ARGS_NAME = BENCHS_DEFAULT_ARGS.keys()

class bench (Task.Task):
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
		cmd = self.generator.bld.options.benchs_precmd.split()
		cmd += [ self.bench_bin, "--bench-file", self.bch_file ]
		for k in BENCHS_ARGS_NAME:
			_k = k.replace("_", "-")
			cmd += [ "--%s" % _k, str(getattr(self, k)) ]
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
		title = self.bch_node.relpath().replace ('_', '\_')
		g = (self.data_file, self.graph_file, title)
		args = "datafile='%s' ; outputfile='%s' ; title='%s'" % g
		cmd = [ self.env.GNUPLOT[0], "-e", args, self.generator.graph_scriptfile ]
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
	if self.bld.options.with_graphs and getattr(self, "graph_scriptfile", None):
		fignode = node.change_ext('.pdf', '.bch')
		outputs += [ fignode ]
	
	# Create the task
	tsk = self.create_task ('bench', node, outputs, **kw)
	tsk.set_run_after (self.bintask)

# Format the output of benchmarks, using the list bench_results
def benchmarks_format_output (bch):
	# Create the logger to store the output of this function
	logfile = os.path.join (bch.bldnode.abspath(), "benchmarks.log")
	bch.logger = Logs.make_logger (logfile, "benchmarks")

	lst = sorted (getattr (bch, 'bench_results', []), key = lambda x:x[0])
	for (f, data, outputs) in lst:
		bch.start_msg (f.relpath ())
		n = len (data)
		tot_time = sum ([t for _,t in data])
		bch.end_msg ("%d measure%s, total time = %.2fs" % (n, "s" if n > 1 else "", tot_time))

######################
###### options #######
######################
def options (opt):
	"""
	Options for the benchmarks
	"""
	grp = opt.add_option_group ("Options for benchmarks")
	grp.add_option ("--benchs-long", action = "store_true", default = False,
	                help = "(Very) Long benchmarks", dest = "benchs_long")
	grp.add_option ("--with-graphs", action = "store_true", default = False,
	                help = "Generate graphics from benchs (when available)",
	                dest = "with_graphs")
	grp.add_option ("--benchs-precmd", action = "store", default = "",
	                help = "Prefix the benchmarks command with this string",
	                dest = 'benchs_precmd')

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

	# We need GNUPLOT to generate graphs
	if bch.options.with_graphs and not bch.env.GNUPLOT:
		bch.fatal ("gnuplot is required for the option '--with-graphs'")
