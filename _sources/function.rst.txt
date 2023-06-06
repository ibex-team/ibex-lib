.. _mod-func:

*****************************************************
              Functions
*****************************************************


===================
Introduction
===================

The purpose of this chapter is to show how to create and manipulate
objects corresponding to the mathematical concept of *function*.

.. _mod-intro-mean:

----------------------------------------------
What we mean by "variable" and "function"
----------------------------------------------

Let us rule out a potential ambiguity.

Since we are in the C++ programming language, the term *variable* and
*function* already refers to something precise. For instance, the following
piece of code introduces a *function* `sum` and a *variable* `x`:

.. code-block:: cpp

  int sum(int x, int y) { 
    return x+y;
  }

  int x=2;

The variable `x` may represent, say, the balance of a bank account.
The account number is what we call the *semantic* of `x`, that is, what `x` is supposed
to represent in the user's mind. So, on one side, we have *what we write*, that is, a program with variables and functions, and on the other side, *what we represent*, that is, concepts 
like a bank account.

With IBEX, we write programs to represent mathematical concepts
that are also called *variables* and *functions*.
The mapping :math:`(x,y)\mapsto\sin(x+y)` is an example of function that
we want to represent. It shall not be confused with the function `sum`
above.

To avoid ambiguity, we shall talk about *mathematical*
variables (resp. functions) versus *program* variables (resp. functions).
We will also use italic symbol like *x* to denote a mathematical variable
and postscript symbols like `x` for program variables.
In most of our discussions, variables and functions will refer
to the mathematical objects so that the mathematical meaning will be the implicit one. 

Mathematical functions are represented by objects of the class ``Function``.

.. _mod-intro-arg-vs-var:

------------------------------
Arguments versus variables
------------------------------

A (mathematical) variable does not necessarily represent a single real value.
It can also be a vector or a matrix.
One can, e.g., build the following function 

.. math::

   \begin{array}{cccc}
     f: & \mathbb{R}^2\times\mathbb{R}^3 & \to & \mathbb{R}\\
        & (x,y) & \mapsto & x_1\times y_1+x_2\times y_2 - x_3
   \end{array}.

In this case, *x* and *y* are vector variables with 2 and 3 components respectively.

We see, at this point, that the term *variable* becomes ambiguous.
For instance, if I say that the function *f* takes 2 variables, we don't really know if
it means that the function takes two arguments (that might be vectors or matrices) or if
the total input size is a vector of :math:`\mathbb{R}^2`.

For this reason, from now on, we will call **argument** the formal parameters
or input symbols the function has been defined with and **variable** a component of the
latters. 

Hence, the function *f* in the previous paragraphs has two arguments, *x* and *y*
and 5 variables :math:`x_1, x_2, y_1, y_2` and :math:`y_3`.

Note that, as a consequence, variables are always real-valued.



.. _mod-func-arg:

------------------------------
Arguments
------------------------------

Before telling you which class represents the arguments of a function, let us say first that
this class does not play a big role.
Indeed, the only purpose of declaring an argument *x* in IBEX is
for building a function right after, like :math:`x\mapsto x+1`.
Functions play, in contrast, a big role.

In other words, *x* is nothing but a syntaxic leaf in the expression 
of the function. In particular, an argument is not a slot for
representing domain.
E.g, if you want to calculate the range of *f* for :math:`x\in[0,1]`,
you just call a (program) function ``eval`` with a plain box in argument.
It's just as if *f* was the function that takes one argument and
increment it, whatever the name of this argument is.

Once *f* has been built, we can almost say that *x* is no longer useful.
Arguments must be seen only as temporary objects, in the process of function construction.

Before going on, let us slightly moderate this point.
We have assumed here that, as a user of IBEX the operations you are interested in are: *evaluate* *f* on a box, 
calculate *f'* on a box, solve *f(x)=0* and so on. All these operations can be qualified as numerical: they take
intervals and return intervals. You don't need to deal again with the expression of the function, once built.
But if you need to handle, for any reason, the symbolic form of the function then you have to inspect the syntax
and arguments appear again.

