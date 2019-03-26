#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-sip", action="store_true",
			dest="WITH_SIP", help = "Use Semi-infinite programming plugin")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_SIP = conf.options.WITH_SIP
		
	conf.start_msg ("Semi-infinite programming plugin")
	if not conf.env.WITH_SIP: 
		conf.end_msg ("not used")
		return
	
	conf.end_msg ("enabled")
		
	if not conf.options.WITH_OPTIM:
		conf.fatal ("The SIP plugin requires IbexOpt: please run configure with --with-optim")

	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "SIP")
	
	# Add information in ibex_Setting
	conf.setting_define ("WITH_SIP", 1)

	# add SIP plugin include directory
	for f in conf.path.ant_glob ("src/** src", dir = True, src = False):
		conf.env.append_unique("INCLUDES_SIP", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add SIP headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add SIP source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

	# The utest step will be run from the main tests/wscript script so we need to
	# give path relative to the main tests directory
	maintests = conf.srcnode.make_node ("tests")

	# add SIP test files
	for f in conf.path.ant_glob ("tests/**/*.cpp"):
		conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

	# Add sip/tests directory to list of INCLUDES for TESTS
	testsnode = conf.path.make_node ("tests")
	conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())

######################
####### build ########
######################
def build (bld):
	
	if bld.env.WITH_SIP:
		# build solver binary
		bld.program (
		target = "ibexsip",
		use = [ "ibex" ], # add dependency on ibex library
		source = bld.path.ant_glob ("main/ibexsip.cpp"),
		install_path = bld.env.BINDIR,
		)
		bld.program (
		target = "ibexopt-sip",
		use = [ "ibex" ], # add dependency on ibex library
		source = bld.path.ant_glob ("main/optimizer.cpp"),
		install_path = bld.env.BINDIR,
		)
		bld.program (
		target = "ibexsolve-sip",
		use = [ "ibex" ], # add dependency on ibex library
		source = bld.path.ant_glob ("main/solver.cpp"),
		install_path = bld.env.BINDIR,
		)
