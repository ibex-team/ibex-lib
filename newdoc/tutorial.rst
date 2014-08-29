.. _tuto: 

**************************************
              Tutorial
**************************************

===============
Getting started
===============

----------------------
Run the default solver
----------------------

A couple of example programs are installed with Ibex under the ``examples/`` subfolder.

One of them is the *default solver* that solves a system of nonlinear equations rigorously
(that is, it does not lose any solution and return each solution under the form of a 
small box enclosing the true value). It is called the "default" solver because is resorts to
a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that Ibex also supplies a "generic" solver that is much more customizable.

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

---------------
Start a program
---------------

To write a program with Ibex, use the following canvas:

.. code-block:: cpp

  #include "ibex.h"

  using namespace std;
  using namespace ibex;

  int main(int argc, char** argv) {

  // write your own code here

  }

You can execute by yourself all the code snippets of this tutorial, using this canvas.

To compile a program, the easiest way is to copy-paste the ``makefile`` of the ``examples/`` subfolder of Ibex.
See also :ref:`install-compiling-running`.

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


=============================
Basic Interval computations
=============================

------------------------------ 
Creating intervals
------------------------------

Here are examples of intervals

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-create-itv
   :end-before: basic-create-itv

------------------------------ 
Operation between intervals
------------------------------

C++ operator overloading allows you to calculate the sum of two
intervals by using directly the "+" symbol:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-op-itv
   :end-before: basic-op-itv

You can use the other operators similarly (``-``, ``*``, ``/``).

---------------------------------- 
Applying a function to an interval
----------------------------------

All the elementary functions can be applied to intervals, and composed in an arbitrarly way:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-func-itv
   :end-before: basic-func-itv

------------------------------ 
Interval vectors
------------------------------

You can create an interval vector by using an intermediate array of n*2 ``double``, representing the lower and
uppoer bounds of each components.
The first argument of the constructor of ``IntervalVector in this case is the dimension (here, 3),
the second the array of ``double``.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-create-vec1
   :end-before: basic-create-vec1

You can also create an interval vector by duplicating a given interval or simply create the empty interval vector.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-create-vec2
   :end-before: basic-create-vec2

------------------------------ 
Interval matrices
------------------------------

Interval matrices can be created in a similar way. However, since we cannot build 3-dimensional arrays in C++,
all the bounds must be set in a single n*2 array representing the matrix row by row (and n is the total number of entries
of the matrix). The two first arguments of the constructor are the number of rows and columns respectively. The last one
is the array of ``double``.
Here is an example of a 3x3 matrix:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-create-mat
   :end-before: basic-create-mat

----------------------------------------
Operations between matrices and vectors
----------------------------------------

You can use the usual operations of linear algebra between matrices and vectors (*sum of vectors,
transpose of vectors, sum of matrices, left multiplication of a matrix by a scalar, etc.*).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-mat-vec
   :end-before: basic-mat-vec

---------------------------------
Midpoint, radius, magnitude, etc.
---------------------------------

These usual properties can be obtained for intervals. They are also all extended to interval vectors or matrices
componentwise. For instance, the radius of an interval matrix is the (real) matrix of the radii.

As a consequence, Ibex also has classes to handle real (versus interval) vectors and matrices.
Mathematical Operations (like the sum) can also be applied to these objects but, of course, using this times
floating-point arithmetic (not interval).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-mid-rad
   :end-before: basic-mid-rad

================== 
Functions
==================

------------------------------ 
Creating functions
------------------------------

The easiest way to create a function is with a string directly:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-func-create-1
   :end-before: basic-func-create-1

However, this has some limitations (see :ref:`mod-func-ex`).
Another (more flexible) way to create function is using C++ operator overloading.
The only difference is that you must have to first build *variables*:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-func-create-2
   :end-before: basic-func-create-2

------------------------------
Constants inside functions
------------------------------

