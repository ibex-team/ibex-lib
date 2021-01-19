
.. _ibex-install-cmake:

*******************************************
Installation with Cmake (experimental)
*******************************************

.. _MacPorts: https://www.macports.org
.. _Gaol: http://sourceforge.net/projects/gaol
.. _Filib: http://www2.math.uni-wuppertal.de/~xsc/software/filib.html
.. _Profil/Bias: http://www.ti3.tu-harburg.de/keil/profil/index_e.html
.. _Cplex: http://www.ibm.com/software/commerce/optimization/cplex-optimizer
.. _Soplex 1.7.x: http://soplex.zib.de
.. _CLP: https://projects.coin-or.org/Clp
.. _ZIB: http://scip.zib.de/academic.txt

===================================
Standard install
===================================

**Note:** Ibex can either be compiled with `Gaol`_ or `Filib`_ . 
The library installed by default is Gaol, since our experiments have shown that this library prevails over the others.
So the standard installation will automatically extract and build the Gaol library (and its dependencies) from the bundle.
However, because of some installation problems with Gaol under Windows, the library installed by default under this platform
as of today is Filib (this will be changed in the future).

---------------
Linux and MacOS
---------------

If you are on MacOS and use the `MacPorts`_ package manager, you can install Ibex by simply executing::

  sudo port install ibex-lib
  
Otherwise, you can install Ibex from source with the following instructions.
The installation assumes your system meets some `requirements`_.

Save the archive ``ibex-2.8.8.tar.gz`` in some ``Ibex`` folder and::

  ~/Ibex$ tar xvfz ibex-2.8.8.tar.gz 
  ~/Ibex$ cd ibex-2.8.8 
  ~/Ibex/ibex-2.8.8$ mkdir build
  ~/Ibex/ibex-2.8.8/build$ cd build
  ~/Ibex/ibex-2.8.8/build$ cmake ..
  ~/Ibex/ibex-2.8.8/build$ make
  ~/Ibex/ibex-2.8.8/build$ sudo make install
      
If you want to install Ibex in a local folder, use ``-DCMAKE_INSTALL_PREFIX``, ex::

	~/Ibex/ibex-2.8.8/build$ cmake -DCMAKE_INSTALL_PREFIX=$HOME ..

See the :ref:`options <install-cmake-options>`. The folder full path must not contain any **white space** or wierd characters like ``'"\()`*[]``.

-----------------
Windows
-----------------

*(to be completed)*
  
---------------
Requirements
---------------

The following applications must be installed.

- g++
- gcc
- flex
- bison
- cmake
- pkg-config (*optionnal*)

On Ubuntu, you can install all you need with::

  ~$ sudo apt-get install -y g++ gcc flex bison cmake pkg-config
  
.. _install-cmake-options:

==============================
Configuration options
==============================	

``cmake`` supports the following options:
                    
======================  ======================================================================================
Option                  Description
======================  ======================================================================================
CMAKE_INSTALL_PREFIX    Ex: ``-DCMAKE_INSTALL_PREFIX=$HOME``
                         
                        Set the folder where Ibex must be installed (by default, ``/usr/local`` under Linux).
                        
                        .. warning::
                        
                          The full path of the folder must not contain white space or weird characters like ``'"\()`*[]``.

INTERVAL_LIB            Ex: ``-DINTERVAL_LIB=filib``
                        
                        Set the underlying interval library.

                        Possible values are either ``gaol``, ``filib`` or ``direct``. Default is ``gaol``.  
                        The ``direct`` library is a simple non-rigorous interval arithmetic, designed 
                        essentially for embedded systems with specific processor architectures that do not 
                        support rounding modes.
						
                        **TODO**: ``direct`` mode.
                       
LP_LIB                  Ex: ``-DLP_LIB=soplex``
                        
                        Set the linear programming library. 

                        Possible values are either ``soplex``, ``cplex`` or ``none``. Default is ``none``. 
                        **TODO**: ``cplex``.
                        The archive contains a version of Soplex so it is not necessary to have Soplex 
                        already installed on your system. 

						.. warning::
							
							Soplex is under `ZIB`_ academic licence. If you intend to use Ibex with Soplex 
							commercially, you may consider contacting Soplex for a commercial licence.
                        
                        If you install your own version of Soplex, use the ``SOPLEX_DIR`` option.
                        Soplex must have been compiled with the following argument::
                        
							make ZLIB=false
                        
                        and if Ibex is installed as a dynamic library (``--enable-shared``), Soplex must also 
                        be installed as a dynamic library. For this, add the option SHARED=true to the 
                        previous command::
						
							make ZLIB=false SHARED=true
					
                        Under Windows, add also ``SHAREDLIBEXT=dll`` to the previous command.
                        
                        .. 	note::
						                     
							- The current release of Ibex (>=2.8.8) is compatible with Soplex 4.
							- The link with cplex is **experimental**, i.e., support for installation issues 
							  may not be guaranteed. 


