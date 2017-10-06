
*********************************************
      Installation (IbexOpt with Ibex 2.6)
*********************************************



.. note::

   Read how to :ref:`install Ibex <ibex-install>` before installing this plugin.

.. _Cplex: http://www.ibm.com/software/commerce/optimization/cplex-optimizer
.. _Soplex: 1.7.x: http://soplex.zib.de
.. _CLP: https://projects.coin-or.org/Clp
.. _ZIB: http://scip.zib.de/academic.txt

The IbexOpt plugin relies on a LP solver that can either be `Soplex`_, `Cplex`_ or `CLP`_.

.. warning::
   
   Because of licence compatibility, the LP solver installed by default is CLP (under Eclipse Public License). 
   However, the installation process with CLP is still unstable and fails on some platforms. Installation with Cplex is also experimental.

   Support for installation is only guaranteed with Soplex so, if possible, we recommend to use Soplex (see `Configuration options`_).

==========================
Standard install
==========================

**Note:** Under Windows, the following instructions must be typed in the shell of MinGW.

Uncompress the archive ibex-2.6.0.tar.gz in some Ibex folder::

  ~/Ibex/$ tar xvfz ibex-2.6.0.tar.gz

Uncompress the archive ibex-optim.tar.gz in the plugin folder::

  ~/Ibex/$ tar xvfz ibex-optim.tar.gz --directory=ibex-2.6.0/plugins

Add the following option to waf configure (ommiting ``--with-soplex`` if you don't use Ibex for academic purposes)::

  ~/Ibex/ibex-2.6.0/$ ./waf configure [...] --with-optim --with-soplex

And then::

  ~/Ibex/ibex-2.6.0/$ sudo ./waf install

==============================
Compiling and running programs
==============================

To compile and run the default optimizer::

	~/Ibex/ibex-2.6.0/$ cd plugins/optim/examples
	~/Ibex/ibex-2.6.0/plugins/optim/$ export PKG_CONFIG_PATH=[ibex-path]/share/pkgconfig
	~/Ibex/ibex-2.6.0/plugins/optim/$ make defaultsolver
	~/Ibex/ibex-2.6.0/plugins/optim/$ ./defaultsolver ../benchs/cyclohexan3D.bch 1e-05 10

The ``PKG_CONFIG_PATH`` variable is only necessary if you have installed Ibex with ``--prefix``.

The default solver solves the systems of equations in argument (cyclohexan3D) with a precision less than 1e-05 and within a time limit of 10 seconds.


==========================
Troubleshooting
==========================

--------------------------------------------
Linking problem with CoinOR
--------------------------------------------

If the linker fails with undefined reference to dgetrf and dgetrs, it is probably because you have installed Lapack. You can either:

    try to adapt the makefile to link with Lapack.
    Remove Lapack, reinstall Ibex and reinstall Lapack (in this order).

