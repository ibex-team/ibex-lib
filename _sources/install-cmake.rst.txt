
.. _ibex-install-cmake:

*******************************************
Installation
*******************************************

.. _MacPorts: https://www.macports.org
.. _Gaol: http://sourceforge.net/projects/gaol
.. _Filib: http://www2.math.uni-wuppertal.de/~xsc/software/filib.html
.. _Profil/Bias: http://www.ti3.tu-harburg.de/keil/profil/index_e.html
.. _Cplex: http://www.ibm.com/software/commerce/optimization/cplex-optimizer
.. _Soplex 1.7.x: http://soplex.zib.de
.. _CLP: https://projects.coin-or.org/Clp
.. _ZIB: http://scip.zib.de/academic.txt
.. _Choco: https://choco-solver.org/

===================================
Standard install
===================================


---------------
Linux and MacOS
---------------

**Notice:** 

1. Ibex can either be compiled with `Gaol`_ or `Filib`_ . The library installed by default on Linux and MacOS is Gaol, since our experiments have shown that this library prevails over the other.
   If Gaol is not found on your system, the installation automatically extract and build the Gaol library (and its dependencies) from the bundle. Is is actually better to resort to this automatic installation because we use some optimization flags that might not be set if you have installed it on you own. So if Gaol is already installed, we advice you to force the automatic installation by using ``-DGAOL_DIR=`` (i.e., with empty argument)

2.  It is also highly recommend to compile Ibex with the ``-DLP_LIB`` option and to use soplex. However, Soplex is under `ZIB`_ academic licence. **If you intend to use Ibex with Soplex commercially**, you need to contact Soplex for a commercial licence.

First, make sure the following applications are installed on your system:

- g++
- flex
- bison
- cmake
- git
- pkg-config (*optionnal but recommended*)

On Ubuntu, you can install all you need with::

  ~$ sudo apt-get install -y g++ gcc flex bison cmake git pkg-config

Installation::

  ~/Ibex$ git clone https://github.com/ibex-team/ibex-lib.git
  ~/Ibex$ cd ibex-lib
  ~/Ibex/ibex-lib$ mkdir build
  ~/Ibex/ibex-lib/build$ cd build
  ~/Ibex/ibex-lib/build$ cmake -DLP_LIB=soplex ..
  ~/Ibex/ibex-lib/build$ make
  ~/Ibex/ibex-lib/build$ sudo make install

You can now run ibexsolve::

  ~/Ibex/ibex-lib/build$ ibexsolve -v
        
If you want to install Ibex in a local folder, use ``-DCMAKE_INSTALL_PREFIX``, ex::

	~/Ibex/ibex-lib/build$ cmake -DCMAKE_INSTALL_PREFIX=$HOME ..

See the :ref:`options <install-cmake-options>`. The folder full path must not contain any **white space** or wierd characters like ``'"\()`*[]``.

-----------------
Windows
-----------------

**Important notice:** 

1. Ibex can either be compiled with `Gaol`_ or `Filib`_  and our experiments have shown that Gaol prevails over Filib. However, because Gaol does not compile under Windows, the library installed by default under this platform to date is Filib (this may be changed in the future). So, you may not have the best performances of Ibex under this platform.

2.  It is also highly recommend to compile Ibex with the ``-DLP_LIB`` option and to use soplex. However, Soplex is under `ZIB`_ academic licence. **If you intend to use Ibex with Soplex commercially**, you need to contact Soplex for a commercial licence.

The following procedure has been tested with Visual Studio compiler.

First, install `Visual Studio <https://visualstudio.microsoft.com/fr/>`_, `Git <https://git-scm.com>`_  and `Chocolatey <https://chocolatey.org/install>`_.

Then, open a command with **administrator** privileges and type the following, potentially replacing the version of VisualStudio
with another one::

  git clone https://github.com/ibex-team/ibex-lib.git
  choco install patch
  choco install winflexbison
  cd ibex-lib
  mkdir build
  cd build
  cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -DLP_LIB=soplex ..
  cmake --build . --config Release
  cmake --install . --config Release
  
