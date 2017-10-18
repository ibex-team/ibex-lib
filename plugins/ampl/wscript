#! /usr/bin/env python
# encoding: utf-8

import ibexutils
import os, sys
from waflib import Logs

######################
###### options #######
######################
def options (opt):
    opt.add_option ("--with-ampl", action="store_true", dest="WITH_AMPL",
            help = "Use AMPL")
######################
##### configure ######
######################
def configure (conf):

    conf.env.WITH_AMPL = conf.options.WITH_AMPL
        
    conf.start_msg ("AMPL plugin")
    if not conf.env.WITH_AMPL: 
        conf.end_msg ("not used")
        return

    conf.end_msg ("enabled")

    conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "AMPL")
    
    if (conf.env.DEST_OS == "win32"):
        # need to compile dlfcn           
        conf.msg ("Using library dl from", "3rd/ subdirectory")
        dl_archive = "dlfcn-win32-master.tar.gz"
        dl_ret = conf.configure_3rd_party_with_autotools (dl_archive, False, False)
        _, dl_include, dl_lib = dl_ret
        conf.env.INSTALL_3RD = True
        conf.env.append_unique ("LIB_3RD_LIST", "dl" )
        
        # Looking for dlfcn header and library (called in amplsolvers)
        conf.check_cxx (header_name = "dlfcn.h", includes = dl_include,
                            use = [ "IBEX", "AMPL" ], uselib_store= "AMPL")
        

    # Add information in ibex_Setting
    conf.setting_define ("WITH_AMPL", 1)
    
    ampl_cflags =  ""
    # Add option for compatibility with GAOL
    if (conf.env["INTERVAL_LIB"] == "GAOL"):
        ampl_cflags = " -DNo_dtoa "
    
    conf.msg ("Using library AMPL from", "3rd/ subdirectory")
    ampl_archive = "amplsolvers.tar.gz"
    ampl_ret = conf.configure_3rd_party_with_autotools (ampl_archive, False, False, "", ampl_cflags)
    _, ampl_include, ampl_lib = ampl_ret
    conf.env.INSTALL_3RD = True
    conf.env.append_unique ("LIB_3RD_LIST", "amplsolvers" )

    # Looking for ampl header and library (called amplsolvers)
    for h in [ "amplsolvers/asl.h", "amplsolvers/nlp.h", "amplsolvers/getstub.h", "amplsolvers/opcode.hd", "amplsolvers/r_opn.hd" ]:
        conf.check_cxx (header_name = h, includes = ampl_include,
                        use = [ "IBEX", "AMPL" ], uselib_store= "AMPL")

    for l in ["amplsolvers", "dl" ]:
        conf.check_cxx (lib = l, libpath = ampl_lib,
                        use = [ "IBEX", "AMPL" ], uselib_store = "AMPL")
        
    # Put every _AMPL variables in _IBEX_DEPS (for ibex.pc) ## no need anymore
#    new_inc = os.path.join (conf.env.INCDIR_3RD, "amplsolvers")
#    conf.env.append_unique ("INCLUDES_IBEX_DEPS", new_inc)
    if conf.env.ENABLE_SHARED:
        # if shared lib is used, 3rd party libs are compiled as static lib with
        # -fPIC and are contained in libibex
        for lib in conf.env.LIB_AMPL:
            if not lib in conf.env.LIB_3RD_LIST:
                conf.env.append_unique ("LIB_IBEX_DEPS", lib)
    else:
        conf.env.append_unique ("LIB_IBEX_DEPS", conf.env.LIB_AMPL)
        
    
    # add AMPL plugin include directory
    for f in conf.path.ant_glob ("src/**", dir = True, src = False):
        conf.env.append_unique("INCLUDES_AMPL", f.abspath())

    # The build and install steps will be run from the main src/wscript script so
    # we need to give path relative to the main src directory
    mainsrc = conf.srcnode.make_node ("src")

    # add AMPL headers
    for f in conf.path.ant_glob ("src/**/ibex_*.h"):
        conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

    # add AMPL source files
    for f in conf.path.ant_glob ("src/**/ibex_*.cpp"):
        conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))
        
    
    # The utest step will be run from the main tests/wscript script so we need to
    # give path relative to the main tests directory
    maintests = conf.srcnode.make_node ("tests")

    # add AMPL test files
    for f in conf.path.ant_glob ("tests/**/*.cpp"):
        conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

    # Add ampl/tests directory to list of INCLUDES for TESTS
    testsnode = conf.path.make_node ("tests")
    conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())
######################
####### build ########
######################
def build (bld):
    pass # nothing to do, everything is done in the main src/wscript script
