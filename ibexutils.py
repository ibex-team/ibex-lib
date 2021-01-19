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
		return [ d for d in node.listdir() if
		                               os.path.isdir (os.path.join (node.name, d)) ]

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

try:
  unicode
except NameError:
  unicode = None

def to_unicode (s):
    r = None
    # for Python3, unicode does not exist (set to None above), we return s
    # for Python2, if s is already of type 'unicode', we also return s
    if unicode is None or isinstance (s, unicode):
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
def path_pc_remove_prefix (conf, path):
    if Utils.is_win32:
      if path.startswith (conf.env.PREFIX + "\\"):
        path = path[len(conf.env.PREFIX)+1:]
      path = escape_backslash_on_win32 (path)
    else:
      if path.startswith (conf.env.PREFIX + "/"):
        path = path[len(conf.env.PREFIX)+1:]
    return path

@conf
def path_pc (conf, path):
    path = path.replace (conf.env.INCDIR, "${includedir}")
    path = path.replace (conf.env.LIBDIR, "${libdir}")
    path = path.replace (conf.env.PREFIX, "${prefix}")
    return escape_backslash_on_win32 (path)

@conf
def configure_3rd_party_with_autotools (conf, archive_name,
            without_configure=False, without_make_install=False, conf_args = "", cflags_args = "", make_args = []):
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
        cmd_make = conf.env.MAKE + make_args
    else:
        conf_args += " --prefix=%s" % destnode.abspath ()
        cmd_conf = "./configure %s" % (conf_args)
        cmd_make = conf.env.MAKE + ["-j%d"%conf.options.jobs] + make_args
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
