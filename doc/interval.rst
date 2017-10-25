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

.. _itv-vector-boxes:

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

===================================
Container operations
===================================

This section presents operations on interval vectors and interval matrices viewed as containers.

Most of the operations are also possible with vectors (``Vector``) and matrices (``Matrix``) by
replacing ``Interval`` with ``double`` and so on. They are not detailed again here, please refer
to the API.

.. table:: Container operations for an interval vector x

   +------------------+-------------------------------+-------------------------------------------------------+
   | Return type      |*C++ code*                     |  *Meaning*                                            |
   +==================+===============================+=======================================================+
   | int              | x.size()                      | The size (number of components)                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | Interval&        | x[i]                          | The ith component. By reference.                      |
   |                  |                               |                                                       |
   |                  | *(int i)*                     |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | IntervalVector   | x.subvector(i,j)              | Return the subvector of length (j-i+1) starting at    |
   |                  |                               | index i and ending at index j (both included).        |
   |                  | *(int i, int j)*              | By copy. [x] must be **non-empty.**                   |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | x.resize(n)                   | Resize the vector. If the vector has been enlarged    |
   |                  |                               | the extra components are set to                       |
   |                  | *(int n)*                     | :math:`(-\infty,+\infty)` even if the vector is empty.|
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | x.put(i, y)                   | Write the vector [y] in [x] at index i.               |
   |                  |                               |                                                       |
   |                  | *(int i, IntervalVector& y)*  |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | x.clear()                     | Set all the elements to [0,0] (even if [x] is empty). |
   |                  |                               | Emptiness is "overridden".                            |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | x.init(y)                     | Set all the elements to [y] (even if [x] is empty).   |
   |                  |                               | Emptiness is "overridden".                            |
   |                  | *(Interval& y)*               |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+

.. table:: Container operations for an interval matrix m

   +------------------+-------------------------------+-------------------------------------------------------+
   | Return type      |*C++ code*                     |  *Meaning*                                            |
   +==================+===============================+=======================================================+
   | int              | m.nb_rows()                   | Number of rows                                        |
   +------------------+-------------------------------+-------------------------------------------------------+
   | int              | m.nb_cols()                   | Number of column                                      |
   +------------------+-------------------------------+-------------------------------------------------------+
   | Interval&        | m[i][j]                       | The (i,j)th entry. By reference.                      |
   |                  |                               |                                                       |
   |                  | *(int i, int j)*              |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | IntervalVector&  | m.row(i)                      | The ith row. By **reference** (fast).                 |
   |                  |                               |                                                       |
   |                  | *(int i)*                     |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | IntervalVector   | m.col(j)                      | The jth column. By **copy** (slow, in O(n))           |
   |                  |                               |                                                       |
   |                  | *(int j)*                     |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | IntervalMatrix   | m.rows(i1,i2)                 | The submatrix obtained by selecting rows between      |
   |                  |                               | index i1 and i2. By copy.                             |
   |                  | *(int i1, int i2)*            |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | IntervalMatrix   | m.cols(j1,j2)                 | The submatrix obtained by selecting columns between   |
   |                  |                               | index j1 and j2. By copy.                             |
   |                  | *(int j1, int j2)*            |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | IntervalMatrix   | m.submatrix(i1,i2,j1,j2)      | The submatrix by selecting rows from index i1         |
   |                  |                               | to i2 (both included) and columns from index j1 to j2 |
   |                  | *(int i1, int i2,             | (both included).                                      |
   |                  | int j1, int j2)*              |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.resize(n1,n2)               | Resize the matrix to a n1xn2 matrix. Extra components |
   |                  |                               | are set to :math:`(-\infty,+\infty)` even if the      |
   |                  | *(int n1, int n2)*            | matrix is empty.                                      |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.set_row(i, y)               | Write the vector [y] in the ith row of [m].           |
   |                  |                               |                                                       |
   |                  | *(int i, IntervalVector& y)*  |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.set_col(i, y)               | Write the vector [y] in the ith column of [m].        |
   |                  |                               |                                                       |
   |                  | *(int i, IntervalVector& y)*  |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.put(i,j,m2)                 | Write the matrix [m2] in [m] at (i,j) and subsequent  |
   |                  |                               | indices                                               |
   |                  | *(int i, int j,               |                                                       |
   |                  | IntervalMatrix& m2)*          |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.put(i,j,v,b)                | Write the vector at (i,j) either in row (if b==true)  |
   |                  |                               | or in column (if b==false).                           |
   |                  | *(int i, int j,               |                                                       |
   |                  | IntervalVector& v, bool b)*   |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.clear()                     | Set all the elements to [0,0] (even if [m] is empty). |
   |                  |                               | Emptiness is "overridden".                            |
   +------------------+-------------------------------+-------------------------------------------------------+
   | void             | m.init(y)                     | Set all the elements to [y] (even if [m] is empty).   |
   |                  |                               | Emptiness is "overridden".                            |
   |                  | *(Interval& y)*               |                                                       |
   +------------------+-------------------------------+-------------------------------------------------------+

