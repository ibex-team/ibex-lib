***************************
Installation (Release 2.2)
***************************


.. _Gaol: http://sourceforge.net/projects/gaol
.. _Filib: http://www2.math.uni-wuppertal.de/~xsc/software/filib.html
.. _Profil/Bias: http://www.ti3.tu-harburg.de/keil/profil/index_e.html
.. _Cplex: http://www.ibm.com/software/commerce/optimization/cplex-optimizer
.. _Soplex 1.7.x: http://soplex.zib.de
.. _CLP: https://projects.coin-or.org/Clp
.. _Choco: http://www.emn.fr/z-info/choco-solver

===================================
Linux and MacOS
===================================

---------------
Fast install
---------------

The installation assumes your system meets some `requirements`_.

Save the archive ``ibex-2.2.0.tar.gz`` in some ``Ibex`` folder and::

  ~/Ibex/$ tar xvfz ibex-2.2.0.tar.gz 
  ~/Ibex/$ cd ibex-2.2.0 
  ~/Ibex/ibex-2.2.0/$ ./waf configure
  ~/Ibex/ibex-2.2.0/$ sudo ./waf install
    
**Note:** Ibex can either be compiled with `Gaol`_, `Filib`_ or `Profil/Bias`_. 
If your platform is 32 bits, the standard installation will automatically extract and build the Gaol library (and its dependencies) from the bundle, because Gaol is the fastest one. However, if your platform is 64 bits, it will extract and install Filib instead because the current release of Gaol does not support 64 bit platform. 

---------------------
Configuration options
---------------------

``waf`` ``configure`` supports the following options:
                    
--enable-shared     
                    Compile Ibex as a dynamic library.


--prefix=PREFIX     
                    Set the folder where Ibex must be installed (by default, ``/usr/local``).

                    You can use this option to install Ibex in a local folder.


--with-debug        
                    Compile Ibex in debug mode 

                    Compiler optimizations are all discarded (``-O0``), low-level assertions in the code are activated and debug information is stored (``-g -pg``)

                    Once Ibex has been compiled with this option, you should also compile your executable in debug mode. If you use the ``makefile`` of ``examples/``, simply write:: 

                           make DEBUG=yes ...
    
   

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

.. _install-compiling-running:

--------------------------------
Compiling and running examples
--------------------------------

There is a simple "makefile" in the ``examples/`` folder that you can use to compile examples as well as your own programs.
This makefile however assumes ``pkg-config`` is installed on your system (which is done by default on many Linux distribution).

**Note:** it may be necessary to set the ``PKG_CONFIG_PATH`` to *PREFIX*\ ``/share/pkgconfig`` where *PREFIX* is 
``/usr/local`` by default or whatever path specified via ``--prefix``::

  ~/Ibex/ibex-2.2.0/$ export PKG_CONFIG_PATH=/usr/local/share/pkgconfig/ 

Here is an example::

  ~/Ibex/ibex-2.2.0/$ cd examples 
  ~/Ibex/ibex-2.2.0/$ make defaultsolver 
  ~/Ibex/ibex-2.2.0/$ ./defaultsolver ../benchs/cyclohexan3D.bch 1e-05 10 

The default solver solves the systems of equations in argument (cyclohexan3D) with a precision less than 1e-05 and
within a time limit of 10 seconds.

To compile your own program, just copy-paste the makefile of ``Ibex/examples``.

**Note:** this makefile uses the extended syntax of GNU make.

------------
Uninstall
------------

Simply type in the path of IBEX (under the shell of MinGW for Windows)::

  ~/Ibex/ibex-2.2.0$ sudo ./waf uninstall  
  ~/Ibex/ibex-2.2.0$ ./waf distclean  

**Note:** sudo is useless if Ibex is installed in a local folder.

It is highly recommended to uninstall Ibex like this before upgrading to a new release.

===================================
Windows
===================================

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

- Save the archive ``ibex-2.2.0.tar.gz`` in ``C:\MinGW\msys\1.0\home\[user]\Ibex``
- Configure Ibex (still in the shell of MinGW)::

  ~/Ibex/$ export PATH="$PATH:/c/Python27" 
  ~/Ibex/$ tar xvfz ibex-2.2.0.tar.gz 
  ~/Ibex/$ cd ibex-2.2.0 
  ~/Ibex/ibex-2.2.0/$ ./waf configure --prefix=C:\\MinGW\\msys\\1.0\\home\\[user]\\Ibex\\ibex-2.2.0 
  
  **Note:** the paths must be entered in Windows-style and with double backslash ("\\") as separator.
  
- Install Ibex::

  ~/Ibex/ibex-2.2.0/$ ./waf install

  **Warning:** for mysterious reasons, the command sometimes gets frozen (this was observed while compiling Filib). Use Control-C to interrupt the command and run it again. Do this several times until compilation is over.

  **Note:** if g++ is not found, it probably means that you have not run the "postinstall" script of MinGW (see above).
  

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
        

