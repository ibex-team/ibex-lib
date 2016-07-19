import os, tarfile, functools, sys

from waflib import Logs, Errors, Utils
from waflib.Configure import conf, WAF_CONFIG_LOG

# not @Configure.conf because, the function is also called by 'options'
def get_dirlist (node):
	folders = node.ant_glob('*',dir=True,src=False)
	return [ os.path.basename(str(f)) for f in folders ]
	
def archive_name_without_suffix (archive):
	suffixes = [".tar.gz", ".tgz", ".tar" ]
	if any(archive.endswith (suffix) for suffix in suffixes):
		for suffix in suffixes:
			if archive.endswith (suffix):
				return archive[:-len(suffix)]
	else:
		conf.fatal ("Cannot handle archive %s (based on its suffix)" % archive)

@conf
def extract_archive (conf, archive_path, name, destdir):
	# path is the destination folder where the file will be extracted 
	path = os.path.join (destdir, name)

	conf.start_msg("Extracting %s" % os.path.basename(archive_path))
	
	if not os.path.isdir (path): # if not already extracted
		# extract the sources
		os.makedirs (path)

		t = tarfile.open (os.path.join (archive_path))
		t.extractall (destdir)
		conf.end_msg("done")
	else:
		conf.end_msg("destination already exists", color = 'YELLOW')

	return path

# Parameters:
# - src_dir_node  : source directory: where the archive is located
# - name          : name of the directory where the file will be extracted
# - filename      : filename of the archive (in src_dir)
# - dest_dir_node : destination directory. If None, the file is extracted in src_dir
# 
# Return         : the node of the directory where the file has been extracted
#
def unpack_archive (src_dir_node, name, filename = None, dest_dir_node = None):
	if not dest_dir_node:
		dest_dir_node = src_dir_node
		

	src_dir = src_dir_node.abspath()

	dest_dir = dest_dir_node.abspath()

	# path is the destination folder where the file is extracted 
	path = os.path.join (dest_dir, name)
	
	if not os.path.isdir (path): # if not already extracted
		# extract the sources
		os.makedirs (path)

		if not filename:
			filename = "%s.tar.gz" % name

		Logs.pprint ("NORMAL", "Unpacking %s" % filename)
		t = tarfile.open (os.path.join (src_dir, filename))
		
		t.extractall (dest_dir)
	
	node = dest_dir_node.find_dir (name)
	
	assert node
	return node

# convert path from windows format to format compatible with mingw.
# Ex: C:\path/to/dir becomes /c/path/to/dir
def convert_path_win2msys (path):
	path = os.path.abspath (path)
	drv, path = os.path.splitdrive (path)

	return "/%s%s" % (drv[0], path.replace("\\", "/"))

@conf
def configure_3rd_party_with_autotools (conf, archive_name, conf_args = ""):
	name = archive_name_without_suffix (archive_name)
	Logs.pprint ("BLUE", "Starting configuration for %s"%name)
	conf.to_log ("="*40 + "\nStarting configuration for %s\n"%name + "="*40)

	cur_dir = os.path.abspath(str(conf.path))
	archive_path = os.path.join (cur_dir, "3rd", archive_name)
	destnode = conf.bldnode.make_node("3rd")
	destdir = os.path.abspath(str(destnode))

	# Install everything in build directory, in '3rd' subdirectory (the 'lib' and
	# 'include' directory can be copied in conf.env.PREFIX when ./waf install is
	# called, if needed)
	incdir = os.path.join (destdir, "include")
	libdir = os.path.join (destdir, "lib")

	libsrcdir = conf.extract_archive (archive_path, name, destdir)

	conf.find_program ("make")

	# Built shared library, if ibex is built as a shared library.
	if conf.env.ENABLE_SHARED:
		conf_args += " --enable-shared --disable-static"
	else:
		conf_args += " --enable-static --disable-shared"

	if Utils.is_win32:
		conf_args += " --prefix=%s" % convert_path_win2msys (destdir)
		conf.find_program ("sh")
		cmd_conf = [conf.env.SH, "-c", "./configure %s"%conf_args]
		cmd_make = [conf.env.SH, "-c", conf.env.MAKE]
		cmd_install = [conf.env.SH, "-c", conf.env.MAKE + ["install"]]
	else:
		conf_args += " --prefix=%s" % destdir
		cmd_conf =	"./configure %s" % (conf_args)
		cmd_make = conf.env.MAKE
		cmd_install = conf.env.MAKE + ["install"]

	stages=[(cmd_conf, "configure"), (cmd_make, "make"), (cmd_install, "install")]
	for cmd, stage in stages:
		conf.start_msg("Calling %s" % stage)
		try: 
			out = conf.cmd_and_log (cmd, cwd=libsrcdir)
			conf.end_msg("done")
		except Errors.WafError as e:
			conf.end_msg("failed", color="RED")
			print e
			conf.fatal ("failed to %s %s (%s)" % (stage, name, cmd))

	conf.to_log ("="*40 + "\nConfiguration for %s: done\n"%name + "="*40)

	return incdir, libdir
