
*********************************************
      Installation (IbexOpt with Ibex 2.4)
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

Uncompress the archive ibex-2.4.0.tar.gz in some Ibex folder::

  ~/Ibex/$ tar xvfz ibex-2.4.0.tar.gz

Uncompress the archive ibex-optim.tar.gz in the plugin folder::

  ~/Ibex/$ tar xvfz ibex-optim.tar.gz --directory=ibex-2.4.0/plugins

Add the following option to waf configure (ommiting ``--with-soplex`` if you don't use Ibex for academic purposes)::

  ~/Ibex/ibex-2.4.0/$ ./waf configure [...] --with-optim --with-soplex

And then::

  ~/Ibex/ibex-2.4.0/$ sudo ./waf install


==============================
Configuration options
==============================	

``waf`` ``configure`` supports the following options with this plugin :
                    
--with-optim        Install IbexOpt (mandatory)

					
--optim-lib=soplex  Install IbexOpt with Soplex. The plugin archive contains a version of soplex so it is 
					not necessary to have Soplex already installed on your system. 
					Soplex is under `ZIB`_ academic licence. If you intend to use Ibex with Soplex commercially,
					you may consider contacting Soplex for a commercial licence.
                    
					If you install your own version of Soplex, use the following argument::
						
						make ZLIB=false
					
						
					and if Ibex is installed as a dynamic library (``--enable-shared``), Soplex must also be installed as a 
					dynamic library. For this, add the option SHARED=true to the previous command::
						
						make ZLIB=false SHARED=true
					
					
					Under Windows, add also ``SHAREDLIBEXT=dll`` to the previous command.

					.. 	Warning::

						The current release of Ibex is not compatible with Soplex 2.0.


--soplex-path=PATH  Set the (absolute) path of Soplex to PATH (to be used with ``--optim-lib=soplex``). The plugin archive contains 
                    a version of Soplex so this option is not required.
                    PATH is the absolute path where Soplex is installed (don’t use relative path like ``--soplex-path=../soplex-xx``).
                    
                    If Ibex is compiled as a shared library, you must also add the libpath of Soplex in ``LD_LIBRARY_PATH``::
						
						~/Ibex/ibex-2.4.0/$ export LD_LIBRARY_PATH=[prefix]/lib/:[soplex-path]/lib/
					
                    Under Windows, if you run a program from a command window, the ``PATH`` variable must also be updated::
					
						 > set IBEX_PATH=C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.4.0
						 > set SOPLEX_PATH=...
						 > set PATH=%PATH%;%IBEX_PATH%\lib;%SOPLEX_PATH%\lib;C:\MinGW\bin
					
					
--optim-lib=clp     Install IbexOpt with CLP (linear Solver from the COIN-OR project). The plugin archive contains a version of CLP so it is 
                    not necessary to have CLP already installed on your system.
                    This option is **experimental**, i.e., support for installation issues may not be guaranteed. 
                    
--clp-path=PATH     Set the (absolute) path of Soplex to PATH (to be used with ``--optim-lib=clp``). The plugin archive contains 
                    a version of CLP so this option is not required.
                    PATH is the absolute path where CLP is installed (don’t use relative path like ``--clp-path=../clp-xx``).       
                    If Ibex is compiled as a shared library, you must also add the libpath of CLP in ``LD_LIBRARY_PATH``.

--optim-lib=cplex   Install IbexOpt with CPLEX. The path of CPLEX must be provided with the ``--cplex-path`` option.
					This option is **experimental**, i.e., support for installation issues may not be guaranteed. 

--cplex-path=PATH   Set the path of CPLEX (to be used with ``--optim-lib=cplex``).
                    PATH is the absolute path where CPLEX is installed (don’t use relative path).
                    If Ibex is compiled as a shared library, you must also add the libpath of CPLEX in ``LD_LIBRARY_PATH``.
   


==============================
Compiling and running programs
==============================

To compile and run the default optimizer::

	~/Ibex/ibex-2.4.0/$ cd plugins/optim/examples
	~/Ibex/ibex-2.4.0/plugins/optim/$ export PKG_CONFIG_PATH=[ibex-path]/share/pkgconfig
	~/Ibex/ibex-2.4.0/plugins/optim/$ make defaultsolver
	~/Ibex/ibex-2.4.0/plugins/optim/$ ./defaultsolver ../benchs/cyclohexan3D.bch 1e-05 10

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

