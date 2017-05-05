#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-param-estim", action="store_true",
			dest="WITH_PARAM_ESTIM", help = "Use Parameter Estimation plugin")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_PARAM_ESTIM = conf.options.WITH_PARAM_ESTIM
		
	conf.start_msg ("Parameter Estimation plugin")
	if not conf.env.WITH_PARAM_ESTIM: 
		conf.end_msg ("not used")
		return
	
	conf.end_msg ("enabled")
	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "PARAM_ESTIM")
	
	# Add information in ibex_Setting
	conf.setting_define ("WITH_PARAM_ESTIM", 1)

	# add PARAM_ESTIM plugin include directory
	for f in conf.path.ant_glob ("src/** src", dir = True, src = False):
		conf.env.append_unique("INCLUDES_PARAM_ESTIM", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add PARAM_ESTIM headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add PARAM_ESTIM source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp src/cliquer/*cpp",
			excl="src/cliquer/cl.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

	# The utest step will be run from the main tests/wscript script so we need to
	# give path relative to the main tests directory
	maintests = conf.srcnode.make_node ("tests")

	# add PARAM_ESTIM test files
	for f in conf.path.ant_glob ("tests/**/*.cpp"):
		conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

	# Add param_estim/tests directory to list of INCLUDES for TESTS
	testsnode = conf.path.make_node ("tests")
	conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())

######################
####### build ########
######################
def build (bld):
	pass # nothing to do, everything is done in the main src/wscript script
