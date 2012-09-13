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
	opt.load ("compiler_cxx compiler_cc")

	opt.add_option ("--with-gaol",   action="store", type="string", dest="GAOL_PATH",
			help = "location of the Gaol lib")
	opt.add_option ("--with-bias",   action="store", type="string", dest="BIAS_PATH",
			help = "location of the Profil/Bias lib")
	opt.add_option ("--with-soplex", action="store", type="string", dest="SOPLEX_PATH",
			help = "location of the Soplex lib")
	opt.add_option ("--with-java", action="store", type="string", dest="JVM_PATH",
			help = "location of the Java Virtual Machine")


		

def configure (conf):
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
			conf.env["INTERVAL_LIB"] = "gaol"
	
	# then try with the Profil/Bias lib
	if not use_gaol:
		path = candidate_lib_path ("BIAS_PATH", "Profil-")
		if path:
			use_bias = True
			conf.env["INTERVAL_LIB"] = "bias"
	
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
	jvm_path = conf.options.JVM_PATH
	if not jvm_path:
		jvm_path = "/usr/lib/jvm/java-6-sun"
	conf.check_cxx (header_name	= "jni.h",
			includes	= [	os.path.join (jvm_path, "include"),
						os.path.join (jvm_path, "include", "linux")], # FIXME
			uselib_store	= "jni")
	
	conf.find_program (os.path.join (jvm_path, "bin", "javac"), var = "JAVAC")
	conf.find_program (os.path.join (jvm_path, "bin", "javah"), var = "JAVAH")

	conf.env.append_unique ("BISONFLAGS", ["--name-prefix=ibex", "--report=all", "--file-prefix=parser"])
	conf.env.append_unique ("FLEXFLAGS", "-Pibex")

def build (bld):
	bld.recurse ("src examples")
