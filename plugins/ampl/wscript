#! /usr/bin/env python
# encoding: utf-8

#import os, sys, shutil
from waflib import Logs, Errors
import os, shutil
import ibexutils

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-ampl", action="store_true", dest="WITH_AMPL",
			help = "Use AMPL")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_AMPL = conf.options.WITH_AMPL
		
	conf.start_msg ("AMPL plugin")
	if not conf.env.WITH_AMPL: 
		conf.end_msg ("not used")
		return

	conf.end_msg ("enabled")
	
	# AMPL plugin is not compatible with Windows
	if (conf.env.DEST_OS == "win32"):
		conf.fatal ("The AMPL plugin is not compatible with Windows")

	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "AMPL")

	# Add information in ibex_Setting
	conf.setting_define ("WITH_AMPL", 1)

	# XXX: Inspired from configure_3rd_party_with_autotools from ibexutils.py.
	# Unfortunately, we cannot use that function, because the build system of
	# amplsolvers is too peculiar
	ampl_archive_name = "amplsolvers.tar.gz"
	ampl_name = ibexutils.archive_name_without_suffix (ampl_archive_name)
	Logs.pprint ("BLUE", "Starting installation of %s" % ampl_name)
	conf.to_log ((" Starting installation of %s " % ampl_name).center (80, "="))

	ampl_archive_node = conf.path.make_node("3rd").make_node (ampl_archive_name)
	ampl_archive_path = ampl_archive_node.abspath()
	destnode = conf.bldnode.make_node ("3rd")
	ampl_inc = destnode.make_node("include").make_node("amplsolvers")
	ampl_inc.mkdir()
	ampl_incdir = ampl_inc.abspath()
	ampl_libnode = destnode.make_node("lib")
	ampl_libnode.mkdir()
	ampl_libdir = ampl_libnode.abspath()

	ampl_srcdir = conf.extract_archive (ampl_archive_path, ampl_name, destnode)
	ampl_srcnode = destnode.make_node(os.path.basename(ampl_srcdir))

	conf.find_program ("make")

	# Apply patches 
	conf.apply_all_relevant_patches (ampl_name)

	if conf.env.ENABLE_SHARED:
		cflags = os.getenv("CFLAGS", "")
		cxxflags = os.getenv("CXXFLAGS", "")
		os.environ["CFLAGS"] = cflags + " ".join(conf.env.CFLAGS_cshlib)
		os.environ["CXXFLAGS"] = cxxflags+" "+" ".join(conf.env.CXXFLAGS_cxxshlib)

	cmd_conf = "./configurehere"
	cmd_make = conf.env.MAKE + [ "-j%d" % conf.options.jobs ]
	stages = [ (cmd_conf, "configure"),  (cmd_make, "make"), (None, "install") ]
	for cmd, stage in stages:
		conf.start_msg("Calling %s" % stage)
		try: 
			if stage == "install":
				for f in ampl_srcnode.ant_glob ("*.h *.hd lib*.a"):
					src = os.path.join (ampl_srcdir, f.name)
					if f.name.startswith ("lib"):
						dest = os.path.join (ampl_libdir, f.name)
					else:
						dest = os.path.join (ampl_incdir, f.name)
					shutil.copyfile (src, dest)
			else:
				out = conf.cmd_and_log (cmd, cwd=ampl_srcdir, env=os.environ)
			conf.end_msg("done")
		except Errors.WafError as e:
			conf.end_msg("failed", color="RED")
			print e
			conf.fatal ("failed to %s %s (%s)" % (stage, ampl_name, cmd))

	conf.to_log ((" Installation of %s: done " % ampl_name).center (80, "="))

	if conf.env.ENABLE_SHARED:
		os.environ["CFLAGS"] = cflags
		os.environ["CXXFLAGS"] = cxxflags

	conf.env.INSTALL_3RD = True
	conf.env.append_unique ("LIB_3RD_LIST", "amplsolvers")


	for h in [ "asl.h", "nlp.h", "getstub.h", "opcode.hd", "r_opn.hd" ]:
		conf.check_cxx (header_name = h, includes = ampl_incdir,
		                use = [ "IBEX", "AMPL" ], uselib_store= "AMPL")

	for l in [ "dl", "amplsolver" ]:
		conf.check_cxx (lib = l, libpath = ampl_libdir,
		                use = [ "IBEX", "AMPL" ], uselib_store = "AMPL")

	# Put every _AMPL variables in _IBEX_DEPS (for ibex.pc)
	new_inc = os.path.join (conf.env.INCDIR_3RD, "amplsolvers")
	conf.env.append_unique ("INCLUDES_IBEX_DEPS", new_inc)
	if conf.env.ENABLE_SHARED:
		# if shared lib is used, 3rd party libs are compiled as static lib with
		# -fPIC and are contained in libibex
		for lib in conf.env.LIB_AMPL:
			if not lib in conf.env.LIB_3RD_LIST:
				conf.env.append_unique ("LIB_IBEX_DEPS", lib)
	else:
		conf.env.append_unique ("LIB_IBEX_DEPS", conf.env.LIB_AMPL)

	# add AMPL plugin include directory
	for f in conf.path.ant_glob ("src/**", dir = True, src = False):
		conf.env.append_unique("INCLUDES_AMPL", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add AMPL headers
	for f in conf.path.ant_glob ("src/**/ibex_*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add AMPL source files
	for f in conf.path.ant_glob ("src/**/ibex_*.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

######################
####### build ########
######################
def build (bld):
	pass # nothing to do, everything is done in the main src/wscript script
