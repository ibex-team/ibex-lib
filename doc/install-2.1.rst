***************************
Installation (Release 2.1)
***************************

.. _Gaol: http://sourceforge.net/projects/gaol
.. _Filib: http://www2.math.uni-wuppertal.de/~xsc/software/filib.html
.. _Profil/Bias: http://www.ti3.tu-harburg.de/keil/profil/index_e.html
.. _Cplex: http://www.ibm.com/software/commerce/optimization/cplex-optimizer
.. _Soplex 1.7.x: http://soplex.zib.de
.. _CLP: https://projects.coin-or.org/Clp
.. _Choco: http://www.emn.fr/z-info/choco-solver

===================================
Standard installation (recommended)
===================================

Ibex can either be compiled with `Gaol`_, `Filib`_ or `Profil/Bias`_. 
Ibex also relies on a LP solver that can either be `Cplex`_, `Soplex 1.7.x`_ or `CLP`_.

If your platform is 32 bits, the standard installation will automatically extract and build the Gaol library (and its dependencies) from the bundle, because Gaol is the fastest one. However, if your platform is 64 bits, it will extract and install Filib instead because the current release of Gaol does not support 64 bit platform. 
Note that it is still possible to compile Ibex with Gaol under 64 bits platform
using the ``--with-gaol`` option but, in this case, Ibex will be installed as a static 32-bits library (which may
cause linking problems with other libraries).

The standard installation will also make you install Soplex in a specific folder.
Finally, it will install Ibex on your system.


See also the `customized installation`_.

---------------
Linux and MacOS
---------------

The installation assumes your system meets the `requirements`_


^^^^^^^^^^^^^^^^^^^^^^^^^^^
Fast install (experimental)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Save the archive ``ibex-2.1.16.tar.gz`` in some ``Ibex`` folder and::

  ~/Ibex/$ tar xvfz ibex-2.1.16.tar.gz 
  ~/Ibex/$ cd ibex-2.1.16 
  ~/Ibex/ibex-2.1.16/$ ./waf configure
  ~/Ibex/ibex-2.1.16/$ sudo ./waf install

**Note:** the fast install automatically extracts and install CLP from the bundle. However, the compilation of CLP is not working on all platforms.
If it fails, try to install the ubuntu package for Clp (see the `requirements`_) or try the procedure below with Soplex.

^^^^^^^^^^^^^^^^^^^^^^^^^
Standard install (stable)
^^^^^^^^^^^^^^^^^^^^^^^^^

- Create a new directory ``Ibex``::

  ~$ mkdir Ibex

- Download `Soplex 1.7.x`_ and save the archive in ``Ibex``. **Warning:** The current release of Ibex is not compatible with Soplex 2.0.
- Compile Soplex::

  ~$ cd Ibex 
  ~/Ibex$ tar xvfz soplex-1.7.2.tgz  
  ~/Ibex$ cd soplex-1.7.2  
  ~/Ibex/soplex-1.7.2$ make ZLIB=false
  ~/Ibex/soplex-1.7.2$ cd ..


