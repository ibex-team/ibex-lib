.. _mod-sys:

*****************************************************
              Systems
*****************************************************


A *system* in IBEX is a set of :ref:`constraints <mod-sys-ctrs>` (equalities or inequalities) with, optionnaly, a goal function to minimize
and an initial domain for variables.
It corresponds to the usual concept of system in mathematical programming.
Here is an example of system:

   Minimize :math:`x+y,`

   :math:`x \in[-1,1], y\in[-1,1]`

   such that

     :math:`x^2+y^2\le1`  

     :math:`y\ge x^2`.


One is usually interested in solving the system while minimizing the criterion, if any.


========================
Class and Fields
========================

The class for representing a system is ``System``.

------------------------------
Systems fields
------------------------------

A system is not as simple as a collection of *any* constraints because
each constraint must exactly relates the same set of arguments. And this set must
also coincide with that of the goal function.
Many algorithms of IBEX are based on this assumption.
This is why they requires a system as argument (and not just an array of constraints).
This makes systems a central concept in IBEX.

A system is an object of the ``System`` class. This object is made of several fields
that are detailed below.


-  ``const int nb_var``: the total number of variables or, in other words, the
   *size* of the problem. This number is basically the sum of all arguments' components. For instance,
   if one declares an argument *x* with 10 components and an argument *y* with 5, the value of this field
   will be 15.
-  ``const int nb_ctr``: the number of constraints.
-  ``Function* goal``: a pointer to the goal function. If there is no goal function, this
   pointer is ``NULL``.
-  ``Function f``: the (usually vector-valued) function representing the constraints. 
   For instance, if one defines three constraints: :math:`x+y\leq0,\ x-y=1`, and :math:`x-y\geq0`, the function f will be 
   :math:`(x,y)\mapsto (x+y,x-y-1,x-y)`. Note that the constraints are automatically transformed so that the right side 
   is 0 but, however, without changing the comparison sign. It is however possible to :ref:`normalize <mod-sys-transfo-normalize>` a system so that
   all inequalities are defined with the :math:`\le` sign (see ).
-  ``IntervalVector box``: when a system is :ref:`loaded from a file  <mod-minibex>`,
   a initial box can be specified. It is contained in this field.
-  ``Array<NumConstraint> ctrs``: the array of constraints. The ``Array`` class of IBEX can
   be used as a regular C array.


.. _mod-sys-auxfunc:

------------------------------
Auxiliary functions
------------------------------

*(to be completed)*

.. _mod-sys-cpp:

================================================
Creating systems (in C++)
================================================

The first alternative for creating a system is to do it programmatically, that is, directly in your C++ program.
Creating a system in C++ resorts to a temporary object called a *system factory*. The task is done in a few simple steps:

-  declare a new system factory (an object of ``SystemFactory``)
-  add arguments in the factory using ``add_var``.
-  (optional) add the expression of the goal function using ``add_goal``
-  add the constraints using ``add_ctr``
-  create the system simply by passing the factory to the constructor of ``System``


Here is an example:


.. code-block:: cpp

   Variable x,y;

   SystemFactory fac;
   fac.add_var(x);
   fac.add_var(y);
   fac.add_goal(x+y);
   fac.add_ctr(sqr(x)+sqr(y)<=1);

   System sys(fac);

If you compare the declaration of the constraint here with the examples given :ref:`here <mod-sys-ctrs-cpp>`,
you notice that we do not list here the arguments before writing ``sqr(x)+sqr(y)<=1``.
The reason is simply that, as said above, the goal function and the constraints in a system
share all the same list of arguments. This list is defined via ``add_var`` once for all.


.. _mod-sys-transfo:

========================
System Transformation
========================

We present in this section the different transformations that can be applied to a system.

.. _mod-sys-transfo-copy:

--------------
Copy
--------------


The first transformation you can apply on a system is a simple copy. Of course, this is done via
the copy constructor of the ``System`` class.

When calling the copy constructor, you can decide to copy everything,
only the equations or only the inequalities. For this, set the second parameter of the
constructor to either: 

==========  =================================
value       def
==========  =================================
COPY        duplicate all the constraints
INEQ_ONLY   duplicate only inequalities
EQ_ONLY     duplicate only equalities
==========  =================================

The first argument of the constructor is the system to copy of course.

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: sys-copy-C
   :end-before: sys-copy-C

The display is:

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: sys-copy-O
   :end-before: sys-copy-O

