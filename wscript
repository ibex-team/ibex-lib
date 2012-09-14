#! /usr/bin/env python
# encoding: utf-8

import datetime, os

from distutils.version import LooseVersion

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
	env = conf.env
	conf.load ('compiler_cxx compiler_cc flex bison')

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

	# check which interval lib is requested
	use_gaol = conf.options.GAOL_PATH != None
	use_bias = conf.options.BIAS_PATH != None
	path     = None

	if use_gaol and use_bias:
		conf.fatal ("cannot use --with-gaol and --with-bias together")

	# try with the Gaol lib first
	if not use_bias:
		#TODO: test it
		path = candidate_lib_path ("GAOL_PATH", "gaol-")
		if path:
			use_gaol = True
			env["INTERVAL_LIB"] = "gaol"
	
	# then try with the Profil/Bias lib
	if not use_gaol:
		path = candidate_lib_path ("BIAS_PATH", "Profil-")
		if path:
			use_bias = True
			env["INTERVAL_LIB"] = "bias"
	
	if not (use_gaol or use_bias):
		conf.fatal ("cannot find any interval library, please use --with-bias=BIAS_PATH or --with-gaol=GAOL_PATH")

	conf.msg ("Candidate interval lib", path)

	conf.check_cxx (header_name	= "BIAS/BiasF.h",
			includes	= os.path.join (path, "include"),
			uselib_store	= "interval")

	conf.check_cxx (lib		= ["Profil", "Bias", "lr"],
			libpath		= os.path.join (path, "lib"),
			uselib_store	= "interval")

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

def build (bld):
	bld.recurse ("src examples")