.. _mod-func-arg-dim:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Dimensions and ordering
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We have said in the previous paragraph that an argument *x* can actually represent
n variables :math:`x_1,\ldots,x_n`. So each argument has some associated information about
its dimension(s). 

Let us consider again this function: 

.. math::
   \begin{array}{cccc}
   f: & \mathbb{R}^2\times\mathbb{R}^3 & \to & \mathbb{R}\\
      &  (x,y) & \mapsto & x_1\times y_1+x_2\times y_2 - x_3
   \end{array}.

From the user standpoint, the function *f* (once built) is "flattened" (or "serialized") to a mapping from :math:`\mathbb{R}^5` to :math:`\mathbb{R}`.
Each C++ function (eval, etc.) expects a 5-dimensional box as parameter.

The way intervals are mapped to the variables components follows a straightforward ordering:
everytime we call a (program) function of *f* with the box :math:`[b]=([b]_1,\ldots,[b]_5)` in argument, we simply enforce

.. math::
   x\in[b]_1\times[b]_2 \quad \mbox{and} \quad y\in[b]_3\times[b]_4\times[b]_5.

If you don't want to create functions in C++, you can move now to :ref:`function operations  <mod-func-op>`.

.. _mod-func-arg-fiels:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Class name and fields
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As we have just said, arguments are just symbols in expression. For this reason,
they are represented by a class named ``ExprSymbol``.
In fact, there is also another class we introduced for convenience, called ``Variable``.
It is, of course, a very confusing name from the programer's viewpoint since a ``Variable``
does actually not represent :ref:`a variable but an argument <mod-intro-arg-vs-var>`. However, from the user's viewpoint,
this distinction is not visible and "variable" is more meaningful than "argument".
Anyway, the programer never has to deal with a "Variable" object. Without going further into details, the ``Variable``
class must be seen as a kind of "macro" that generates ``ExprSymbol`` objects. 
This macro is only useful if you :ref:`build arguments in C++ <mod-func-arg-cpp>`.

Once built, an argument is always typed ``ExprSymbol``.

If ``x`` is an ``ExprSymbol`` object, you can obtain the information about its dimensions via ``x``.dim.
The ``dim`` field is of type ``Dim``, a class that simply contains 3 integers (one for each dimension, see
the API for further details).

Finally, an argument also has a name, that is only useful for displaying. It is a 
regular C string (``char*``) stored in the field ``name``.



.. _mod-func-op:

======================================
Interval Computations
======================================

Various interval computations can be performed with a function. We detail below the main ones.

.. _mod-func-op-fwd:

--------------------------------------
Evaluation (forward computation)
--------------------------------------

Take a look first at the :ref:`tutorial <tuto-func-eval>` for introductory examples.

.. _mod-func-op-bwd:

Since function overloading does not work for return types in C++, you have to either
call ``eval``, ``eval_vector`` or ``eval_matrix`` depending if your function
respectively returns a scalar, a vector or a matrix.

All ``eval_XXX`` functions expects a single box in argument that represents all the arguments (scalars, vectors, matrices) stored in a single flat array (see :ref:`mod-func-arg-dim`).

To build this vector, the best is to use *backward projection functions*. 

Here is an example with f(A,B,C)=A+B-C where A, B and C are matrices.

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-eval-C
   :end-before: func-eval-C


--------------------------------------
Backward
--------------------------------------
	
One of the main feature of Ibex is the ability to *contract* a box representing the domain of a variable
x with respect to the constraint that f(x) belongs to a restricted input range [y]. 
The range [y] can be any constant (real value, interval, inteval vector, etc.).
Rigorously, given two intervals [x] and [y], the contraction gives a new interval [z] such that

.. math::
   \forall x\in[x], \quad f(x)\in[y] \Longrightarrow x \in[z] \subseteq [x].

