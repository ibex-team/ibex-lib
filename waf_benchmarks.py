import os
from waflib import TaskGen, Task, Logs, Utils, Errors
benchlock = Utils.threading.Lock()

class bench (Task.Task):
	"""
	Execute a benchmark
	"""

	color = 'CYAN'
	vars = []

	def runnable_status (self):
		# for the moment always return true.
		# TODO: distinguish between short and long bench (and use option)
		for t in self.run_after:
			if not t.hasrun:
				return Task.ASK_LATER
		return Task.RUN_ME

	def get_bench_env (self):
		return self.generator.bench_env

	def get_bench_bin (self):
		return self.generator.bintask.outputs[0].abspath()

	def get_bch_file (self):
		return self.inputs[0].abspath()

	def run (self):
		cmd = [ self.get_bench_bin (), self.get_bch_file (), "1e-3", "1e-3", "5"]
		Logs.debug('run bench: %r', cmd)

		src = None
		with open (self.outputs[0].abspath(), "w") as outfile:
			proc = Utils.subprocess.Popen (cmd, env = self.get_bench_env (),
			                               stdout = outfile, stderr = outfile)
			rc = proc.wait ()

		res = (self.inputs[0], rc, self.outputs[0])
		benchlock.acquire()
		try:
			return self.generator.register_bench_results (res)
		finally:
			benchlock.release()

	def keyword (self):
		return "Benchmarking"


@TaskGen.taskgen_method
def register_bench_results (self, res):
	Logs.debug("register bench result: %r", res)
	
	try:
		self.bld.bench_results.append (res)
	except AttributeError:
		self.bld.bench_results = [res]

@TaskGen.extension(".bch")
def add_bch (self, node):
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

	self.bench_env = dict (os.environ)
	outnode = node.change_ext('.bench_result', '.bch')
	tsk = self.create_task ('bench', node, outnode)
	tsk.set_run_after (self.bintask)

def benchmarks_format_output (bch):
	# The logger is freed at the end of the benchmarks function but the tasks are
	# performed afterwards, so we recreate the logger.
	logfile = os.path.join (bch.bldnode.abspath(), "benchmarks_run.log")
	bch.logger = Logs.make_logger (logfile, "benchmarks_run")

	lst = sorted (getattr (bch, 'bench_results', []), key = lambda x:x[0])
	for (f, ret, log) in lst:
		bch.start_msg (f.relpath ())
		if ret == 0:
			bch.end_msg (log.relpath())
		else:
			bch.end_msg ("Error", color = "RED")

	Logs.free_logger (bch.logger)
	bch.logger = None

def options (opt):
	"""
	Options for the benchmarks
	"""
	grp = opt.add_option_group ("Options for benchmarks")
	grp.add_option ("--benchs-long", action = "store_true", default = False,
	                help = "(Very) Long benchmarks", dest = "benchs_long")
	grp.add_option ("--benchs-precmd", action = "store", default = "",
	                help = "Prefix the benchmarks command with this string",
	                dest = 'benchs_precmd')

def benchmarks (bch):
	bch.add_post_fun (benchmarks_format_output)

	# Benchmarks are done with the Ibex library in the build tree
	ibexlibpath = os.path.join (bch.bldnode.abspath(), "src")
	bch.env.append_unique ("LIBPATH_BENCHMARKS", ibexlibpath)
	bch.env.append_unique ("LIB_BENCHMARKS", "ibex")

	bch.USELIST = ["BENCHMARKS", "IBEX", "ITV_LIB"] + bch.env.IBEX_PLUGIN_USE_LIST