You can insert interval constants in the expresion of a function, even in C++-style.
For instance, if you want to create the function :math:`x\mapsto\sin(2x)`, just write:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-func-create-cst1
   :end-before: basic-func-create-cst1


Assume now that the function to be created is :math:`x\mapsto\sin(\pi x)`. It is still possible to
use a ``double`` representing approximately :math:`\pi`; but to keep
numerical reliability, it is required in this case to use an interval constant enclosing
:math:`\pi`. Next function must be seen as a "thick" function that rigorously encloses :math:`\sin(\pi x)`:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-func-create-cst2
   :end-before: basic-func-create-cst2

Or with strings directly:


.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: basic-func-create-cst3
   :end-before: basic-func-create-cst3

--------------------------------
Functions with vector arguments
--------------------------------

Arguments of a function are not necessarily scalar variables. They can also be vectors
or matrices. In the following example, we build the distance function:
:math:`dist:(a,b)\mapsto\| a-b \|`
where a and b are 2-dimensional vectors.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-vec-arg-1
   :end-before: func-vec-arg-1

We can also create the same function with string directly (note that the syntax quite :ref:`differs <mod-minibex-cpp>`).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-vec-arg-2
   :end-before: func-vec-arg-2


*Note*: :ref:`Evaluation <tuto-func-eval>` of a thick function will necessarily result in an interval with non-null diameter, even if the argument is reduced to a point.

.. _tuto-func-compo:

------------------------------ 
Composing functions
------------------------------

You can compose functions to build new functions. We build here the function that maps a vector x
to its distance with a constant point (1,2). To this end, we first define a generic distance function 
dist(a,b) as above.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-compo
   :end-before: func-compo

The display is as folllows. Note that constant values like 0 are automatically replaced
by degenerated intervals (like [0,0])::

  f:(x)->(dist(x,(<0, 0> ; <0, 0>));dist(x,(<1, 1> ; <1, 1>)))

.. _tuto-func-vec-value:

------------------------------ 
Vector-valued functions
------------------------------

Let us start with a basic example: the function :math:`x\mapsto(x-1,x+1)`.

With strings:


.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-vec-value-1
   :end-before: func-vec-value-1

With operator overloading:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-vec-value-2
   :end-before: func-vec-value-2

*Note:* The ``Return`` keyword is only necessary when the output of a function is a vector (or a matrix).

Now, in line with the previous sections, let us define a more complicated example:
the function that associates to a vector x its distance with two fixed points ``pt1`` and ``pt2``
initialized in our program to (0,0) and (1,1):

.. math::
   
   f:x\mapsto(\| x-(1,1)\|, \| x-(0,0)\|).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-vec-value-3
   :end-before: func-vec-value-3

The last construction is much more cumbersome with strings.

------------------------------ 
Matrix-valued functions
------------------------------

You can also create functions that return matrices.
Here is an example of a function from :math:`R` to :math:`R^{2\times 2}` where:

.. math::
   
   f: x \mapsto ( (2x, -x) ; (-x,3x) ).

With strings:
	
.. literalinclude:: ../examples/doc-tutorial.cpp 
   :start-after: func-mat-value-1
   :end-before: func-mat-value-1

With C++ operator overloading:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-mat-value-2
   :end-before: func-mat-value-2

The boolean value ``true`` given here to the two embedded ``Return``
means that, each time, the two components must be put in rows, and not in column as it is by default.
In contrast, the enclosing ``Return`` keeps the default behaviour since the two rows are
put in column in order to form a 2x2 matrix.


------------------------------ 
Using the Minibex syntax
------------------------------

To create sophisticated functions we advice you to use an intermediate "minibex" input file as follows instead of
embedding the function directly in your C++ program.
The previous example can be written in a plain text file::

  function f(x)
    return ((2*x,-x);(-x,3*x));
  end

Save this file under the name "myfunction.txt". Now, you can load this function in your C++ program::

  Function f("myfunction.txt");

--------------------------------------------
Minibex syntax with intermediate variables
--------------------------------------------

