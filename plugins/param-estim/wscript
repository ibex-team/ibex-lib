#! /usr/bin/env python
# encoding: utf-8

from waflib import Logs

def options (opt):
	opt.add_option ("--with-param-estim", action="store_true", dest="WITH_PARAM_ESTIM",
			help = "Use Parameter Estimation plugin")

def configure (conf):
	conf.env.WITH_PARAM_ESTIM = conf.options.WITH_PARAM_ESTIM
		
	if not conf.env.WITH_PARAM_ESTIM: 
		return
	
	Logs.pprint ("BLUE", "Configure the Parameter Estimation plugin")
	
	# add PARAM_ESTIM plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/param-estim/src")
	conf.env.append_unique("INCLUDES","../../plugins/param-estim/src/cliquer")
	
def build (bld):
		
	if not bld.env.WITH_PARAM_ESTIM: 
		return
	
	Logs.pprint ("BLUE", "Build the Parameter Estimation plugin")

	# add PARAM_ESTIM plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
	# add CLIQUER sources (excluding the "cl" application)
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/cliquer/*.cpp", excl="**/cl.cpp"))
	# add PARAM_ESTIM plugin headers
	bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))