You can now run ibexsolve::

  set IBEX_PATH=C:\Program Files (x86)\IBEX
  set PATH=%PATH%;%IBEX_PATH%\bin
  ibexsolve -b
  
The first instruction add the binary folder of Ibex to your path.
    
.. _install-cmake-options:

==============================
Configuration options
==============================	

``cmake`` supports the following generic and ibex-specific options:
                    
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
                       
LP_LIB                  Ex: ``-DLP_LIB=soplex``
                        
                        Set the linear programming library. 

                        Possible values are either ``soplex``, ``clp`` or ``none``. Default is ``none``. 
                        
                        .. warning::
							
							Using Ibex with CLP is experimental. 
							We do not provide yet any guarantee nor support with this LP solver.									

                        We recommend to use soplex.
                        
                        The archive contains a version of Soplex and CLP so it is not necessary to have Soplex or CLP
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
						                     
							 The current release of Ibex (>=2.9.0) is compatible with Soplex 4.


GAOL_DIR                Ex: ``-DINTERVAL_LIB=goal -DGAOL_DIR=$HOME/gaol``

                        Set the path of Goal when using Ibex with your own version of Goal.
                        Note that the plugin archive embeds a version of Gaol so it is not necessary to have 
                        Goal already installed on your system. Default value is "" (means: use embedded version).
						
                        Only to be used with ``-DINTERVAL_LIB=goal``.
                        
                        .. 	note::
						                     
							 To date, this option does not work under Windows.

MATHLIB_DIR             Ex: ``-DINTERVAL_LIB=goal -DMATHLIB_DIR=$HOME/matlib``
                        
                        Set the path of Mathlib when using Ibex with your own version of Goal.
                        Mathlib is a tierce library required by Gaol.
						
                        Note that the plugin archive embeds a version of Gaol so it is not necessary to have 
                        Goal already installed on your system. Default value is "" (means: use embedded version).
                        
                        .. 	note::
						                     
							 To date, this option does not work under Windows.
					
FILIB_DIR               Ex: ``-DINTERVAL_LIB=filib -DFILIB_DIR=$HOME/filib``

                        Set the path of Filib when using Ibex with your own version of Filib. 
                        Note that the plugin archive embeds a version of Filib so it is not necessary to have 
                        Filib already installed on your system. Default value is "" 
                        (means: use embedded version).
						
                        Only to be used with ``-DINTERVAL_LIB=filib``.
						
						.. note::
							
						   To date, this option is not available under MacOS with arm64 architecture because
						   (Filib give unreliable results).

SOPLEX_DIR              Ex: ``-DLP_LIB=soplex -DSOPLEX_DIR=$HOME/soplex``

                        Set the path of Soplex when using Ibex with your own version of Soplex. Note that the 
                        plugin archive contains a version of Soplex so this option is not required.
                        
                        If Ibex is compiled as a shared library, you must also add the libpath of Soplex 
                        in ``LD_LIBRARY_PATH``::
						
							~/Ibex/ibex-lib/$ export LD_LIBRARY_PATH=[prefix]/lib/:[soplex-path]/lib/
					
                        Under Windows, if you run a program from a command window, the ``PATH`` variable must 
                        also be updated::
					
							> set IBEX_PATH=C:\Program Files (x86)\IBEX
							> set SOPLEX_PATH=...
							> set PATH=%PATH%;%IBEX_PATH%\bin;%SOPLEX_PATH%\lib
							
                        Only to be used with ``-DLP_LIB=soplex``.
						
CLP_DIR                 Ex: ``-DLP_LIB=clp -DCLP_DIR=$HOME/clp``

                        Set the path of CLP when using Ibex with your own version of CLP. 
                        Note that the plugin archive embeds a version of CLP so it is not necessary to have 
                        CLP already installed on your system. Default value is "" 
                        (means: use embedded version).
						
                        Only to be used with ``-DLP_LIB=clp``.
                        
                        .. warning::
							
							Using Ibex with CLP is experimental. 
							We do not provide yet any guarantee nor support with this LP solver.									
                        
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

                        $ export LD_LIBRARY_PATH=[prefix]/lib/:[prefix]/lib/ibex/3rd

                        Under MacOS::

                        $ export DYLD_LIBRARY_PATH=[prefix]/lib:[prefix]/lib/ibex/3rd

                        Under Windows::
                        
                        > set IBEX_PATH=C:\Program Files (x86)\IBEX
                        > set PATH=%PATH%;%IBEX_PATH%\bin;%IBEX_PATH%\lib
                        