When several occurrences of the same subexpression occur in a function, it
is a good idea for readibility (and, actually, efficiency) to put
this subexpression into intermediate variables.

The following example is the function thar returns the rotation matrix
from the three Euler angles. In this function an experssion like cos(phi)
occurs several times.::

  /* Computes the rotation matrix from the Euler angles:
     roll(phi), the pitch (theta) and the yaw (psi)  */

  function euler(phi,theta,psi)
    cphi   = cos(phi);
    sphi   = sin(phi);
    ctheta = cos(theta);
    stheta = sin(theta);
    cpsi   = cos(psi);
    spsi   = sin(psi);
 
    return
     ( (ctheta*cpsi, -cphi*spsi+stheta*cpsi*sphi, spsi*sphi+stheta*cpsi*cphi) ;  
       (ctheta*spsi, cpsi*cphi+stheta*spsi*sphi, -cpsi*sphi+stheta*cphi*spsi) ; 
       (-stheta,     ctheta*sphi,                 ctheta*cphi) ); 
    end

---------------------------------------
Evaluation over floating-point numbers
---------------------------------------

Given input ``double`` values x, you can obtain a rigorous inclusion of f(x) either using
``eval``, ``eval_vector`` or ``eval_matrix``.
These functions return interval enclosures of the true result.

These functions are presented below in a more general setting where the inputs are intervals as well.

So, to get the image by f of fixed floating-point values, simply create degenerated intervals in the
next examples.

.. _tuto-func-eval:

------------------------------ 
Interval evaluation
------------------------------

The interval evaluation of f is the image of the given input interval vector [x] by f, this range being noted by f([x]):

.. math::
   f([x]) := \{ f(x), x\in[x] \}.

Let us start with a real-valued function f with scalar arguments:

.. literalinclude:: ../examples/doc-tutorial.cpp
   :language: cpp
   :start-after: func-eval
   :end-before: func-eval

The sine function is not monotonic on [4,6] and actually reaches its
minimum at :math:`3\pi/2`.

Note that the ``eval`` takes an ``IntervalVector``
as argument, even if there is only one variable. So, in the latter case, you have to build a vector
reduced to a single component.

We consider now a vector-valued function.
Since the return type of an evaluation is not anymore an ``Interval`` but
an ``IntervalVector``, we have to use a method with a different 
signature, namely, ``eval_vector``:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-eval-vec
   :end-before: func-eval-vec

Finally, for a matrix-valued function, the evaluation is obtained via ``eval_matrix``.
We assume again that the following matrix-valued function

.. math::
   f: x \mapsto ( (2x, -x) ; (-x,3x) )

has been written in a "minibex" input file (see above).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-eval-mat
   :end-before: func-eval-mat

------------------------------ 
Interval gradient
------------------------------

For a scalar-valued function, you can get an interval enclosure of the gradient:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-grad
   :end-before: func-grad

------------------------------
Interval Jacobian matrix
------------------------------

For a vector-valued function, you can get an interval enclosure of the Jacobian matrix:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-jac
   :end-before: func-jac

------------------------------ 
Backward (or contraction)
------------------------------

One of the main feature of Ibex is the ability to *contract* a box representing the domain of a variable
x with respect to the constraint that f(x) belongs to a restricted input range [y]. 
Rigorously, given two intervals [x] and [y], the contraction gives a new interval [z] such that

.. math:: 
   \forall x\in[x], \quad f(x)\in[y] \Longrightarrow x \in[z] \subseteq [x] 

One way to do this is by using the famous *backward* algorithm. 
This algorithm does not return a new interval [z] but contract the input interval [x] which is therefore 
an input-output argument.

In the following snippet we require the function sin(x+y) to take the value -1 (a degenerated interval).
With an initial box (x,y)=([1,2],[3,4]), we obtain the result that (x,y) must lie in the subdomain 
([1, 1.7123] ; [3, 3.7124]).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: func-bwd
   :end-before: func-bwd


