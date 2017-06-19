
.. _ibex-install:

***************************
Installation (Release 2.4)
***************************


.. _Gaol: http://sourceforge.net/projects/gaol
.. _Filib: http://www2.math.uni-wuppertal.de/~xsc/software/filib.html
.. _Profil/Bias: http://www.ti3.tu-harburg.de/keil/profil/index_e.html
.. _Cplex: http://www.ibm.com/software/commerce/optimization/cplex-optimizer
.. _Soplex 1.7.x: http://soplex.zib.de
.. _CLP: https://projects.coin-or.org/Clp
.. _Choco: http://www.emn.fr/z-info/choco-solver

===================================
Standard install
===================================

**Note:** Ibex can either be compiled with `Gaol`_, `Filib`_ or `Profil/Bias`_. 
The library installed by default is Gaol, since our experiments have shown that this library prevails over the others.
So the standard installation will automatically extract and build the Gaol library (and its dependencies) from the bundle.
However, because of some installation problems with Gaol under Windows, the library installed by default under this platform
as of today is Filib (this will be changed in the future).

---------------
Linux and MacOS
---------------

The installation assumes your system meets some `requirements`_.

Save the archive ``ibex-2.4.0.tar.gz`` in some ``Ibex`` folder and::

  ~/Ibex/$ tar xvfz ibex-2.4.0.tar.gz 
  ~/Ibex/$ cd ibex-2.4.0 
  ~/Ibex/ibex-2.4.0/$ ./waf configure
  ~/Ibex/ibex-2.4.0/$ sudo ./waf install
    


-----------------
Windows
-----------------


.. _MinGW+Msys: https://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/
.. _Python2: https://www.python.org/download/releases/2.7.3/

- Install `MinGW+Msys`_. 
   Select the following components to be installed:

   * A MSYS Installation for MinGW Developers
   * A Basic MinGW Installation 
   * The GNU C++ Compiler

- With recent releases of MinGW, you have to run the executable ``pi.bat`` in ``C:\MinGW\msys\1.0\postinstall`` 
- Install `Python2`_ (**warning**: the script are currently not compatible with python3)
- Create a new directory ``Ibex`` in the shell of MinGW (to open the shell of MinGW, click on ``Start -> MinGW -> MinGWShell``)::

    mkdir Ibex
    
  **Note:** if you don't have MinGW in your Start menu, run the executable file ``msys.bat`` in ``C:\MinGW\msys\1.0``.

- Assuming you have installed MinGW in ``C:\``, the ``Ibex`` directory you have created should be located on your system here::

    C:\MinGW\msys\1.0\home\[user]\Ibex

  We will assume now that this folder is the root folder of ibex.

- Save the archive ``ibex-2.4.0.tar.gz`` in ``C:\MinGW\msys\1.0\home\[user]\Ibex``
- Configure Ibex (still in the shell of MinGW)::

  ~/Ibex/$ export PATH="$PATH:/c/Python27" 
  ~/Ibex/$ tar xvfz ibex-2.4.0.tar.gz 
  ~/Ibex/$ cd ibex-2.4.0 
  ~/Ibex/ibex-2.4.0/$ ./waf configure --prefix=/c/MinGW/msys/1.0/home/[user]/Ibex/ibex-2.4.0 
  
  **Note:** the paths must be entered in Linux-style (don't use backslash ("\\") as separator).
  
- Install Ibex::

  ~/Ibex/ibex-2.4.0/$ ./waf install

.. note:: 

   For mysterious reasons, the command sometimes gets frozen (this was observed while compiling Filib). Use Control-C to interrupt the command and run it again. Do this several times until compilation is over.


**Note:** if g++ is not found, it probably means that you have not run the "postinstall" script of MinGW (see above).
  
  
---------------
Requirements
---------------

The following applications must be installed.

- g++
- gcc
- flex
- bison
- python2.x (**warning**: the script are currently not compatible with python3)
- make
- pkg-config (*optionnal*)

On Ubuntu, you can install all you need with::

  ~$ sudo apt-get install -y python2.7 flex bison gcc g++ make pkg-config
  
==============================
Configuration options
==============================	

The full list of options supported by ``waf configure`` can be obtained with::

	~/Ibex/ibex-2.4.0/$ ./waf --help
	
This will display the full list of installed interval libraries and plugins with their specific options.

In particular, ``waf`` ``configure`` supports the following options:
                    
--enable-shared     
                    Compile Ibex as a dynamic library.

					If Ibex is compiled as a dynamic library in a local folder, you must set the library path accordingly to execute a program. Under Linux::

						$ export LD_LIBRARY_PATH=[prefix]/lib/

					Under MacOS::

						$ export DYLD_LIBRARY_PATH=[prefix]/lib

					Under MinGW::

						$ export PATH=$PATH:/c/MinGW/msys/1.0/home/[user]/Ibex/ibex-2.4.0/lib

					Under a Windows command window::

						C:\...> set PATH=%PATH%;C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.4.0\lib;C:\MinGW\bin

--prefix=PREFIX     
                      Set the folder where Ibex must be installed (by default, ``/usr/local``).

                      You can use this option to install Ibex in a local folder.


--with-debug        
                      Compile Ibex in debug mode 

                      Compiler optimizations are all discarded (``-O0``), low-level assertions in the code are activated and debug information is stored (``-g -pg``)

                      Once Ibex has been compiled with this option, you should also compile your executable in debug mode. If you use the ``makefile`` of ``examples/``, simply write:: 

                           make DEBUG=yes ...

--interval-lib=gaol   Use Gaol as interval library (recommended)

--interval-lib=filib  Use Filib as interval library

--interval-lib=bias   Use Profil/Bias as interval library (legacy: support not guaranteed)

--interval-lib=direct 
                      Use non-rigorous interval arithmetic (essentially for embedded systems with specific processor architectures that
                      do not support rounding modes) (experimental: support not guaranteed)


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
    cout << "My first interval: << x << endl; 
  }