BUILD_JAVA_INTERFACE	Ex: ``-DBUILD_JAVA_INTERFACE=ON -DBUILD_SHARED_LIBS=ON``
                        
                        Enable the java interface. 
                        The Java interface of Ibex allows to use Ibex with `Choco`_, for solving mixed integer-continuous CSP (constraint satisfaction problems).
                        This option requires a JDK to be installed.
                        
                        This option requires JAVA_HOME to be set.
                        
                        **Note**
                        
                        - Up to ibex 2.8.9, the java interface comes as a separate `plugin <https://github.com/ibex-team/ibex-java/>`_. See instructions there.
                        - building ibex as a dynamic library is mandatory for enabling the Java interface.
                        
                           
JAVA_PACKAGE            Ex: ``-DJAVA_PACKAGE=org.chocosolver.solver.constraints.real``
                        
                        This option is only to be used with ``-DBUILD_JAVA_INTERFACE=ON``.
                        
                        Set the name of the Java package in which the ibex Java interface must be generated.
                        This option will create a [package name].jar file and put it into ``[prefix]/share/java`` where [prefix] is 
                        ``/usr/local`` under Linux/Macos or ``C:\Program Files (x86)\IBEX`` under Windows by default or whatever path specified via ``CMAKE_INSTALL_PREFIX``
                        
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

This makefile however assumes ``pkg-config`` is installed on your system, which is done by default on many Linux distribution.

So, place the file ``foo.cpp`` in the ``examples/`` folder and::

  ~/Ibex/ibex-lib/$ cd examples 
  ~/Ibex/ibex-lib/examples$ make foo 
  ~/Ibex/ibex-lib/examples$ ./foo 
  
.. note::
   
   1. It may be necessary to set the ``PKG_CONFIG_PATH`` to *[prefix]*\ ``/share/pkgconfig`` where *[prefix]* is 
      ``/usr/local`` by default or whatever path specified via ``-DCMAKE_INSTALL_PREFIX``::

        ~/Ibex/ibex-lib/$ export PKG_CONFIG_PATH=/usr/local/share/pkgconfig/ 


Under Windows, if you have compiled Ibex with ``-DBUILD_SHARED_LIBS=1`` you can run the program from a command window. 
Just update the path to dynamically link against Ibex::

	 > set IBEX_PATH=C:\Program Files (x86)\IBEX
	 > set PATH=%PATH%;%IBEX_PATH%\bin;%IBEX_PATH%\lib
	 > cd %IBEX_PATH%\examples
	 > foo.exe

.. _install-compiling-java:

==========================================
Compiling a Test Program (Java interface)
==========================================

Copy-paste the following example code in a file named ``Test.java``,
possibly replacing the package name (here `ibex`) with the appropriate one
(the one specified with the ``JAVA_PACKAGE`` option)::

  import ibex.Ibex;


  class Test {
    public static void main(String[] args) {
		
      Ibex ibex=new Ibex(new double[]{1e-2,-1});
      ibex.add_ctr("{0}-{1}=0"); 
		
		
      double domains[]={1.5,10.5,5.5,12.0};
      System.out.println("Before contract:");
      System.out.println("(["+domains[0]+","+domains[1]+"] ; ["+domains[2]+","+domains[3]+"])");
		
      int result=ibex.contract(0,domains,1e-3);
		
      if (result==Ibex.FAIL) {
        System.out.println("Failed!");
      } else if (result==Ibex.CONTRACT) {
        System.out.println("After contract #0:");
        System.out.println("(["+domains[0]+","+domains[1]+"] ; ["+domains[2]+","+domains[3]+"])");
      } else {
        System.out.println("Nothing.");	
      }		
    }
  }