One way to do this is by using the famous *forward-backward* (alias ``HC4Revise``). 
It is quick since it runs in linear time w.r.t. the size
of the constraint syntax and optimal when arguments have all one occurrence
in this syntax.
This algorithm does not return a new interval [z] but contract the input interval [x] which is therefore 
an input-output argument.

In the following snippet we require the function sin(x+y) to take the value -1 (a degenerated interval).
With an initial box (x,y)=[1,2],[3,4], we obtain the result that (x,y) must lie in the subdomain 
([1, 1.7123] ; [3, 3.7124]). 

.. literalinclude:: ../examples/doc-tutorial.cpp
   :language: cpp
   :start-after: func-bwd
   :end-before: func-bwd

One can indeed check that the resulting box is a consistent narrowing
of the initial one.

.. _mod-func-op-grad:

--------------------------------------
Gradient
--------------------------------------

Consider :math:`f:(x,y)\mapsto x\times y`.
The first and most simple way of calculating the gradient is:

.. code-block:: cpp

   double init_xy[][2] = { {1,2}, {3,4} };
   IntervalVector box(2,init_xy);
   cout << "gradient=" << f.gradient(box) << endl;

Since :math:`\frac{\partial{f}}{\partial{x}}=y` and :math:`\frac{\partial{f}}{\partial{y}}=x` we get:

.. code-block:: cpp

   gradient=([3,4] ; [1,2])

In this first variant, the returned vector is a new object created each time the function is called.
When we have to compute many times different values of the gradient for the same function, we can
also build a vector once for all and ask the ``gradient`` to store the result in this slot:

.. code-block:: cpp

  IntervalVector g(4);
  f.gradient(box,g);
  cout << "gradient=" << g << endl;

.. _mod-func-op-jac:

--------------------------------------
Jacobian and Hansen's matrix
--------------------------------------

The interval Jacobian matrix of a function *f* on a box *[x]* is

.. math::
   J=\left(\begin{array}{ccc}
   \frac{\partial{f_1}}{\partial{x_1}}([x]) & \ldots & \frac{\partial{f_1}}{\partial{x_n}}([x])\\
   \vdots \\
   \frac{\partial{f_m}}{\partial{x_1}}([x]) & \ldots & \frac{\partial{f_m}}{\partial{x_n}}([x])\\
   \end{array}\right)

The interval Jacobian matrix is obtained exactly as for the gradient.
Just write:

.. code-block:: cpp

   f.jacobian(box)

to get an ``IntervalMatrix`` containing an enclosure of the Jacobian matrix of *f* on the box in argument.

There is also a variant where the matrix is passed as parameter
(as for the gradient) in order to avoid allocating memory for the calculated matrix:
 
.. code-block:: cpp

   f.jacobian(box,J)


You can also compute with IBEX the "Hansen matrix". This matrix
is another *slope* matrix, thiner than the interval Jacobian (but slower to be calculated).
It is, for example, used inside the interval Newton operator.
The Hansen matrix corresponds to the following matrix, where :math:`(x_1,\ldots,x_n)` denotes
the midvector of :math:`[x]`.

.. math::
   \left(\begin{array}{cccc}
   \frac{\partial{f_1}}{\partial{x_1}}([x]_1,x_2,\ldots,x_n) & 
   \frac{\partial{f_1}}{\partial{x_2}}([x]_1,[x]_2,\ldots,x_n) & 
   \ldots & 
   \frac{\partial{f_1}}{\partial{x_n}}([x]_1,[x]_2,\ldots,[x]_n) \\
   \vdots & \\
   \frac{\partial{f_m}}{\partial{x_1}}([x]_1,x_2,\ldots,x_n) & 
   \frac{\partial{f_n}}{\partial{x_2}}([x]_1,[x]_2,\ldots,x_n) & 
   \ldots & 
   \frac{\partial{f_m}}{\partial{x_n}}([x]_1,[x]_2,\ldots,[x]_n) \\
   \end{array}\right)

Here is an example:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-hansen-C
   :end-before: func-hansen-C

The display is:

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: func-hansen-O
   :end-before: func-hansen-O

.. _mod-func-cpp:

