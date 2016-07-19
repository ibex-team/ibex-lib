#! /usr/bin/env python
# encoding: utf-8

import os, sys
from distutils.version import LooseVersion
from waflib import Scripting

# the following two variables are used by the target "waf dist"
VERSION="2.3.0"
APPNAME='ibex-lib'

top = '.'
out = '__build__'

ITVLIB_PLUGIN_PREFIX = "interval_lib_"
PLUGINS_DIR = "plugins"

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
	plugin_node = opt.path.make_node(PLUGINS_DIR)
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

	# help string for --with-interval-lib command line option
	help_string = "Possible values are: " + ", ".join(list_of_interval_lib_plugin)
	help_string += " (default value is " + str(default_interval_lib) + ")"

	# add the option --with-interval-lib
	opt.add_option ("--with-interval-lib", action="store", dest="INTERVAL_LIB",
                  choices = list_of_interval_lib_plugin,
									default = default_interval_lib, help = help_string)

	# recurse on plugins directory
	opt.recurse(PLUGINS_DIR)

######################
##### configure ######
######################
def configure (conf):
	conf.load ('compiler_cxx compiler_c bison flex')
	
	conf.prepare_env(conf.env)

	# Set LIBDIR and INCDIR, set them in env and put them in *_IBEX_DEPS
	libdir = os.path.join (conf.env.PREFIX, "lib")
	conf.env.LIBDIR = os.path.join (libdir, "ibex")
	conf.env.append_unique ("LIBPATH_IBEX_DEPS", conf.env.LIBDIR)
	includedir = os.path.join (conf.env.PREFIX, "include")
	conf.env.INCDIR = os.path.join (includedir, "ibex")
	conf.env.append_unique ("INCLUDES_IBEX_DEPS", conf.env.INCDIR)
	conf.env.INCDIR_3RD = os.path.join (conf.env.INCDIR, "3rd")
	conf.env.LIBDIR_3RD = os.path.join (conf.env.LIBDIR, "3rd")

	# Did we used 3rd party software ? If true we need to install them.
	conf.env.INSTALL_3RD = False

	## TODO (or already done by prepare_env ?)
	# conf.add_os_flags(CXXFLAGS) and for other environment variables

	# set opt.env.PLUGINS_DIR (or the node ??? to "plugins")
	conf.env.ITVLIB_PLUGIN_PREFIX = ITVLIB_PLUGIN_PREFIX
	# set opt.env.BUILD_DIR

	# put useful variables in conf.env
	conf.env.VERSION = VERSION
	conf.env.APPNAME = APPNAME
	conf.env.ITVLIB_PLUGIN_PREFIX = ITVLIB_PLUGIN_PREFIX

	# optimised compilation flags
	if conf.options.DEBUG:
		Logs.info("Enabling debug mode")
		flags = "-O0 -g -pg -Wall -Wno-unknown-pragmas -Wno-unused-variable"
		flags += " -fmessage-length=0"
		conf.define ("DEBUG", 1)
	else:
		flags = "-O3"
		conf.define ("NDEBUG", 1)
	for f in flags.split():
		if conf.check_cxx (cxxflags = f, mandatory = False):
			conf.env.append_unique ("CXXFLAGS", f)

	# build as shared lib
	if conf.options.ENABLE_SHARED:
		conf.env.ENABLE_SHARED = True

	# Bison / Flex
	conf.env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	conf.env.append_unique ("FLEXFLAGS", "-Pibex")

	# add src to libpath
	conf.env.append_unique ("LIBPATH", ["src"])

	# set settings to the empty dict (everything put in this dict will be written
	# in the settings header)
	conf.env.settings = {}

	# Add release version to the settings
	conf.env.settings['_IBEX_RELEASE_'] = "\""+conf.env['VERSION']+"\""

	# recurse on the interval library directory
	if conf.options.INTERVAL_LIB is None:
		conf.fatal ("No interval library is available.")
	conf.msg ("Library for interval arithmetic", conf.options.INTERVAL_LIB)
	itvlib_dir = ITVLIB_PLUGIN_PREFIX + conf.options.INTERVAL_LIB
	conf.recurse (os.path.join(PLUGINS_DIR, itvlib_dir))

	# Add info on the interval library used to the settings
	conf.env.settings['_IBEX_WITH_%s_' % conf.env['INTERVAL_LIB']] = "1"
	conf.env.settings['_INTERVAL_LIB_'] = "\""+conf.env['INTERVAL_LIB']+"\""

	# recurse
	conf.recurse ("plugins src")

	conf.env.prepend_value ("LIB_IBEX_DEPS", "ibex")

######################
####### build ########
######################
def build (bld):
	if bld.cmd == "clean":
		# We only remove 'src' directory in the build tree. If we let waf take care
		# of the clean process, it would remove also the '3rd' directory. We do not
		# want that because it contains libraries generated during configure. We
		# also need to keep ibex_Setting.h and ibex.h in the 'src' directory.
		keep_patterns = "src/ibex_Setting.h src/ibex.h"
		for f in bld.bldnode.ant_glob("src/**", excl=keep_patterns, dir=True):
			f.delete()
		sys.exit (0) # We exit now or else waf call its internal 'clean' function
	else:
		bld.recurse ("plugins src")

		if bld.env.INSTALL_3RD:
			incnode = bld.bldnode.make_node("3rd").make_node("include")
			incfiles = incnode.ant_glob ("**")
			bld.install_files (bld.env.INCDIR_3RD, incfiles, cwd = incnode,
				relative_trick = True)

			libnode = bld.bldnode.make_node("3rd").make_node("lib")
			libfiles = libnode.ant_glob ("**")
			bld.install_files (bld.env.LIBDIR_3RD, libfiles, cwd = libnode,
				relative_trick = True)

######################
##### distclean  #####
######################
def distclean (ctx):
	Scripting.distclean (ctx) # remove the build directory

######################
####### dist #########
######################
def dist (ctx):
  files_patterns = "wscript benchs/** src/** examples/** waf"
  files_patterns += " COPYING.LESSER LICENSE ibexutils.py"
  files_patterns += " plugins/wscript plugins/interval_lib_gaol/"
  ctx.files = ctx.path.ant_glob(files_patterns)
