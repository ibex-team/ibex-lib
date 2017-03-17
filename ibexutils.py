import os, tarfile, functools, sys, shutil, copy

from waflib import Logs, Errors, Utils
from waflib.Configure import conf, ConfigurationContext
sys.path.append(os.path.abspath ("3rd"))
import patch

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
			without_configure=False, without_make_install=False, conf_args = ""):
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
		os.environ["CFLAGS"] = cflags + " ".join(conf.env.CFLAGS_cshlib)
		os.environ["CXXFLAGS"] = cxxflags+" "+" ".join(conf.env.CXXFLAGS_cxxshlib)

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
			print e
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
