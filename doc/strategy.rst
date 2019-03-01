**************************************************
              Strategies  
**************************************************


.. _bxp:

==================
Box Properties
==================

**(In release 2.7)**

The box (``IntervalVector`` class) is the central concept in Ibex and often this structure is too simple
and required to be extended.

Consider for example a search tree, such as the one performed by ibexsolve.
Assume you have several contractors involved in this search that need to calculate at some
point the image of the current box by a function ``f``. 
Imagine also that this function has quite a long expression so that calculating an image takes significant time:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-ctc-1
   :end-before: prop-ctc-1

**Note:** For simplicity, we assume from now on that ``f`` is a fixed object that has been declared somewhere in the context 
(it could also be given in argument of all objects).

Recalculating this image each time a contractor is called represents a waste of time if
the box hasn't changed meanwhile. One would like to store this information in the box. 
This is the kind of things ''box properties'' allow to do.

All is based on the ``Bxp`` interface. This ``Bxp`` interface allows to extend the simple ``IntervalVector`` data structure and to make this extension being propagated through a strategy (search tree). The extended box is then visible by all operators involved in the strategy (contractors, bisectors, cell buffers, etc.).

Note that the name of this interface is a trigram (like ``Ctc`` or ``Bsc``) 
just to encourage programers to prefix subclasses by ``Bxp`` (this is a recommended usage).
A box property, such as the image of the box by a given function, has to be a subclass of ``Bxp`` so let us name it ``BxpImage``:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-class-name
   :end-before: prop-class-name

Of course, this class contains a field named ``image`` that will store the information.
We could also add whatever data needed.

.. _bxp_construct:

----------------------------------------
Constructor
----------------------------------------

It is natural to ask the constructor of ``BxpImage`` to take a box in argument and to set the ``image`` field appropriately.

The constructor of the mother class ``Bxp`` also requires an identifying number. Here is why. A box property is actually a set of *instances* of the ``Bxp`` interface: if the solver handles 1000 boxes at a given time, every box has it own image, hence its specific instance of ``BxpImage``. These 1000 instances represent the same ''property'' and since there may be other properties attached to the boxes at the same time, we can retreive a given property thanks to its ``id`` field. (**Note**: using the class name directly as identifier would be too restrictive as there may be different ``BxpImage`` properties attached to different functions ``f``). 
You can simply fix this identifier to any random ``long`` number or use the ``next_id()`` function of Ibex as follows:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-id
   :end-before: prop-id

**Note**: In the case of several ``BxpImage`` properties (one for each function ``f``) you can store identifying numbers in a map structure (see examples in the Ibex code).

To avoid confusion, we call for now "property value" an instance of the same property. So, ``BxpImage`` is a property (or a set of properties, one for each ``f``) and the instances of ``BxpImage`` are property values.

.. _bxp_update:

----------------------------------------
Property update
----------------------------------------

The next step is to specify how property values are updated when the box is modified.
This amounts to implement an ``update(...)`` function as follows. This function will 
be called at different points of the stragegy, through the *trust chain* principle
to be explained further.

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-update
   :end-before: prop-update
 
Note that a :ref:`smarter implementation <bxp_lazy>` is often desired.
This is however not enough. You also have to state how the property is transformed
when the box is copied (copies occur in a search each time a box is bisected
or, e.g., to perform some temporary computations). This is done by implementing the ``copy()`` function:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-copy
   :end-before: prop-copy
 
.. _bxp_using:

----------------------------------------
Using properties
----------------------------------------

Now, let us modify the implementation of our contractor.
To take benefit of properties, two steps are required.
First, we have to override the ``add_property`` function of the ``Ctc`` interface.
This function is called by all strategies at initialization.
This function takes as argument the initial box (of the search) and a container for property values: an instance of ``BoxProperties``. This object basically just stores pointers to ``Bxp*``, except that it can handle :ref:`inter-dependencies <bxp_dependencies>`.

Second, we have to override a variant of the ``contract`` function,
which takes in argument not only the box, but also a ``ContractContext`` object which contains, among other things, the current property values container (again, an instance of ``BoxProperties``).
The ``BoxProperties`` class works like a simple map: by using the bracket operator ``[...]`` 
with the property id inside the brackets, you get the corresponding property value associated to the box:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-ctc-2
   :end-before: prop-ctc-2

.. _bxp_lazy:
 
----------------------------------------
Lazy update
----------------------------------------

So far, we have centralized in a unique place the result of the image computation which is already good but
not optimal at all. Worse, the running time of our program will likely be longer than without
introducing this property! Indeed, the ``update`` function will be called basically whenever an operator
change the box, which means that the funtion f will be evaluated again and again!

