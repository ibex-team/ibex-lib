*************************************************************
              Interval Computations
*************************************************************

This chapter present the basic structures used to represent sets and the operations directly related to them.

=========================================
Intervals, vectors and matrices 
=========================================

Ibex allows you to create interval, interval vectors, interval matrices and array of interval matrices.
All these objects represent sets. These objects are respectively of the following classes:

- ``Interval``
- ``IntervalVector``
- ``IntervalMatrix``
- ``IntervalMatrixArray``.

It should be emphasized that, mainly for efficiency reasons, the previous list is not an inheritance hierarchy.
So, an ``Interval`` is **not** a particular ``IntervalVector`` and so on, altough one might legitimately expect this
(this would also be a perfectly valid design). This is in contrast with Matlab, where, basically, everything is 
an array of interval matrices.

Some functions return points, not intervals; this is typically the case for the function `mid()` that, given
an interval, returns the midpoint. The returned type of ``Interval::mid()`` is theferore ``double``.
In the case of an interval vector, the function `mid()` returns a vector of points. In order to keep our system
homogeneous, a type has been introduced to represent a vector of reals, namely, ``Vector`` and similarly for matrices
or matrices arrays.

So the following types allow to represent real (floating-point) values:

- ``double`` (primitive type)
- ``Vector``
- ``Matrix``
- ``MatrixArray``.


.. _itv-empty-set:

----------------------------
The empty set
----------------------------

One unpleasant consequence of our inheritance-free design is that the empty set does not have a unique representation in Ibex. The empty set is "typed" so that
an empty interval is not the same object as an empty interval vector or an empty interval matrix. But, of course, they can all be interpreted as the same
mathematical object, *the empty set*. 

Worse, an empty interval vector has a size just as any other vectors (and similarly, an empty interval matrix has a certain number of rows/columns, etc.). 
So the empty vector of 3 components is not the same object as the empty vector of 4 components. 
Imposing a size for the empty vector may seem clumsy at first glance, but it actually simplifies programming in several situations.
For instance, if one asks for the complementary of the empty set represented by a n-sized vector, the result will be the n-dimensional box :math:`(-\infty,\infty)\times\ldots\times(-\infty,\infty)`.
Anyway, this multiple representation is harmless in practice because we always manipulate sets of a priori known dimensions.

It holds the same with matrices and arrays of matrices.

----------------------------
Intervals
----------------------------

Intervals are represented by the class  ``Interval`` (this class wraps the actual class of the underlying sub-library that implements interval arithmetic).

Here are examples of interval defintions.
In particular, it is possible to give a single value to define a degenerated interval (reduced to a point).
Note the usage of the constants ``NEG_INFINITY`` and ``POS_INFINITY`` for infinite bounds.

**Note:** In Ibex, an interval should never contains a "NaN" (*not a number*), whatever operation you perform with it. If this occurs, something is wrong.

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-C
   :end-before: itv-build-C

----------------------------
Interval Constants
----------------------------

Some commonly used intervals are already defined as static variables:

=======================  ==================================
``Interval::PI``         a thin enclosure of :math:`\pi`
``Interval::TWO_PI``     a thin enclosure of :math:`\pi`
``Interval::HALF_PI``    a thin enclosure of :math:`\pi/2`
``Interval::EMPTY_SET``  :math:`\emptyset`
``Interval::ALL_REALS``  :math:`(-\infty,+\infty)`
``Interval::ZERO``       :math:`[0,0]`
``Interval::ONE``        :math:`[1,1]`
``Interval::POS_REALS``  :math:`[0,+\infty)`
``Interval::NEG_REALS``  :math:`(-\infty,0]`
=======================  ==================================

Let us print all these constants:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-constants-C
   :end-before: itv-constants-C

The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-constants-O
   :end-before: itv-constants-O


----------------------------
Vectors or Boxes
----------------------------

Vectors of reals are represented by the class ``Vector`` and interval vectors (or "boxes") are represented by the class  ``IntervalVector``.