.. _tuto-ctr:

================== 
Constraints
==================

To create a constraint, you can also either use strings or C++ objects:

With strings:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctr-1
   :end-before: ctr-1

With C++ objects:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctr-2
   :end-before: ctr-2

You can also refer to a previously defined function f to create, e.g., f(x)<=0:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctr-3
   :end-before: ctr-3

.. _tuto-ctc:

================== 
Contractors
==================

--------------------------------- 
What is a contractor programming?
---------------------------------

The key idea behind *contractor programming* :ref:`[Chabert & Jaulin, 2009] <Chabert09a>` is to abstract the algorithm
from the underlying constraint and to view it a function "C":

.. math::
   C: \mathbb{IR}^n \to \mathbb{IR}^n \ \mbox{such that} \ C([x])\subseteq[x],

where :math:`\mathbb{IR}` denotes the set of real intervals.

In other word, we take as primary concept the *operational* definition of a constraint.

In this way, operators (like the intersection and the others below) can be extended to contractors. 

Since contractors implicitly represent sets, the fundamental advantage of extending operations
to contractors is that we actually extend these operations to sets. 

All contractors in Ibex are algorithms represented by different classes. 
See the `strategy pattern`_ for more information on this design choice.
Classes representing contractors are prefixed by ``Ctc``.

.. _strategy pattern: http://en.wikipedia.org/wiki/Strategy_pattern


.. _tuto-fwd-bwd:

------------------------------ 
Forward-Backward
------------------------------

The standard way to contract with respect to a constraint is by using the *forward-bacwkard* algorithm.
The corresponding class is ``CtcFwdBwd``.

A constraint has to be built first using the ``NumConstraint`` class.
In the following piece of code, we build a forward-backward contractor with respect to x+y=z.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-fwd-bwd
   :end-before: ctc-fwd-bwd

Of course, the expression of a constraint can involve a previously defined function.
Furthermore, if the constraint is simply "f=0", where f is a ``Function`` object, it is not 
necessary in this case to build an intermediate ``NumConstraint`` object. 
One can directly give the function f that has to be nullify to ``CtcFwdBwd``. 
In the next example, we consider the problem of finding the point which distance from
both (0,0) and (1,1) is sqrt(2)/2. The solution is (0.5,0.5).

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-fwd-bwd2
   :end-before: ctc-fwd-bwd2

Of course, the result is rather crude. Remember that the purpose of ``CtcFwdBwd`` 
is to contract *quickly* with respect to *any* numerical constraint: it is widely applicable and takes
a time that is only proportional to the expression size. In the other hand, it is not accurate in general.

:ref:`See more <ctc-fwd-bwd>`

------------------------------ 
Fixpoint
------------------------------

The fixpoint operator applies a contractor C iteratively:

.. math::
   fixpoint(C): [x] \mapsto C(\ldots C([x])\ldots),

while the "gain" is more than the given ``ratio``. More precisely, the "gain"
is the relative Hausdorff distance between the input box [x] and the output box C([x]) but, often, you can ignore 
the precise meaning of this gain and just consider that the procedure will loop until the contracted box
will roughly differ "by ratio" from the input one.

Let us now follow the previous example. As said, the solution is (0.5,0.5). We can see that simply embedding
the ``CtcFwdBwd`` contractor in a fixpoint loop (with a ``ratio``
set to 0.1) gives a box with sharp bounds.

.. literalinclude:: ../examples/doc-tutorial.cpp    
   :language: cpp
   :start-after: ctc-fixpoint
   :end-before: ctc-fixpoint

.. _tuto-inter-union-compo:

--------------------------------- 
Intersection, union & composition
---------------------------------

Given two of more contractors, we can apply the two logical operators *union* and *intersection*:

.. math::
   union(C_1,\ldots,C_n): [x] \mapsto C_1([x]) \cup\ldots\cup C_n([x]).

   inter(C_1,\ldots,C_n): [x] \mapsto C_1([x]) \cap\ldots\cap C_n([x]).

