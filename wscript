#! /usr/bin/env python
# encoding: utf-8

import os, sys
from distutils.version import LooseVersion
from waflib import Scripting, Logs
from waflib.Build import BuildContext

# The following variable is used to build ibex.pc and by "waf dist"
VERSION="2.3.0"
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
	if list_of_interval_lib_plugin == []: # this will raise a error at configure
		default_interval_lib = None
	elif "gaol" in list_of_interval_lib_plugin:
		default_interval_lib = "gaol"
	elif any(lib.startswith("gaol") for lib in list_of_interval_lib_plugin):
		L = [ lib for lib in list_of_interval_lib_plugin if lib.startswith("gaol") ]
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

	# recurse on plugins directory
	opt.recurse("plugins")

######################
##### configure ######
######################
def configure (conf):
	conf.load ('compiler_cxx compiler_c bison flex')

	conf.prepare_env(conf.env)

	# Set LIBDIR and INCDIR, set them in env and put them in *_IBEX_DEPS
	conf.env.LIBDIR = os.path.join (conf.env.PREFIX, "lib")
	conf.env.append_unique ("LIBPATH_IBEX_DEPS", conf.env.LIBDIR)
	conf.env.INCDIR = os.path.join (conf.env.PREFIX, "include")
	conf.env.INCDIR_HDR = os.path.join (conf.env.INCDIR, "ibex")
	conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR)
	conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR_HDR)
	conf.env.INCDIR_3RD = os.path.join (conf.env.INCDIR_HDR, "3rd")
	conf.env.LIBDIR_3RD = os.path.join (conf.env.LIBDIR, "ibex", "3rd")

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
		flags = "-O0 -g -pg -Wall -Wno-unknown-pragmas -Wno-unused-variable"
		flags += " -fmessage-length=0"
		conf.define ("DEBUG", 1)
	else:
		flags = "-O3"
		conf.define ("NDEBUG", 1)
	for f in flags.split():
		conf.check_cxx(cxxflags=f, use="IBEX", mandatory=False, uselib_store="IBEX")

	# Build as shared lib is asked
	if conf.options.ENABLE_SHARED:
		conf.env.ENABLE_SHARED = True

	# Bison / Flex
	conf.env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	conf.env.append_unique ("FLEXFLAGS", "-Pibex")

	# recurse on the interval library directory
	if conf.options.INTERVAL_LIB is None:
		conf.fatal ("No interval library is available.")
	conf.msg ("Library for interval arithmetic", conf.options.INTERVAL_LIB)
	itvlib_dir = ITVLIB_PLUGIN_PREFIX + conf.options.INTERVAL_LIB
	conf.recurse (os.path.join("plugins", itvlib_dir))
	# Copy in _IBEX_DEPS some important variables from _ITV_LIB
	conf.env.append_unique ("CXXFLAGS_IBEX_DEPS", conf.env.CXXFLAGS_ITV_LIB)
	conf.env.append_unique ("LIB_IBEX_DEPS", conf.env.LIB_ITV_LIB)

	# Add info on the interval library used to the settings
	conf.setting_define("WITH_" + conf.env["INTERVAL_LIB"], 1)
	conf.setting_define("INTERVAL_LIB", conf.env["INTERVAL_LIB"])

	# recurse
	conf.recurse ("plugins src tests")

	# If we used a 3rd party library, add the install path (for *.pc file)
	if conf.env.INSTALL_3RD: # It may not be necessary but it costs nothing
		conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR_3RD)
		conf.env.append_unique ("LIBPATH_IBEX_DEPS", conf.env.LIBDIR_3RD)

	# Generate header file containing Ibex settings
	conf.env.ibex_header_setting = "ibex_Setting.h"
	conf.write_setting_header (configfile = conf.env.ibex_header_setting,
		top = True, guard = "__IBEX_SETTING_H__")

	# Generate the main Ibex header which includes all the others headers
	conf.env.ibex_header = "ibex.h"
	conf.env.include_key = [ conf.env.ibex_header_setting ]
	conf.env.include_key += [ os.path.basename(h) for h in conf.env.IBEX_HDR ]
	conf.write_config_header (conf.env.ibex_header, defines = False, top = True,
			headers = True, guard = "__IBEX_H__", remove = False)
	del conf.env.include_key

######################
####### build ########
######################
def build (bld):
	if bld.cmd == "clean":
		# We only remove 'src' directory in the build tree. If we let waf take care
		# of the clean process, it would remove also the '3rd' directory. We do not
		# want that because it contains libraries generated during configure. We
		# also need to keep ibex_Setting.h and ibex.h in the 'src' directory.
		keep_patterns = ""
		clean_patterns = "src/** tests/**"
		for f in bld.bldnode.ant_glob(clean_patterns, excl=keep_patterns, dir=True):
			f.delete()
		sys.exit (0) # We exit now or else waf call its internal 'clean' function
	else:
		bld.recurse ("plugins src")

		# Generate ibex.pc, the pkg-config file
		bld (features = "subst", source = "ibex.pc.in", target = "ibex.pc",
				install_path = "${PREFIX}/share/pkgconfig",
				INCDIR = bld.env.INCDIR.replace(bld.env.PREFIX, "${prefix}"),
				LIBDIR = bld.env.LIBDIR.replace(bld.env.PREFIX, "${prefix}"),
				INCLUDES = " ".join(["-I" + i.replace (bld.env.INCDIR, "${includedir}")
										for i in bld.env.INCLUDES_IBEX_DEPS]),
				CXXFLAGS = " ".join(bld.env.CXXFLAGS_IBEX_DEPS),
				LIBPATH = " ".join(["-L" + i.replace (bld.env.LIBDIR, "${libdir}")
										for i in bld.env.LIBPATH_IBEX_DEPS]),
				LIBS = " ".join(["-l" + l for l in bld.env.LIB_IBEX_DEPS])
		)

		# install ibex main header and header with settings
		bld.install_files (bld.env.INCDIR, bld.env.ibex_header)
		bld.install_files (bld.env.INCDIR_HDR, bld.env.ibex_header_setting)

		if bld.env.INSTALL_3RD:
			incnode = bld.bldnode.find_node("3rd").find_node("include")
			incfiles = incnode.ant_glob ("**")
			bld.install_files (bld.env.INCDIR_3RD, incfiles, cwd = incnode,
				relative_trick = True)

			libnode = bld.bldnode.find_node("3rd").find_node("lib")
			libfiles = libnode.ant_glob ("**")
			bld.install_files (bld.env.LIBDIR_3RD, libfiles, cwd = libnode,
				relative_trick = True)

######################
####### dist #########
######################
def dist (ctx):
	files_patterns = "wscript benchs/** src/** examples/** waf"
	files_patterns += " COPYING.LESSER LICENSE ibexutils.py"
	files_patterns += " plugins/wscript plugins/interval_lib_gaol/"
	ctx.files = ctx.path.ant_glob(files_patterns)

######################
####### utest ########
######################
def utest (tst):
	'''run the unitary tests'''
	logfile = os.path.join (tst.bldnode.abspath(), "utest_config.log")
	tst.logger = Logs.make_logger (logfile, "utest_config")
	tst.recurse ("tests")
	Logs.free_logger (tst.logger)
	tst.logger = None

class UTestContext (BuildContext):
	cmd = "utest"
	fun = "utest"
