
.. _ibex-dev-misc:

***************************
Miscellaneous for developers
***************************

.. note::

  This section of the documentation is for developers. If you just want to use
  Ibex as a library or via binaries, you can skip this section entirely

===============================
Using Ninja instead of Make with CMake
===============================

.. warning::
    Currently, using Ninja does not work when Ibex needs to install Gaol or
    Soplex.
    It seems that the CMake function used to do the installation
    (ExternalProject_Add from the module ExternalProject) does not works well
    with ninja (in particular with the detection of dependencies between
    targets).

To use Ninja build system instead of Makefiles you need to do the following:

#. Install Ninja (for example, by installing the ninja-build package on Debian)
#. Add, in the CMake command line the following parameter::

  -G Ninja

#. Replace the command make by the command ninja, for example::

   $ ninja # to compile the library and the binaries
   $ ninja check # to run the checks
   $ ninja install # to install Ibex
   $ ninja uninstall # to uninstall it


===============================
Building the doc
===============================

Run the following command from the root directory of Ibex's source tree::

  $ sphinx-build doc/ <output directory>

