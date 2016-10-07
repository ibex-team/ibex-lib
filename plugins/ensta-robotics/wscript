#! /usr/bin/env python
# encoding: utf-8

from waflib import Logs

def options (opt):
	opt.add_option ("--with-ensta-robotics", action="store_true", dest="WITH_ENSTA_ROBOTICS",
			help = "Use ENSTA robotics plugin")

def configure (conf):
	conf.env.WITH_ENSTA_ROBOTICS = conf.options.WITH_ENSTA_ROBOTICS
		
	if not conf.env.WITH_ENSTA_ROBOTICS: 
		return
	
	Logs.pprint ("BLUE", "Configure the ENSTA robotics plugin")
	
	# add ensta-robotics plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/ensta-robotics/src")

def build (bld):
		
	if not bld.env.WITH_ENSTA_ROBOTICS: 
		return
	
	Logs.pprint ("BLUE", "Build the ENSTA robotics plugin")

	# add ensta-robotics plugin sources
	bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
	# add ensta-robotics plugin headers
	bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))
	
