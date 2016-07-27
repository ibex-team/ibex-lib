#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-affine-extended", action="store_true", dest="WITH_AFFINE_EXTEND",
			help = "Use all the Affine Arithmetic plugin")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_AFFINE_EXTEND = conf.options.WITH_AFFINE_EXTEND
		
	conf.start_msg ("plugin Affine Extended Arithmetic")
	if not conf.env.WITH_AFFINE_EXTEND: 
		conf.end_msg ("not used")
		return

	conf.end_msg ("enabled")
	
	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "AFFINE_EXTENDED")

	# Add information in ibex_Setting
	conf.setting_define ("WITH_AFFINE_EXTENDED", 1)

	# add AFFINE plugin include directory
	for f in conf.path.ant_glob ("src/**", dir = True, src = False):
		conf.env.append_unique("INCLUDES_AFFINE_EXTENDED", f.abspath())

	# The build and install steps will be done from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add AFFINE headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add AFFINE source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

######################
####### build ########
######################
def build (bld):
	pass # nothing to do, everything is done in the main src/wscript script
