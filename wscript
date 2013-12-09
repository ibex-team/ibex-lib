#! /usr/bin/env python
# encoding: utf-8

import os, shutil, re

from distutils.version import LooseVersion
from waflib import Logs, Scripting

# the following two variables are used by the target "waf dist"
VERSION="2.0.0alpha"
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

	opt.load ("compiler_cxx compiler_cc javaw")

	opt.add_option ("--enable-shared", action="store_true", dest="ENABLE_SHARED",
			help = "build ibex as a shared lib")

	opt.add_option ("--disable-sse2", action="store_true", dest="DISABLE_SSE2",
			help = "do not use SSE2 optimisations")

	opt.add_option ("--with-debug",  action="store_true", dest="DEBUG",
			help = "enable debugging")

	opt.add_option ("--with-gaol",   action="store", type="string", dest="GAOL_PATH",
			help = "location of the Gaol lib")
	opt.add_option ("--with-bias",   action="store", type="string", dest="BIAS_PATH",
			help = "location of the Profil/Bias lib")
	opt.add_option ("--with-filib",   action="store", type="string", dest="FILIB_PATH",
			help = "location of the filib lib")
	opt.add_option ("--with-soplex", action="store", type="string", dest="SOPLEX_PATH",
			help = "location of the Soplex lib")
	opt.add_option ("--with-cplex", action="store", type="string", dest="CPLEX_PATH",
			help = "location of the Cplex lib")
	opt.add_option ("--with-ampl",   action="store", type="string", dest="AMPL_PATH",
			help = "location of the amplsolver lib")

	opt.add_option ("--with-jni", action="store_true", dest="WITH_JNI",
			help = "enable the compilation of the JNI adapter (note: your JAVA_HOME environment variable must be properly set if you want to use this option)")
	opt.add_option ("--with-java-package", action="store", type="string", dest="JAVA_PACKAGE",
			default="ibex", help="name of the java package to be build (default is ibex)")


def configure (conf):

	def switch_to_32bits():
		env = conf.env
		if env.DEST_CPU == "x86_64":
			if env.COMPILER_CC == "gcc" and env.COMPILER_CXX == "g++":
				# fall-back to 32bits
				Logs.pprint ("YELLOW", "Warning: x86_64 is not supported, will build for i386 instead")
				
				for var in ("CFLAGS", "LINKFLAGS", "CXXFLAGS"):
					env.append_unique (var, "-m32")

				conf.check_cc (cflags = "-m32")
				conf.check_cxx (cxxflags = "-m32")
			else:
				conf.fatal ("64bit platform are not supported")

	conf.switch_to_32bits = switch_to_32bits


	env = conf.env
	conf.load ('compiler_cxx compiler_cc bison')
	conf.load ('flex', '.')


	env.VERSION = VERSION

	# GAOL cannot be built on 64-bit cpu
	if conf.options.GAOL_PATH is not None:
		switch_to_32bits()

	# optimised compilation flags
	if conf.options.DEBUG:
		flags = "-O0 -g -pg -Wall -Wno-unknown-pragmas -Wno-unused-variable -fmessage-length=0"
	else:
		flags = "-O3 -Wno-deprecated"
		conf.define ("NDEBUG", 1)
	for f in flags.split():
		if conf.check_cxx (cxxflags = f, mandatory = False):
			env.append_unique ("CXXFLAGS", f)

	# build as shared lib
	if conf.options.ENABLE_SHARED or conf.options.WITH_JNI:
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

	path_so = candidate_lib_path ("SOPLEX_PATH", "soplex-")
	path_c = candidate_lib_path ("CPLEX_PATH", "cplex")
	
	# Soplex lib
	if path_so:
		conf.msg ("Candidate directory for lib Soplex", path_so)

		env.append_unique ("INCLUDES",  os.path.join (path_so, "src"))

		conf.check_cxx (header_name	= "soplex.h")

		# Try without and with -lz (soplex may be built without zlib)
		for l in ("soplex", ["soplex", "z"]):
			if (conf.check_cxx (lib = l, uselib_store = "IBEX_DEPS",
					libpath = [os.path.join (path_so, "lib")],
					mandatory = False,
					fragment = """
						#include <soplex.h>
						int main (int argc, char* argv[]) {
							soplex::SPxLP lp;
							lp.read(std::cin);
							return 0;
						}
					""")):
				break
		else:
			conf.fatal ("cannot link with the Soplex library")
			
	# CPLEX lib
	elif path_c:
		conf.msg ("Candidate directory for lib Cplex", path_c)

		env.append_unique ("INCLUDES",  os.path.join (path_c, "cplex/include/"))
		conf.check_cxx (header_name	= "ilcplex/cplex.h")
		
		# FIXME ATTENTION  IL faut adapter en fonction des differentes configurations (32/64, linux, mac, win)
		if (not(conf.check_cxx (lib = ["cplex", "pthread"], uselib_store = "IBEX_DEPS",
				libpath = [os.path.join (path_c, "cplex/lib/x86-64_sles10_4.1/static_pic")],  # FIXME changer le chemin
				mandatory = False,
				fragment = """
					#include "ilcplex/cplex.h"
					int main (int argc, char* argv[]) {
						CPXENVptr  envcplex;
						CPXLPptr lpcplex;
						return 0;
					}
				"""))):
			conf.fatal ("cannot link with the Cplex library")			
	else:
		conf.fatal ("cannot find the Soplex library or the Cplex library,  please use --with-soplex=SOPLEX_PATH or --with-cplex=CPLEX_PATH")
	

	# JNI
	env.WITH_JNI = conf.options.WITH_JNI
	if env.WITH_JNI:
		java_home = os.environ.get("JAVA_HOME")
		if java_home:
			env["JAVA_HOME"] = [java_home]
		
		conf.load ('javaw', funs = [])

		conf.check_jni_headers()

		conf.msg ("Checking for java sdk", java_home)
		del env["JAVAC"]
		conf.find_program (os.path.join (java_home, "bin", "javac"), var = "JAVAC")
		conf.find_program (os.path.join (java_home, "bin", "javah"), var = "JAVAH")
		conf.find_program (os.path.join (java_home, "bin", "jar"),   var = "JAR")

		conf.env.JAVA_PACKAGE = conf.options.JAVA_PACKAGE

		if env.DEST_OS == "win32":
			# fix name-mangling for linking with the JVM on windows
			#   http://permalink.gmane.org/gmane.comp.gnu.mingw.user/6782
			#   http://stackoverflow.com/questions/8063842/mingw32-g-and-stdcall-suffix1
			env.append_unique ("LINKFLAGS_JAVA", "-Wl,--kill-at")
			

	# Bison / Flex
	env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	env.append_unique ("FLEXFLAGS", "-Pibex")

	conf.env.append_unique ("LIBPATH", ["3rd", "src"])
	conf.recurse ("3rd src")
    
def build (bld):
	bld.recurse ("src examples 3rd")

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
		