Creation and initialization of interval vectors can either be done separately (you first create a vector of the desired size and then fill it with the intervals) or
simultaneously, by giving an array of ``double`` in argument of the constructor.
The following example creates twice ([0,1],[2,3],[4,5]):

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-vec-C
   :end-before: itv-build-vec-C

This is the same for vectors:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-vec2-C
   :end-before: itv-build-vec2-C


You can also create an (interval) vector by duplicating n times the same double/interval and copy another (interval) vector:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-vec3-C
   :end-before: itv-build-vec3-C

Finally, you can create an empty interval vector of a given size (see :ref:`itv-empty-set`) or create a degenerated box from a vector of reals:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-vec4-C
   :end-before: itv-build-vec4-C


--------------------------------
Matrices and Array of matrices
--------------------------------

They work exactly the same way as vectors.
Here are first listed different ways to build a ``Matrix``:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-mat-C
   :end-before: itv-build-mat-C

The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-build-mat-O
   :end-before: itv-build-mat-O

Now, for interval matrices, we shall take as example a matrix that frequently appears, the "perturbed" idendity, that is:

.. math::
   \begin{pmatrix}
   [1-\varepsilon,1+\varepsilon]    & \ldots    &  [-\varepsilon,\varepsilon] \\
   \vdots                           & \ddots & \vdots\\
   [-\varepsilon,\varepsilon]       & \ldots & [1-\varepsilon,1+\varepsilon]
   \end{pmatrix}


.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-build-itv-mat-C
   :end-before: itv-build-itv-mat-C

The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-build-itv-mat-O
   :end-before: itv-build-itv-mat-O

============================
Set-membership operations
============================

The operations described here are valid for any "sets" x and y and
any "point" p, where a "set" designates here on object of either of the following classes:

- ``Interval``
- ``IntervalVector``
- ``IntervalMatrix``
- ``IntervalMatrixArray``.

Similarly, a "point" either refers to a ``double`` or an instance of:

- ``Vector``
- ``Matrix``
- ``MatrixArray``.

Given equidimensional sets x, y and a point p, the following table gives the set-membership
predicates currently available in Ibex (from release |release|) and their mathematical intepretation:

+-------------------------------+--------------------------------------------------+------------------+
|*C++ code*                     |  *meaning*                                       |  available from  |
+===============================+==================================================+==================+
|x==y                           | :math:`x=y`                                      |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x!=y                           | :math:`x\neq y`                                  |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.is_empty()                   | :math:`x=\emptyset`                              |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.is_subset(y)                 | :math:`x\subseteq y`                             |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.is_strict_subset(y)          | :math:`x\subseteq y\wedge x\neq y`               |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.is_interior_subset(y)        |  :math:`x\subseteq \mathring{y}`                 | release |release||
+-------------------------------+--------------------------------------------------+------------------+
|x.is_strict_interior_subset(y) | :math:`x\subseteq \mathring{y}\wedge x\neq y`    | release |release||
+-------------------------------+--------------------------------------------------+------------------+
|x.is_superset(y)               | :math:`x\supseteq y`                             |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.is_strict_superset(y)        | :math:`x\supseteq y\wedge x\neq y`               |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.contains(p)                  | :math:`d\in x`                                   |                  |
+-------------------------------+--------------------------------------------------+------------------+
|x.interior_contains(p)         | :math:`d\in \mathring{x}`                        | release |release||
+-------------------------------+--------------------------------------------------+------------------+
|x.intersects(y)                | :math:`x\cap y\neq\emptyset`                     | release |release||
+-------------------------------+--------------------------------------------------+------------------+
|x.overlaps(y)                  |:math:`\mathring{x}\cap \mathring{y}\neq\emptyset`| release |release||
+-------------------------------+--------------------------------------------------+------------------+
|x.is_disjoint(y)               |  :math:`x\cap y=\emptyset`                       | release |release||
+-------------------------------+--------------------------------------------------+------------------+


Possible set-membership operations between sets are:

=============================== ==================================================
*C++ code*                        *meaning*
------------------------------- --------------------------------------------------
x&y                             :math:`x\cap y`
x|y                             :math:`\square(x\cup y)`
x.set_empty()                   :math:`x\leftarrow \emptyset`
x=y                             :math:`x\leftarrow y`
x&=y                            :math:`x\leftarrow (x\cap y)`
x|=y                            :math:`x\leftarrow \square(x \cup y)`
=============================== ==================================================

---------------------------------
Complementary and set difference
---------------------------------

These set operations are only available for intervals (from release |release|) and interval vectors.

In the case of intervals, the function ``complementary``  stores the complementary of ``*this`` in two intervals ``c1`` and ``c2``. More exactly, the result is the
closure of the complementary since open bounds are not representable (except for infinity and minus infinity). E.g, the "complementary" of :math:`(-\infty,0]`
is :math:`[0,+\infty)`.

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-compl-C
   :end-before: itv-compl-C


The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-compl-O
   :end-before: itv-compl-O

In the case of interval vectors, the function ``complementary`` calculates the complementary under the form of a union of non-overlapping interval vectors,
and stores this union into an array that is allocated by the function itself (the variable ``result`` in the code below). The function returns the size of the union/array.

To illustrate this, let us first build a function that prints the complementary of an interval vector:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-func-compl-vec-C
   :end-before: itv-func-compl-vec-C

We can call it now with different vectors. Note that when the vector is the empty set with n components, the complementary is
a n-dimensional box: :math:`(-\infty,\infty)\times\ldots\times(-\infty,\infty)`.
Note also that if the difference is empty, ``result`` is an array of one element set to the empty box. It is *not* a zero-sized array containing no element.

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-compl-vec-C
   :end-before: itv-compl-vec-C

The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-compl-vec-O
   :end-before: itv-compl-vec-O


The set difference works exactly the same way except that the function takes as first argument another set y.

Example with intervals:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-diff-C
   :end-before: itv-diff-C

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-diff-O
   :end-before: itv-diff-O

Example with interval vectors:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-diff-vec-C
   :end-before: itv-diff-vec-C

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-diff-vec-O
   :end-before: itv-diff-vec-O

============================
Interval arithmetic
============================

Interval arithmetic is the main device upon wich IBex is built.

For a complete introduction of interval arithmetic and interval analysis, see :ref:`[Moore 1966] <Moore66>`, :ref:`[Neumaier 1990] <Neumaier90>` or :ref:`[Jaulin et al. 2001] <Jaulin01>`.

We start by detailing the binary operators (+,-,*,/) and then proceed with the nonlinear ones.

--------------------------------------
Addition, subtraction, multiplication
--------------------------------------

The following tables sumarize the basic operations of linear algebra in Ibex.
It is important to notice that (so far?), theses operations are **not** optimized, as compared to
specialized softwares like Intlab_. They are all based on naive implementations.

.. _Intlab: http://www.ti3.tu-harburg.de/rump/intlab/

- Symbols x and y denote scalars (``double`` or instances of ``Interval``);
- Symbols v and w denote vectors (instances of ``Vector`` or ``IntervalVector``)
- Symbols A and B denotes matrices (instances of ``Matrix`` or ``IntervalMatrix``).

Notice that it is not possible (at least in the current release) to appply such operations with objects of type ``IntervalMatrixArray``, the latter
being considered as a pure "set" or, more pragmmatically, as a matrix container. In other words, arrays of interval matrices are not a 3rd order tensor.

Possible additions and subtractions (with optionally assignments) are:

== === === ==== ==== 
-x x+y x-y x+=y x-=y
-v v+w v-w v+=w v-=w
-A A+B A-B A+=B A-=B
== === === ==== ==== 

Possible multiplications are:


=============================== ==================================================
*C++ code*                        *meaning*
------------------------------- --------------------------------------------------
x*y                             multiplication of two "scalars" (double/intervals)
x*=y                            mulitplication and assignment
x*v                             scalar multiplication of a vector
x*A                             scalar multiplication of a matrix
v*w                             dot product (:math:`v^Tw`)
hadamard_product(v,w)           :math:`(v_1w_1,\ldots,v_nw_n)`
outer_product(v,w)              outer product (:math:`vw^T`)
A*x           			matrix-vector product
A*B                             matrix product
=============================== ==================================================