.. _itv-set-membership:

============================
Set-membership operations
============================

The operations described here are valid for any "sets" [x] and [y] and
any "point" p, where a "set" designates here on object of either of the following classes:

- ``Interval``
- ``IntervalVector``
- ``IntervalMatrix``
- ``IntervalMatrixArray``.

Similarly, a "point" either refers to a ``double`` or an instance of:

- ``Vector``
- ``Matrix``
- ``MatrixArray``.

Given equidimensional sets [x], [y] and a point p, the following table gives the set-membership
predicates currently available in Ibex (from release |release|) and their mathematical intepretation:

+-------------------------------+------------------------------------------------------+------------------+
|*C++ code*                     |  *Meaning*                                           |  available from  |
+===============================+======================================================+==================+
|x==y                           | :math:`[x]=[y]`                                      |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x!=y                           | :math:`[x]\neq [y]`                                  |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.is_empty()                   | :math:`[x]=\emptyset`                                |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.is_subset(y)                 | :math:`[x]\subseteq [y]`                             |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.is_strict_subset(y)          | :math:`[x]\subseteq  [y]\wedge x\neq [y]`            |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.is_interior_subset(y)        |  :math:`[x]\subseteq \mathring{[y]}`                 | release |release||
+-------------------------------+------------------------------------------------------+------------------+
|x.is_strict_interior_subset(y) | :math:`[x]\subseteq \mathring{[y]}\wedge x\neq [y]`  | release |release||
+-------------------------------+------------------------------------------------------+------------------+
|x.is_superset(y)               | :math:`[x]\supseteq [y]`                             |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.is_strict_superset(y)        | :math:`[x]\supseteq [y]\wedge [x]\neq [y]`           |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.contains(p)                  | :math:`d\in [x]`                                     |                  |
+-------------------------------+------------------------------------------------------+------------------+
|x.interior_contains(p)         | :math:`d\in \mathring{[x]}`                          | release |release||
+-------------------------------+------------------------------------------------------+------------------+
|x.intersects(y)                | :math:`[x]\cap [y]\neq\emptyset`                     | release |release||
+-------------------------------+------------------------------------------------------+------------------+
|x.overlaps(y)                  |:math:`\mathring{[x]}\cap \mathring{[y]}\neq\emptyset`| release |release||
+-------------------------------+------------------------------------------------------+------------------+
|x.is_disjoint(y)               |  :math:`[x]\cap [y]=\emptyset`                       | release |release||
+-------------------------------+------------------------------------------------------+------------------+


Possible set-membership operations between sets are:

=============================== ==================================================
*C++ code*                        *Meaning*
------------------------------- --------------------------------------------------
x&y                             :math:`[x]\cap [y]`
x|y                             :math:`\square([x]\cup [y])`
x.set_empty()                   :math:`[x]\leftarrow \emptyset`
x=y                             :math:`[x]\leftarrow [y]`
x&=y                            :math:`[x]\leftarrow ([x]\cap [y])`
x|=y                            :math:`[x]\leftarrow \square([x] \cup [y])`
=============================== ==================================================

Finally, given two interval vectors [x] and [y] one can call::

  cart_prod(x,y)

to create the Cartesian product.

.. _itv-diff:

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
Note also that if the difference is empty, ``result`` is an array of one element set to the empty box. It is *not* a zero-sized array containing no element
(this is illegal in ISO C++). However the returned number is 0 (not 1). The interesting point is that you can call ``delete[]`` safely, in all cases.

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-compl-vec-C
   :end-before: itv-compl-vec-C

The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-compl-vec-O
   :end-before: itv-compl-vec-O


