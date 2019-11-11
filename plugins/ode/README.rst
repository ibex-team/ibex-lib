

============
ODE plugin
============

This plugin is intended to gather several tools for
the simulation and anaylsis of ODEs.

Currently, it contains a single program ``ibexode-attract-region``, 
that allows to calculate a region inside wich all points converge to a
(exponentially) stable point of a dynamical system.

The principle is as follows.

The user gives as input:
- an ODE under the form x'=f(x);
- a Lyapunov function v(x);
- an approximation of the fixpoint x* of f;

and the program finds an attracting region for the considered fixpoint:
any solution trajectory of the ODE starting inside this region
is proven to converge to this fixpoint.

The region is returned as a sub-level set of the Lyapunov function, that
is, a set described by an inequality v(x)<=c. So the programs actually just returns
a scalar: the value of "c".

For further details, see the paper::

  Estimating the Robust Domain of Attraction for Non-Smooth Systems using an Interval Lyapunov Equation
  by A. Goldsztejn and G. Chabert
  in Automatica, Vol. 100, pp. 371-377, Elsevier, 2019
 

***************
Installation
***************

Just use ``--with-ode`` in the configure step::

   ibex$ ./waf configure --with-optim --with-ode --lp-lib=soplex
   ibex$ sudo ./waf install

The executable ``ibexode-attract-region`` is then installed by default
on the ``bin/`` folder of IBEX. 

The examples of the paper cited above are all in the ``examples/`` subfolder of this plugin.

***************
Usage
***************

Try for instance::

   ibex$ ./ibexode-attract-region plugins/ode/examples/Example1.txt

The output is::

  c_dichotomy_derivative= [0.0247955322265624, 0.02481460571289053] t=1.742ms
  c_dichotomy_hansen=     [0.05653381347656235, 0.05657196044921861] t=1.71101ms
  c_dichotomy_hessian=    [0.009870529174804626, 0.00988006591796869] t=2.65201ms
  c_optimizer=            [0.3208558139611485, 0.3211766697751097] t=39.3421ms

  A point x such that v(x-x*)=c and v'(x-x*)>0:
  (0.461418 ; 0.329046 ; 0.321177)


The 4 first lines gives the estimated value of "c" by 4 different methods.

The last line gives a point that violates the non-positiveness of the
Lie derivative.

To use this executable on your own problem, just copy-paste one of the Minibex examples
and adapt it to your problem. The Minibex file must contains:

- a function f(x) that represents your **vector field**, i.e. the mapping of your ODE x'=f(x).  
  Note that x can be a vector argument of any size. So you can declare a function f(x[3]) for instance .
  The function f can have an extra argument "theta" (i.e. we have f(x[...],theta[...])) if your vector field has an uncertain parameter "theta".
  This argument can also be a vector. See the paper for more details on how uncertainty is considered in this tool.
- a function v(x) that represents the chosen **Lyapunov function**.
- a **quadratic** function vminor(x) **minoring** v(x), if v(x) is not already quadratic. The function vminor is ignored if v is already quadratic.
- a declaration of a (vector) variable x with an approximation of **the fixpoint** of as domain. The variable is declared just as in a
  standard Minibex system of equations.
- a declaration of a (vector) variable theta if the system has an uncertain parameter theta, with potentially an initial domain
- optionally, constraints on the uncertain parameter theta, in complement with the initial domain

***************
Advanced
***************

To run the scalable "academic benchmark" of the paper::

1- move to the plugin folder::
   
   ibex$ cd plugins/ode

1- set the PKG_CONFIG_PATH variable::

   export PKG_CONFIG_PATH=[path-to-ibex]/share/pkgconfig

2- compile the programs (from this directory)::

   $ make

4- run the programs. Example::

   $ ./dicho-scalable examples/scalable/Scalable2_ 2 10 1e-3 10

   or 
   
   $ ./optim-scalable examples/scalable/Scalable2_ 2 10 1e-3 10
   