.. _mod-sys-transfo-normalize:

--------------
Normalization
--------------

It is confortable in some situations to assume that a system is made of inequalities only, and
that each inequality is under the forme :math:`g(x)\le0`, that is, it is a "less or equal" inequality. 
This is called a "normalized" system.

This need arises, e.g., in optimization methods where the calculation of
Lagrange multipliers is simplified when the normalization assumption holds.

It is possible to automatically transform a system into a normalized one.
The process is immediate.
If a constraint is:

- :math:`g(x)\le0`                    
         it is already normalized so it is left unchanged.
- :math:`g(x)<0`                      
         it is replaced by :math:`g(x)\le0` (yes, there is a little loss of precision here)
- :math:`g(x)>0` or :math:`g(x)\ge0`  
         it is replaced by :math:`-g(x)\le0`
- :math:`g(x)=0`                      
         it is replaced by two constraints: :math:`g(x)\le0` and :math:`-g(x)\le0`.
	 It is also possible to introduce an inflation value or "thickness", that is, to replace
         the equality by :math:`g(x)\le\varepsilon` and :math:`-g(x)\le \varepsilon`
         where :math:`\varepsilon` can be fixed to any value.

**Note:** There is a special treatment for "thick equalities", that is, equations of the form
:math:`g(x)=[l,u]`. This kind of equations appear often in, e.g., robust parameter estimation problems.
In this case, the equality is replaced by two inequalities, :math:`g(x)\le u` and :math:`-g(x)\le-l`,
and the :math:`\varepsilon`-inflation is not applied unless :math:`|u-l|<\varepsilon`.

Normalization is done by calling the constructor of ``NormalizedSystem``, a sub-class of ``System``.
Here is an example where ``sys`` is a system built previously:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: sys-normalize-C
   :end-before: sys-normalize-C


We get the following display:

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: sys-normalize-O
   :end-before: sys-normalize-O

.. _mod-sys-transfo-extend:

----------------------------
Extended System
----------------------------

An extended system is a system where the goal function is transformed into a constraint.

For instance, the extension of the system given above:

   Minimize :math:`x+y,`

   :math:`x \in[-1,1], y\in[-1,1]`

   such that

     :math:`x^2+y^2\le1`  

     :math:`y\ge x^2`.

is the following unconstrained system of constraints:

   :math:`x \in[-1,1], y\in[-1,1], \mbox{__goal__}\in(-\infty,\infty)`

   such that

     :math:`x+y=\mbox{__goal__}`

     :math:`x^2+y^2\le1`
   
     :math:`y\ge x^2`

Once built, an extended system is a system like any other one, but it has also some extra information:

- the name of the goal variable which is automatically generated (it is "__goal__" in our previous example). 
- the index of the goal variable (the last (2) in our previous example)
- the index of the "goal constraint" (the first (0) in our previous example)

For this reason, an extended system is represented by a subclass of ``System`` named ``ExtendedSystem``.

To create an extended system just use the constructor of ``ExtendedSystem``.
We assume in the following example that the variable ``sys`` is a ``System`` previously built.

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: sys-extended-C
   :end-before: sys-extended-C

We get the following display:

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: sys-extended-O
   :end-before: sys-extended-O

.. _mod-sys-transfo-fritz-john:

--------------------------------------------------------
Fritz-John (Khun-Tucker) conditions
--------------------------------------------------------

The generalized Khun-Tucker (aka Fritz-John) conditions can be obtained from a system.
This produces a new system of **n+M+R+K+1** variables where

- n is the number of basic variables (the ones of the original system)
- M is the number of Lagrange multipliers for inequalities (i.e., the number of inequalities in the original system)
- R is the number of Lagrange multipliers for equalities (i.e., the number of equalities in the original system)
- K is the number of Lagrange multipliers for bounding constraints. These bounding constraints correspond to the ``box`` 
  field of the original system which is taken into account as 2n additional inequalities.
- The last variable is the "special coefficient" of the goal function that is equal to 0 in the case where constraint
  qualification (linear independency of constraints gradients) does not hold.

Generation of the Fritz-John conditions is based on :ref:`mod-func-symbolic-diff`.

Example:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: sys-fritz-john-C
   :end-before: sys-fritz-john-C

We get the following display. The variable ``_u`` is the coefficient of the goal function. The variable ``_l``
is the multiplier of the constraint.

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: sys-fritz-john-O
   :end-before: sys-fritz-john-O


