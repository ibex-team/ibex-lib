#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-ensta-robotics", action="store_true",
		dest="WITH_ENSTA_ROBOTICS", help = "Use ENSTA robotics plugin")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_ENSTA_ROBOTICS = conf.options.WITH_ENSTA_ROBOTICS
		
	conf.start_msg ("ENSTA robotics plugin")
	if not conf.env.WITH_ENSTA_ROBOTICS: 
		conf.end_msg ("not used")
		return
	
	conf.end_msg ("enabled")
	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "ENSTA_ROBOTICS")

	# add ENSTA_ROBOTICS plugin include directory
	for f in conf.path.ant_glob ("src/** src", dir = True, src = False):
		conf.env.append_unique("INCLUDES_ENSTA_ROBOTICS", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add ENSTA_ROBOTICS headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add ENSTA_ROBOTICS source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

	# The utest step will be run from the main tests/wscript script so we need to
	# give path relative to the main tests directory
	maintests = conf.srcnode.make_node ("tests")

	# add ENSTA_ROBOTICS test files
	for f in conf.path.ant_glob ("tests/**/*.cpp"):
		conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

	# Add ensta_robotics/tests directory to list of INCLUDES for TESTS
	testsnode = conf.path.make_node ("tests")
	conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())

######################
####### build ########
######################
def build (bld):
	pass # nothing to do, everything is done in the main src/wscript script
