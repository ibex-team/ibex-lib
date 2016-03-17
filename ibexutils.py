import os, tarfile

from waflib import Logs

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

######################################################################################################
class subdir:
	def __init__ (self, name_or_node):
		self.name = name_or_node.abspath() if hasattr (name_or_node, "abspath") else str (name_or_node)

	def __enter__ (self):
		Logs.pprint ("BLUE", "Entering %r" % self.name)

	def __exit__ (self, a, b, c):
		Logs.pprint ("BLUE", "Leaving %r" % self.name)

