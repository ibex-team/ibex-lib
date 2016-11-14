#! /usr/bin/env python
# encoding: utf-8

import os, shutil, re

from distutils.version import LooseVersion
from waflib import Logs, Scripting

# the following two variables are used by the target "waf dist"
VERSION="2.3.0"
APPNAME='ibex-lib'

top = '.'
out = '__build__'


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

	opt.add_option ("--disable-sse2", action="store_true", dest="DISABLE_SSE2",
			help = "do not use SSE2 optimizations")

	opt.add_option ("--with-debug",  action="store_true", dest="DEBUG",
			help = "enable debugging")

	opt.add_option ("--with-gaol",   action="store", type="string", dest="GAOL_PATH",
			help = "location of the Gaol lib")

	opt.add_option ("--with-bias",   action="store", type="string", dest="BIAS_PATH",
			help = "location of the Profil/Bias lib")

	opt.add_option ("--with-filib",   action="store", type="string", dest="FILIB_PATH",
			help = "location of the Filib lib")
	
	opt.add_option ("--without-rounding", action="store_true", dest="WITHOUT_ROUNDING",
			help = "do not use a reliable interval")
	
	opt.add_option ("--standalone", action="store_true", dest="WITH_STANDALONE",
			help = "do not use any external library (excepted standard C++ library)")	
	
	opt.recurse("plugins")

def configure (conf):

	def switch_to_32bits():
		env = conf.env
		if env.DEST_CPU == "x86_64":
			if env.COMPILER_CC == "gcc" and env.COMPILER_CXX == "g++":
				# fall-back to 32bits
				Logs.pprint ("YELLOW", "Warning: x86_64 is not supported with GAOL, we will build IBEX for i386 instead")
				
				for var in ("CFLAGS", "LINKFLAGS", "CXXFLAGS","CXXFLAGS_IBEX_DEPS"):
					env.append_unique (var, "-m32")

				conf.check_cc (cflags = "-m32")
				conf.check_cxx (cxxflags = "-m32")
			else:
				conf.fatal ("64bit platform are not supported")

	conf.switch_to_32bits = switch_to_32bits


	env = conf.env
	conf.load ('compiler_cxx compiler_c bison')
	conf.load ('flex')
	
	conf.env.LIBDIR = conf.env['PREFIX'] + '/lib'

	env.VERSION = VERSION

	# GAOL cannot be built on 64-bit cpu
	if conf.options.GAOL_PATH is not None:
		switch_to_32bits()

	# optimised compilation flags
	if conf.options.DEBUG:
		flags = "-O0 -g -pg -Wall -Wno-unknown-pragmas -Wno-unused-variable -fmessage-length=0 "
		conf.define ("DEBUG", 1)
	else:
		flags = "-O3"
		conf.define ("NDEBUG", 1)
	for f in flags.split():
		if conf.check_cxx (cxxflags = f, mandatory = False):
			env.append_unique ("CXXFLAGS", f)

	# build as shared lib
	if conf.options.ENABLE_SHARED:
		env.ENABLE_SHARED = True

	def find_lib (prefix):

		parent_dir = os.path.abspath ("..")

		v = LooseVersion ("0")
		for path in os.listdir (parent_dir):
			if path.startswith (prefix) and os.path.isdir (os.path.join (parent_dir, path)):
				new_v = LooseVersion (path)
				if new_v > v:
					v = new_v
		
		if v.vstring == "0":
			return None
		else:
			lib = os.path.join (parent_dir, v.vstring)
			return lib

	def candidate_lib_path (opt_name, prefix):

		path = getattr (conf.options, opt_name)

		return os.path.abspath (os.path.expanduser (path)) if path else find_lib (prefix)

	##################################################################################################
	# Disable Rounding interval
	if (conf.options.WITHOUT_ROUNDING):
		conf.env.WITHOUT_ROUNDING =True 
			
	##################################################################################################
	# Disable rounding interval
	if (conf.options.WITH_STANDALONE):
		conf.env.WITHOUT_ROUNDING =True 
								
	##################################################################################################
	# Bison / Flex
	env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	env.append_unique ("FLEXFLAGS", "-Pibex")

	##################################################################################################
	conf.env.append_unique ("LIBPATH", ["3rd", "src"])
	conf.recurse ("3rd src")
	
##################################################################################################
def build (bld):
	bld.recurse ("src 3rd")

def distclean (ctx):
	Scripting.distclean (ctx)
	ctx.recurse ("3rd")

def dist (ctx):
	# do not include 3rd/* subdirectories when building the archive
	get_files_orig = ctx.get_files
	def get_files():
		def is_not_3rd_subdir (node):
			rel_path = node.path_from (ctx.path.find_dir ("3rd"))
			if rel_path.startswith (".."):
				return True
			else:
				# exclude subdirectories in 3rd
				return not os.sep in rel_path

		return list(filter (is_not_3rd_subdir, get_files_orig()))
	ctx.get_files = get_files
		
