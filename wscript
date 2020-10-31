#! /usr/bin/env python
# encoding: utf-8

import os, sys
from distutils.version import LooseVersion
from waflib import Scripting, Logs, Options, Utils
import ibexutils

# The following variable is used to build ibex.pc and by "waf dist"
VERSION="2.8.9"
# The following variable is used only by "waf dist"
APPNAME='ibex-lib'

top = '.'
out = '__build__'

######################
###### options #######
######################
def options (opt):
	# when running on windows: do not attempt to detect msvc automatically
	# -> will use gcc by default
	import waflib.Tools.compiler_c
	import waflib.Tools.compiler_cxx
	waflib.Tools.compiler_c.c_compiler["win32"].remove ("msvc")
	waflib.Tools.compiler_cxx.cxx_compiler["win32"].remove ("msvc")

	opt.load ("compiler_cxx compiler_c javaw")
	opt.load ("waf_benchmarks", tooldir='.')

	opt.add_option ("--enable-shared", action="store_true", dest="ENABLE_SHARED",
			help = "build ibex as a shared library")

	opt.add_option ("--with-debug",  action="store_true", dest="DEBUG",
			help = "enable debugging")

	opt.recurse ("interval_lib_wrapper")
	opt.recurse ("lp_lib_wrapper")

	# recurse on plugins directory
	opt.recurse("plugins")

######################
##### configure ######
######################
def configure (conf):
	conf.load ('compiler_cxx compiler_c bison flex')

	conf.prepare_env(conf.env)

	# For information
	conf.msg ("sys.platform", sys.platform)

	# Set LIBDIR and INCDIR, set them in env
	conf.env.LIBDIR = os.path.join (conf.env.PREFIX, "lib")
	conf.env.INCDIR = os.path.join (conf.env.PREFIX, "include")
	conf.env.INCDIR_HDR = os.path.join (conf.env.INCDIR, "ibex")
	conf.env.INCDIR_3RD = os.path.join (conf.env.INCDIR_HDR, "3rd")
	conf.env.LIBDIR_3RD = os.path.join (conf.env.LIBDIR, "ibex", "3rd")
	conf.env.PKGDIR = os.path.join (conf.env.PREFIX, "share", "pkgconfig")

	# Add 'build' node to INCLUDES_IBEX (for generated headers)
	conf.env.append_unique ("INCLUDES_IBEX", conf.bldnode.abspath())

	# Did we used 3rd party software ? If set to True during configure, we need to
	# install them during install
	conf.env.INSTALL_3RD = False

	# Put VERSION number in conf.env (needed to build ibex.pc) and in the settings
	conf.env.VERSION = VERSION
	conf.setting_define("RELEASE", conf.env["VERSION"])

	# Optimised compilation flags
	if conf.options.DEBUG:
		Logs.info("Enabling debug mode")
		# note: -Wno-int-in-bool-context for Gaol (but unknown option by clang...)
		flags = "-std=c++11 -O0 -g -pg -Wall -Wno-int-in-bool-context"
		flags += " -fmessage-length=0"
		conf.define ("DEBUG", 1)
		conf.env.DEBUG = True
	else:
		# note: -Wno-int-in-bool-context for Gaol
		flags = "-std=c++11 -O3 -Wno-deprecated -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-function"
		conf.define ("NDEBUG", 1)
		conf.env.DEBUG = False
	for f in flags.split():
		conf.check_cxx(cxxflags=f, use="IBEX", mandatory=False, uselib_store="IBEX")
		
	# To fix Windows compilation problem (strdup with std=c++11, see issue #287)
	conf.check_cxx(cxxflags = "-U__STRICT_ANSI__", uselib_store="IBEX")

	# Build as shared lib is asked
	conf.start_msg ("Ibex will be built as a")
	if conf.options.ENABLE_SHARED:
		conf.env.ENABLE_SHARED = True
		conf.end_msg ("shared library")
	else:
		conf.end_msg ("static library")


	# Bison / Flex
	conf.env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	conf.env.append_unique ("FLEXFLAGS", "-Pibex")

	conf.recurse ("interval_lib_wrapper")
	conf.recurse ("lp_lib_wrapper")

	# recurse
	Logs.pprint ("BLUE", "Configuration of the plugins")
	conf.recurse ("plugins")
	Logs.pprint ("BLUE", "Configuration of the src directory")
	conf.recurse ("src")
	Logs.pprint ("BLUE", "Configuration of the tests")
	conf.recurse ("tests")

	Logs.pprint ("BLUE", "Load benchmarks module")
	conf.load ("waf_benchmarks")

	# If we used a 3rd party library, add the install path (for *.pc file)
	if conf.env.INSTALL_3RD: # It may not be necessary but it costs nothing
		conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR_3RD)
		# if shared lib is used, we only depends on libibex (3rd party libs are
		# compiled as static lib with -fPIC and are contained in libibex)
		if not conf.env.ENABLE_SHARED:
			conf.env.append_unique ("LIBPATH_IBEX_DEPS", conf.env.LIBDIR_3RD)

	# Generate header file containing Ibex settings
	conf.env.ibex_header_setting = "ibex_Setting.h"
	conf.write_setting_header (configfile = conf.env.ibex_header_setting,
		top = True, guard = "__IBEX_SETTING_H__")

	# Generate the main Ibex header which includes all the others headers
	conf.env.ibex_header = "ibex.h"
	conf.env.include_key = [ conf.env.ibex_header_setting ]
	conf.env.include_key += [ os.path.basename(h) for h in conf.env.IBEX_HDR
	                                                  if not h.endswith (".inl") ]
	conf.env.include_key = [ h[:-3] if h.endswith(".in") else h
                                            for h in conf.env.include_key ]
	conf.write_config_header (conf.env.ibex_header, defines = False, top = True,
			headers = True, guard = "__IBEX_H__", remove = False)
	del conf.env.include_key

	# In order for './waf clean' not to remove everything in subdirectory 3rd of
	# the build tree, we put every file of this subdirectory in cfg_files (those
	# files are not deleted by clean). In order not to pollute the current env,
	# we put then in a new one called "3rd".
	node_3rd = conf.bldnode.find_node ("3rd")
	if not node_3rd is None: # does this subdirectory exists ?
		conf.setenv ("3rd")
		for f in node_3rd.ant_glob("**"):
			conf.env.append_unique ("cfg_files", f.abspath())
		conf.setenv("")

