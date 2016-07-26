#!/usr/bin/env python
# encoding: utf-8

import os

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-jni", action="store_true", dest="WITH_JNI",
			help = "enable the compilation of the JNI adapter (note: your JAVA_HOME environment variable must be properly set if you want to use this option)")
	opt.add_option ("--with-java-package", action="store", type="string",
			dest="JAVA_PACKAGE", default="ibex",
			help="name of the java package to be build (default is ibex)")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_JNI = conf.options.WITH_JNI
	
	if not conf.env.WITH_JNI: 
		return
	conf.start_msg ("JNI plugin")
	if not conf.env.WITH_JNI:
		conf.end_msg ("not used")
		return
	
	conf.end_msg ("enabled")

	if not conf.env.ENABLE_SHARED:
		conf.fatal ("To install the Java plugin, you must set the option --enable-shared")	
    	
	# Load java tools and detect binaries and headers
	conf.load ("javaw")
	pathdir = os.path.dirname(conf.env.JAVAC[0])
	conf.find_program ("javah", var = "JAVAH", path_list = pathdir)
	conf.check_jni_headers()

	# Set java package name
	conf.env.JAVA_PACKAGE = conf.options.JAVA_PACKAGE

	if conf.env.DEST_OS == "win32":
		# fix name-mangling for linking with the JVM on windows
		#   http://permalink.gmane.org/gmane.comp.gnu.mingw.user/6782
		#   http://stackoverflow.com/questions/8063842/mingw32-g-and-stdcall-suffix1
		conf.env.append_unique ("LINKFLAGS_JAVA", "-Wl,--kill-at")

	# Set important env variables about Java build
	conf.env.JAVA_INSTALLDIR = os.path.join (conf.env.PREFIX, "share", "java")
	conf.env.JAVA_SIGNATURE = conf.env.JAVA_PACKAGE.replace (".", "_")
	
	# Define rule for compiling with javac
	conf.env.JAVAC_RULE = "${JAVAC} -cp ${CLASSPATH} -d ${OUTDIR} ${JAVACFLAGS} ${SRC}"

	# Define rule for compiling with jar
	conf.env.JAR_RULE = "${JAR} ${JARCREATE} ${TGT} ${SRC}"
	conf.env.JARCREATE = "cf"

	# Define rule for compiling with javah
	conf.env.JAVAH_RULE = "${JAVAH} -jni -cp ${CLASSPATH} -o ${TGT} ${SRC}"

######################
####### build ########
######################
def build (bld):
	if not bld.env.WITH_JNI:
			return
	
	java_ibex_header = "src/%s_Ibex.h" % bld.env.JAVA_SIGNATURE
	path = bld.env.JAVA_PACKAGE.replace (".", os.path.sep)
	java_ibex_class_path = os.path.join ("src", path, "Ibex.class")

	# Generate src/ibex_Java.cpp from src/ibex_Java.cpp.in (need JAVA_SIGNATURE)
	bld (
		features = "subst",
		source = "src/ibex_Java.cpp.in",
		target = "src/ibex_Java.cpp",
		SIGNATURE = bld.env.JAVA_SIGNATURE
	)

	# Generate src/Ibex.java from src/Ibex.java.in (need bld.env.JAVA_PACKAGE)
	bld (
		features = "subst",
		source = "src/Ibex.java.in",
		target = "src/Ibex.java",
		PACKAGE = bld.env.JAVA_PACKAGE
	)
	
	bld.env.CLASSPATH = bld.path.make_node("src").get_bld().abspath()
	bld.env.OUTDIR = bld.env.CLASSPATH

	# Compile src/Ibex.java into Ibex.class with javac
	bld (
		source = "src/Ibex.java",
		target = java_ibex_class_path,
		rule   = bld.env.JAVAC_RULE,
	)

	# Generate C++ header with javah
	bld (
		target = java_ibex_header,
		# This is a hack: the real source of the rule is java_ibex_class_path, but
		# I do not know to do the conversion in the rule from ${SRC} to the class
		# name needed by javah. So i use after to register the dependency and
		# replace directly ${SRC} in the rule by the correct class name.
		after = java_ibex_class_path,
		rule = bld.env.JAVAH_RULE.replace("${SRC}", bld.env.JAVA_PACKAGE + ".Ibex")
	)

	# Generate jar
	bld (
		source = java_ibex_class_path,
		target = bld.env.JAVA_PACKAGE + ".jar",
		rule = bld.env.JAR_RULE,
		install_path = bld.env.JAVA_INSTALLDIR,
	)

	# Generate shared library
	bld.shlib (
		target = "ibex-java",
		source = "src/ibex_Java.cpp",
		use = [ "JAVA", "ibex", java_ibex_header ],
		install_path = bld.env.LIBDIR,
	)