The set difference works exactly the same way except that the function takes as first argument another set [y].

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

.. _itv-geom:

============================
Geometric operations
============================

Here are the functions that can be applied to an **interval [x]**, seen as a segment of the line.

+------------------+-------------------+----------------------------------------------------------------------+
| *Return type*    | *C++ code*        | *Meaning*                                                            |
+==================+===================+======================================================================+
| double           | x.lb()            | :math:`\underline{x}`, the lower (left) bound of [x]                 |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.ub()            |  :math:`\overline{x}`, the upper (right) bound of [x]                |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.diam()          |  diameter, :math:`|\overline{x}-\underline{x}|`                      |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.rad()           |  radius, half of the diameter                                        |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.mid()           |  the midpoint, (:math:`(\underline{x}+\overline{x})/2`)              |
+------------------+-------------------+----------------------------------------------------------------------+
| Interval         | x.inflate(eps)    |  an interval with the same midpoint and radius increased by eps%     |
|                  |                   |                                                                      |
|                  | *(double eps)*    |                                                                      |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | distance(x,y)     | the (Hausdorff) distance between [x] and [y]. The distance is 0      |
|                  |                   | iff [x]==[y]. Otherwise, it returns the minimal value by which one of|
|                  | *(Interval& y)*   | the intervals ([x] or [y)) has to be inflated so that it entirely    |
|                  |                   | overlaps the other interval.                                         |
+------------------+-------------------+----------------------------------------------------------------------+
| double           | x.rel_distance(y) |  the "relative" distance, that is, distance(x,y)/x.diam()            |
|                  |                   |                                                                      |
|                  | *(Interval& y)*   |                                                                      |
+------------------+-------------------+----------------------------------------------------------------------+
| bool             | x.is_unbounded()  |  true iff [x] has one of its bounds infinite.                        |
+------------------+-------------------+----------------------------------------------------------------------+
| bool             | x.is_bisectable() |  true iff [x], as an interval of floating point numbers, can be      |
|                  |                   |  bisected in two non-degenerated intervals of floating point numbers.|
|                  |                   |  The empty interval or an interval of two consecutive floating points| 
|                  |                   |  are not bisectable.                                                 |
+------------------+-------------------+----------------------------------------------------------------------+

Here are the functions that can be applied to an **interval vector [x]**, seen as a box.

+------------------+-------------------------+----------------------------------------------------------------------+
| *Return type*    | *C++ code*              | *Meaning*                                                            |
+==================+=========================+======================================================================+
| Vector           | x.lb()                  | lower-left corner (vector of  lower bounds of [x])                   |
+------------------+-------------------------+----------------------------------------------------------------------+
| Vector           | x.ub()                  | upper-right corner (vector of  upper bounds of [x])                  |
+------------------+-------------------------+----------------------------------------------------------------------+
| Vector           | x.diam()                | vector of diameters, :math:`|\overline{x_i}-\underline{x_i}|`        |
+------------------+-------------------------+----------------------------------------------------------------------+
| double           | x.min_diam()            | minimal diameter, among all components of [x]                        |
+------------------+-------------------------+----------------------------------------------------------------------+
| double           | x.max_diam()            | maximal diameter, among all components of [x]                        |
+------------------+-------------------------+----------------------------------------------------------------------+
| int              | x.extr_diam_index(b)    | the index of a component with minimal (if b==true) or maximal        |
|                  |                         |  (if b==false) diameter.                                             |
|                  | *(bool b)*              |                                                                      |
+------------------+-------------------------+----------------------------------------------------------------------+
| void             | x.sort_indices(b, tab)  | Write into ``tab`` the indices of all the components, sorted by      |
|                  |                         | increasing (if b==true) /decreasing (if b==false) diameters.         |
|                  |                         |                                                                      |
|                  | *(bool b, int tab[])*   | The array ``tab`` must have been allocated before calling this       |
|                  |                         | function.                                                            |
+------------------+-------------------------+----------------------------------------------------------------------+
| Vector           | x.rad()                 | vector of radii  (halves of diameters)                               |
+------------------+-------------------------+----------------------------------------------------------------------+
| Vector           | x.mid()                 | the midpoint, (:math:`(\underline{x}+\overline{x})/2`)               |
+------------------+-------------------------+----------------------------------------------------------------------+
| double           | x.volume()              | the volume of the box                                                |
+------------------+-------------------------+----------------------------------------------------------------------+
| double           | x.perimeter()           | the perimeter of the box                                             |
+------------------+-------------------------+----------------------------------------------------------------------+
| bool             | x.is_flat()             | true if the volume is null (one dimension is degenerated)            | 
+------------------+-------------------------+----------------------------------------------------------------------+
| IntervalVector   | x.inflate(eps)          | a box  with the same midpoint and each radius increased by eps%      |
|                  |                         |                                                                      |
|                  | *(double eps)*          |                                                                      |
+------------------+-------------------------+----------------------------------------------------------------------+
| double           | distance(x,y)           | the (Hausdorff) distance between [x] and [y]. The distance is 0      |
|                  |                         | iff [x]==[y]. Otherwise, it returns the minimal value by which one of|
|                  | *(IntervalVector& y)*   | the boxes ([x] or [y]) has to be inflated so that it                 |
|                  |                         | entirely overlaps the other box.                                     |
+------------------+-------------------------+----------------------------------------------------------------------+
| double           | x.rel_distance(y)       |  the "relative" distance, that is, distance(x,y)/x.max_diam()        |
|                  |                         |                                                                      |
|                  | *(IntervalVector& y)*   |                                                                      |
+------------------+-------------------------+----------------------------------------------------------------------+
| bool             | x.is_unbounded()        |  true iff [x] has one of its bounds infinite.                        |
+------------------+-------------------------+----------------------------------------------------------------------+
| bool             | x.is_bisectable()       |  true iff at least one component of [x] is bisectable (see above).   |
+------------------+-------------------------+----------------------------------------------------------------------+

