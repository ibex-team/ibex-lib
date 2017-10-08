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