This event-oriented design of a property can be sometimes interesting but, clearly, it does not fit well here.

What we actually want is the function to postpone the evaluation at the latest time, that is, when someone requires it.
This is called laziness. This principle can be simply applied here as follows:

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-lazy
   :end-before: prop-lazy

It is easy to adapt this code so that an update is performed only when the box modification is significant (e.g., when
a contraction removes more than 1% of a component width).

.. _bxp_dependencies:

----------------------------------------
Dependencies
----------------------------------------

It may happen that a property is based on another one.
Imagine that you want to create a property that stores the width of the function image 
(of course, this example is caricatural as the width is not something you really need to store).
You can extend the ``BxpImage`` class but you can also create a separate property, say ``BxpImageWidth``.

The ``BxpImageWidth`` need to "see" the ``BxpImage`` property in the ``update_xxx(...)`` function. This is why there is also a ``BoxProperties`` map in the argument of these functions. 
Furthermore, we must be sure that the ``BxpImage`` is updated before ``BxpImageWidth``. To this end, we simply have to add the
identifier of ``BxpImage`` in the *dependencies* of ``BxpImageWidth``. This must be done in the constructor of ``BxpImageWidth`` as shown in the following code.

.. literalinclude:: ../examples/doc-strategy.cpp 
   :language: cpp
   :start-after: prop-dependencies
   :end-before: prop-dependencies

For the sake of concision, we haven't used laziness in this code. A lazy variant is necessary here.

----------------------------------------
Trust chain principle
----------------------------------------
The trust chain principle is the following:

- Property values are always up-to-date when given to argument of a function.

Consider a function that handles properties (e.g., an implementation of the ``contract`` variant with ``BoxProperties``, as above).
It the box is modified at several points in the code, it is not necessary to perform updates as long
as properties are not used elsewhere. The update can be postponed to the
point where property values are transmitted to another function or, on last resort, before returning.

Note that updating all property values can be simply done via the ``update`` function of ``BoxProperties`` (this
also allows to respect dependencies).

As a consequence, if the function does not modify itself the box (would it calls other functions that potentially modify it), it does not
have to perform at all any update of property values.


.. _cov:

==================
COV files
==================

File Input/Output operations in Ibex are based on the COV format.  

The COV format is actually a *family* of formats because Ibex is a library
upon which different executables are built, each handling different types of data.
There is no one-fits-all definition in this context. 
When we talk about 'programs' we mean not only the official ones supplied with
the library itself (like ``ibexsolve``, ``ibexopt``, ...) but all the programs that
can be potentially developed by Ibex users.

The common point of these programs is that they usually calculate sets with boxes,
because this is what Ibex is all about.
More precisely, they calculate boxes that form a *covering* of some set, whence the
name 'COV'.

So, instead of creating a monolithic file format for each program, we can imagine a
file format made of two parts:

- a part common to all programs containing a description of the covering;
- a part specific to each program gathering additional information handled by the proram.

This way, all the programs can now exchange data by extracting from a file the 
common part. Let us call this part after the corresponding C++ class: ``CovList``.

In fact, we can go a step further. 
In a covering of a set, we often distinguish *inner* boxes, that is, the ones that
are proven to be inside the set,  and *unknwon* boxes, the other ones.
But not all programs do. So, instead of replacing the ``CovList`` by a more elaborated
structure, we rather insert an intermediate level of information in the format; now,
a program that makes a distinction between inner and unknown boxes has an associated 
file format made of three parts:

- the ``CovList`` part that tells which boxes form the covering
- the ``CovIUList`` part that tells which among the ``CovList`` boxes are inner (<-> 'I'), the
  other being implicitly unknown (<> 'U').
- the part specific to the program

This way, the program can communicate with another one handling inner boxes. But
it can still communicate with all the other programs through the ``CovList`` level.

This principle can be extended to any level and gives rise to a hierarchy of formats,
where each level refines the semantics of the previous one. This is how the COV format works.
This principle allows programs to store and exchange data at the right level
and without restriction on what amount of data they actually need. 
The top-level format is not ``CovList`` in fact, but something more general simply called ``Cov``,
because we may represent a covering by something else than a list (example: a quadtree).

The file format managed by ``ibexsolve`` is called ``CovSolverData`` and is 5 levels
below the root format ``Cov``. We have:

- ``Cov``: a covering (root format)
- ``CovList``: a Cov described by a list of boxes
- ``CovIUList``: a Covlist with some boxes marked as *inner*
- ``CovIBUList``: a CovIUlist with some boxes marked as *boundary* 
- ``CovManifold``: a CovIBUList with some boxes marked as *solutions* (i.e., they enclose 
  a subset of the manifold in a precise way).
