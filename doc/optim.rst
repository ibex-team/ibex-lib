
.. _optim: 

**************************************
              IbexOpt
**************************************

This page describes IbexOpt, the plugin installed with the ``--with-optim`` option.

.. _optim-run-default:

=================
Getting started
=================

IbexOpt is a end-user program that solves a NLP problem (non-linear programming).
It minimizes a (nonlinear) objective function under (nonlinear) inequality and equality constraints.
It resorts to
a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that this program is based on the :ref:`generic optimizer <optim-generic>`, a C++ class
that allows to build a more customizable optimizer.

You can directly apply this optimizer on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the `Minibex syntax`_ and stored in an arborescence under ``plugins/optim/benchs/``.
If you compare the Minibex syntax of these files with the ones given to IbexSolve, you will see that a "minimize"
keyword has appeared.

.. _Minibex syntax: #func-minibex

Open a terminal, move to the ``bin`` subfolder and run IbexSolve with, for example, the problem named ex3_1_3 located at the specified path::

  ~/ibex-2.6.0$ cd bin
  ~/ibex-2.6.0/bin$ ./ibexopt ../plugins/optim/benchs/easy/ex3_1_3.bch
	
The following result should be displayed::


 ************************ setup ************************
   file loaded:	../plugins/optim/benchs/easy/ex3_1_3.bch
 *******************************************************

 running............

  optimization successful!

  best bound in: [-310.309999984,-309.999999984]
  relative precision obtained on objective function: 0.001  [passed]
  absolute precision obtained on objective function: 0.309999999985  [failed]
  best feasible point: (4.9999999999 ; 1 ; 5 ; 0 ; 5 ; 10)
  cpu time used: 0.00400000000001s.
  number of cells: 1

The program has proved that the minimum of the objective lies in a small interval enclosing -310. It also gives
a point close to (5 ; 1 ; 5 ; 0 ; 5 ; 10) which satisfies the constraints and for which
the value taken by the objective function is inside this interval. The process took less than 0.005 seconds.

.. _optim-options:

================== 
Options
================== 

+--------------------------------------+------------------------------------------------------------------------------+
| -r<*float*>, --rel-eps-f=<*float*>   |  Relative precision on the objective. Default value is 1e-3.                 |
+--------------------------------------+------------------------------------------------------------------------------+                                        
| -a<*float*>, --abs-eps-f=<*float*>   | Absolute precision on the objective. Default value is 1e-7.                  |
+--------------------------------------+------------------------------------------------------------------------------+
| --eps-h=<*float*>                    | Equality relaxation value. Default value is 1e-8.                            |
+--------------------------------------+------------------------------------------------------------------------------+
| -t<*float*>, --timeout=<*float*>     | Timeout (time in seconds). Default value is +oo.                             |
+--------------------------------------+------------------------------------------------------------------------------+
| --random-seed=<*float*>              | Random seed (useful for reproducibility). Default value is 1.                |
+--------------------------------------+------------------------------------------------------------------------------+
| --eps-x=<*float*>                    | Precision on the variable (**Deprecated**). Default value is 0.              |
+--------------------------------------+------------------------------------------------------------------------------+
| --initial-loup=<*float*>             | Intial "loup" (a priori known upper bound).                                  |
+--------------------------------------+------------------------------------------------------------------------------+
| --rigor                              | Activate rigor mode (certify feasibility of equalities).                     |
+--------------------------------------+------------------------------------------------------------------------------+
| --trace                              | Activate trace. Updates of loup/uplo are printed while minimizing.           |
+--------------------------------------+------------------------------------------------------------------------------+

.. _optim-call-default:

==================================== 
Calling IbexOpt from C++
==================================== 

Calling the default optimizer is as simple as for the :ref:`default solver <solver>`.
The loaded system must simply correspond to an optimization problem. The default optimizer
is an object of the class ``DefaultOptimizer``.

Once the optimizer has been executed(), the main information is stored in three fields, 
where f is the objective:

- ``loup`` ("lo-up") is the lowest upper bound known for min(f).
- ``uplo`` ("up-lo") is the uppest lower bound known for min(f).
- ``loup_point`` is the vector for which the value taken by f is less or equal to the loup.

Example:

.. literalinclude:: ../examples/doc-optim.cpp 
   :language: cpp
   :start-after: optim-call-default-C
   :end-before:  optim-call-default-C

The output is:

.. literalinclude:: ../examples/doc-optim.txt
   :start-after: optim-call-default-O
   :end-before:  optim-call-default-O

.. _optim-argmin:

=============================================
Getting en enclosure of all global minima
=============================================
Given a problem:

.. math::

	{\mbox Minimize} \ f(x)
	
	{\mbox s.t.} \ h(x)=0 \wedge g(x)\leq 0
	
IbexOpt gives  a feasible point that is *sufficiently* close to the real minimum :math:`f^*` of the function, i.e., a point
that satisfies

.. math::

	{\mbox uplo} \leq f(x)\leq {\mbox loup}
	
	{\mbox s.t.} \ h(x)=0 \wedge g(x)\leq 0
	
with *loup* and *uplo* are respectively a valid upper and lower bound of :math:`f^*`, whose accuracy depend on the input precision parameter 
(note: validated feasibility with equalities requires ''rigor'' mode).

From this, it is possible, in a second step, to get an enclosure of all global minima thanks to :ref:`solver`.
The idea is simply to ask for all the points that satisfy the previous constraints.
We give now a code snippet that illustrate this.

.. literalinclude:: ../examples/doc-optim.cpp 
   :language: cpp
   :start-after: optim-all-minima-C
   :end-before:  optim-all-minima-C


.. _optim-generic:
  
==================================== 
The generic optimizer
==================================== 

Just like the :ref:`generic solver <solver-generic>`, the generic optimizer is the main C++ class 
(named ``Optimizer``) behind the implementation of IbexOpt. 
It takes as the solver:
 
- a **contractor**
- a **bisector**
- a **cell buffer**

but also requires an extra operator:

- a **loup finder**. A loup finder is in charge of the *goal upper bounding* step of the optimizer.

We show below how to re-implement the default optimizer from the generic ``Optimizer`` class.

.. _optim-implem-default:

=============================================
Implementing IbexOpt (the default optimizer)
=============================================

The contraction performed by the default optimizer is the same as the default solver 
(see :ref:`solver-implem-default`) except that it is not applied on the system 
itself but the :ref:`mod-sys-transfo-extend`.

The loup finder (``LoupFinderDefault``) is a mix of differents strategies. The basic idea is to 
create a continuum of feasible points (a box or a polyhedron) where the goal function can be evaluated quickly, 
that is, without checking for constraints satisfaction.
The polyhedron (built by a ``LinearizerXTaylor`` object) corresponds to a :ref:`linerization technique <ctc-linear-relax>` described in 
:ref:`[Araya et al. 2014] <Araya14>` and :ref:`[Trombettoni et al. 2011] <Trombettoni11>`, based on *inner region extraction*. 
It also resorts to the linerization offered by affine arithmetic (a ``LinearizerAffine`` object) if the affine plugin is installed.
The box (built by a ``LoupFinderInHC4`` object) is a technique based on :ref:`inner arithmeric <itv-inner-arith>` also described in 
the aforementionned articles. 

Finally, by default, the cell buffer (``CellDoubleHeap``) is basically a sorted heap that allows to get in priority boxes minimizing 
either the lower or upper bound of the objective enclosure (see :ref:`strategy-cell-heap`).


.. literalinclude:: ../examples/doc-optim.cpp 
   :language: cpp
   :start-after: optim-implem-default-C
   :end-before:  optim-implem-default-C



