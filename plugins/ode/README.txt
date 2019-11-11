

============
ODE plugin
============

This plugin is intended to gather several tools for
the simulation and anaylsis of ODEs.

Currently, it contains a single program ibexode-attract-region, 
that allows to calculate a region inside wich all points converge to a
(exponentially) stable point of a dynamical system.

See the paper::

  Estimating the Robust Domain of Attraction for Non-Smooth Systems using an Interval Lyapunov Equation
  by A. Goldsztejn and G. Chabert
  in Automatica, Vol. 100, pp. 371-377, Elsevier, 2019
 

***************
Installation
***************

Just use --with-ode in the configure step::

   ibex$ ./waf configure --with-optim --with-ode --lp-lib=soplex
   ibex$ sudo ./waf install

The executable "ibexode-attract-region" is installed by default
on the "bin" folder of IBEX. 

The examples of the paper cited above are all in the examples/ subfolder of this plugin.


***************
Usage
***************

Try for instance::

   ibex$ ./ibexode-attract-region plugins/ode/examples/Example1.txt

To use this executable on your own problem, just copy-paste one of the Minibex examples
and adapt it to your problem. The Minibex file must contains:

- a function f(x) that represents your vector field, i.e. the mapping of your ODE x'=f(x).  
  Note that x can be a vector argument of any size. So you can declare a function f(x[3]) for instance .
  The function f can have an extra argument "theta" (i.e. we have f(x,theta)) if your vector field has an uncertain parameter "theta".
  This argument can also be a vector. See the paper for more details on how uncertainty is considered in this tool.
- a function v(x) that represents the chosen Lyapunov function.
- a quadratic function vminor(x) minoring v(x), if v(x) is not already quadratic. The function vminor is ignored if v is already quadratic.
- a declaration of a (vector) variable x with an approximation of the fixpoint of as domain. The variable is declared just as in a
  standard Minibex system of equations.
- a declaration of a (vector) variable theta if the system has an uncertain parameter theta, with potentially an initial domain
- optionally, constraints on the uncertain parameter theta, in complement with the initial domain



To run the "academic benchmark" of the paper::

1- set the PKG_CONFIG_PATH variable:

   $ export PKG_CONFIG_PATH=[path-to-ibex]/share/pkgconfig

2- compile the programs (from this directory)::

   $ make

4- run the program. Example::

   $ ./dicho-scalable examples/scalable/Scalable2_ 2 10 1e-3 10


