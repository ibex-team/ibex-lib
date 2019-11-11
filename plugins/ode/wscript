#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-ode", action="store_true",
			dest="WITH_ODE", help = "Use ODE plugin")

######################
##### configure ######
######################
def configure (conf):	

	conf.env.WITH_ODE = conf.options.WITH_ODE
		
	conf.start_msg ("ODE plugin")
	if not conf.env.WITH_ODE: 
		conf.end_msg ("not used")
		return
	
	if not conf.options.WITH_OPTIM:
		conf.fatal ("ODE plugin requires IbexOpt (--with-optim)")

	conf.end_msg ("enabled")
	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "ODE")
	
	# Add information in ibex_Setting
	conf.setting_define ("WITH_ODE", 1)

	# add ODE plugin include directory
	for f in conf.path.ant_glob ("src/** src", dir = True, src = False):
		conf.env.append_unique("INCLUDES_ODE", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add ODE headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add ODE source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

	# The utest step will be run from the main tests/wscript script so we need to
	# give path relative to the main tests directory
	maintests = conf.srcnode.make_node ("tests")

	# add ODE test files
	for f in conf.path.ant_glob ("tests/**/*.cpp"):
		conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

	# Add ode/tests directory to list of INCLUDES for TESTS
	testsnode = conf.path.make_node ("tests")
	conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())

######################
####### build ########
######################
def build (bld):
	
	if bld.env.WITH_ODE:
		# build optim binary
		bld.program (
		target = "ibexode-attract-region",
		# gch: i don't know if specifying "IBEXOPT" is necessary here
		use = [ "ibex", "IBEXOPT" ], # add dependency on ibex library
		# note: the two other executables are not installed by default
		source = bld.path.ant_glob ("main/ibexode-attract-region.cpp"),
		install_path = bld.env.BINDIR,
		)