GAOL_DIR                Ex: ``-DINTERVAL_LIB=goal -DGAOL_DIR=$HOME/gaol``

                        Set the path of Goal when using Ibex with your own version of Goal.
                        Note that the plugin archive embeds a version of Gaol so it is not necessary to have 
                        Goal already installed on your system. Default value is "" (means: use embedded version).
						
                        Only to be used with ``-DINTERVAL_LIB=goal``.

MATHLIB_DIR             Ex: ``-DINTERVAL_LIB=goal -DMATHLIB_DIR=$HOME/matlib``
                        
                        Set the path of Mathlib when using Ibex with your own version of Goal.
                        Mathlib is a tierce library required by Gaol.
						
                        Note that the plugin archive embeds a version of Gaol so it is not necessary to have 
                        Goal already installed on your system. Default value is "" (means: use embedded version).
					
FILIB_DIR               Ex: ``-DINTERVAL_LIB=filib -DFILIB_DIR=$HOME/filib``

                        Set the path of Filib when using Ibex with your own version of Filib. 
                        Note that the plugin archive embeds a version of Filib so it is not necessary to have 
                        Filib already installed on your system. Default value is "" 
                        (means: use embedded version).
						
                        Only to be used with ``-DINTERVAL_LIB=filib``.
						

SOPLEX_DIR              Ex: ``-DLP_LIB=soplex -DSOPLEX_DIR=$HOME/soplex``

                        Set the path of Soplex when using Ibex with your own version of Soplex. Note that the 
                        plugin archive contains a version of Soplex so this option is not required.
                        
                        If Ibex is compiled as a shared library, you must also add the libpath of Soplex 
                        in ``LD_LIBRARY_PATH``::
						
							~/Ibex/ibex-2.8.8/$ export LD_LIBRARY_PATH=[prefix]/lib/:[soplex-path]/lib/
					
                        Under Windows, if you run a program from a command window, the ``PATH`` variable must 
                        also be updated::
					
							> set IBEX_PATH=C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.8.8
							> set SOPLEX_PATH=...
							> set PATH=%PATH%;%IBEX_PATH%\lib;%SOPLEX_PATH%\lib;C:\MinGW\bin
							
                        Only to be used with ``-DLP_LIB=soplex``.
						
									
CPLEX_DIR               Ex: ``-DLP_LIB=cplex -DCPLEX_DIR=$HOME/cplex``

                        **TODO**

                        Set the path of Cplex when using Ibex with your own version of Cplex. 
						
                        If Ibex is compiled as a shared library, you must also add the libpath of Cplex in 
                        ``LD_LIBRARY_PATH``.
                        
                        Only to be used with ``-DLP_LIB=cplex``.


                        
CMAKE_BUILD_TYPE        Ex: ``-DCMAKE_BUILD_TYPE=Debug``

                        Set the build mode either to ``Relase`` or ``Debug``. Default is ``Release``.
                        
                        In debug mode, compiler optimizations are all discarded (``-O0``), low-level assertions 
                        in the code are activated and debug information is stored (``-g -pg``)
                        
                        Once Ibex has been compiled with this option, you should also compile your executable
                        in debug mode. If you use the ``makefile`` of ``examples/``, simply write:: 

							make DEBUG=yes ...
							
BUILD_TESTING			Ex: ``-DBUILD_TESTING=0``

BUILD_SHARED_LIBS       Ex: ``-DBUILD_SHARED_LIBS=1``. 

                        Whether Ibex is also compiled as a dynamic library.
                        Possibles values are 0 (*only static libraries*) or 1 (*static and
                        dynamic libraries*).
                                           
                        If Ibex is compiled as a dynamic library in a local folder, you must set the library 
                        path accordingly to execute a program. Under Linux::

                        $ export LD_LIBRARY_PATH=[prefix]/lib/

                        Under MacOS::

                        $ export DYLD_LIBRARY_PATH=[prefix]/lib

                        Under MinGW::
                        
                        $ export PATH=$PATH:/c/MinGW/msys/1.0/home/[user]/Ibex/ibex-2.8.8/lib
                        
                        Under a Windows command window::
                        
                        > set PATH=%PATH%;C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.8.8\lib;C:\MinGW\bin
======================  ======================================================================================

                        
                        


   
.. _install-compiling-running:

=======================================
Compiling a Test Program
=======================================

