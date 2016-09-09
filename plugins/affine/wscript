#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-affine", action="store_true", dest="WITH_AFFINE",
			help = "Use Affine Arithmetic plugin")

######################
##### configure ######
######################
def configure (conf):
	# The affine plugin is enabled if --with-affine or --with-affine-extended
	# options are used.
	if conf.options.WITH_AFFINE or conf.options.WITH_AFFINE_EXTEND:
		conf.env.WITH_AFFINE = True

	conf.start_msg ("plugin Affine Arithmetic")
	if not conf.env.WITH_AFFINE: 
		conf.end_msg ("not used")
		return

	conf.end_msg ("enabled")

	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "AFFINE")

	# Add information in ibex_Setting
	conf.setting_define ("WITH_AFFINE", 1)

	conf.start_msg ("Will use LinearRelaxAffine class")
	if not conf.options.WITH_OPTIM:
		conf.end_msg ("no (need --with-optim)", color = "YELLOW")
		excl = "src/**/numeric" # exclude everything from the src/numeric subdir
	else:
		conf.end_msg ("yes (--with optim is used)")
		excl = ""

	# add AFFINE plugin include directory
	for f in conf.path.ant_glob ("src/**", dir = True, src = False, excl = excl):
		conf.env.append_unique("INCLUDES_AFFINE", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add AFFINE headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h", excl = excl):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add AFFINE source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp", excl = excl):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

	# The utest step will be run from the main tests/wscript script so we need to
	# give path relative to the main tests directory
	maintests = conf.srcnode.make_node ("tests")

	# add AFFINE test files
	for f in conf.path.ant_glob ("tests/**/*.cpp"):
		conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

	# Add affine/tests directory to list of INCLUDES for TESTS
	testsnode = conf.path.make_node ("tests")
	conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())
	
######################
####### build ########
######################
def build (bld):
	pass # nothing to do, everything is done in the main src/wscript script
