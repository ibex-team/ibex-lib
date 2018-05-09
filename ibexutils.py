import os, tarfile, functools, sys, shutil, copy

from waflib import Logs, Errors, Utils, Build
from waflib.Configure import conf, ConfigurationContext
sys.path.append(os.path.abspath ("3rd"))
import patch

# Custom commands derived from build
class UTestContext (Build.BuildContext):
    cmd = "utest"
    fun = "utest"

class BenchmarksContext (Build.BuildContext):
    cmd = "benchmarks"
    fun = "benchmarks"
    bench_results = {}
    bench_cmp = {}
    bench_errors = []

# not @Configure.conf because, the function is also called by 'options'
def get_dirlist (node):
    folders = node.ant_glob('*',dir=True,src=False)
    return [ os.path.basename(str(f)) for f in folders ]

@conf
def setting_define (conf, key, val, key_format = "_IBEX_%s_", **kwargs):
    conf.setenv ("setting")
    conf.define (key_format % key, val, **kwargs)
    conf.setenv("")

@conf
def apply_all_relevant_patches (conf, name):
    patch_ant_glob = "3rd/%s.all.all*patch" % name
    patch_ant_glob += " 3rd/%s.%s.all*patch" % (name, sys.platform)
    patch_ant_glob += " 3rd/%s.all.%s*patch" % (name, conf.env.CC_NAME)
    patch_ant_glob += " 3rd/%s.%s.%s*patch" % (name,sys.platform,conf.env.CC_NAME)
    for p in conf.path.ant_glob (patch_ant_glob):
        conf.apply_patch (p.abspath())


@conf
def write_setting_header (conf, **kwargs):
    conf.setenv ("setting")
    conf.write_config_header (**kwargs)
    bak = conf.env.cfg_files
    conf.setenv("")
    conf.env.append_unique ("cfg_files", bak)
    conf.all_envs.pop("setting", None)

@conf
def add_build_targets (self):
    if not isinstance (self, Build.BuildContext):
        n = self.__class__.__name__
        err = "Error in add_build_targets, %s is not a subclass of BuildContext" % n
        self.fatal (err)
    elif self.__class__ is Build.BuildContext:
        pass # no need to add build targets to the base class BuildContext
    else:
        bak = self.fun
        self.fun = "build"
        self.recurse (self.run_dir)
        self.fun = bak

def archive_name_without_suffix (archive):
    suffixes = [".tar.gz", ".tgz", ".tar" ]
    if any(archive.endswith (suffix) for suffix in suffixes):
        for suffix in suffixes:
            if archive.endswith (suffix):
                return archive[:-len(suffix)]
    else:
        conf.fatal ("Cannot handle archive %s (based on its suffix)" % archive)

@conf
def apply_patch (conf, patch_abspath):
    conf.msg ("Applying patch", os.path.basename (patch_abspath))
    p = patch.fromfile (patch_abspath)
    if not p.apply (root = conf.bldnode.make_node ("3rd").abspath()):
        conf.fatal ("Cannot apply patch %s" % patch_abspath)

@conf
def extract_archive (conf, archive_path, name, destnode):
    # path is the destination folder where the file will be extracted 
    path = os.path.join (destnode.abspath (), name)

    conf.start_msg("Extracting %s" % os.path.basename(archive_path))

    if os.path.isdir (path): # if output directory already exists, remove it
        shutil.rmtree (path, ignore_errors = True)

    # extract the sources
    os.makedirs (path)

    t = tarfile.open (archive_path)
    t.extractall (destnode.abspath ())
    t.close()

    conf.end_msg("done")

    return path

# convert path from windows format to format compatible with mingw.
# Ex: C:\path/to/dir becomes /c/path/to/dir
def convert_path_win2msys (path):
    path = os.path.abspath (path)
    drv, path = os.path.splitdrive (path)

    return "/%s%s" % (drv[0], path.replace("\\", "/"))

def escape_backslash_on_win32 (s):
    if Utils.is_win32:
        return s.replace ("\\", "\\\\")
    else:
        return s

