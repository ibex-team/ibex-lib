import os, tarfile, functools, sys, shutil

from waflib import Logs, Errors, Utils
from waflib.Configure import conf, ConfigurationContext

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
def write_setting_header (conf, **kwargs):
	conf.setenv ("setting")
	conf.write_config_header (**kwargs)
	bak = conf.env.cfg_files
	conf.setenv("")
	conf.env.append_unique ("cfg_files", bak)
	conf.all_envs.pop("setting", None)

@conf
def append_unique_lib_store (conf, dest_store, src_store):
	prefix_list = [ "LIB", "LIBPATH", "STLIB", "STLIBPATH", "LINKFLAGS" ]
	prefix_list += [ "RPATH", "CFLAGS", "CXXFLAGS", "DFLAGS", "INCLUDES" ]
	prefix_list += [ "CXXDEPS", "CCDEPS", "LINKDEPS", "DEFINES", "FRAMEWORK" ]
	prefix_list += [ "FRAMEWORKPATH", "ARCH" ]
	for prefix in prefix_list:
		src = "%s_%s" % (prefix, src_store)
		dest = "%s_%s" % (prefix, dest_store)
		if conf.env[src]:
			conf.env.append_unique (dest, conf.env[src])

def archive_name_without_suffix (archive):
	suffixes = [".tar.gz", ".tgz", ".tar" ]
	if any(archive.endswith (suffix) for suffix in suffixes):
		for suffix in suffixes:
			if archive.endswith (suffix):
				return archive[:-len(suffix)]
	else:
		conf.fatal ("Cannot handle archive %s (based on its suffix)" % archive)

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

@conf
def configure_3rd_party_with_autotools (conf, archive_name,
			without_configure=False, without_make_install=False, conf_args = "",):
	name = archive_name_without_suffix (archive_name)
	Logs.pprint ("BLUE", "Starting installation of %s"%name)
	conf.to_log ((" Starting installation of %s " % name).center (80, "="))

	archive_path = os.path.join (conf.path.abspath (), "3rd", archive_name)
	destnode = conf.bldnode.find_or_declare ("3rd")

	# Install everything in build directory, in '3rd' subdirectory (the 'lib' and
	# 'include' directory can be copied in conf.env.PREFIX when ./waf install is
	# called, if needed)
	incdir = destnode.find_or_declare ("include").abspath()
	libdir = destnode.find_or_declare ("lib").abspath()

	srcdir = conf.extract_archive (archive_path, name, destnode)

	conf.find_program ("make")

	# Built shared library, if ibex is built as a shared library.
	if conf.env.ENABLE_SHARED:
		conf_args += " --enable-shared --disable-static"
	else:
		conf_args += " --enable-static --disable-shared"

	if Utils.is_win32:
		conf_args += " --prefix=%s" % convert_path_win2msys (destnode.abspth ())
		conf.find_program ("sh")
		cmd_conf = [conf.env.SH, "-c", "./configure %s"%conf_args]
		cmd_make = [conf.env.SH, "-c", conf.env.MAKE]
		cmd_install = [conf.env.SH, "-c", conf.env.MAKE + ["install"]]
	else:
		conf_args += " --prefix=%s" % destnode.abspath ()
		cmd_conf =	"./configure %s" % (conf_args)
		cmd_make = conf.env.MAKE
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
			out = conf.cmd_and_log (cmd, cwd=srcdir)
			conf.end_msg("done")
		except Errors.WafError as e:
			conf.end_msg("failed", color="RED")
			print e
			conf.fatal ("failed to %s %s (%s)" % (stage, name, cmd))

	conf.to_log ((" Installation of %s: done " % name).center (80, "="))

	return srcdir, incdir, libdir

# Add verbose wrapper around pre_recurse and post_recurse methods of
# ConfigurationContext class, in order to a a more verbose output.
def verbose_pre_recurse (f):
	def fun (ctx, node):
		Logs.pprint ("BLUE", "Starting configure from ./%s" % node.srcpath ())
		f (ctx, node)
	fun.__name__ = f.__name__
	return fun

ConfigurationContext.pre_recurse = verbose_pre_recurse (ConfigurationContext.pre_recurse)

def verbose_post_recurse (f):
	def fun (ctx, node):
		Logs.pprint ("BLUE", "Leaving configure from ./%s" % node.srcpath ())
		f (ctx, node)
	fun.__name__ = f.__name__
	return fun

ConfigurationContext.post_recurse = verbose_post_recurse (ConfigurationContext.post_recurse)
