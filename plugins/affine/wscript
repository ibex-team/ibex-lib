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
	
	if not conf.options.WITH_OPTIM:
		Logs.pprint ("YELLOW", "Warning: The LinearRelaxAffine class (part of option --with-affine) will not be generated (requires --with-optim).")

	# add AFFINE plugin include directory
	conf.env.append_unique("INCLUDES","../../plugins/affine/src/arithmetic")
	conf.env.append_unique("INCLUDES","../../plugins/affine/src/function")
	conf.env.append_unique("INCLUDES","../../plugins/affine/src/numeric")
	
def build (bld):
		
	if not bld.env.WITH_AFFINE: 
		return
	
	Logs.pprint ("BLUE", "Build the Affine Arithmetic plugin")

	# LinearRelaxAffine requires the LinearRelax base class from the optim plugin.
	if bld.env.WITH_OPTIM:
		# add AFFINE plugin sources
		bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp"))
		# add AFFINE plugin headers
		bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h"))
	else:
		# add AFFINE plugin sources (no LinearRelaxAffine2)
		bld.env.IBEX_SRC.extend(bld.path.ant_glob ("src/**/ibex_*.cpp", excl="src/numeric/ibex_LinearRelaxAffine2.*"))
		# add AFFINE plugin headers (no LinearRelaxAffine2)
		bld.env.IBEX_HDR.extend(bld.path.ant_glob ("src/**/ibex_*.h", excl="src/numeric/ibex_LinearRelaxAffine2.*"))
	
	# Add information in ibex_Setting
	bld.env.settings['_IBEX_WITH_AFFINE_']='1'
	
	INCDIR  = "${PREFIX}/include/ibex"	
	bld.install_files (INCDIR, bld.path.ant_glob ("src/**/ibex_*.h_"))