===========================================
Creating functions (in C++)
===========================================

``Function`` objects are very easy to build. 

This section explains how to build them  
using C++ operator overloading but using
the :ref:`Minibex <mod-minibex>` syntax is even simpler.

.. _mod-func-arg-cpp:

---------------------------------
Creating arguments (in C++)
---------------------------------

The following piece of code creates an argument ``x`` and prints it:

.. code-block:: cpp

   Variable x;
   cout << x << endl;

The first instruction creates a (program) variable ``x``. It is initialized by default, since
nothing is given here to the constructor.
By default, the argument is real (or *scalar)*, meaning it is not a vector nor a matrix. 
Furthermore, the argument has a name that is automatically
generated. Of course, the name of the argument does not necessarily correspond to the name of the 
program variable.
For instance, ``x`` is the name of a C++ variable but the corresponding argument is named *_x_0*.
The second instruction prints the name of the argument on the standard output::

  _x_0

It is possible to rename arguments, see below.

.. _mod-func-arg-vec-cpp:

------------------------------------------------------------------
Creating vector and matrix arguments (in C++)
------------------------------------------------------------------


To create a n-dimensional vector argument, just
give the number n as a parameter to the constructor:

.. code-block:: cpp

  Variable y(3);   // creates a 3-dimensional vector

To create a mxn matrix, give m (number of rows) and n (number of columns) as parameters:

.. code-block:: cpp
 
   Variable z(2,3);   // creates a 2*3-dimensional matrix

We can go like this up to 3 dimensional arrays:

.. code-block:: cpp
  
   Variable t(2,3,4);   // creates a 2*3*4-dimensional array

.. _mod-func-arg-rename:

---------------------------------
Renaming arguments
---------------------------------


Usually, you don't really care about the names of arguments since you handle
program variables in your code.
However, if you want a more user-friendly display, you can specify
the name of the argument as a last parameter to the constructor.

In the following example, we create a scalar, a vector and a matrix argument each
time with a chosen name.:

.. code-block:: cpp

   Variable x("x");     // creates a real argument named "x"
   Variable y(3,"y");   // creates a vector argument named "y"
   Variable z(2,3,"z"); // creates a matrix argument named "z"
   cout << x << " " << y << " " << z << endl;

Now, the display is::

  x y z
  
---------------------------------
Examples
---------------------------------


The following piece of code creates the function :math:`(x,y)\mapsto \sin(x+y)`:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp1-C
   :end-before: func-cpp1-C

The display is:

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: func-cpp1-O
   :end-before: func-cpp1-O

You can directly give up to 20 variables in argument of the ``Function`` constructor:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp2-C
   :end-before: func-cpp2-C

If more than 20 variables are needed, you need to build an intermediate array for collecting the arguments.
More precisely, this intermediate object is an ``Array<const ExprSymbol>``. The usage is summarized below. In this
example, we have 7 variables. But instead of creating the function

.. math::
   x\mapsto x_1+\ldots+x_7

with one argument (a vector with 7 components), we decide to create the function

.. math::
   (x_1,\ldots,x_7)\mapsto x_1+\ldots+x_7.

with 7 arguments (7 scalar variables):

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp3-C
   :end-before: func-cpp3-C

The display is:

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: func-cpp3-O
   :end-before: func-cpp3-O

**Note:** Because of a potential conflict with ``std::min`` (or ``std::max``), you might be forced to prefix the min (max) function with ``ibex::``:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp4-C
   :end-before: func-cpp4-C

.. _mod-func-cpp-vecargs:

---------------------------------
Functions with vector arguments
---------------------------------


If arguments are vectors, you can refer to the component
of an argument using square brackets. Indices start by 0,
following the convention of the C language.

We rewrite here the previous distance function using 2-dimensional
arguments ``a`` and ``b`` instead:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp-vec-args-C
   :end-before: func-cpp-vec-args-C

.. _mod-func-cpp-vecvalued:

---------------------------------
Vector-valued functions
---------------------------------


To define a vector-valued function, the ``Return`` keword allows
you to list the function's components.

