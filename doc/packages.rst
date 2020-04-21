
.. _ibex-packages:

***************************
Building packages
***************************

.. note::

  This section of the documentation is for developers. If you just want to use
  Ibex as a library or via binaries, you can skip this section entirely

.. note::

  To build packages, you need to :ref:`use CMake to build Ibex
  <ibex-install-cmake>`.


To build the package, execute the following command from the **build**
directory::

  $ make package

It will produce 4 files called ibex-<version>-<System>-binaries.deb,
ibex-<version>-<System>-devtools.deb, ibex-<version>-<System>.tar.gz and
ibex-<version>-<System>.zip.

===============================
Debian package
===============================

.. note::

  Debian package can only be build on Unix machines


If you are under Debian or Ubuntu, you can inspect the generated deb files with
the following commands::

  $ dpkg-deb -I <package.deb> # print information about the package
  $ dpkg-deb -c <package.deb> # print directories and files inside the package



.. _deb-wiki-packaging https://wiki.debian.org/Packaging
.. _deb-manual-debmake https://www.debian.org/doc/manuals/debmake-doc/index.en.html
.. _deb-manual-list https://www.debian.org/doc/devel-manuals
.. _deb-manual-tuto https://www.debian.org/doc/devel-manuals#packaging-tutorial


References on debian packaging: `here <deb-wiki-packaging>`,
`here <deb-manual-debmake>`, `here <deb-manual-list>` and
`here <deb-manual-tuto>`.