There is a simple "makefile" in the ``examples`` folder that you can use to compile your own programs (note: this makefile uses the extended syntax of GNU make).

This makefile however assumes ``pkg-config`` is installed on your system, which is done by default on many Linux distribution).
To install ``pkg-config`` under MinGW, follow the steps given `here <http://stackoverflow.com/questions/1710922/how-to-install-pkg-config-in-windows/22363820#22363820>`_.

So, place the file ``foo.cpp`` in the ``examples/`` folder and::

  ~/Ibex/ibex-2.4.0/$ cd examples 
  ~/Ibex/ibex-2.4.0/examples$ make foo 
  ~/Ibex/ibex-2.4.0/examples$ ./foo 
  
.. note::
   
   1. It may be necessary to set the ``PKG_CONFIG_PATH`` to *[prefix]*\ ``/share/pkgconfig`` where *[prefix]* is 
      ``/usr/local`` by default or whatever path specified via ``--prefix``::

        ~/Ibex/ibex-2.4.0/$ export PKG_CONFIG_PATH=/usr/local/share/pkgconfig/ 


Under Windows, if you have compiled Ibex with ``--enable--shared`` you can run the program from a command window. 
Just update the path to dynamically link against Ibex::

	 > set IBEX_PATH=C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.4.0
	 > set PATH=%PATH%;%IBEX_PATH%\lib;C:\MinGW\bin
	 > cd %IBEX_PATH%\examples
	 > foo.exe

=======================================
Running unit tests
=======================================
You can also run the whole unit tests suite with the **installed** version of Ibex.

To this end, you must install first the `cppunit library <https://sourceforge.net/projects/cppunit/>`_. Then run::

	~/Ibex/ibex-2.4.0/$ ./waf utest
	
Note also the following command::

	~/Ibex/ibex-2.4.0/$ ./waf check
	
as a handy shortcut for::

	~/Ibex/ibex-2.4.0/$ ./waf build install clean utest


=============
Uninstall
=============

Simply type in the path of IBEX (under the shell of MinGW for Windows)::

  ~/Ibex/ibex-2.4.0$ sudo ./waf uninstall  
  ~/Ibex/ibex-2.4.0$ ./waf distclean  

**Note:** sudo is useless under MinGW or if Ibex is installed in a local folder.

It is highly recommended to uninstall Ibex like this before upgrading to a new release or installing a plugin.


===============
Troubleshooting
===============

-------------------------
Headers of Gaol not found
-------------------------

When running ``waf configure``, I get messages like this::

  Checking for header ['gaol/gaol.h', 'gaol/gaol_interval.h']   : not found
   ...

Does it mean that Ibex is not properly installed?


**Answer:** No, this mesage simply indicates that gaol was not found on your system and that it will be automatically extracted from the bundle. It is not an error message.
        