See the example in the :ref:`tutorial <tuto-func-vec-value>`.

.. _mod-func-ex:

--------------------------------------
Advanced examples
--------------------------------------

.. _mod-func-dag:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Building DAGs (directed acyclic graphs)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

C++ operator overloading allows you to create a DAG instead of an expression tree. This will result in a gain in performance.
For that, you need to handle references of shared subexpressions with variables types ``const ExprNode&``.

In the following example we create the function :

.. math::
   f:x\mapsto ((\cos(x)+1)^2, (\cos(x)+1)^3)

and we want the subexpression cos(x)+1 to be shared:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-dag-C
   :end-before: func-dag-C

.. _mod-func-ex-iterated-sum:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Iterated sum
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Let us build a function that returns the sum of the square of ``N`` variables, where ``N`` is some constant.

The only difficulty is that we cannot assign references in C++, so we need to use pointers to ``(const ExprNode&``) instead:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-iterated-sum-C
   :end-before: func-iterated-sum-C

The display is:

.. literalinclude:: ../examples/doc-modeling.txt
   :language: cpp
   :start-after: func-iterated-sum-O
   :end-before: func-iterated-sum-O


.. _mod-func-cpp-renaming:

--------------------------------------
Renaming functions
--------------------------------------


By default, function names are also generated. But you can also set your own function name, as the last parameter of the constructor:

.. code-block:: cpp

   Function f(x,y,sin(x+y),"f");

.. _mod-func-cpp-symbols:

--------------------------------------
Allowed symbols
--------------------------------------

The following symbols are allowed in expressions:

**sign, min, max,
sqr, sqrt, exp, log, pow, 
cos, sin, tan, acos, asin, atan,
cosh, sinh, tanh, acosh, asinh, atanh
atan2.**


Power symbols ``^`` are not allowed. You must
either use ``pow(x,y)``, or simply ``sqr(x)`` for the square function.

Here is an example of the distance function between ``(xa,ya)`` and ``(xb,yb)``:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp-symbols-C
   :end-before: func-cpp-symbols-C
   
--------------------------------------
Serialization
--------------------------------------

(to do)


======================================
Operations on Functions
======================================

.. _mod-func-cpp-compo:

--------------------------------------
Composition
--------------------------------------


You can compose functions. Each argument of the called function can be substitued
by an argument of the calling function, a subexpression or a constant value.

See the example in the :ref:`tutorial <tuto-func-compo>`.

.. _mod-func-symbolic-diff:


----------------------------------------------------------------------------
Applying a function with numerous arguments
----------------------------------------------------------------------------


We have explained how to create a function with  :ref:`an arbitrary number of arguments <mod-func-cpp>`. 
We explain now how to call (perform composition) with such function.

It is as simple as storing all the actual arguments in an array structure, namely, a structure
of expression nodes (typed ``Array<const ExprNode>``).

However, when an actual argument is not a formal expression but a numerical constant (data), it is necessary to 
explicitly encapsulate this constant in a expression node. This is what the ``ExprConstant`` class stands for.

Here is an example. We create the function :math:`f:(x,y)\mapsto x+y` and apply it to the hybrid couple (z,1) where
z is another variable. We do it in the generic way, using arrays:


.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-apply-array-C
   :end-before: func-apply-array-C

The display is:

.. literalinclude:: ../examples/doc-modeling.txt
   :language: cpp
   :start-after: func-apply-array-O
   :end-before: func-apply-array-O

You can also use this construct with vector/matrix variables and mix functions declared with different style:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-apply-array-2-C
   :end-before: func-apply-array-2-C
   
--------------------------------------
Symbolic differentiation
--------------------------------------

Differentiation of a function is another function.
So symbolic differentiation is obtained via a copy constructor where the copy "mode" is set to the special value ``Function::DIFF``:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-diff-C
   :end-before: func-diff-C

The output is

.. literalinclude:: ../examples/doc-modeling.txt
   :start-after: func-diff-O
   :end-before: func-diff-O



