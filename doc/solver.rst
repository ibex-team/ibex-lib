
.. _solver: 

**************************************
             IbexSolve
**************************************

This page describes IbexSolve, the plugin installed with the ``--with-solver`` option.

.. _solver-run-default:

=================
Getting started
=================

IbexSolve is a end-user program that solves a system of nonlinear equations rigorously
(that is, it does not lose any solution and return each solution under the form of a 
small box enclosing the true value). It resorts to
a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that this program is based on the :ref:`generic solver <solver-generic>`, a C++ class
that allows to build a more customizable solver.

You can directly apply this solver on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the `Minibex syntax`_ and stored in an arborescence under ``plugins/solver/benchs/``.

.. _Minibex syntax: #func-minibex

Open a terminal, move to the ``bin`` subfolder and run IbexSolve with, for example, the problem named kolev36 located at the specified path::

  ~/ibex-2.6.0$ cd bin
  ~/ibex-2.6.0/bin$ ./ibexsolve ../plugins/solver/benchs/others/kolev36.bch
	
The following result should be displayed::

 ***************************** setup *****************************
   file loaded:		../plugins/solver/benchs/others/kolev36.bch
   output file:		../plugins/solver/benchs/others/kolev36.mnf
 *****************************************************************

 running............

 solving successful!

 number of inner boxes:		1
 number of boundary boxes:	0
 number of unknown boxes:	0
 number of pending boxes:	0
 cpu time used:			0.0640001s
 number of cells:		25

 results written in ../plugins/solver/benchs/others/kolev36.mnf

The number of "inner boxes" correspond to the number of solutions found (there is just one here). To see the solution, use the option ``-s``.

In the report, the "number of cells" correspond to the number of hypothesis (bisections) that was required to solve the problem.

.. _solver-options:

================== 
Options
================== 

+--------------------------------------+------------------------------------------------------------------------------+
| -e<*float*>, --eps-min=<*float*>     | Minimal width of output boxes. This is a criterion to *stop* bisection: a    |
|                                      | non-validated box will not be larger than 'eps-min'. Default value is 1e-3.  |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+      
| -E<*float*>, --eps-max=<*float*>     | Maximal width of output boxes. This is a criterion to *force* bisection: a   |
|                                      | validated box will not be larger than 'eps-max' (unless there is no equality |
|                                      | and it is fully inside inequalities). Default value is +oo (none)            |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| -t<*float*>, --timeout=<*float*>     | Timeout (time in seconds). Default value is +oo (none).                      |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| -i<*filename*>, --input=<*filename*> | Manifold input file. The file contains a (intermediate) description of the   |
|                                      | manifold with boxes in the MNF (binary) format.                              |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+      
| -o<*filename*>, --output=<*filename*>| Manifold output file. The file will contain the description of the manifold  |
|                                      | with boxes in the MNF (binary) format.                                       |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| -s, --sols                           | Display the "solutions" (output boxes) on the standard output.               |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+      
| --bfs                                | Perform breadth-first search (instead of depth-first search, by default)     |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| --trace                              | Activate trace. "Solutions" (output boxes) are displayed as and when they    |
|                                      | are found.                                                                   |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| --boundary=...                       | Boundary test strength. Possible values are:                                 |        
|                                      |                                                                              |
|                                      | - ``true``: always satisfied. Set by default for under constrained problems  |
|                                      |   (0<m<n).                                                                   |
|                                      | - ``full-rank``: the gradients of all constraints (equalities and potentially|
|                                      |   activated inequalities) must be linearly independent.                      |
|                                      | - ``half-ball``: (**not implemented yet**) the intersection of the box and   |
|                                      |   the solution set is homeomorphic to a half-ball of R^n                     |
|                                      | - ``false``: never satisfied. Set by default if m=0 or m=n (inequalities     |
|                                      |   only/square systems)                                                       | 
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| --random-seed=<*float*>              | Random seed (useful for reproducibility). Default value is 1.                |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| -q, --quiet                          | Print no report on the standard output.                                      |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+

.. _solver-call-default:

==================================== 
Calling IbexSolve from C++
==================================== 

You can call IbexSolve (the default solver) and get the solutions from C++.

Two objects must be built: the first represents the problem (namely, a :ref:`system <mod-sys>`), the second
the solver itself. Then, we just run the solver. Here is a simple example:

.. literalinclude:: ../examples/doc-solver.cpp
   :language: cpp
   :start-after: solver-call-default-C
   :end-before: solver-call-default-C
 
The output is:

.. literalinclude:: ../examples/doc-solver.txt
   :start-after: solver-call-default-O
   :end-before: solver-call-default-O

.. _solver-generic:
  
==================================== 
The generic solver
==================================== 

The generic solver is the main C++ class behind the implementation of ``ibexsolve``.
It is a classical branch and prune algorithm that interleaves contraction and branching (bisection) until
boxes get sufficiently small. However, it performs a more general task that just finding solution points of square
systems of equations: it also knows how to deal with under-constrained systems and handle manifolds. 

.. note::
   A more detailed documentation about under-constrained systems will be available soon.

Compared to ``ibexsolve``, the generic solver allows the following additional operators as inputs:

