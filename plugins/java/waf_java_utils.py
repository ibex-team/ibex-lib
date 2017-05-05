#!/usr/bin/env python
# encoding: utf-8

import os
from waflib import Task, Utils, TaskGen, Node
from waflib.Tools import ccroot

######################
##### configure ######
######################
def configure (conf):
	conf.load ("javaw")
	pathdir = os.path.dirname (conf.env.JAVAC[0])
	conf.find_program ("javah", var = "JAVAH", path_list = pathdir)
	conf.check_jni_headers()

	# Define rule for compiling with javac
	run_str = "${JAVAC} -cp ${CLASSPATH} -d ${OUTDIR} ${JAVACFLAGS} ${SRC}"
	conf.env.JAVAC_RUN_STR = run_str

	# Define rule for compiling with javah (${_CLASS} is computed from ${SRC})
	conf.env.JAVAH_RUN_STR = "${JAVAH} -jni -cp ${CLASSPATH} -o ${TGT} ${_CLASS}"
	
	# Define rule for compiling with jar (${JAROPTS} is computed from ${SRC})
	conf.env.JAR_RUN_STR = "${JAR} ${JARCREATE} ${TGT} ${JAROPTS}"
	if not conf.env.JARCREATE:
		conf.env.JARCREATE = "cf"

######################
####### Utils ########
######################
@TaskGen.feature ("myjavac", "myjavah")
@TaskGen.after_method ("process_source")
def process_use_wrapper (self):
	self.process_use ()

@TaskGen.feature ("myjavac", "myjavah")
@TaskGen.after_method ("process_use")
def propagate_uselib_vars_wrapper (self):
	self.propagate_uselib_vars ()

@TaskGen.feature ("myjavac", "myjavah")
@TaskGen.after_method ("propagate_uselib_vars")
def set_classpath_wrapper (self):
	self.set_classpath ()

######################
####### JAVAH ########
######################
ccroot.USELIB_VARS['myjavah']=set(['CLASSPATH'])

@TaskGen.feature("myjavah")
@TaskGen.before_method("process_rule")
def myjavah_feature (self):
	self.color = "GREEN"
	self.rule = self.env.JAVAH_RUN_STR
	_class = self.source.replace (os.path.sep, ".")
	if _class.endswith (".class"):
		_class = _class[:-6]
	self.env._CLASS = _class

######################
####### JAVAC ########
######################
ccroot.USELIB_VARS['myjavac']=set(['CLASSPATH','JAVACFLAGS'])

@TaskGen.feature("myjavac")
@TaskGen.before_method("process_rule")
def myjavac_feature (self):
	self.color = "GREEN"
	self.rule = self.env.JAVAC_RUN_STR
	outdir = getattr (self, "outdir", None)
	if outdir:
		if not isinstance(outdir, Node.Node):
			outdir = self.path.get_bld().make_node(self.outdir)
	else:
		outdir = self.path.get_bld()
	outdir.mkdir()
	self.env.OUTDIR = outdir.abspath()

######################
######## JAR #########
######################
@TaskGen.feature("myjar")
@TaskGen.before_method("process_rule")
def myjar_feature (self):
	self.color = "YELLOW"
	self.rule = self.env.JAR_RUN_STR
	basedir = getattr (self, "basedir" , None)
	if basedir:
		if not isinstance (self.basedir, Node.Node):
			basedir = self.path.get_bld ().make_node (basedir)
	else:
		basedir = self.path.get_bld()
	if not basedir:
		self.bld.fatal ("Could not find the basedir %r for %r"%(self.basedir, self))

	self.env.JAROPTS = [ "-C", basedir.bldpath () ]
	if isinstance (self.source, list):
		self.env.JAROPTS += self.source
	else:
		self.env.JAROPTS += [ self.source ] 
	
######################
##### Java test ######
######################
ccroot.USELIB_VARS['javatest']=set(["CLASSPATH", "LD_LIBRARY_PATH"])

@TaskGen.feature("javatest")
@TaskGen.after_method("process_source")
def make_test_java (self):
	if len (self.tasks) != 1 or len(self.tasks[0].outputs) != 1:
		self.bld.fatal ("Expecting 1 task with 1 output in %r" % self)
	tgtnode = self.tasks[0].outputs[0]
	cl = os.path.splitext(os.path.basename(tgtnode.abspath()))[0]
	self.name = "utest_java_" + cl
	if hasattr (self, "classname_extra"):
		cl = self.classname_extra + " " + cl
	outnode = tgtnode.parent.make_node (self.name)

	tsk = self.create_task ("java_gen_test", tgt = outnode, classnames = cl,
		classpath_extra = [ tgtnode.parent.abspath(), "." ])

	tsk.set_run_after (self.tasks[0])
	self.link_task = tsk
	self.ut_paths = self.path.abspath()
	self.meths.append("make_test")

class java_gen_test (Task.Task):
	"""
	Generate a java test
	"""
	color = "PINK"
	vars = [ "CLASSPATH", "JAVA", "LD_LIBRARY_PATH" ]

	script_format = "#!/bin/bash\n%s -Djava.library.path=%s -cp %s %s\n"
	script_format += "if [ $? -eq 0 ] ; then\n  echo 'OK (1 test)'\n  exit 0\n"
	script_format += "else\n  echo 'Run: 1 Failures: 1 Errors: 0'\n  exit 1\nfi\n"

	def run (self):
		if isinstance (self.env.JAVA, list):
			java = self.env.JAVA[0]
		else:
			java = self.env.JAVA

		if isinstance (self.env.LD_LIBRARY_PATH, list):
			ldlibpath = ":".join(self.env.LD_LIBRARY_PATH)
		else:
			ldlibpath = self.env.LD_LIBRARY_PATH

		cp = self.env.CLASSPATH + os.pathsep.join(self.classpath_extra) + os.pathsep

		s = self.script_format % (java, ldlibpath, cp, self.classnames) 
		self.outputs[0].write(s)
		self.outputs[0].chmod (Utils.O755)