def to_unicode (s):
    r = None
    if isinstance (s, unicode):
        r = s
    else:
        for f in [ 'ascii', 'cp1251', 'cp1252', 'latin1', 'utf-8' ] :
            try:
                r = s.decode(f)
                break
            except UnicodeDecodeError:
                continue
    assert (not r is None)
    return r

@conf
def path_pc_prefix (conf, path):
    path = path.replace (conf.env.PREFIX, "${prefix}")
    return escape_backslash_on_win32 (path)

@conf
def path_pc (conf, path):
    path = path.replace (conf.env.INCDIR, "${includedir}")
    path = path.replace (conf.env.LIBDIR, "${libdir}")
    return escape_backslash_on_win32 (path)

@conf
def configure_3rd_party_with_autotools (conf, archive_name,
            without_configure=False, without_make_install=False, conf_args = "", cflags_args = ""):
    name = archive_name_without_suffix (archive_name)
    Logs.pprint ("BLUE", "Starting installation of %s"%name)
    conf.to_log ((" Starting installation of %s " % name).center (80, "="))

    archive_path = os.path.join (conf.path.abspath (), "3rd", archive_name)
    destnode = conf.bldnode.make_node ("3rd")

    # Install everything in build directory, in '3rd' subdirectory (the 'lib' and
    # 'include' directory can be copied in conf.env.PREFIX when ./waf install is
    # called, if needed)
    incdir = destnode.find_or_declare ("include").abspath()
    libdir = destnode.find_or_declare ("lib").abspath()

    srcdir = conf.extract_archive (archive_path, name, destnode)

    conf.find_program ("make")

    # Apply patches 
    conf.apply_all_relevant_patches (name)

    # always build static library, even if ibex is built as a shared library.
    conf_args += " --enable-static --disable-shared"

    if conf.env.ENABLE_SHARED:
        cflags = os.getenv("CFLAGS", "")
        cxxflags = os.getenv("CXXFLAGS", "")
        os.environ["CFLAGS"] = cflags + " " + cflags_args +"  ".join(conf.env.CFLAGS_cshlib)
        os.environ["CXXFLAGS"] = cxxflags+" " + " ".join(conf.env.CXXFLAGS_cxxshlib)
    else:
        cflags = os.getenv("CFLAGS", "")
        cxxflags = os.getenv("CXXFLAGS", "")
        os.environ["CFLAGS"] = cflags + " " + cflags_args 
        os.environ["CXXFLAGS"] = cxxflags+" "
        

    if Utils.is_win32:
        conf_args += " --prefix=%s" % convert_path_win2msys (destnode.abspath ())
        conf.find_program ("sh")
        cmd_conf = [conf.env.SH, "-c", "./configure %s"%conf_args]
        cmd_make = conf.env.MAKE
    else:
        conf_args += " --prefix=%s" % destnode.abspath ()
        cmd_conf = "./configure %s" % (conf_args)
        cmd_make = conf.env.MAKE + ["-j%d"%conf.options.jobs]
    cmd_install = conf.env.MAKE + ["install"] 

    stages = []
    if not without_configure:
        stages += [ (cmd_conf, "configure") ]
    stages += [ (cmd_make, "make") ]
    if not without_make_install:
        stages += [ (cmd_install, "install") ]
    for cmd, stage in stages:
        conf.start_msg("Calling %s" % stage)
        try: 
            out = conf.cmd_and_log (cmd, cwd=srcdir, env=os.environ)
            conf.end_msg("done")
        except Errors.WafError as e:
            conf.end_msg("failed", color="RED")
            print(e)
            conf.fatal ("failed to %s %s (%s)" % (stage, name, cmd))

    conf.to_log ((" Installation of %s: done " % name).center (80, "="))

    if conf.env.ENABLE_SHARED:
        os.environ["CFLAGS"] = cflags
        os.environ["CXXFLAGS"] = cxxflags

    return srcdir, incdir, libdir

# Add verbose wrapper around pre_recurse and post_recurse methods of
# ConfigurationContext class, in order to a a more verbose output.
def verbose_pre_recurse (f):
    def fun (ctx, node):
        if Logs.verbose:
            Logs.pprint ("BLUE", "Starting configure from ./%s" % node.srcpath ())
        f (ctx, node)
    fun.__name__ = f.__name__
    return fun