--------------------------------------
Division
--------------------------------------

First, it is possile to use division between two interval/doubles using C++ operator overloading, as for the previous operators.
You can either write ``x/y`` (division) or ``x/=y`` (division and assignment).

However, as a set-membership operator, and because the division of reals is non-continuous, dividing two intevals may result in a union of two intervals.
E.g.,

.. math::
   
   \,\![2,3] / [-1,2] =  \{ x/y, x\in[2,3], y\in[-1,2] \} = (-\infty,-2]\cup[1,+\infty)

This variant of the interval division is called "generalized division" in the litterature.
The previous division operator / simply gives (in Ibex) the hull of this union. So, in C++, [2,3] / [-1,2] gives
:math:`(-\infty,\infty)`. If you want to handle the possible outcome of two intervals, 
you have to call the "div2" function.

Given four intervals x,y, out1 and out2:

  ``div2(x,y,out1,out2)``

will store the union representing x/y in out1 and out2. If the result is a single interval, out2 is set to the empty interval. 
If the result is the empty set, both out1 and out2 are set to the empty interval.

**Note:** Contrary to the "cset" theory, the result is empty if y=[0,0] (whatever x is).

For convenience, there is also a function in the ``Interval`` class that simultaneously performs divsion and intersection::

  bool div2_inter(const Interval& x, const Interval& y, Interval& out2) 

This function sets ``*this`` to the intersection of itself with the division of two others, x and y.

In return, ``*this`` and out2 contains the lower and upper part respectively of the division. 
If the result of the generalized division and intersection is a single interval, out2 is set to the empty interval.

The function returns true if the intersection is non empty.


Example:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-div2-C
   :end-before: itv-div2-C

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-div2-O
   :end-before: itv-div2-O


--------------------------------
Non-linear elementary functions
--------------------------------

The following operations are allowed for an interval x.
They are all interpreted according to the formula:

.. math::

   f([x]) := \square\{f(\alpha), \ \alpha\in [x] \}

The function f, when applied to intervals, is called an "interval extension" of the original function f (applied on real numbers).
The :math:`\square` operator (called "hull") means that an enclosure of the possibly discontinuous set is returned. Note that, among all the functions below,
only ``tan``, ``sign`` and ``integer`` fail to be continuous. So, in the other cases, the "hull" operator can be ommitted.

^^^^^^^^^^^^^^^^^^^^^^^^
Power and roots
^^^^^^^^^^^^^^^^^^^^^^^^

===========   ===================
*C++ code*    *meaning*
-----------   -------------------
sqr(x)        :math:`x^2`          
sqrt(x)       :math:`\sqrt{x}`
pow(x,n)      :math:`x^n`          
pow(x,y)      :math:`x^y = e^{y\log(x)}`
root(x,n)     :math:`\sqrt[n]{x}`  
===========   ===================

^^^^^^^^^^^^^^^^^^^^^^^^
Exponenial, logarithm 
^^^^^^^^^^^^^^^^^^^^^^^^
==========   ========
exp(x)       log(x)
==========   ========

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Trigonometric and hyperbolic functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

==========   ======== ========
cos(x)       sin(x)   tan(x)
acos(x)      asin(x)  atan(x)
cosh(x)      sinh(x)  tanh(x)
acosh(x)     asinh(x) atanh(x)
atan2(x,y)
==========   ======== ========


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Max, min and miscellaneous
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The sign function below is the interval extension of

