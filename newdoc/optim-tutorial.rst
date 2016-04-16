
.. _ibex-opt: 

**************************************
              Tutorial (IbexOpt)
**************************************


=================
Getting started
=================

----------------------
Run the default solver
----------------------

A couple of example programs are installed with Ibex under the ``plugins/optim/examples/`` subfolder.

One of them is the *default solver* that solves a system of nonlinear equations rigorously
(that is, it does not lose any solution and return each solution under the form of a 
small box enclosing the true value). It is called the "default" solver because is resorts to
a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that Ibex also supplies a :ref:`generic solver <tuto-generic-solver>` that is much more customizable.

You can directly apply this default solver on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the `Minibex syntax`_ and stored in an arborescence under ``benchs/``.

.. _Minibex syntax: #func-minibex

Open a terminal, move to the examples subfolder and compile the default solver::

  ~/Ibex/ibex-2.0/$ cd examples
  ~/Ibex/ibex-2.0/__build__/examples/$ make defaultsolver

**Note**: If you have installed Ibex in a local folder, you have to first set the ``PKG_CONFIG_PATH`` variable.
See :ref:`install-compiling-running`.

Now, run the default solver with, for example, the problem named kolev36 located at the specified path::

  ~/Ibex/ibex-2.0/__build__/examples/$./defaultsolver ../../benchs/benchs-satisfaction/benchlib2/kolev36.bch 1e-07 10

The second argument is the precision required on solution (1e-07). The third and last argument is the time limit in seconds (10 seconds).

The following result should be displayed::

  load file ../../benchs/benchs-satisfaction/benchlib2/kolev36.bch.
  sol 1 nb_cells 6 ([0.1173165676349034, 0.1173165676349242] ;
                    [0.4999999999996189, 0.5000000000003763] ;
                    [0.8826834323643938, 0.8826834323657756] ;
                    [-0.2071067811866001, -0.2071067811865033] ;
                    [1.207106781186472, 1.207106781186622] ;
                    [-2.000000000000119, -1.999999999999871])
  number of solutions=1
  cpu time used=0.036s.
  number of cells=4

The first simply says that the file has been successfuly loaded.
The second line (broken here into 6 lines for clarity) details the first solution found. There are 6 variables so 6 intervals are displayed.

The third line is the total number of solutions found (there is just one here).
The two last lines report the CPU time and the number of hypothesis (bisections) that was required to solve the problem.

------------------------------ 
Run the default optimizer
------------------------------ 

Similarly to the default solver, a default optimizer is installed with Ibex.
This program minimizes a (nonlinear) objective function under (nonlinear) inequality constraints.
Let us execute this optimizer with the problem ex3_1_3 got from the Coconut library. If you compare
the Minibex syntax of this benchmark with that of the previous example, you will see that a "minimize"
keyword has appeared.::

  ~/Ibex/ibex-2.0/__build__/examples/$./defaultoptimizer ../../benchs/benchs-optim/coconutbenchmark-library1/ex3_1_3.bch 1e-07 1e-07 10

As you see, the optimizer requires 3 arguments, besides the name of the benchmark.
The extra argument corresponds to the precision on the objective (both relative and absolute). Here, this precision is also set to 1e-07.

The following result should be displayed::

  best bound in: [-310.000030984,-309.999999984]
  Relative precision obtained on objective function: 9.9999998936e-08  [passed]  1e-07
  Absolute precision obtained on objective function: 3.09999999776e-05  [failed]  1e-07
  best feasible point (4.9999999999 ; 1 ; 5 ; 1.00000119296e-10 ; 5 ; 10)
  cpu time used 0.012s.
  number of cells 5

The program has proved that the minimum of the objective lies in [-310,-309.999999984]. It also gives
a point (4.9999999999 ; 1 ; 5 ; 1.00000119296e-10 ; 5 ; 10) which satisfies the constraints and for which
the value taken by the objective function is inside this interval.

--------------------------------
Call the default solver from C++
--------------------------------

