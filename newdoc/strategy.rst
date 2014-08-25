**************************************************
              Strategies  (*under construction*)
**************************************************

.. _strategy-bisectors:

==========
Bisectors
==========

A bisector is an operator that takes a box :math:`[x]` as input and returns two sub-boxes :math:`([x]^{(1)},[x]^{(2)})` that
form a partition of :math:`[x]`, that is, :math:`[x]=[x]^{(1)}\cup[x]^{(2)}`. This partition is obtained by
selecting one component :math:`[x_i]` and splitting this interval at some point.

Each bisector implements a specific strategy for chosing the component. The bisection point in the interval is
defined as a *ratio* of the interval width, e.g., a ratio of 0.5 corresponds to the midpoint.

-------------------------------------
Bisecting each component in turn
-------------------------------------

*(to be completed)*


-------------------------------------
Bisecting the largest component
-------------------------------------

*(to be completed)*


------------------------------------------
Setting different precision for variables
------------------------------------------
In real-world applications, variables often correspond to physical quantities with different units.
The order of magnitude greatly varies with the unit. 
For example, consider Coulomb's law:

.. math::
  F=k_e\frac{q_1q_2}{r^2}

applied to two charges :math:`q_1` and :math:`q_2` or ~1e-6 coulomb, with a distance :math:`r` of ~1e-2 meter.
With Coulomb's contant ~ 1e10, the resulting force will be in the order of 1e2 Newton.

If one introduces Coulomb's equation in a solver, using a bisector that handles
variables uniformly, i.e., that uses the same precision value for all of them, is certainly not adequate.

Each bisector can be given a vector of different precisions (one for each variable) instead of a
unique value. We just have to give a ``Vector`` in argument in place of a ``double``. 
For instance, with the round-robin bisector:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: bsc-different-prec
   :end-before: bsc-different-prec

-------------------------------------
Respecting proportions of a box
-------------------------------------

If you want to use a relative precision that respects the proportion betweeen
the interval widths of an "initial" box, you can simply initialize the vector
of precision like this:

.. code-block:: cpp
  
   Vector prec=1e-07*box.diam(); // box is the initial domain
   

.. _strategy-smear-function:

----------------------------------------
The Smear Function
----------------------------------------

*(to be completed)*

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Smear function with maximum impact
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*(to be completed)*

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Smear function with maximal sum of impacts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*(to be completed)*

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Smear function with maximal normalized impact
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*(to be completed)*

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Smear function with maximal sum of normalized impacts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*(to be completed)*

maximal sum of impacts

.. _strategy-cell-buffers:

============
Cell buffers
============

*(to be completed)*

.. _strategy-cell-stack:

----------------------------------------
Cell Stack
----------------------------------------

*(to be completed)*

.. _strategy-cell-heap:

----------------------------------------
Cell Heap
----------------------------------------

*(to be completed)*

==========================
Solver
==========================


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
Implementing SIVIA
==========================


.. literalinclude:: ../examples/doc-sivia.cpp 
   :language: cpp
   