#. a **contractor** 
       
    Operator that contracts boxes by removing non-solution points. The contraction operator must be compatible with the 
    system given (equations/inequalities). The solver performs no check (it is the user responsability). See
    :ref:`ctc`.

#. a **bisector**           

    Operator that splits a box. Note that some bisectors have a *precision* parameter: the box is bisected providing 
    it is large enough. But this precision is not directly seen by the solver which has its own precision variables 
    (see ``-e`̀  and ``-E``). If however the bisector does not split a box, this will generate an exception caught by the solver,
    which will not continue the search and backtrack. So fixing the bisector internal precision gives basically the same effect
    as fixing it with ``--e``. See :ref:`strategy-bisectors` for more details. 

#. a **cell buffer**
  
    Operator that manages the list of pending boxes (a *cell* is a box with a little bit of extra information used by the search).
    See :ref:`strategy-cell-buffers` for more details.

Our next example creates a solver for the intersection of two circles of radius :math:`d`, 
one centered on :math:`(0,0)` and the other in :math:`(1,0)`. 

To this end we first create a vector-valued function:

.. math::
	(x,y) \mapsto \begin{pmatrix} x^2+y^2-d \\ (x-1)^2+y^2-d \end{pmatrix}

Then, we build two contractors; a :ref:`forward-bacwkard <ctc-fwd-bwd>` contractor and (because the system is square),
an :ref:`interval Newton <tuto-newton>` contractor.		


We chose as bisection operator the *round-robin* operator, that splits each component in turn.
The precision of the solver is set to 1e-7.

Finally, the cell buffer is a stack, which leads to a depth-first search.

.. literalinclude:: ../examples/doc-solver.cpp 
   :language: cpp
   :start-after: solver-solver-generic-C
   :end-before: solver-solver-generic-C

The output is:

.. literalinclude:: ../examples/doc-solver.txt
   :start-after: solver-generic-O
   :end-before: solver-solver-generic-O

.. _solver-implem-default:

=============================================
Implementing IbexSolve (the default solver)
=============================================

IbexSolve is an instance of the generic solver with (almost) all parameters set by default.

We already showed how to `Calling IbexSolve from C++`_.
To give a further insight into the generic solver and its possible settings, we explain now how to re-create the default solver 
by yourself.

The contractor of the default solver is obtained with the following receipe.
This is a :ref:`composition <tuto-inter-union-compo>` of

#. :ref:`ctc-HC4`
#. :ref:`ACID <ctc-acid>`
#. :ref:`Interval Newton <tuto-newton>` (only if it is a square system of equations)
#. A :ref:`fixpoint <tuto-fixpoint>` of the :ref:`ctc-polytope-hull` of two linear relaxations combined:
    - the relaxation called X-Taylor;
    - the relaxation generated by affine arithmetic. See :ref:`ctc-linear-relax`.
   
The bisector is based on the :ref:`strategy-smear-function` with maximal relative impact.

So the following program exactly reproduces the default solver.

.. literalinclude:: ../examples/doc-solver.cpp 
   :language: cpp
   :start-after: solver-implem-default-C
   :end-before: solver-implem-default-C
   
.. _solver-parallel:

=============================================
Parallelizing search
=============================================

It is possible to parallelize the search by running (in parallel) solvers for different subboxes of the initial box.

Be aware however that Ibex has not been designed (so far) to be parallelized and the following lines only reports our preliminary
experiments.

Here are the important observations:

- The sub-library gaol is **not** thread-safe. You must compile Ibex with **filib** which seems to be OK (see :ref:`install-options`).
- The linear solver Soplex (we have not tested yet with Cplex) seems to be thread-safe but sometimes generates error messages on the
  console like::

	    ISOLVE56 stop: 0, basis status: PRIMAL (2), solver status: RUNNING (-1)

  So, calling Soplex several times simultaneously seems not to be allowed, but Soplex at least manages the case properly, that is,
  stops. As far as we have observed, we don't lose solutions even when this kind of message appear. 
- Ibex objects are not thread-safe which means that the solvers run in parallel must share no information. In particular,
  each solver must have its **own copy** of the system.

Here is an example:

.. literalinclude:: ../examples/doc-solver.cpp
   :language: cpp
   :start-after: solver-parallel-C-1
   :end-before:  solver-parallel-C-1

If I remove the ``#pragma`` the program displays::

  solver #1 found 64
  solver #2 found 64

  real	0m5.121s        // <-------- total time
  user	0m5.088s

With the ``#pragma``, I obtain::

  solver #1 found 64
  solver #2 found 64

  real	0m2.902s        // <-------- total time
  user	0m5.468s

**Note:** It is pure luck that by bisecting the 4th variable, we obtain exactly half of the solutions on each sub-box.
Also, looking for the 64 first solutions takes here around the same time than looking for the 64 subsequent ones, which
is particular to this example. So, contrary to what this example seems to prove, splitting the box in two subboxes does 
not divide the running time by two in general. Of course :)

If you are afraid about the messages of the linear solver, you can replace the ``DefaultSolver`` by your own dedicated solver
that does not resort to the simplex, ex:

.. literalinclude:: ../examples/doc-solver.cpp
   :language: cpp
   :start-after: solver-parallel-C-2
   :end-before:  solver-parallel-C-2

