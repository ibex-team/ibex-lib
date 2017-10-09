#!/usr/bin/env python
# encoding: utf-8

import os
import waf_java_utils

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-jni", action="store_true", dest="WITH_JNI",
			help = "enable the compilation of the JNI adapter (note: your JAVA_HOME environment variable must be properly set if you want to use this option)")

	grp = opt.add_option_group ("Options for the plugin JNI/Java")
	grp.add_option ("--java-package-name", action="store", type="string",
			dest="JAVA_PACKAGE", default="ibex",
			help="name of the java package to be build (default is ibex)")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_JNI = conf.options.WITH_JNI

	conf.start_msg ("JNI plugin")
	if not conf.env.WITH_JNI:
		conf.end_msg ("not used")
		return

	conf.end_msg ("enabled")

	if not conf.env.ENABLE_SHARED:
		conf.fatal ("To install the Java plugin, you must set the option --enable-shared")

	if not conf.options.WITH_SOLVER:
		conf.fatal ("The Java plugin requires IbexSolve (--with-solver)")

	# Load java tools, detect binaries and jni headers
	conf.load ("waf_java_utils")

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

######################
####### build ########
######################
def build (bld):
	if not bld.env.WITH_JNI:
			return

	# Generate src/* from src/*.*.in (need bld.env.JAVA_PACKAGE and
	# bld.env.JAVA_SIGNATURE)
	for f in bld.path.ant_glob ("src/**/*.in"):
		bld (
			features = "subst",
			source = f,
			target = f.change_ext ("", ".in"),
		)

	# Set CLASSPATH for JAVA
	java_bld = bld.path.get_bld()
	bld.env.append_unique ("CLASSPATH_JAVA", java_bld.abspath())
	bld.env.append_unique ("CLASSPATH_JAVA", java_bld.make_node("src").abspath())

	# Compile src/Ibex.java into Ibex.class with javac
	path = bld.env.JAVA_PACKAGE.replace (".", os.path.sep)
	java_ibex_class = os.path.join (path, "Ibex.class")
	bld (
		features = "myjavac",
		source = "src/Ibex.java",
		target = java_ibex_class,
		use = ["JAVA"],
	)

	# Generate C++ header with javah
	java_ibex_header = "src/%s_Ibex.h" % bld.env.JAVA_SIGNATURE
	bld (
		features = "myjavah",
		source = java_ibex_class,
		target = java_ibex_header,
		use = ["JAVA"],
	)

	# Generate jar
	bld (
		features = "myjar",
		source = java_ibex_class,
		target = bld.env.JAVA_PACKAGE + ".jar",
		install_path = bld.env.JAVA_INSTALLDIR,
	)

	# Generate shared library
	if bld.env.INSTALL_3RD:
		rpath = bld.env.LIBDIR + ":" + bld.env.LIBDIR_3RD
	else:
		rpath = bld.env.LIBDIR
	bld.shlib (
		target = "ibex-java",
		source = "src/ibex_Java.cpp",
		use = [ "JAVA", "ibex", java_ibex_header ],
		rpath = rpath,
		install_path = bld.env.LIBDIR,
	)

######################
####### utest ########
######################
def utest (tst):
	if not tst.env.WITH_JNI:
			return

	# Generate tests/* from tests/*.in (need bld.env.JAVA_PACKAGE)
	for f in tst.path.ant_glob ("tests/**/*.in"):
		tst (
			features = "subst",
			source = f,
			target = f.change_ext ("", ".in"),
		)

	# Add to LD_LIBRARY_PATH the path to the install directory of libs
	tst.env.append_unique ("LD_LIBRARY_PATH_JAVA_TESTS", tst.env.LIBDIR)

	# Add to CLASSPATH the path to the needed jar archive
	ibexjar = os.path.join (tst.env.JAVA_INSTALLDIR, tst.env.JAVA_PACKAGE + ".jar")
	tst.env.append_unique ("CLASSPATH_JAVA_TESTS", ibexjar)
	java_tst = tst.path.find_node ("tests").abspath()
	for jar in [ "junit-4.11.jar", "hamcrest-core-1.3.jar" ]:
		tst.env.append_unique ("CLASSPATH_JAVA_TESTS", os.path.join (java_tst, jar))

	# Compile src/Test.java into Test.class with javac and run the test
	tst (
		features = "myjavac javatest",
		source = "tests/Test.java",
		target = "tests/Test.class",
		outdir = "tests",
		use = ["JAVA_TESTS"],
	)

	# Compile src/IbexTest.java into IbexTest.class with javac and run the test
	tst (
		features = "myjavac javatest",
		source = "tests/IbexTest.java",
		target = "tests/IbexTest.class",
		outdir = "tests",
		use = ["JAVA_TESTS"],
		classname_extra = "org.junit.runner.JUnitCore"
	)