--------------------------------------
Under Linux/Macos
--------------------------------------

Compile it as follows (possibly replacing `/usr/local` by the folder you specified via ``CMAKE_INSTALL_PREFIX``)::


  javac -cp /usr/local/share/java/ibex.jar Test.java 

Then run the test program as follows::

  export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/ibex/3rd
  java -cp .:/usr/local/share/java/ibex.jar Test

It should display::

  Before contract:
  ([1.5,10.5] ; [5.5,12.0])
  Nothing.
  
If this test works, you're ready to go with Choco!

--------------------------------------
Under Windows
--------------------------------------

Under the command window::

  set IBEX_PATH=C:\Program Files (x86)\IBEX
  set PATH=%PATH%;%IBEX_PATH%\bin;%IBEX_PATH%\lib
  javac -cp "%IBEX_PATH%\share\java\ibex.jar" Test.java

Then run the test program as follows::

  java -cp ".;%IBEX_PATH%\share\java\ibex.jar" Test

It should display::

  Before contract:
  ([1.5,10.5] ; [5.5,12.0])
  Nothing.
  
If this test works, you're ready to go with Choco!

=======================================
Additional Packages
=======================================

There are many developments based on ibex which offer additional functionalities, typically algorithms tailored to some class of problems (e.g., parameter estimation, semi-infinite programming, etc.).

.. 	note::
  
    Up to release 2.8, Ibex is compiled with waf (instead of cmake) and packages are rather called *plugins* as they are installed and compiled with the core library. With Cmake (Release 2.9), they are compiled separately and generate their own libraries.

The same steps must be done to compile a package as for the core library. See the current list of available packages on Github in the `ibex team page <https://github.com/ibex-team>`_.

We assume here that the package to be installed is ibex-ampl::

  ~/Ibex/ibex-ampl$ mkdir build
  ~/Ibex/ibex-ampl$ cd build
  ~/Ibex/ibex-ampl/build$ cmake ..
  ~/Ibex/ibex-ampl/build$ make
  ~/Ibex/ibex-ampl/build$ sudo make install
  
The ``CMAKE_INSTALL_PREFIX`` option allows to install the package locally, ex::

  ~/Ibex/ibex-ampl$ cmake -DCMAKE_INSTALL_PREFIX=~/Ibex/ibex-ampl ..

In this case the ``sudo`` before ``make install`` is unecessary.

If Ibex has been installed in a local folder, say ``~/Ibex/ibex-lib`` you need to indicate this 
path using the ``CMAKE_PREFIX_PATH`` option::

  ~/Ibex/ibex-ampl$ cmake -DCMAKE_PREFIX_PATH=~/Ibex/ibex-lib ..
  
Of course, you can combine both::

	~/Ibex/ibex-ampl$ cmake -DCMAKE_PREFIX_PATH=~/Ibex/ibex-lib -DCMAKE_INSTALL_PREFIX=~/Ibex/ibex-ampl ..

As said above, packages result in separate libraries. To link you own code with a package, you will 
have to add the following line in the ``CMakeLists.txt`` file of your project::

    find_package(ibex-ampl REQUIRED)

=======================================
Running unit tests
=======================================
You can also run the whole unit tests suite with the installed version of Ibex.

To this end, you must install first the `cppunit library <https://sourceforge.net/projects/cppunit/>`_. 

The, under Linux/MacOS::

	~/Ibex$ cd ibex-lib/build
	~/Ibex/ibex-lib/build$ make check

Under Windows, use::

  cmake --build . --config Release --target check

=============
Uninstall
=============

.. _cmake_faq_clean: 

---------------
Linux/Macos
---------------

After running ``make install`` you can uninstall by running::

	~/Ibex$ cd ibex-lib/build
	~/Ibex/ibex-lib/build$ make uninstall


Then, you can remove the build directory::

  ~/Ibex$ cd ~/Ibex/ibex-lib
  ~/Ibex/ibex-lib$ rm -Rf build
	
---------------
Windows
---------------

*(to be completed)*
