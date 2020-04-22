
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

It will produce 4 files called:

* ibex-<version>-<System>-binaries.deb
* ibex-<version>-<System>-devtools.deb
* ibex-<version>-<System>.tar.gz and
* ibex-<version>-<System>.zip.

===============================
Debian package
===============================

.. note::

  Debian package can only be built on Unix machines


If you are under Debian or Ubuntu, you can inspect the generated deb files with
the following commands::

  $ dpkg-deb -I <package.deb> # print information about the package
  $ dpkg-deb -c <package.deb> # print directories and files inside the package



References on debian packaging:
`here <https://wiki.debian.org/Packaging>`_,
`here <https://www.debian.org/doc/manuals/debmake-doc/index.en.html>`_,
`here <https://www.debian.org/doc/devel-manuals>`_ and
`here <https://www.debian.org/doc/devel-manuals#packaging-tutorial>`_.