However, the latter operation is barely used and usually replaced by the *composition*:

.. math::
   compo(C_1,\ldots,C_n): [x] \mapsto C_n(\ldots(C_1([x])\ldots).

Indeed, one can see that the composition amounts to the same logical operation (the intersection of each contractor's set), 
but in a more efficient way since we take advantage of the contraction performed by :math:`C_1,...,C_{i-1}` when contracting with :math:`C_i`.
In contrast, the intersection operator calls each contractor independently on the same initial box.

The corresponding classes are ``CtcUnion`` and ``CtcCompo``.

As a rule of thumb, use ``CtcUnion`` for the union of two contractors and ``CtcComp`` for the intersection.
Here is an example with the union:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-union
   :end-before: ctc-union

Here is an example with the intersection (composition):

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-inter
   :end-before: ctc-inter

------------------------------ 
Interval Newton
------------------------------

When a function is "square" (the dimension is the same as the codimension, i.e., :math:`f:\mathbb{R}^n\to\mathbb{R}^n`), you can contract a 
box with respect to the constraint f(x)=0 using the interval Newton iteration. 

You just have to build a ``CtcNewton`` object with the function and call ``contract``.

This operator can give extremly accurate bounds proving that the input box is already "sufficiently" small (that is, 
"inside the convergence basin" of Newton's iteration). In the following example, we give a box that encloses the
solution (1,0) with a radius of 10^-3. Newton's iteration contracts this box downto the maximal precision:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-newton
   :end-before: ctc-newton

.. _tuto-propag:

------------------------------ 
Propagation
------------------------------

The *propagation* operator calculates the fixpoint of (the composition of) n contractors by using a more 
sophisticated ("incremental") strategy than a simple loop.
So, semantically, the propagation operator can be defined as follows:

.. math::
  propagation(C_1,\ldots,C_n):=fixpoint(compo(C_1,\ldots,C_n)).

(see above for the definition of the fixpoint and composition operators).

The key idea behind this operator is to avoid calling contractors that will certainly leave the box intact.
Contractors that can potentially enforce a contraction are determined typically from the syntax of their 
underlying constraint. Consider for instance two contractors, C1 w.r.t. f(x,z)=0 and C2 w.r.t. g(x,y)=0.
Assume that the fixpoint for C1 is reached with the current box ([x],[y],[z]). If a call to C2 only
contracts the second interval (the one corresponding to y), it is then useless to call C1 again.

So, by using such principle, the propagation calculates the fixpoint by "awaking" contractors only when
necessary. Of course, the more sparse the constraint system, the more valuable the propagation, when
compared to a simple fixpoint.

The following example compares the number of contractors

:ref:`See more <ctc-propag>`

--------------------------------------------
Q-Intersection (robustness w.r.t. outliers)
--------------------------------------------

The Q-intersection is typically used in a context where we have a set of contractors that result
from measurements (each measurement enforces a constraint), some of which can be incorrect.

If we are sure that at least q measurements are correct (which amounts to say that the number of
outliers is bounded by N-q) then we can contract the box in a robust way, by calculating the union
of the boxes resulting from the contraction with all combinaisons of q contractors among N.

Mathematicaly, with :math:`(i_1 , . . . , i_q)` ranging over the set of all q distinct indices between 0 and N-1:

.. math::
   q-inter(C_1,\ldots,C_n,q):=union(\ldots,inter(Ci1,\ldots,Ciq),\ldots)

Here is a simple example inspired from parameter estimation.

We assume a point (x,y) has to be localized. We measure 4 distances "bD" from 6 (approximately known) points (bX,bY).
Each position bX, bY and each distance bD has an uncertainty [-0.1,0.1]. We also know there may be at most one outlier.

The solution point is: x=6.32193 y=5.49908

First of all, let us enter the coordinates of the points (bX,bY) and the distances. This data will simulate our measurements.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-qinter1
   :end-before: ctc-qinter1

define the measurement intervals (with uncertainty taken into account)

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-qinter2
   :end-before: ctc-qinter2

Now, we artificially introduce an outlier by shifting the interval for one measurement (here, x position n°5) by a
large value:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-qinter3
   :end-before: ctc-qinter3

Now, all our simulated data is set up. We just have to define the contractors.
We first declare the distance function and then 6 contractors corresponding to
the distance with each (bX,bY):

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-qinter4
   :end-before: ctc-qinter4

We can contract now a box with the q-intersection of these contractors:

.. literalinclude:: ../examples/doc-tutorial.cpp
   :language: cpp
   :start-after: ctc-qinter5
   :end-before: ctc-qinter5

The displayed result is ([3.9667, 7.2381] ; [4.5389, 8.1479]). Of course, we can do better by calculating a fixpoint of the q-intersection:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-qinter6
   :end-before: ctc-qinter6

The displayed result is ([5.9277, 6.8836] ; [5.0914, 5.7996]) which, indeed, better encloses the solution point x=6.32193 y=5.49908.

------------------------------ 
Build your own contractor
------------------------------

To create a contractor, you just have to 
- declare a class that extends ``Ctc`` 
- create inside a function ``contract`` that takes a reference to a box (``IntervalVector&``) and contracts it. The function returns ``void``.

In the following example, we create a contractor that simply divides by two the radius of each component.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-own1
   :end-before: ctc-own1

Then, if we create this contractor and applies it several time to the same box, we can observe the expected result:

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-own2
   :end-before: ctc-own2

This contractor can now be combined with the ones built-in. For instance, we can decide to calculate the fixpoint.  Then, the result is a small box enclosing (0.5,0.5,0.5):

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: ctc-own3
   :end-before: ctc-own3

================== 
Strategies
==================

------------------------------
The generic solver
------------------------------

The generic solver is a classical branch and prune algorithm that interleaves contraction and branching (bisection) until
boxes get sufficiently small. It takes three operators in input:

#. a **contractor** 
       
    Operator that contracts boxes by removing non-solution points. It should be emphasized that the solver only sees the
    contraction operator, not the underlying system (of equations or inequalities). This is one of the reason why it is
    said to be "generic". In particular, one can introduce contractors of other kind, that are not based on mathematical 
    expressions.

#. a **bisector**           

    Operator that splits a box. The box is bisected providing it is large enough, so this operator takes also a *precision* parameter. 
    If the box is too small, the solver will not continue the search and backtrack. See :ref:`strategy-bisectors` for more details. 

#. a **cell buffer**
  
    Operator that manages the list of pending boxes (a *cell* is a box with a little bit of extra information used by the search).
    See :ref:`strategy-cell-buffers` for more details.

Our next example creates a solver for the intersection of two circles of radius :math:`d`, 
one centered on :math:`(0,0)` and the other in :math:`(1,0)`. 

To this end we first create a vector-valued function:

.. math::
	(x,y) \mapsto \begin{pmatrix} x^2+y^2-d \\ (x-1)^2+y^2-d \end{pmatrix}

Then, we build two contractors; a `forward-backward`_ contractor and (because the system is square),
an `interval Newton`_ contractor.		


We chose as bisection operator the *round-robin* operator, that splits each component in turn.
The precision of the solver is set to 1e-7.

Finally, the cell buffer is a stack, which leads to a depth-first search.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: strat-basic-solver
   :end-before: strat-basic-solver

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
#. `Interval Newton`_ (only if it is a square system of equations)
#. A `fixpoint`_ of the :ref:`ctc-polytope-hull` of two linear relaxations combined:
    - the relaxation called X-Taylor;
    - the relaxation generated by affine arithmetic. See :ref:`ctc-linear-relax`.
   
The bisector is based on the :ref:`strategy-smear-function` with maximal relative impact.

So the following program exactly reproduces the default solver.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: strat-default-solver
   :end-before: strat-default-solver

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



