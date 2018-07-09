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


==================
Box Properties
==================

**Release 2.7**

The box (``IntervalVector`` class) is the central concept in Ibex and often this structure is too simple
and required to be extended.

We give here a caricaturated example, for the sake of clarity. 

Consider a search tree, such as the one performed by ibexsolve.
Assume you have designed two contractors that need to calculate at some
point the width (i.e., the maximal diameter) of the input box in order to make the right decision.
E.g., if the box width is greater than 0.1, the contractor makes some coarse filtering; If it less than 0.1, it makes a finer filtering. Your code would roughly look like:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-ctc-1
   :end-before: prop-ctc-1

Imagine now that the ``max_diam()`` function takes a long time to be executed.
It is clear that recalculating the width each time the contractor is called represents a waste of time once
the box width has been proven to be less than 0.1 upstream in the search tree.
One would like to store this information in the box. This is precisely what ''box properties'' allow to do.

All is based on the ``Bxp`` interface. This ``Bxp`` interface allows to extend the simple ``IntervalVector`` data structure and to make this extension being propagated through a search tree. The extended box is then visible by all operators involved in the strategy (contractors, bisectors, cell buffers, etc.).

Note that the name of this interface is a trigram (like ``Ctc`` or ``Bsc``) 
just to encourage programers to prefix subclasses by ``Bxp`` (this is a recommended usage).
A box property, such as whether the box is small engouh, has to be a subclass of ``Bxp`` so let us name it ``BxpIsSmall``:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-class-name
   :end-before: prop-class-name

Of course, this class contains a field named ``is_small`` that will store the information.
We could also add whatever data needed.

----------------------------------------
Constructor
----------------------------------------

It is natural to ask the constructor of ``BxpIsSmall`` to take a box in argument and to set the ``is_small`` field appropriately.

The constructor of the mother class ``Bxp`` also requires an identifying number. Here is why. A box property is actually a set of *instances* of the ``Bxp`` interface: if the solver handles 1000 boxes at a given time, every box has it own width, hence its specific instance of ``BxpIsSmall``. These 1000 instances represent the same ''property'' and since there may be other properties attached to the boxes at the same time, we can retreive a given property thanks to its ``id`` field.
You can simply fix this identifier to any random ``long`` number or use the ``next_id()`` function of Ibex as follows:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-id
   :end-before: prop-id

To avoid confusion, we call for now "property value" an instance of the same property. So, ``BxpIsSmall`` is a property and the instances of ``BxpIsSmall`` are property values.

----------------------------------------
Property update
----------------------------------------

The next step is to specify how property values are updated when the box is modified.
This amounts to implement an ``update(...)`` function as follows. This function will 
be called at different points of the search process, through the *trust chain* principle
to be explained further.

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-update
   :end-before: prop-update
 
This is however not enough. You also have to state how the property is transformed
when the box is copied (a copy of a box may occur in a search, e.g., to perform some
temporary computations). This is done by implementing the ``update_copy(...)`` function:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-copy
   :end-before: prop-copy
 

----------------------------------------
Using properties
----------------------------------------

Now, let us modify the implementation of our contractor.
To take benefit of properties, two steps are required.
First, we have to override the ``add_property`` function of the ``Ctc`` interface.
This function is called by the search at initialization.
This function takes as argument the initial box (of the search) and a set of properties
which is an instance of ``BoxProperties``. This object basically just stores pointers to ``Bxp*``,
except that it can handle inter-dependencies (see below).

Second, we have to override a variant of the ``contract`` function,
which takes in argument not only the box, but again, the current set of property values. The latter object 
is an instance of ``BoxProperties`` and also works like a simple map. By using the bracket operator ``[...]`` 
with the property id inside the brackets, you get the corresponding property value associated to the box:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-ctc-2
   :end-before: prop-ctc-2
 
----------------------------------------
Inter-dependencies
----------------------------------------

It may happen that a property is based on another one.
To illustrate, let us refine the behaviour of our contractor.
Assume now that we need to calculate the perimeter of the input box when its width is greater than 0.1 and, as before,
we imagine that this call to ``perimeter'' represents a piece of code you don't want to execute again and again. 
The ''naive'' implementation would be:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-ctc-3
   :end-before: prop-ctc-3

We can create two properties, ``BxpIsSmall`` (the same as before) and ``BxpPerimeter`` (that stores the perimeter).
However, it is useless to spend time updating the perimeter if the box is small. So the ``BxpPerimeter`` class
need to "see" the ``BxpIsSmall`` property in the ``update_xxx(...)`` function. This is why there is also a BoxProperties
the argument of these functions. However, note that the ``BxpPerimer`` value of a small box (with width less than 0.1)
will not represent its perimeter anymore.