ConfigurationContext.pre_recurse = verbose_pre_recurse (ConfigurationContext.pre_recurse)

def verbose_post_recurse (f):
    def fun (ctx, node):
        if Logs.verbose:
            Logs.pprint ("BLUE", "Leaving configure from ./%s" % node.srcpath ())
        f (ctx, node)
    fun.__name__ = f.__name__
    return fun

ConfigurationContext.post_recurse = verbose_post_recurse (ConfigurationContext.post_recurse)


# functions that handle lp library

LPLIB_PLUGIN_PREFIX = "lp_lib_"

def lp_lib_options (opt):
    plugin_node = opt.path.find_node ("plugins")

    # get the list of all possible lp library
    libdir = plugin_node.ant_glob (LPLIB_PLUGIN_PREFIX + "*", dir=True, src=False)
    libdir = [ os.path.basename (str (d)) for d in libdir ]
    list_of_lp_lib_plugin = [ d[len (LPLIB_PLUGIN_PREFIX):] for d in libdir ]
    default_lp_lib = "none" if "none" in list_of_lp_lib_plugin else None
    # if the none plugin does not exist, it will create a error during configure

    # help string for --lp-lib command line option
    help_string = "Possible values: " + ", ".join (list_of_lp_lib_plugin)
    help_string += " [default: " + str(default_lp_lib) + "]"

    # add the option --lp-lib
    opt.add_option ("--lp-lib", action="store", dest="LP_LIB",
                                    choices = list_of_lp_lib_plugin,
                                    default = default_lp_lib, help = help_string)

@conf
def lp_lib (conf):
    # Set env variable with the prefix of plugins which handle lp
    conf.env.LPLIB_PLUGIN_PREFIX = LPLIB_PLUGIN_PREFIX

    Logs.pprint ("BLUE", "Configuration of the library for LP")

    if conf.options.LP_LIB is None:
        conf.fatal ("The lp_lib_none plugin is not available.")

    conf.msg ("Library for LP", conf.options.LP_LIB)
    lplib_dirname = LPLIB_PLUGIN_PREFIX + conf.options.LP_LIB
    plugin_node = conf.path.find_node ("plugins")
    lplib_node = plugin_node.find_node (lplib_dirname)

    # Recurse on the plugin
    conf.recurse (os.path.join ("plugins", lplib_dirname))

    # Add lplib_dir to INCLUDES
    conf.env.append_unique ("INCLUDES_LP_LIB", lplib_node.abspath())

    # check that mandatory files exist
    for f in [ "ibex_LPWrapper.cpp_" ]:
        if lplib_node.find_node (f) is None:
            conf.fatal ("A LP plugin must contain a file named %s" % f)

    # The following variables must be defined in env by the plugin called to
    # handle the LP library.
    for var in [ "LP_LIB", "IBEX_LP_LIB_INCLUDES",
                 "IBEX_LP_LIB_EXTRA_ATTRIBUTES" ]:
        if not var in conf.env:
            err = "%s must be defined in env by the plugin %s" % (var, lplib_dirname)
            conf.fatal (err)

    # Copy in _IBEX_DEPS some important variables from _LP_LIB
    # The plugin must use the store LP_LIB (uselib_store argument with
    # conf.check* functions).
    conf.env.append_unique ("CXXFLAGS_IBEX_DEPS", conf.env.CXXFLAGS_LP_LIB)
    if conf.env.ENABLE_SHARED:
        # if shared lib is used, 3rd party libs are compiled as static lib with
        # -fPIC and are contained in libibex
        for lib in conf.env.LIB_LP_LIB:
            if not lib in conf.env.LIB_3RD_LIST:
                conf.env.append_unique ("LIB_IBEX_DEPS", lib)
    else:
        conf.env.append_unique ("LIB_IBEX_DEPS", conf.env.LIB_LP_LIB)

    # Add info on the LP library used to the settings
    conf.setting_define ("LP_LIB", conf.env["LP_LIB"])
