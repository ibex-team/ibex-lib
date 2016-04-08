#! /usr/bin/env python
# encoding: utf-8

import os, tarfile #, functools

from waflib import Logs

def options (opt):
	opt.add_option ("--with-jni", action="store_true", dest="WITH_JNI",
			help = "enable the compilation of the JNI adapter (note: your JAVA_HOME environment variable must be properly set if you want to use this option)")

	opt.add_option ("--with-java-package", action="store", type="string", dest="JAVA_PACKAGE",
			default="ibex", help="name of the java package to be build (default is ibex)")


def configure (conf):
	conf.env.WITH_JNI = conf.options.WITH_JNI
	
	if not conf.env.WITH_JNI: 
		return
	
	if not conf.env.ENABLE_SHARED:
		conf.fatal ("To install the Java plugin, you must set the option --enable-shared")	
    	
	Logs.pprint ("BLUE", "Configure the java plugin")
	
	java_home = os.environ.get("JAVA_HOME")
	
	if java_home:
		conf.env["JAVA_HOME"] = [java_home]
	
	conf.load ('javaw', funs = [])

	conf.check_jni_headers()

	conf.msg ("Checking for java sdk", java_home)
	del conf.env["JAVAC"]
	conf.find_program (os.path.join (java_home, "bin", "javac"), var = "JAVAC")
	conf.find_program (os.path.join (java_home, "bin", "javah"), var = "JAVAH")
	conf.find_program (os.path.join (java_home, "bin", "jar"),   var = "JAR")

	conf.env.JAVA_PACKAGE = conf.options.JAVA_PACKAGE

	if conf.env.DEST_OS == "win32":
		# fix name-mangling for linking with the JVM on windows
		#   http://permalink.gmane.org/gmane.comp.gnu.mingw.user/6782
		#   http://stackoverflow.com/questions/8063842/mingw32-g-and-stdcall-suffix1
		conf.env.append_unique ("LINKFLAGS_IBEX", "-Wl,--kill-at")

	JAVA_SIGNATURE = conf.env.JAVA_PACKAGE.replace (".", "_")
	fin_cpp=open(os.path.join(conf.path.abspath(),"src","ibex_Java.cpp_"), 'r')
	fout=open(os.path.join(conf.path.abspath(),"src","ibex_Java.cpp"), 'w')
	fout.write(fin_cpp.read().replace ("Java_", "Java_%s_" % JAVA_SIGNATURE))

#	conf.env.append_unique ("INCLUDES_IBEX_DEPS", os.path.join(java_home,"include"))

def build (bld):

	if not bld.env.WITH_JNI:
			return
	
	Logs.pprint ("BLUE", "Build the java plugin")
				
	JAVADIR        = "${PREFIX}/share/java"
	JAVA_PACKAGE   = bld.env.JAVA_PACKAGE
	JAVA_SIGNATURE = JAVA_PACKAGE.replace (".", "_")
	JAVA_PATH      = JAVA_PACKAGE.replace (".", "/")
	
	bld.env.GLOBAL_DEPS = bld.env.GLOBAL_DEPS + " JAVA"
	
#	@bld.rule (
#		target = "src/ibex_Java.cpp",
#		source = "src/ibex_Java.cpp_",
#		vars = ["JAVA_PACKAGE"],
#	)
#	def _(tsk):
#		tsk.outputs[0].write (
#			"// This file is generated from %s.\n"
#			'#include "%s_Ibex.h_"\n%s'
#			% (tsk.inputs[0].name, JAVA_SIGNATURE, 
#			tsk.inputs[0].read().replace ("Java_", "Java_%s_" % JAVA_SIGNATURE)
#		))


	#add plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/ibex_Java.cpp"))

	for (name, snippet) in (
		("Ibex", "package %s;\n" % JAVA_PACKAGE),
		("Test", "import %s.Ibex;\n" % JAVA_PACKAGE),
	):
		@bld.rule (
		target = "src/%s.java" % name,
		source = "src/%s.java_" % name,
		vars = ["JAVA_PACKAGE"],
		snippet = snippet,
		)
		def _(tsk):
			tsk.outputs[0].write (
				"// This file is generated from %s.\n"
				"%s\n%s"
				% (tsk.inputs[0].name, tsk.generator.snippet, tsk.inputs[0].read()))

	bld (
		target = "src/%s/Ibex.class" % JAVA_PATH,
		source = "src/Ibex.java",
		rule   = "${JAVAC} -d plugins/java/src ${SRC}"
	)

	bld (
		target = "src/Test.class",
		source = "src/Test.java",
		rule   = "${JAVAC} -d plugins/java/src -cp plugins/java/src ${SRC}",
		after  = "src/%s/Ibex.class" % JAVA_PATH,
	)

	bld (
		target = "src/%s_Ibex.h_" % JAVA_SIGNATURE,
		source = "src/%s/Ibex.class" % JAVA_PATH,
		rule   = "${JAVAH} -jni -classpath plugins/java/src -o ${TGT} %s.Ibex" % JAVA_PACKAGE
	)

	bld (
		target = "%s.jar" % JAVA_PACKAGE,
		source = "src/%s/Ibex.class" % JAVA_PATH,
		rule = "${JAR} cf ${TGT} plugins/java/src/%s/Ibex.class" % JAVA_PATH,
		install_path = JAVADIR,
	)


    # --- cannot assure compilation order with "recurse" (libibex.so has to be created before
    #     libibex-java.so)
	#bld.shlib (
	#	target = "ibex-java",
	#	source = "src/ibex_Java.cpp",
	#	use = "JAVA ibex",
	#	install_path = bld.env.LIBDIR,
	#)

