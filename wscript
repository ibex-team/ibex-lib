#! /usr/bin/env python
# encoding: utf-8

import os, sys
from distutils.version import LooseVersion
from waflib import Scripting, Logs, Options, Utils
import ibexutils

# The following variable is used to build ibex.pc and by "waf dist"
VERSION="2.8.0"
# The following variable is used only by "waf dist"
APPNAME='ibex-lib'

top = '.'
out = '__build__'

ITVLIB_PLUGIN_PREFIX = "interval_lib_"

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

	# get the list of all possible interval library
	plugin_node = opt.path.find_node("plugins")
	libdir = plugin_node.ant_glob(ITVLIB_PLUGIN_PREFIX+"*", dir=True, src=False)
	libdir = [ os.path.basename(str(d)) for d in libdir ]
	list_of_interval_lib_plugin = [ d[len(ITVLIB_PLUGIN_PREFIX):] for d in libdir]
	# set default interval library
	deflib = "gaol" if not Utils.is_win32 else "filib"
	if list_of_interval_lib_plugin == []: # this will raise a error at configure
		default_interval_lib = None
	elif deflib in list_of_interval_lib_plugin:
		default_interval_lib = deflib
	elif any(lib.startswith(deflib) for lib in list_of_interval_lib_plugin):
		L = [ lib for lib in list_of_interval_lib_plugin if lib.startswith(deflib) ]
		L.sort(key=LooseVersion)
		default_interval_lib = L[-1] # choose the latest version
	else: # use the first of the list as default
		default_interval_lib = list_of_interval_lib_plugin[0]

	# help string for --interval-lib command line option
	help_string = "Possible values: " + ", ".join(list_of_interval_lib_plugin)
	help_string += " [default: " + str(default_interval_lib) + "]"

	# add the option --interval-lib
	opt.add_option ("--interval-lib", action="store", dest="INTERVAL_LIB",
									choices = list_of_interval_lib_plugin,
									default = default_interval_lib, help = help_string)

	ibexutils.lp_lib_options (opt)

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

	# Set LIBDIR and INCDIR, set them in env and put them in *_IBEX_DEPS
	conf.env.LIBDIR = os.path.join (conf.env.PREFIX, "lib")
	conf.env.append_unique ("LIBPATH_IBEX_DEPS", conf.env.LIBDIR)
	conf.env.INCDIR = os.path.join (conf.env.PREFIX, "include")
	conf.env.INCDIR_HDR = os.path.join (conf.env.INCDIR, "ibex")
	conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR)
	conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR_HDR)
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

	# Set env variable with the prefix of plugins which handle interval arithmetic
	conf.env.ITVLIB_PLUGIN_PREFIX = ITVLIB_PLUGIN_PREFIX

	# Optimised compilation flags
	if conf.options.DEBUG:
		Logs.info("Enabling debug mode")
		flags = "-std=c++11 -O0 -g -pg -Wall"
		flags += " -fmessage-length=0"
		conf.define ("DEBUG", 1)
		conf.env.DEBUG = True
	else:
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

	# Recurse on the interval library directory.
	if conf.options.INTERVAL_LIB is None:
		conf.fatal ("No interval library is available.")
	Logs.pprint ("BLUE", "Configuration of the library for interval arithmetic")
	conf.msg ("Library for interval arithmetic", conf.options.INTERVAL_LIB)
	itvlib_dir = ITVLIB_PLUGIN_PREFIX + conf.options.INTERVAL_LIB
	conf.recurse (os.path.join("plugins", itvlib_dir))
	# Copy in _IBEX_DEPS some important variables from _ITV_LIB
	# The plugin must use the store ITV_LIB (uselib_store argument with
	# conf.check* functions).
	conf.env.append_unique ("CXXFLAGS_IBEX_DEPS", conf.env.CXXFLAGS_ITV_LIB)
	if conf.env.ENABLE_SHARED:
		# if shared lib is used, 3rd party libs are compiled as static lib with
		# -fPIC and are contained in libibex
		for lib in conf.env.LIB_ITV_LIB:
			if not lib in conf.env.LIB_3RD_LIST:
				conf.env.append_unique ("LIB_IBEX_DEPS", lib)
	else:
		conf.env.append_unique ("LIB_IBEX_DEPS", conf.env.LIB_ITV_LIB)

	# The following variables must be defined in env by the plugin called to
	# handle the library for interval arithmetic.
	for var in ["INTERVAL_LIB", "IBEX_INTERVAL_LIB_WRAPPER_CPP",
        "IBEX_INTERVAL_LIB_WRAPPER_H", "IBEX_INTERVAL_LIB_INCLUDES",
        "IBEX_INTERVAL_LIB_NEG_INFINITY", "IBEX_INTERVAL_LIB_POS_INFINITY",
        "IBEX_INTERVAL_LIB_EXTRA_DEFINES",
        "IBEX_INTERVAL_LIB_ITV_EXTRA", "IBEX_INTERVAL_LIB_ITV_WRAP",
        "IBEX_INTERVAL_LIB_ITV_ASSIGN", "IBEX_INTERVAL_LIB_ITV_DEF",
        "IBEX_INTERVAL_LIB_DISTANCE"]:
		if not conf.env[var]:
			conf.fatal ("%s must be defined in env by the plugin %s"%(var,itvlib_dir))
	if isinstance (conf.env.IBEX_INTERVAL_LIB_INCLUDES, list):
		l = [ "#include \"%s\"" % s for s in conf.env.IBEX_INTERVAL_LIB_INCLUDES ]
		conf.env.IBEX_INTERVAL_LIB_INCLUDES  = os.linesep.join (l)

	# Add info on the interval library used to the settings
	conf.setting_define("INTERVAL_LIB", conf.env["INTERVAL_LIB"])

	# Configure LP library
	conf.lp_lib ()

	# recurse
	Logs.pprint ("BLUE", "Configuration of the plugins")
	conf.options.WITH_SOLVER = True
	Logs.pprint  ("YELLOW", "Note: IbexSolve automatically installed.")
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
	conf.env.include_key += [ os.path.basename(h) for h in conf.env.IBEX_HDR ]
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
				PREFIX = ibexutils.escape_backslash_on_win32 (bld.env.PREFIX),
				INCDIR = bld.path_pc_prefix (bld.env.INCDIR),
				LIBDIR = bld.path_pc_prefix (bld.env.LIBDIR),
				INCLUDES = " ".join(["-I" + bld.path_pc (i)
										for i in bld.env.INCLUDES_IBEX_DEPS]),
				CXXFLAGS = " ".join(bld.env.CXXFLAGS_IBEX_DEPS),
				LIBPATH = " ".join(["-L" + bld.path_pc (i)
										for i in bld.env.LIBPATH_IBEX_DEPS]),
				LIBS = " ".join(["-l" + l for l in bld.env.LIB_IBEX_DEPS])
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
	files_patterns += " plugins/wscript plugins/interval_lib_gaol/"
	files_patterns += " plugins/lp_lib_none/"
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
