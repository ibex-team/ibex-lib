#! /usr/bin/env python
# encoding: utf-8

from waflib import Logs

def options (opt):
	opt.add_option ("--with-affine-extended", action="store_true", dest="WITH_AFFINE_EXTEND",
			help = "Use all the Affine Arithmetic plugin")

def configure (conf):
	conf.env.WITH_AFFINE_EXTEND = conf.options.WITH_AFFINE_EXTEND
		
	if not conf.env.WITH_AFFINE_EXTEND: 
		return
	
	Logs.pprint ("BLUE", "Configure all the Affine Arithmetic plugin")
	
	# add AFFINE plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/affine-extended/src/arithmetic")
	
def build (bld):
		
	if not bld.env.WITH_AFFINE_EXTEND: 
		return
	
	Logs.pprint ("BLUE", "Build the other Affine Arithmetic plugin")

		# add AFFINE plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
		# add AFFINE plugin headers
	bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))
	
	# Add information in ibex_Setting
	bld.env.settings['_IBEX_WITH_AFFINE_EXTENDED_']='1'
	
