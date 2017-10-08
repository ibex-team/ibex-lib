
****************************************************
 Java Plugin (for Choco)
****************************************************

.. _Choco: http://www.emn.fr/z-info/choco-solver


   +-----------------------------+---------------------+------------------------------+
   | .. image:: images/ibex.jpg  |                     |  .. image:: images/choco.png |
   |    :width: 200 px           |                     |     :width: 200 px           |
   +-----------------------------+---------------------+------------------------------+
   
The Java plugin of Ibex allows to use Ibex with `Choco`_, for solving mixed integer-continuous CSP
(constraint satisfaction problems).

===============================
Installation (with Ibex 2.6)
===============================

.. note::

   Read how to :ref:`install Ibex <ibex-install>` before installing this plugin.


.. warning::

   The option ``--with-java-package`` has been changed to ``--java-package-name``
   
The installation of the plugin will generate, in addition to the Ibex library, the libibex-java library that contains the glue code between C++ and Java.

.. Note:: 

   Under Windows, Ibex is compiled as a 32-bit library although the platform is 64 bits (this is mainly because the MinGW environment is 32-bits). Hence, Java will fail in loading Ibex unless you have a 32-bits JVM.

   The following instructions must be typed in the shell of MinGW.

Uncompress the archive ibex-2.6.0.tar.gz in some Ibex folder::

	~/Ibex/$ tar xvfz ibex-2.6.0.tar.gz

Uncompress the archive ibex-java.tar.gz in the plugin folder::

	~/Ibex/$ tar xvfz ibex-java.tar.gz --directory=ibex-2.6.0/plugins

Set the environment variable ``JAVA_HOME``. Typical paths are ``/Library/Java/Home`` (MacOS) or ``/usr/lib/jvm/java-7-openjdk-i38`` (Linux). Example::

	~/Ibex/$ export JAVA_HOME=/Library/Java/Home

Under MinGW, the variable must be set in Liux-style (don't use backslash ("\\") as separator), e.g.::

	~/Ibex/$ export JAVA_HOME=/c/Java/jdk1.7.1_17

.. Warning:

   The path must not contain white spaces, like ”/c/Program Files/...”. Create a symbolik link of your Java directory if necessary.

Then configure Ibex as follows::

	~/Ibex/$ cd ibex-2.6.0
	~/Ibex/ibex-2.6.0/$ ./waf configure [...] --enable-shared --with-solver --with-jni --java-package-name=org.chocosolver.solver.constraints.real

**Note**: the ``--enable-shared`` option is mandatory. The ``-with-solver`` option can be ommited as this plugin
is automatically installed (with Release 2.6.0). 


============================
Configuration options
============================

The IbexOpt plugin supports the following options (to be used with waf configure) :

--with-jni 	                      Activate the Java plugin.
--java-package-name=PACKAGE_NAME
 	                              The plugin will create the PACKAGE_NAME.jar file. This file is put into the ``[prefix]/share/java`` 
 	                              where [prefix] is ``/usr/local`` by default or whatever path specified via ``--prefix``.

============================
Troubleshooting
============================

----------------------------------
UnsatisfiedLinkError with Choco
----------------------------------

When running the “CycloHexan” example from Choco using Ibex, the following error appears::

	Exception in thread "main" java.lang.UnsatisfiedLinkError: org.chocosolver.solver.constraints.real.Ibex.add_ctr(ILjava/lang/String;I)V
	at org.chocosolver.solver.constraints.real.Ibex.add_ctr(Native Method)
	at org.chocosolver.solver.constraint.propagators.real.RealPropagator.&lt;init&gt;(RealPropagator.java:77)
	at org.chocosolver.solver.constraints.real.RealConstraint.addFunction(RealConstraint.java:82)
	at samples.real.CycloHexan.buildModel(CycloHexan.java:87)
	at samples.AbstractProblem.execute(AbstractProblem.java:130)
	at samples.real.CycloHexan.main(CycloHexan.java:134)

Solution: You probably did not set the Java package properly. The java package of the Ibex class in Choco is org.chocosolver.solver.constraints.real, try::

	./waf configure [....] --java-package-name=org.chocosolver.solver.constraints.real

----------------------------------------------
JAVA_HOME does not seem to be set properly
----------------------------------------------

I get this message when running waf configure.

Solution: The ``JAVA_HOME`` must be the path of the JDK and contain a subdirectoy include which, in turn, contains the jni.h header file. 
On MacOS this path can be ``/Library/Java/JavaVirtualMachines/jdkXXXX.jdk/Contents/Home``.