You can call the default solver and get the solutions from C++.
Two objects must be built: the first represents the problem (or "system"), the second
the solver itself. Then, we just run the solver. Here is a simple example:

.. literalinclude:: ../examples/doc-tutorial.cpp
   :language: cpp
   :start-after: start-call-solver
   :end-before: start-call-solver
 
----------------------------------------------------
Call the default optimizer from C++
----------------------------------------------------

Calling the default optimizer is as simple as for the default solver.
The loaded system must simply correspond to an optimization problem. The default optimizer
is an object of the class ``DefaultOptimizer``.

Once the optimizer has been executed(), the main information is stored in three fields, 
where f is the objective:

- ``loup`` ("lo-up") is the lowest upper bound known for min(f).
- ``uplo`` ("up-lo") is the uppest lower bound known for min(f).
- ``loup_point`` is the vector for which the value taken by f is less or equal to the loup.

Example:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: start-call-optim
   :end-before: start-call-optim


==========================
Solver
==========================


   


.. _tuto-strat-default-solver:

--------------------------------
Implementing the default solver
--------------------------------

The default solver is an instance of the generic solver with (almost) all parameters set by default.

We already showed how to `Call the default solver from C++`_.
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

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: strat-default-solver
   :end-before: strat-default-solver
   
.. _solver-parallel:

----------------------------------------
Parallelizing search
----------------------------------------

It is possible to parallelize the search by running (in parallel) solvers for different subboxes of the initial box.

Be aware however that Ibex has not been designed (so far) to be parallelized and the following lines only reports our preliminary
experiments.

Here are the important observations:

- The sub-library gaol is **not** thread-safe. You must compile Ibex with **filib** which seems to be OK. Ibex is automatically
  compiled with Filib on 64 bits platforms (on 32 bits platforms, see :ref:`install-custom`).
- The linear solver Soplex (we have not tested yet with Cplex) seems to be thread-safe but sometimes generates error messages on the
  console like::

    ISOLVE56 stop: 0, basis status: PRIMAL (2), solver status: RUNNING (-1)

  So, calling Soplex several times simultaneously seems not to be allowed, but Soplex at least manages the case properly, that is,
  stops. As far as we have observed, we don't lose solutions even when this kind of message appear. 
- Ibex objects are not thread-safe which means that the solvers run in parallel must share no information. In particular,
  each solver must have its **own copy** of the system.

Here is an example:

.. literalinclude:: ../examples/doc-strategy.cpp
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

.. literalinclude:: ../examples/doc-strategy.cpp
   :language: cpp
   :start-after: solver-parallel-C-2
   :end-before:  solver-parallel-C-2


==========================
Global Optimizer
==========================

------------------------------
The generic optimizer
------------------------------

The generic optimizer is still under active development (at release |release|) and it is not yet
as "generic" as the generic solver. There are some limitations, namely:

- many actions performed by the optimizer are hard-coded. In particular, the *goal upper bounding* 
  step cannot yet be controlled by the user. We use a strategy described in :ref:`[Araya et al. 2014] <Araya14>` and 
  :ref:`[Trombettoni et al. 2011] <Trombettoni11>`, based on *inner region extraction*. The basic idea is to create a continuum of feasible points
  (a box or a polyhedron) where the goal function can be evaluated quickly, that is, without checking for
  constraints satisfaction.
	
- the optimizer only works for scalar (real-valued) constraints. E.g., you cannot
  enter a matrix-vector multiplication constraint like ``A*x=0``. You have to decompose
  such constraint into n scalar constraints. 

However, two key steps are generic: the contraction and bisection.
Note that contrary to the generic solver, the cell buffer cannot be chosen. It is a sorted heap that
allows to get in priority boxes minimizing the objective (see :ref:`strategy-cell-heap`).

-------------------------------------
Implementing the default optimizer
-------------------------------------
The contraction performed by the default optimizer is the same as the default solver 
(see :ref:`tuto-strat-default-solver`) except that it is not applied on the system 
itself but the :ref:`mod-sys-transfo-extend`.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: strat-default-optimizer
   :end-before: strat-default-optimizer