Copy-paste the following example code in a file named ``foo.cpp`` ::
  
  #include "ibex.h"
  #include <iostream>

  using namespace std;
  using namespace ibex;

  int main(int argc, char** argv) {
    Interval x(0,1);
    cout << "My first interval:" << x << endl; 
  }

There is a simple "makefile" in the ``examples`` folder that you can use to compile your own programs (note: this makefile uses the extended syntax of GNU make).

This makefile however assumes ``pkg-config`` is installed on your system, which is done by default on many Linux distribution).
To install ``pkg-config`` under MinGW, follow the steps given `here <http://stackoverflow.com/questions/1710922/how-to-install-pkg-config-in-windows/22363820#22363820>`_.

So, place the file ``foo.cpp`` in the ``examples/`` folder and::

  ~/Ibex/ibex-2.8.8/$ cd examples 
  ~/Ibex/ibex-2.8.8/examples$ make foo 
  ~/Ibex/ibex-2.8.8/examples$ ./foo 
  
.. note::
   
   1. It may be necessary to set the ``PKG_CONFIG_PATH`` to *[prefix]*\ ``/share/pkgconfig`` where *[prefix]* is 
      ``/usr/local`` by default or whatever path specified via ``-DCMAKE_INSTALL_PREFIX``::

        ~/Ibex/ibex-2.8.8/$ export PKG_CONFIG_PATH=/usr/local/share/pkgconfig/ 


Under Windows, if you have compiled Ibex with ``-DBUILD_SHARED_LIBS=1`` you can run the program from a command window. 
Just update the path to dynamically link against Ibex::

	 > set IBEX_PATH=C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.8.8
	 > set PATH=%PATH%;%IBEX_PATH%\lib;C:\MinGW\bin
	 > cd %IBEX_PATH%\examples
	 > foo.exe



=======================================
Plugins/Packages
=======================================

There are many developments based on ibex which offer additional functionalities, typically algorithms tailored to some class of problems (e.g., parameter estimation, semi-infinite programming, etc.).

If Ibex is compiled with waf, they are rather *plugins* as they are installed and compiled with the core library. With Cmake, they are compiled separately and generate their own libraries. So we call them here *packages*.

The same steps must be done to compile a package as for the core library. See the current list of available plugins/packages in the `download page of Ibex <http://www.ibex-lib.org/download>`_.

We assume here that the package to be installed is ibex-ampl::

  ~/Ibex/ibex-ampl$ mkdir build
  ~/Ibex/ibex-ampl$ cd build
  ~/Ibex/ibex-ampl/build$ cmake ..
  ~/Ibex/ibex-ampl/build$ make
  ~/Ibex/ibex-ampl/build$ sudo make install
  
The ``CMAKE_INSTALL_PREFIX`` option allows to install the package locally, ex::

  ~/Ibex/ibex-ampl$ cmake -DCMAKE_INSTALL_PREFIX=~/Ibex/ibex-ampl ..

In this case the ``sudo`` before ``make install`` is unecessary.

If Ibex has been installed in a local folder, say ``~/Ibex/ibex-2.8.8`` you need to indicate this 
path using the ``CMAKE_PREFIX_PATH`` option::

  ~/Ibex/ibex-ampl$ cmake -DCMAKE_PREFIX_PATH=~/Ibex/ibex-2.8.8 ..
  
Of course, you can combine both::

	~/Ibex/ibex-ampl$ cmake -DCMAKE_PREFIX_PATH=~/Ibex/ibex-2.8.8 -DCMAKE_INSTALL_PREFIX=~/Ibex/ibex-ampl ..

As said above, packages result in separate libraries. To link you own code with a package, you will 
have to add the following line in the ``CMakeLists.txt`` file of your project::

    find_package(ibex-ampl REQUIRED)

=======================================
Running unit tests
=======================================
You can also run the whole unit tests suite with the installed version of Ibex.

To this end, you must install first the `cppunit library <https://sourceforge.net/projects/cppunit/>`_. Then run::

	~/Ibex$ cd ibex-2.8.8/build
	~/Ibex/ibex-2.8.8/build$ make check

=============
Uninstall
=============

.. _cmake_faq_clean: 

---------------
Linux/Macos
---------------

After running ``make install`` you can uninstall by running::

	~/Ibex$ cd ibex-2.8.8/build
	~/Ibex/ibex-2.8.8/build$ make uninstall


Then, you can remove the build directory::

  ~/Ibex$ cd ~/Ibex/ibex-2.8.8
  ~/Ibex/ibex-2-8-7$ rm -Rf build
	
---------------
Windows
---------------

*(to be completed)*

===============
Troubleshooting
===============