.. _itv-bisect:

-----------------
Bisection
-----------------

Bisecting a box is a fundamental operation in Ibex. The choice of the component to be bisected is often critical
and defining a strategy for chosing this component is the purpose of :ref:`strategy-bisectors`.

To bisect the ith component, the ``bisect`` function can be used.
The first arugment is the index of the dimension i along which the box has to be splitted.
The second argument is optionnal and gives at which point the ith interval is bisected.
This argument is entered as a ratio of the interval diameter, 0.5 means "midpoint" (and the default value is precisely 0.5).
Example:

.. literalinclude:: ../examples/doc-arithmetic.cpp 
   :language: cpp
   :start-after: itv-bisect-C
   :end-before: itv-bisect-C

The output is:

.. literalinclude:: ../examples/doc-arithmetic.txt
   :start-after: itv-bisect-O
   :end-before: itv-bisect-O

====================
Miscellaneous
====================

Given an interval [x]:

+------------+--------------------------------------------------------------------------+
| *C++ code* |  *Meaning*                                                               |
+============+==========================================================================+
| x.mig()    | Mignitude, :math:`\min_{x\in[x]} |x|` (a double). Also exists for        |
|            | interval vectors and matrices (gives a vector or matrix of mignitudes).  |
+------------+--------------------------------------------------------------------------+
| x.mag()    | Magnitude, :math:`\max_{x\in[x]} |x|` (a double). Also exists for        |
|            | interval vectors and matrices (gives a vector or matrix of magnitudes).  |
+------------+--------------------------------------------------------------------------+
|integer(x)  | The largest interval [a,b] of integers included in [x].                  |
+------------+--------------------------------------------------------------------------+

Finally, for an interval vector [v]:

+------------+--------------------------------------------------------------------------+
|v.random()  | A random point inside x                                                  |
+------------+--------------------------------------------------------------------------+

.. _itv-arith:

============================
Interval arithmetic
============================

Interval arithmetic is the main device upon wich Ibex is built.

For a complete introduction of interval arithmetic and interval analysis, see :ref:`[Moore 1966] <Moore66>`, :ref:`[Neumaier 1990] <Neumaier90>` or :ref:`[Jaulin et al. 2001] <Jaulin01>`.

The interval arithmetic defines for each elementary function :math:`f:\mathbb{R}^n\to\mathbb{R}` an interval function 
:math:`[f]:\mathbb{IR}^n\to\mathbb{IR}` according to the formula:

.. math::

   [f]([x]) \supseteq \square\{f(\alpha), \ \alpha\in [x] \}

