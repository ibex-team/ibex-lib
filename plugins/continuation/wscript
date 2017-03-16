#! /usr/bin/env python
# encoding: utf-8

from waflib import Logs

def options (opt):
	opt.add_option ("--with-continuation", action="store_true", dest="WITH_CONTINUATION",
			help = "Use Continuation plugin")

def configure (conf):
	conf.env.WITH_CONTINUATION = conf.options.WITH_CONTINUATION
		
	if not conf.env.WITH_CONTINUATION: 
		return
	
	Logs.pprint ("BLUE", "Configure the Continuation plugin")
	
	# add Continuation plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/continuation/src")
	
def build (bld):
		
	if not bld.env.WITH_CONTINUATION: 
		return
	
	Logs.pprint ("BLUE", "Build the Continuation plugin")

	# add Continuation plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
	# add Continuation plugin headers
	bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))
