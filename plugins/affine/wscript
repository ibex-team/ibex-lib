#! /usr/bin/env python
# encoding: utf-8

from waflib import Logs

def options (opt):
	opt.add_option ("--with-affine", action="store_true", dest="WITH_AFFINE",
			help = "Use Affine Arithmetic plugin")

def configure (conf):
	conf.env.WITH_AFFINE = conf.options.WITH_AFFINE
		
	if not conf.env.WITH_AFFINE: 
		return
	
	Logs.pprint ("BLUE", "Configure the Affine Arithmetic plugin")
	
	# add AFFINE plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/affine/src/arithmetic")
	conf.env.append_unique("INCLUDES","../../plugins/affine/src/function")

def build (bld):
		
	if not bld.env.WITH_AFFINE: 
		return
	
	Logs.pprint ("BLUE", "Build the Affine Arithmetic plugin")

	# add AFFINE plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
	# add AFFINE plugin headers
	bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))
	