where :math:`\square` (the "hull" symbol) in front of a set means "the smallest box enclosing this set".
The hull symbol is here to highlight the fact that an enclosure of the possibly discontinuous set is returned. Note that, among all the functions below,
only ``tan`` and ``sign`` fail to be continuous. So, in the other cases, the "hull" operator can be ommitted.
The possible roundoff error accounts for the :math:`\supseteq` symbol.

The function [f] is called an "interval extension" of the original function f (applied on real numbers).

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
*C++ code*                        *Meaning*
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

Given four intervals [x],[y], [out1] and [out2]:

  ``div2(x,y,out1,out2)``

will store the union representing [x]/[y] in [out1] and [out2]. If the result is a single interval, [out2] is set to the empty interval. 
If the result is the empty set, both [out1] and [out2] are set to the empty interval.

**Note:** Contrary to the "cset" theory, the result is empty if y=[0,0] (whatever [x] is).

For convenience, there is also a function in the ``Interval`` class that simultaneously performs divsion and intersection::

  bool div2_inter(const Interval& x, const Interval& y, Interval& out2) 

This function sets ``*this`` to the intersection of itself with the division of two others, [x] and [y].

In return, ``*this`` and [out2] contains the lower and upper part respectively of the division. 
If the result of the generalized division and intersection is a single interval, [out2] is set to the empty interval.

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

The following operations are allowed for an interval [x].

^^^^^^^^^^^^^^^^^^^^^^^^
Power and roots
^^^^^^^^^^^^^^^^^^^^^^^^

===========   ===================
*C++ code*    *Meaning*
-----------   -------------------
sqr(x)        :math:`[x]^2`          
sqrt(x)       :math:`\sqrt{[x]}`
pow(x,n)      :math:`[x]^n`          
pow(x,y)      :math:`[x]^{[y]} = e^{[y]\log([x])}`
root(x,n)     :math:`\sqrt[n]{[x]}`  
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
atan2(y,x)
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

+------------+-------------------------------------------------------------------------+
| *C++ code* |  *Meaning*                                                              |
+============+=========================================================================+
|abs(x)      | interval extension of the absolute value. Not to be confused with       |
|            | magnitude; e.g. with [x]=[-2,1], abs([x])=[0,2] and the magnitude is 2. |
+------------+-------------------------------------------------------------------------+
|max(x,y)    | interval extension of the maximum. Not to be confused with              |
|            | :math:`\max ([x]\cup [y])`; e.g., max([0,3],[1,2])=[1,3].               |
+------------+-------------------------------------------------------------------------+
|min(x,y)    | interval extension of the minimum. Not to be confused with              |
|            | :math:`\min ([x]\cup [y])`; e.g., min([0,3],[1,2])=[0,2].               |
+------------+-------------------------------------------------------------------------+
|sign(x)     | (see above)                                                             |
+------------+-------------------------------------------------------------------------+

.. _itv-bwd-arith:

====================
Backward arithmetic
====================

The "backward" or "relationnal" arithmetic consists in contraction operators for elementary constraints like

.. math::
 
   y= x_1+x_2.

More precisely, given a function :math:`f:\mathbb{R}\to\mathbb{R}` (:math:`\sin, \cos, \exp\, \ldots`) the backward operator of f
is an interval function :math:`[f]_{bwd}:\mathbb{IR}^2\to\mathbb{IR}` that satisfies:

.. math::
  
   [f]_{bwd}([x],[y]) \supseteq \{ x\in [x] \ | \ \exists y\in [y], \quad y=f(x) \}.

Similarly, given a binary function :math:`f:\mathbb{R}^2\to\mathbb{R}` (:math:`+, -, \times, \max, \ldots`) the backward operator 
is a function :math:`[f]_{bwd}:\mathbb{IR}^2\to\mathbb{IR}` that satisfies:

.. math::
  
   [f]_{bwd}([x]_1,[x]_2,[y])\supseteq\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=f(x_1,x_2)\}.


In practice, backward operators in Ibex do not return an interval, they directly contracts the input interval [x] (in case of unary functions), that is:

.. math::
  
   [x] \leftarrow [f]_{bwd}([x],[y])

or the two intervals [x1], [x2] (in case of binary functions):

.. math::
   ([x]_1,[x]_2) \leftarrow [f]_{bwd}([x]_1,[x]_2,[y]).

So the bwd_* functions below all return ``void``.

**Note:** One important feature in Ibex is the ability to contract with respect to any constraints and, in particular, with constraints under the form

