#! /usr/bin/env python
# encoding: utf-8

import os, sys, shutil

from waflib import Logs

def options (opt):
	opt.add_option ("--with-ampl", action="store_true", dest="WITH_AMPL",
			help = "Use AMPL")

def configure (conf):
	conf.env.WITH_AMPL = conf.options.WITH_AMPL
		
	if not conf.env.WITH_AMPL: 
		return
	
	##################################################################################################
	# AMPL plugin is disable on Window
	if (conf.env.DEST_OS == "win32" and conf.env.WITH_AMPL):
		Logs.pprint ("YELLOW", "Warning: AMPL is not supported on win32")
		conf.env.WITH_AMPL =False 
		return
		
	Logs.pprint ("BLUE", "Configure the AMPL plugin")
	
	# import utilities
	sys.path.append(conf.srcnode.abspath())
	import ibexutils

	#####################################################################################################
	# Try to find AMPL
	conf.env.BUILD_AMPL = False

	has_asl = conf.check_cxx (
			header_name	= "asl.h",
			mandatory	= False)
	has_arith = conf.check_cxx (
			header_name	= "arith.h",
			mandatory	= False)
	has_ampl = conf.check_cxx (
			lib = ["amplsolver"],
			uselib_store = "IBEX_DEPS",
			mandatory = False)
	
	if not (has_asl and has_arith and has_ampl):
		if (conf.env.WITH_AMPL):
			conf.env.BUILD_AMPL = True
	else:
		if (not conf.check_cxx (
			lib = ["dl"],
			uselib_store = "IBEX_DEPS",
			mandatory = False
			) ):
			conf.fatal ("cannot link with the dl library")

	#####################################################################################################
	# Install and Configure AMPL
	if conf.env.BUILD_AMPL:
		conf.env.AMPL="solvers"
		
		ampl_dir = ibexutils.unpack_archive(conf.path, conf.env.AMPL, "amplsolvers.tar.gz", conf.path.get_bld())

		conf.env.append_unique("INCLUDES", ampl_dir.abspath())
		conf.env.append_unique("LIBPATH", ampl_dir.abspath())

		conf.env.append_unique ("INCLUDES_IBEX_DEPS", os.path.join (conf.env.PREFIX, "include"))
		conf.env.append_unique ("LIB_IBEX_DEPS", ["amplsolver","dl"])

		conf.find_program ("make")
		amplcflag = "-O "
		
		if  (conf.env.INTERVAL_LIB is "GAOL") :
			amplcflag += " -DNo_dtoa -m32 "
               
		if conf.env.ENABLE_SHARED :
			amplcflag += " -fpic "

		cmd = "CFLAGS=\""+amplcflag+ "\" ./configurehere "

		if conf.exec_command (cmd, cwd=ampl_dir.abspath(), stdout=None, stderr=None):
			conf.fatal ("Failed to configure AMPL (%s)" % cmd)
	
	# add AMPL plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/ampl/src")

	
def build (bld):
		
	if not bld.env.WITH_AMPL: 
		return
	
	Logs.pprint ("BLUE", "Build the AMPL plugin")

	# import utilities
	sys.path.append(bld.srcnode.abspath())
	import ibexutils
		
	#####################################################################################################
	# Build AMPL
	if bld.env["BUILD_AMPL"]:
		bld.variant = "ampl"
		path = bld.path.get_bld().find_dir (bld.env.AMPL).abspath()

		with ibexutils.subdir (path):
			cmd = []
			if bld.cmd in ("clean", "uninstall") :
				cmd = ["clean"] 
			elif bld.env.ENABLE_SHARED :
				cmd =["shared"]
			
			if bld.exec_command (
				[bld.env.MAKE, "-j", str(bld.options.jobs)] + cmd,
				cwd	= path,
				stdout	= None,
				stderr	= None,
			):
				bld.fatal ("Failed to build AMPL")
	
			if bld.cmd=="install":
				if not os.path.exists(bld.env.LIBDIR):
					os.makedirs(bld.env.LIBDIR)
				if bld.env.ENABLE_SHARED :
					shutil.copyfile(os.path.join(path,"libamplsolver.so"),os.path.join(bld.env.LIBDIR,"libamplsolver.so"))
				else :
					shutil.copyfile(os.path.join(path,"libamplsolver.a"),os.path.join(bld.env.LIBDIR,"libamplsolver.a"))
			elif  bld.cmd in ("clean", "uninstall"):
				os.remove(os.path.join(bld.env.LIBDIR,"libamplsolver.a"))
				os.remove(os.path.join(bld.env.LIBDIR,"libamplsolver.so"))

		bld.variant = ""

	# add AMPL plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
	# add AMPL plugin headers
	bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))

	# Add information in ibex_Setting
	bld.env.settings['_IBEX_WITH_AMPL_']='1'	

#def distclean (ctx):
#	ampl_dir = ctx.path.find_dir ("solvers")
#	if ampl_dir and os.path.exists (os.path.join (ampl_dir.abspath(), "Makefile")):
#		with ibexutils.subdir (ampl_dir):
#			ctx.exec_command ("make clean && make distclean", cwd = ampl_dir.abspath())