.. math::
   
   sign(\alpha) := \left\{\begin{array}{ll} -1  & \mbox{if} \ \alpha<0 \\
                                             0  & \mbox{if} \ \alpha=0 \\
                                             1  & \mbox{if} \ \alpha>0 \\
                          \end{array}\right.


*(to be completed)*

==========   =========================================================================
*C++ code*   *meaning*
----------   -------------------------------------------------------------------------
abs(x)       absolute value (not to be confused with magnitude)
max(x,y)     maximum (not to be confused with :math:`\max x\cup y`)
min(x,y)     maximum (not to be confused with :math:`\min x\cup y`)
sign(x)      
integer(x)   the largest interval [a,b] of integers included in x
==========   =========================================================================


============================
Segments, Boxes
============================


*(to be completed)*

Here are the functions that can be applied to intervals x and y, seen as a segments of the line.

+------------------+-------------------+----------------------------------------------------------------------+
| *Return type*    | *C++ code*        | *meaning*                                                            |
+==================+===================+======================================================================+
| double           | x.lb()            | :math:`\underline{x}`, the lower (left) bound of x                   |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.ub()            |  :math:`\overline{x}`, the upper (right) bound of x                  |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.diam()          |  diameter, :math:`|\overline{x}-\underline{x}|`                      |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.rad()           |  radius, half of the diameter                                        |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.mid()           |  the midpoint, (:math:`(\underline{x}+\overline{x})/2`)              |
+------------------+-------------------+----------------------------------------------------------------------+
| Interval         | x.inflate(eps)    |  an interval with the same midpoint and radius increased by eps%     |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | distance(x,y)     | the (Hausdorff) distance between x and y. The distance is 0          |
|                  |                   | iff x==y. Otherwise, it returns the minimal value by which one of the|
|                  |                   | intervals (x or y) has to be inflated so that it entirely overlaps   |
|                  |                   | the other interval.                                                  |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.rel_distance(y) |  the "relative" distance, that is, distance(x,y)/x.diam()            |
+------------------+-------------------+----------------------------------------------------------------------+
| bool             | x.is_bisectable() |  true iff x, as an interval of floating point numbers, can be        |
|                  |                   |  bisected in two non-degenerated intervals of floating point numbers.|
|                  |                   |  The empty interval or an interval of two consecutive floating points| 
|                  |                   |  are not bisectable.                                                 |
+------------------+-------------------+----------------------------------------------------------------------+

Here are the functions that can be applied to interval vectors, seen as boxes.

+------------------+-------------------+----------------------------------------------------------------------+
| *Return type*    | *C++ code*        | *meaning*                                                            |
+==================+===================+======================================================================+
| Vector           | x.lb()            | vector of  lower bounds of x                                         |
+------------------+-------------------+----------------------------------------------------------------------+
| Vector           | x.ub()            | vector of  upper bounds of x                                         |
+------------------+-------------------+----------------------------------------------------------------------+
| Vector           | x.diam()          | vector of diameters, :math:`|\overline{x_i}-\underline{x_i}|`        |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.min_diam()      | minimal diameter, among all components of x                          |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.max_diam()      | maximal diameter, among all components of x                          |
+------------------+-------------------+----------------------------------------------------------------------+
| Vector           | x.rad()           | vector of radii  (halves of diameters)                               |
+------------------+-------------------+----------------------------------------------------------------------+
| Vector           | x.mid()           | the midpoint, (:math:`(\underline{x}+\overline{x})/2`)               |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.volume()        | the volume of the box                                                |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.perimeter()     | the perimeter of the box                                             |
+------------------+-------------------+----------------------------------------------------------------------+
| IntervalVector   | x.inflate(eps)    |  a box  with the same midpoint and each radius increased by eps%     |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | distance(x,y)     | the (Hausdorff) distance between x and y. The distance is 0          |
|                  |                   | iff x==y. Otherwise, it returns the minimal value by which one of the|
|                  |                   | intervals (x or y) has to be inflated so that it entirely overlaps   |
|                  |                   | the other interval.                                                  |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.rel_distance(y) |  the "relative" distance, that is, distance(x,y)/x.max_diam()        |
+------------------+-------------------+----------------------------------------------------------------------+

+ std::pair<IntervalVector,IntervalVector> bisect(int i, double ratio=0.5) const;


*(to be completed)*

====================
Backward arithmetic
====================

*(to be completed)*