######################
####### build ########
######################
def build (bld):
	bld.recurse ("plugins src")

	# Generate ibex.pc, the pkg-config file
	bld (features = "subst", source = "ibex.pc.in", target = "ibex.pc",
				install_path = bld.env.PKGDIR,
				IBEX_VERSION = bld.env.VERSION,
				CMAKE_INSTALL_PREFIX = ibexutils.escape_backslash_on_win32 (bld.env.PREFIX),
				CMAKE_INSTALL_INCLUDEDIR = bld.path_pc_remove_prefix (bld.env.INCDIR),
				CMAKE_INSTALL_LIBDIR = bld.path_pc_remove_prefix (bld.env.LIBDIR),
				IBEX_PKGCONFIG_INCDIRS = " ".join(["-I" + bld.path_pc (i)
				                                  for i in bld.env.INCLUDES_IBEX_DEPS]),
				IBEX_PKGCONFIG_CXXFLAGS = " ".join(bld.env.CXXFLAGS_IBEX_DEPS),
				IBEX_PKGCONFIG_LINK_DIRS = " ".join(["-L" + bld.path_pc (i)
				                                   for i in bld.env.LIBPATH_IBEX_DEPS]),
				IBEX_PKGCONFIG_LIBS = " ".join(["-l" + l for l in bld.env.LIB_IBEX_DEPS])
		)

	# Install ibex main header and header with settings
	bld.install_files (bld.env.INCDIR, bld.env.ibex_header)
	bld.install_files (bld.env.INCDIR_HDR, bld.env.ibex_header_setting)

	if bld.env.INSTALL_3RD:
		incnode = bld.bldnode.find_node("3rd").find_node("include")
		incfiles = incnode.ant_glob ("**", quiet=True)
		bld.install_files (bld.env.INCDIR_3RD, incfiles, cwd = incnode,
			relative_trick = True)

		libnode = bld.bldnode.find_node("3rd").find_node("lib")
		libfiles = libnode.ant_glob ("**", quiet=True)
		bld.install_files (bld.env.LIBDIR_3RD, libfiles, cwd = libnode,
			relative_trick = True)

######################
####### dist #########
######################
def dist (ctx):
	files_patterns = "wscript benchs/** src/** examples/** waf"
	files_patterns += " COPYING.LESSER LICENSE ibexutils.py"
	files_patterns += " interval_lib_wrapper/wscript interval_lib_wrapper/gaol"
	files_patterns += " lp_lib_wrapper/wscript lp_lib_wrapper/none"
	ctx.files = ctx.path.ant_glob(files_patterns)

######################
####### check ########
######################
def check (ctx):
	'''run build, install and utest'''
	Options.commands = [ "build", "install", "clean", "utest" ] + Options.commands

######################
####### utest ########
######################
def utest (tst):
	'''run the unitary tests'''
	logfile = os.path.join (tst.bldnode.abspath(), "utest_config.log")
	tst.logger = Logs.make_logger (logfile, "utest_config")
	tst.recurse ("tests plugins", mandatory = False)
	Logs.free_logger (tst.logger)
	tst.logger = None

######################
##### benchmarks #####
######################
def benchmarks (bch):
	'''run the benchmarks'''

	# Make sure all 'build' targets are up-to-date before doing benchmarks
	bch.add_build_targets()

	# load benchmarks tools
	bch.load ("waf_benchmarks")

	bch.recurse ("benchmarks plugins", mandatory = False)