- ``CovSolverData``: a CovManifold with additional data related to the solving process

-------------------------------------
First example
-------------------------------------

We illustrate the usage of COV files with ``ibexsolve``.
Let us first solve a simple problem, like an inequality:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex1-build-C
   :end-before: cov-ex1-build-C
   
The result of the solver is a COV object we can access to with the ``get_data`` function.
So, we can cast the returned object to a ``CovList`` and access to the result as
a simple list of boxes:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex1-list-C
   :end-before: cov-ex1-list-C

The output is:

.. literalinclude:: ../examples/doc-covfiles.txt
   :start-after: cov-ex1-list-O
   :end-before:  cov-ex1-list-O

We can also cast the object to a ``CovIUList`` and access to the result as two
lists: the list of inner boxes and the list of unknown boxes:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex1-IUlist-C
   :end-before: cov-ex1-IUlist-C

The output is:

.. literalinclude:: ../examples/doc-covfiles.txt
   :start-after: cov-ex1-IUlist-O
   :end-before:  cov-ex1-IUlist-O

We jump now directly to the ``CovSolverData`` format.
Among other things, the format distinguishes two types of boxes
in the unknown boxes in the ``CovManifold`` view:

- the boxes that have reached the precision (1.0) and which could neither
  be proven to be inner or outer. They are also called *unknown* at the
  ``CovSolverData`` level.
- the boxes that have not been processed because the solving process
  was interrupted for whatever reason (time out or cell overflow).
  They are called *pending* boxes.
  
We can seen now that the solver was actually interrupted because
of the cell limit we fixed:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex1-SolverData-C
   :end-before: cov-ex1-SolverData-C

The output is:

.. literalinclude:: ../examples/doc-covfiles.txt
   :start-after: cov-ex1-SolverData-O
   :end-before:  cov-ex1-SolverData-O

------------------------------------------------
Exchanging data between IbexSolve and IbexOpt
------------------------------------------------

The power of COV files is now illustrated by feeding the optimizer
with the output of the solver and vice versa.

To this end, we consider a problem where we optimize a simple criterion 
(x+y) over the inequality x^2+y^2<=1. But instead of running the optimizer with the overall 
problem, we treat the constraint and the criterion separately: we
first produce a covering of the constraint with the solver
and then apply the unconstrained optimizer on the covering.

We have already built in the previous example the system ``sys``. Let us
run the solver with a higher precision:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex2-solver-C
   :end-before: cov-ex2-solver-C
   
Let us now build the system representing the unconstrained optimization problem:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex2-optim-C
   :end-before: cov-ex2-optim-C

And now, we run the optimizer on the solver output:


.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex2-run-C
   :end-before: cov-ex2-run-C

The output is:

.. literalinclude:: ../examples/doc-covfiles.txt
   :start-after: cov-ex2-run-O
   :end-before:  cov-ex2-run-O
   
Let us do it now in the other way around. 
We first run the optimizer on the objective function abs(x^2+y^2-1) so that the whole unit circle is made of global minima.
However, if we set the problem like this, the optimizer quickly converges to a unique global minimum because it
resorts to some 'anticipated' bounding techniques.
To force the optimizer to keep all the circle, we replace the constant 1 by an interval thicker than the optimizer precision:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex3-optim-sys-C
   :end-before: cov-ex3-optim-sys-C

Then we run the optimizer. We fix a timeout to prevent him from bisecting again and again inside the thick circle.
This way, we obtain a coarse approximation of the circle.
If we want to communicate the result to the solver, an extra instruction is also necessary; Indeed, by default, the optimizer generates
a COV file in the *extended space*, that is, it generates 3 dimensional boxes, the third dimension corresponding to
the objective values. By default, a box in the optimizer data is ([x],[y],f([x],[y]). If we feed the solver with such
boxes, it will refuse with a dimension mismatching error message. So we need to configure the optimizer 
so that only variables domains are produced.

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex3-optim-run-C
   :end-before: cov-ex3-optim-run-C

We can now build the solver to solve a constraint, like x-y=0. 


.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex3-solver-sys-C
   :end-before: cov-ex3-solver-sys-C

and feed it with the optimizer data:

.. literalinclude:: ../examples/doc-covfiles.cpp 
   :language: cpp
   :start-after: cov-ex3-solver-run-C
   :end-before: cov-ex3-solver-run-C

The output indeeds contains a sequence of boxes enclosing the intersection of the
unit circle and the line x=y:

.. literalinclude:: ../examples/doc-covfiles.txt
   :start-after: cov-ex3-solver-run-O
   :end-before:  cov-ex3-solver-run-O
  
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