.. math::

   y=f(x_1,\ldots,x_n)

where f is an arbirary :ref:`function <mod-func>`. So, as a user, there is probably little interset for you to call the low-level routines presented here. 
Moreover, these routines only contracts the "pre-image argument" [x]; they **don't contract the "image argument" [y]**. This is in contrast with high-level contractors
like :ref:`ctc-fwd-bwd` that do both contractions (on [x] and [y]).


+--------------------------+-----------------------------------+
+ *C++ code*               | *Relation*                        |
+==========================+===================================+
+  bwd_add(y,x1,x2)        | :math:`y=x_1+x_2`                 |
+--------------------------+-----------------------------------+
+  bwd_sub(y,x1,x2)        | :math:`y=x_1-x_2`                 |
+--------------------------+-----------------------------------+
+  bwd_mul(y,x1,x2)        | :math:`y=x_1\times x_2`           |
+--------------------------+-----------------------------------+
+  bwd_div(y,x1,x2)        | :math:`y=x_1/x_2`                 |
+--------------------------+-----------------------------------+
+  bwd_sqr(y,x)            | :math:`y=x^2`                     |
+--------------------------+-----------------------------------+
+  bwd_sqrt(y,x)           | :math:`y=\sqrt{x}`                |
+--------------------------+-----------------------------------+
+  bwd_pow(y,n,x)          | :math:`y=x^n`                     |
+--------------------------+-----------------------------------+
+  bwd_pow(y,x1,x2)        | :math:`y=x_1^{x_2}`               |
+--------------------------+-----------------------------------+
+  bwd_root(y,n,x)         | :math:`y=\sqrt[n]{x}`             |
+--------------------------+-----------------------------------+
+  bwd_exp(y,x)            | :math:`y=\exp{x}`                 |
+--------------------------+-----------------------------------+
+  bwd_log(y,x)            | :math:`y=\ln{x}`                  |
+--------------------------+-----------------------------------+
+  bwd_cos(y,x)            | :math:`y=\cos{x}`                 |
+--------------------------+-----------------------------------+
+  bwd_sin(y,x)            | :math:`y=\sin{x}`                 |
+--------------------------+-----------------------------------+
+  bwd_tan(y,x)            | :math:`y=\tan{x}`                 |
+--------------------------+-----------------------------------+
+  bwd_acos(y,x)           | :math:`y=\arccos{x}`              |
+--------------------------+-----------------------------------+
+  bwd_asin(y,x)           | :math:`y=\arcsin{x}`              |
+--------------------------+-----------------------------------+
+  bwd_atan(y,x)           | :math:`y=\arctan{x}`              |
+--------------------------+-----------------------------------+
+  bwd_cosh(y,x)           | :math:`y=\cosh{x}`                |
+--------------------------+-----------------------------------+
+  bwd_sinh(y,x)           | :math:`y=\sinh{x}`                |
+--------------------------+-----------------------------------+
+  bwd_tanh(y,x)           | :math:`y=\tanh{x}`                |
+--------------------------+-----------------------------------+
+  bwd_acosh(y,x)          | :math:`y=\operatorname{arcosh}(x)`|
+--------------------------+-----------------------------------+
+  bwd_asinh(y,x)          | :math:`y=\operatorname{arsinh}(x)`|
+--------------------------+-----------------------------------+
+  bwd_atanh(y,x)          | :math:`y=\operatorname{artanh}(x)`|
+--------------------------+-----------------------------------+
+  bwd_atan2(y,x1,x2)      |:math:`y=atan2(x_1,x_2)`           |
+--------------------------+-----------------------------------+
+  bwd_abs(y,x)            | :math:`y=|x|`                     |
+--------------------------+-----------------------------------+
+  bwd_sign(y,x)           | :math:`y=sign(x)` (see def. above)|
+--------------------------+-----------------------------------+
+  bwd_integer(y,x)        | :math:`y=x \wedge x\in\mathbb{N}` |
+--------------------------+-----------------------------------+
+  bwd_min(y,x1,x2)        |:math:`y=\min(x_1,x_2)`            |
+--------------------------+-----------------------------------+
+  bwd_max(y,x1,x2)        |:math:`y=\max(x_1,x_2)`            |
+--------------------------+-----------------------------------+

.. _itv-inner-arith:

====================
Inner arithmetic
====================