- Save the archive ``ibex-2.1.16.tar.gz`` in ``Ibex``
- Install and configure Ibex::

  ~/Ibex/$ tar xvfz ibex-2.1.16.tar.gz 
  ~/Ibex/$ cd ibex-2.1.16 
  ~/Ibex/ibex-2.1.16/$ ./waf configure --with-soplex=[soplex-path]

  where *[soplex-path]* is the absolute path where soplex is installed (don't use the relative path ``--with-soplex=../soplex-1.7.2``).

- Compile Ibex::

  ~/Ibex/ibex-2.1.16/$ sudo ./waf install


^^^^^^^^^^^^ 
Requirements
^^^^^^^^^^^^ 

.. _COIN-OR: https://projects.coin-or.org/Clp

The following applications must be installed.

- g++
- gcc
- flex
- bison
- python2.x (**warning**: the script are currently not compatible with python3)
- make
- pkg-config (*optionnal*)
- jdk (*optionnal*)

On Ubuntu, you can install all you need with::

  ~$ sudo apt-get install -y python2.7 flex bison gcc g++ make pkg-config

The Linear Solver Clp from the project `COIN-OR`_ can be install from an official ubuntu package::

  ~$ sudo apt-get install -y coinor-libclp-dev


-------
Windows
-------

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

- Download `Soplex 1.7.x`_ and save the archive in  ``C:\MinGW\msys\1.0\home\[user]\Ibex``. 

  **Warning:** The current release of Ibex is not compatible with Soplex 2.0.

- Uncompress and compile Soplex in ``C:\MinGW\msys\1.0\home\[user]\Ibex`` (still in the shell of MinGW)::

  ~$ cd Ibex 
  ~/Ibex$ tar xvfz soplex-1.7.2.tgz  
  ~/Ibex$ cd soplex-1.7.2  
  ~/Ibex/soplex-1.7.2$ make ZLIB=false   
  ~/Ibex/soplex-1.7.2$ cd ..

  **Note:** if g++ is not found, it probably means that you have not run the "postinstall" script of MinGW (see above).
  
- Save the archive ``ibex-2.1.16.tar.gz`` in ``C:\MinGW\msys\1.0\home\[user]\Ibex``
- Configure Ibex (still in the shell of MinGW)::

  ~/Ibex/$ export PATH="$PATH:/c/Python27" 
  ~/Ibex/$ tar xvfz ibex-2.1.16.tar.gz 
  ~/Ibex/$ cd ibex-2.1.16 
  ~/Ibex/ibex-2.1.16/$ ./waf configure --prefix=C:\\MinGW\\msys\\1.0\\home\\[user]\\Ibex\\ibex-2.1.16 --with-soplex=C:\\MinGW\\msys\\1.0\\home\\[user]\\Ibex\\soplex-1.7.2

  **Note:** the paths must be entered in Windows-style and with double backslash ("\\") as separator.
  
  **Note:** if you have any trouble to install a linear solver (Clp, Soplex or Cplex), you can install Ibex without linear solver by added the option ``--without-lp``::
  
      ~/Ibex/ibex-2.1.16/$ ./waf configure --without-lp --prefix=C:\\MinGW\\msys\\1.0\\home\\[user]\\Ibex\\ibex-2.1.16 
  

- Install Ibex::

  ~/Ibex/ibex-2.1.16/$ ./waf install

  **Warning:** for mysterious reasons, the command sometimes gets frozen (this was observed while compiling Filib). Use Control-C to interrupt the command and run it again. Do this several times until compilation is over.

.. _install-custom:

=======================
Customized Installation
=======================

---------------------
Configuration options
---------------------

``waf`` ``configure`` supports the following options:

--with-gaol=GAOL_PATH    
                    Compile Ibex with Gaol. 

		    If *GAOL_PATH* is empty (just type the "=" symbol with nothing after), Gaol will be automatically extracted from the bundle.
                    Otherwise, Gaol will be looked for at the given path (which means that you must have installed it by yourself).
                     
                    **Note if you are under 64 bits platform**. It is recommended in this case to use the version of Gaol supplied
                    with Ibex (empty *GAOL_PATH*). This version has been patched to force compilation as a 32-bits library (Gaol does not support 64 bits). 
                    You must also compile Soplex with the following option::
        
                    ~/Ibex/soplex-1.7.2$ make ARCH=x86 ZLIB=false

                    Finally, g++-multilib is required (for the -m32 option).

                    **Note if you use your own version of Gaol**. The configuration will also automatically look for two other librairies required by Gaol: **gdtoa** and **ultim** (mathlib).
                    It is assumed that these librairies are installed under standard system folders (like ``/usr/lib``) or under *GAOL_PATH*\ ``/lib``. Otherwise, you have to set compiler
                    flags (e.g., ``CPPFLAGS=-I/[ultim-path]/include`` and ``LDFLAGS=-L/[ultim-path]/lib``).
                    To get good performances it is also highly recommended to have configured Gaol with the options ``--disable-preserve-rounding --enable-optimize``.

--with-bias=BIAS_PATH  
                    Compile Ibex with Profil/Bias.

                    Note that the bundle only contains Gaol and Filib++, not Profil/Bias. This library must already be installed on your machine at the given path.

                    **Note:** To compile Profil/Bias on a MacOS 64 bits platform, unzip :download:`this file <x86_64-MaxOSX-compat-gcc.tgz>` under *BIAS_PATH*\ ``/config``.
                    Combining ``--enable-shared`` with ``--with-bias`` is not supported under Mac OS 64 bits.


--with-filib=FILIB_PATH  
                    Compile Ibex with Filib++. 

                    If *FILIB_PATH* is empty (just type the "=" symbol with nothing after), Filib++ will be automatically extracted from the bundle.
                    Otherwise, Filib++ will be looked for at the given path (which means that you must have installed it by yourself).
                    
                    
--with-soplex=SOPLEX_PATH  
                    Look for Soplex at the given path instead of the parent directory.
                    
                    **Warning:** The current release of Ibex is not compatible with Soplex 2.0.


--with-cplex=CPLEX_PATH  
                    Link Ibex with Cplex (instead of Soplex). Cplex is searched at the given path.


--enable-shared     
                    Compile Ibex as a dynamic library. See `installation as a dynamic library`_.


--with-jni          
                    Compile Ibex with the Java interface 

                    This interface is used by `Choco`_. Ibex itself will be compiled as a dynamic library (as if configured with ``--enable-sharerd``).
                    
                    See `installation as a dynamic library`_.


--with-java-package=PACKAGE_NAME
                    To be used with ``--with-jni`` only. Set the Java package name of the ``Ibex`` class. By default, the package name is ``ibex``.
                    

--prefix=PREFIX     
                    Set the folder where Ibex must be installed (by default, ``/usr/local``).

                    You can use this option to install Ibex in a local folder.


--with-debug        
                    Compile Ibex in debug mode 

                    Compiler optimizations are all discarded (``-O0``), low-level assertions in the code are activated and debug information is stored (``-g -pg``)

                    Once Ibex has been compiled with this option, you should also compile your executable in debug mode. If you use the ``makefile`` of examples/, simply write:: 

                           make DEBUG=yes ...


--without-lp        
                    Compile Ibex without Linear Solver

                    You can use this option if you have some trouble to install Clp, Cplex or Soplex.
                    
                    This option will disable the contractor ``CtcPolytopeHull``.
                    
---------------------------------
Installation as a dynamic library
---------------------------------

You can install Ibex as a dynamic library either using ``--enable-shared`` or ``--with-jni``.
        
We recommend you, in this case, to use the interval libraries supplied with Ibex.
Indeed, if Ibex is compiled with Profil/Bias or your own version of Gaol, you may run into problems at some point, even if the installation apparenlty succeeds 
(this is due to the -fPIC option missing in the installation of these libraries). 

**Note:** Combining ``--enable-shared`` with ``--with-bias`` is not supported under Mac OS 64 bits.
   
There are only a few differences with the standard installation:

- **Linux and MacOS**

  Compile Soplex as follows::
  
    ~/Ibex/ibex-2.1.16/soplex-1.7.2$ make SHARED=true ZLIB=false

- **Windows**

  Compile Soplex as follows::
  
    ~/Ibex/ibex-2.1.16/soplex-1.7.2$  make ZLIB=false SHARED=true SHAREDLIBEXT=dll

Then, you can run ``waf`` ``configure`` with either ``--enable-shared`` or ``--with-jni``.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Installation of the Java interface (``--with-jni``)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Important notice:

- This interface is **under development**
- The Ibex-Choco interface is supported by Choco 3 (not Choco 2).
- As for ``--enable-shared``, we recommend you to use the interval libraries supplied with Ibex.
- Some configurations (Ibex+Gaol or Ibex+MinGW under Windows) yield a **32-bit** library although the platform is 64 bits. Hence, 
  Java will fail in loading Ibex unless you have a 32-bits JVM.
- To link with Choco, use ``--with-java-package=org.chocosolver.solver.constraints.real``.

The only additional requirement is that the environment variable ``JAVA_HOME`` must be set. 

- **Linux and MacOS**

  Typical paths are ``/Library/Java/Home`` (MacOS) or
  ``/usr/lib/jvm/java-7-openjdk-i38`` (Linux). Example::

    ~/Ibex/$ export JAVA_HOME=/Library/Java/Home

- **Windows**

  Set the JAVA_HOME variable (under the shell of MinGW) in Windows-style and with double backslash ("\\") as separator, e.g.::
  
    ~/Ibex/$ export JAVA_HOME=C:\\Java\\jdk1.7.1_17

  **Warning: The path must not contain white spaces!**, like "...\\Program Files\\...". Create a symbolik link of your Java directory if necessary.
  Remember that your JDK must be 32 bits.

This installation will generate, in addition to the Ibex library:

- The ``libibex-java`` library that contains the glue code between C++ and Java
- The *PACKAGE_NAME*\.jar file where *PACKAGE_NAME* is the name given with the ``--with-java-package`` option. 
  This file is put into the *PREFIX*\ ``/share/java`` where *PREFIX* is 
  ``/usr/local`` by default or whatever path specified via ``--prefix``.

.. _install-2.1-compiling-running:

==============================
Compiling and running examples
==============================

--------------------------------
Using the Makefile (recommended)
--------------------------------

If you have installed Ibex following the `standard installation (recommended)`_, there is a simple "makefile" you can use to compile examples as well as your own programs.
This makefile however assumes ``pkg-config`` is installed on your system (which is done by default on many Linux distribution).

If you have installed Ibex in the local folder, or if you don't want ``pkg-config`` to be installed,
you may consider using the `basic command line`_ instead.

**Note:** it may be necessary to set the ``PKG_CONFIG_PATH`` to *PREFIX*\ ``/share/pkgconfig`` where *PREFIX* is 
``/usr/local`` by default or whatever path specified via ``--prefix``::

  ~/Ibex/ibex-2.1.16/$ export PKG_CONFIG_PATH=/usr/local/share/pkgconfig/ 

**Note:** if some libraries are linked dynamically (like libultim), it may be necessary to set the ``LD_LIBRARY_PATH`` accordingly.

Here is an example::

  ~/Ibex/ibex-2.1.16/$ cd examples 
  ~/Ibex/ibex-2.1.16/$ make defaultsolver 
  ~/Ibex/ibex-2.1.16/$ ./defaultsolver ../benchs/cyclohexan3D.bch 1e-05 10 

The default solver solves the systems of equations in argument (cyclohexan3D) with a precision less than 1e-05 and
within a time limit of 10 seconds.

To compile your own program, just copy-paste the makefile of ``Ibex/examples``.

**Note:** this makefile uses the extended syntax of GNU make.

------------------
Basic command line
------------------

You can alternatively try the following command line directly, but the success is less guaranteed.

If Ibex is installed with Gaol and Soplex, the basic line is::

  g++ -IPREFIX/include -IPREFIX/include/ibex -ISOPLEX_PATH/src -LSOPLEX_PATH/lib -libex -lsoplex -lultim -o FILENAME FILENAME.cpp 

where *PREFIX* is ``/usr/local`` by default or whatever path specified via ``--prefix``.

**Notes**
   - If your have compiled Ibex with Gaol under 64 bits platform, add the ``-m32`` option 

   - If Ibex is installed in a local folder, add -L\ *PREFIX*\ ``/lib``

If Ibex is installed with Profil/Bias and Soplex, the basic line is::

  g++ -IPREFIX/include -IPREFIX/include/ibex -IPROFIL_PATH/include -ISOPLEX_PATH/src -LPROFIL_PATH/lib -LSOPLEX_PATH/lib -libex -lsoplex -lProfil -lBias -llr -o FILENAME FILENAME.cpp 

If Ibex is installed with Filib and Clp, the basic line is::

  g++ -IPREFIX/include -IPREFIX/include/ibex -IFILIB_PATH/include -ICLP_PATH/include -frounding-math -ffloat-store -LFILIB_PATH/lib -LCLP_PATH/lib -libex -lprim -lClp -lCoinUtils -lz -lm -o FILENAME FILENAME.cpp 

If Ibex is installed with Filib and Cplex, the basic line is::

  g++ -IPREFIX/include -IPREFIX/include/ibex -IFILIB_PATH/include -ICPLEX_PATH/cplex/include -frounding-math -ffloat-store -LFILIB_PATH/lib -LCPLEX_PATH/cplex/lib/x86-64_linux/static_pic/ -libex -lprim -lcplex -lpthread -o FILENAME FILENAME.cpp 

**Notes**
   For the other possible configuration, see the option in the file \ *PREFIX*\ ``/share/pkgconfig/ibex.pc``  or ``/usr/local/share/pkgconfig/ibex.pc``

-------------
Java examples
-------------

If you have installed Ibex with the ``--with-jni`` option, you can run a java test example.

- **Linux**::

    ~$ export LD_LIBRARY_PATH=PREFIX/lib

  where *PREFIX* is ``/usr/local`` by default or whatever path specified via ``--prefix``. Then::
    
    ~$ cd Ibex/ibex-2.1.16/__build__/src/java 
    ~/Ibex/ibex-2.1.16/__build__/src/java$ $JAVA_HOME/bin/java Test   

  Alternatively, you can give the library path directly in argument of the ``java`` command instead of using an environment variable::

    ~$ cd Ibex/ibex-2.1.16/__build__/src/java 
    ~/Ibex/ibex-2.1.16/__build__/src/java$ $JAVA_HOME/bin/java -Djava.library.path=PREFIX/lib Test   

- **MacOS**::

    ~$ export DYLD_LIBRARY_PATH=PREFIX/lib  

  where *PREFIX* is ``/usr/local`` by default or whatever path specified via ``--prefix``. Then::
 
    ~$ cd Ibex/ibex-2.1.16/__build__/src/java 
    ~/Ibex/ibex-2.1.16/__build__/src/java$    

  Alternatively, you can give the library path directly in argument of the ``java`` command instead of using an environment variable::

    ~$ cd Ibex/ibex-2.1.16/__build__/src/java 
    ~/Ibex/ibex-2.1.16/__build__/src/java$ $JAVA_HOME/bin/java -Djava.library.path=PREFIX/lib Test   

- **Windows**

  We assume here that MinGW has been installed in ``C:\`` and Ibex in ``C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.1.16``.


  Open a command window (not the shell of MinGW) and type::

    C:\Documents and Settings\[user]> cd C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.1.16\__build__\src\java 
    C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.1.16\__build__\src\java>set PATH=%PATH%;PREFIX/bin;C:\MinGW\bin
    C:\MinGW\msys\1.0\home\[user]\Ibex\ibex-2.1.16\__build__\src\java>java Test
         
  
Notice: ``$JAVA_HOME`` ensures the JVM is compatible with the version of Java you have compiled Ibex with.

=========
Uninstall
=========

Simply type in the path of IBEX (under the shell of MinGW for Windows)::

  ~/Ibex/ibex-2.1.16$ sudo ./waf uninstall  
  ~/Ibex/ibex-2.1.16$ ./waf distclean  

**Note:** sudo is useless if Ibex is installed in a local folder.

It is highly recommended to uninstall Ibex like this before upgrading to a new release.

===============
Troubleshooting
===============

-------------------------------
UnsatisfiedLinkError with Choco
-------------------------------

When running the "CycloHexan" example from Choco using Ibex, the following error appears::

  Exception in thread "main" java.lang.UnsatisfiedLinkError: org.chocosolver.solver.constraints.real.Ibex.add_ctr(ILjava/lang/String;I)V 
        at org.chocosolver.solver.constraints.real.Ibex.add_ctr(Native Method) 
        at org.chocosolver.solver.constraint.propagators.real.RealPropagator.&lt;init&gt;(RealPropagator.java:77) 
        at org.chocosolver.solver.constraints.real.RealConstraint.addFunction(RealConstraint.java:82) 
        at samples.real.CycloHexan.buildModel(CycloHexan.java:87) 
        at samples.AbstractProblem.execute(AbstractProblem.java:130) 
        at samples.real.CycloHexan.main(CycloHexan.java:134) 

**Solution:** You probably did not set the Java package properly. The java package of the Ibex class in Choco is ``org.chocosolver.solver.constraints.real``, try::

        ./waf configure [....] --with-java-package=org.chocosolver.solver.constraints.real 
        

-------------------------
Headers of Gaol not found
-------------------------

When running ``waf configure``, I get messages like this::

  Checking for header ['gaol/gaol.h', 'gaol/gaol_interval.h']   : not found
   ...

Does it mean that Ibex is not properly installed?


**Answer:** No, this mesage simply indicates that gaol was not found on your system and that it will be automatically extracted from the bundle. It is not an error message.
        

----------------------------------
Trouble to install a Linear Solver
----------------------------------

If you have any trouble to install a linear solver (Clp, Soplex or Cplex), you can install Ibex with the option ``--without-lp``. 

This option will disable the contractor ``CtcPolytopeHull``.::

      ~/Ibex/ibex-2.1.16/$ ./waf configure  [...] --without-lp 
      ~/Ibex/ibex-2.1.16/$ ./waf install
  


------------------------------------------
JAVA_HOME does not seem to be set properly
------------------------------------------

I get this message when running ``waf configure``.
        
**Solution:** The JAVA_HOME must be the path of the JDK and contain a subdirectoy ``include`` which, in turn, contains the ``jni.h`` 
header file. On MacOS this path can be ``/Library/Java/JavaVirtualMachines/jdkXXXX.jdk/Contents/Home``.

----------------------------
Linking problem with CoinOR
----------------------------

If the linker fails with undefined reference to ``dgetrf`` and ``dgetrs``, it is probably because you have installed Lapack.
You can either:

- try to adapt the makefile to link with Lapack. 
- Remove Lapack, reinstall Ibex and reinstall Lapack (in this order).


