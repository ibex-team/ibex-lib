#! /usr/bin/env python
# encoding: utf-8

import datetime, os

from distutils.version import LooseVersion
from waflib import Logs

# the following two variables are used by the target "waf dist"
VERSION=datetime.date.today().strftime ("%Y%m%d")
APPNAME='ibex-lib'

top = '.'
out = '__build__'


def options (opt):
	opt.load ("compiler_cxx compiler_cc javaw")

	opt.add_option ("--with-gaol",   action="store", type="string", dest="GAOL_PATH",
			help = "location of the Gaol lib")
	opt.add_option ("--with-bias",   action="store", type="string", dest="BIAS_PATH",
			help = "location of the Profil/Bias lib")
	opt.add_option ("--with-soplex", action="store", type="string", dest="SOPLEX_PATH",
			help = "location of the Soplex lib")


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
	conf.switch_to_32bits()

	def find_lib (prefix):

		parent_dir = os.path.abspath ("..")

		v = LooseVersion ("0")
		for path in os.listdir (parent_dir):
			if (path.startswith (prefix)):
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

	# Soplex lib
	path = candidate_lib_path ("SOPLEX_PATH", "soplex-")
	if path:
		conf.msg ("Candidate directory for lib Soplex", path)

		conf.check_cxx (header_name	= "soplex.h",
				includes	= os.path.join (path, "src"),
				uselib_store	= "soplex")

		conf.check_cxx (lib		= ["soplex", "z"],
				libpath		= os.path.join (path, "lib"),
				uselib_store	= "soplex")
	else:
		conf.fatal ("cannot find the Soplex library, please use --with-soplex=SOPLEX_PATH")


	# JNI
	java_home = os.environ.get("JAVA_HOME")
	if java_home:
		env["JAVA_HOME"] = [java_home]
	
	conf.load ('javaw', funs = [])

	conf.check_jni_headers (mandatory = False)
	if env["INCLUDES_JAVA"]:
		conf.msg ("Checking for java sdk", java_home)
		del env["JAVAC"]
		conf.find_program (os.path.join (java_home, "bin", "javac"), var = "JAVAC")
		conf.find_program (os.path.join (java_home, "bin", "javah"), var = "JAVAH")
	else:
		conf.msg ("Checking for java sdk", "no (you may need to set JAVA_HOME to detect it properly)", "YELLOW")

	# Bison / Flex
	env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	env.append_unique ("FLEXFLAGS", "-Pibex")

	conf.recurse ("3rd src")

def build (bld):
	bld.recurse ("src examples 3rd")