The inner arithmetic  :ref:`[Chabert & Beldiceanu 2010] <Chabert10>` :ref:`[Araya et al 2014] <Araya14>` consists in two types of operators.
Given a function :math:`f:\mathbb{R}^n\to\mathbb{R}`:

- the **forward inner operator** is an interval function 
  :math:`]f[:\mathbb{IR}^n\to\mathbb{IR}` that satisfies the formula:

  .. math::

     ]f[([x]) \subseteq \{f(\alpha), \ \alpha\in [x] \}

- the **backward inner operator** is an interval function 
  :math:`]f[_{bwd}:\mathbb{IR}^{n+1}\to\mathbb{IR}` that satisfies the formula:

  .. math::

     ]f[_{bwd}([x],[y]) \subseteq \{ x\in [x] \ | \ \exists y\in [y], \quad y=f(x) \}.


Notice that the inclusion symbol has just been reversed, as compared to the classical interval (forward) and backward arithmetic.


-------------------------
Forward operators
-------------------------

**Note:** The inner operators are not implemented for all elementary functions yet.

+------------+--------------+
| *C++ code* | *Function*   |
+============+==============+
|iadd(x1,x2) | x1+x2        |
+------------+--------------+
|isub(x1,x2) | x1-x2        |
+------------+--------------+
|imul(x1,x2) | x1*x2        |
+------------+--------------+
|idiv(x1,x2) | x1/x2        |
+------------+--------------+
|isqr(x)     | x^2          |
+------------+--------------+
|iminus(x)   | -x           |
+------------+--------------+
|ilog(x)     | ln(x)        |
+------------+--------------+
|iexp(x)     | exp(x)       |
+------------+--------------+
|iacos(x)    | cos(x)       |
+------------+--------------+
|iasin(x)    | sin(x)       |
+------------+--------------+
|iatan(x)    | tan(x)       |
+------------+--------------+

-------------------------
Backward operators
-------------------------

Let us denote

  .. math::

     \mathcal{S}:=\{ x\in [x] \ | \ \exists y\in [y], \quad y=f(x) \}.

The backward operator also takes an optional argument ``xin`` that represents an "inner" box, that is a box which already satisfies

  .. math::

     [x]_{in} \subseteq \mathcal{S}.

If this argument is set, the operator guarantees that the contracted box [x] will enclose [xin]. The operator works in this case as an *inflator*, the
inner box [xin] is indeed "inflated" to a larger inner box [x]. More precisely, we have:

  .. math::

     [x]_{in} \ \subseteq \ ]f[_{bwd}([x],[y],[x]_{in}) \subseteq ([x] \cap \mathcal{S}).

**Note:** The inner operators are not implemented for all elementary functions yet.

+------------------------------+------------------+
| *C++ code*                   | *Relation*       |
+============+=================+==================+
|ibwd_add(y,x1,x2,xin1,xin2)   | y=x1+x2          |
+------------------------------+------------------+
|ibwd_sub(y,x1,x2,xin1,xin2)   | y=x1-x2          |
+------------------------------+------------------+
|ibwd_mul(y,x1,x2,xin1,xin2)   | y=x1*x2          |
+------------------------------+------------------+
|ibwd_div(y,x1,x2,xin1,xin2)   | y=x1/x2          |
+------------------------------+------------------+
|ibwd_sqr(y,x,xin)             | y=x^2            |
+------------------------------+------------------+
|ibwd_sqrt(y,x,xin)            |:math:`y=\sqrt{x}`|
+------------------------------+------------------+
|ibwd_minus(y,x,xin)           | y=-x             |
+------------------------------+------------------+
|ibwd_abs(y,x,xin)             | :math:`y=|x|`    |
+------------------------------+------------------+
|ibwd_pow(y,x,p,xin)           | y=x^p            |
+------------------------------+------------------+
|ibwd_log(y,x,xin)             | y=ln(x)          |
+------------------------------+------------------+
|ibwd_exp(y,x,xin)             | y=exp(x)         |
+------------------------------+------------------+
|ibwd_cos(y,x,xin)             | y=cos(x)         |
+------------------------------+------------------+
|ibwd_sin(y,x,xin)             | y=sin(x)         |
+------------------------------+------------------+
|ibwd_tan(y,x,xin)             | y=tan(x)         |
+------------------------------+------------------+



