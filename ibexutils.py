import os, tarfile

from waflib import Logs

def unpack_archive (src_dir_node, name, filename = None):
	src_dir = src_dir_node.abspath()
	path = os.path.join (src_dir, name)

	if not os.path.isdir (path):
		# extract the sources
		os.makedirs (path)

		if not filename:
			filename = "%s.tar.gz" % name

		Logs.pprint ("NORMAL", "Unpacking %s" % filename)
		t = tarfile.open (os.path.join (src_dir, filename))
		t.extractall (src_dir)

	node = src_dir_node.find_dir (name)
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

