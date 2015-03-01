*****************************************************
              Functions & Systems
*****************************************************


===================
Introduction
===================

The purpose of this chapter is to show how to create and manipulate
objects corresponding to the mathematical concepts of
*variable*, *function*, *constraint* and *system*.

We talk about *modeling* as these objects mathematically model the concrete problem you are faced to.

.. _mod-into-cpp-vs-minibex:

------------------------------
C++ versus Minibex
------------------------------

There are three possible alternatives for modeling.
You can either:

-  write C++ code. Variables, functions, constraints
   and systems are C++ object that you declare yourself
   and build by calling the constructors of the corresponding classes
-  write all these basic mathematic data in a text file, following
   the (very intuitive) Minibex syntax. All these data are loaded
   simultaneously and stored in a single `System` object.
-  insert a ``char*`` directly in the code instead of using a file.
   The syntax is exactly the same (see examples in the tutorial).

In all cases, you will access and use the data in the same way.
For instance, you will calculate the interval derivative of a function
by the same code, would it be created in your C++ program or loaded
from a Minibex file.

The chapter is organized as follows: we present
each concept (variable, function, etc.) in turn and
each time explain how objects are created in C++.

All the Minibex syntax is given afterwards, in a separate :ref:`section <mod-minibex>`.
 
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

.. _mod-intro-arg-vs-var:

------------------------------
Arguments versus variables
------------------------------

A (mathematical) variable does not necessarily represent a single real value.
It can also be a vector, a matrix or an array-of-matrices.
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

.. _mod-func:

===================
Functions
===================

Mathematical functions are represented by objects of the class ``Function``.

These objects are very easy to build. You can either build them in :ref:`C++ <mod-func-cpp>`
or in :ref:`Minibex <mod-minibex>`.

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

.. _mod-func-arg-cpp:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Creating arguments (in C++)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Creating vector and matrix arguments (in C++)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Renaming arguments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

.. _mod-func-op:

------------------------------
Operations
------------------------------

Various operations can be performed with a function. We detail below the main ones.

.. _mod-func-op-fwd:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Evaluation (forward computation)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Take a look first at the :ref:`tutorial <tuto-func-eval>` for introductory examples.

.. _mod-func-op-bwd:

Since function overloading does not work for return types in C++, you have to either
call ``eval``, ``eval_vector`` or ``eval_matrix`` depending if your function
respectively returns a scalar, a vector or a matrix. See 

All ``eval_XXX`` functions expects a single box in argument that represents all the arguments (scalars, vectors, matrices) stored in a single flat array (see :ref:`mod-func-arg-dim`).

To build this vector, the best is to use *backward projection functions*. 

Here is an example with f(A,B,C)=A+B-C where A, B and C are matrices.

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-eval-C
   :end-before: func-eval-C


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Backward
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Gradient
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Jacobian and Hansen's matrix
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

------------------------------
Creating functions (in C++)
------------------------------

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


.. _mod-func-cpp-renaming:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Renaming functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, function names are also generated. But you can also set your own function name, as the last parameter of the constructor:

.. code-block:: cpp

   Function f(x,y,sin(x+y),"f");

.. _mod-func-cpp-symbols:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Allowed symbols
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following symbols are allowed in expressions:

sign, min, max,
sqr, sqrt, exp, log, pow, 
cos, sin, tan, acos, asin, atan,
cosh, sinh, tanh, acosh, asinh, atanh
atan2


Power symbols ``^`` are not allowed. You must
either use ``pow(x,y)``, or simply ``sqr(x)`` for the square function.

Here is an example of the distance function between ``(xa,ya)`` and ``(xb,yb)``:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp-symbols-C
   :end-before: func-cpp-symbols-C

.. _mod-func-cpp-vecargs:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Functions with vector arguments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If arguments are vectors, you can refer to the component
of an argument using square brackets. Indices start by 0,
following the convention of the C language.

We rewrite here the previous distance function using 2-dimensional
arguments ``a`` and ``b`` instead:

.. literalinclude:: ../examples/doc-modeling.cpp
   :language: cpp
   :start-after: func-cpp-vec-args-C
   :end-before: func-cpp-vec-args-C

.. _mod-func-cpp-compo:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Composition
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can compose functions. Each argument of the called function can be substitued
by an argument of the calling function, a subexpression or a constant value.

See the example in the :ref:`tutorial <tuto-func-compo>`.

.. _mod-func-symbolic-diff:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Symbolic differentiation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

.. _mod-func-cpp-vecvalued:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Vector-valued functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To define a vector-valued function, the ``Return`` keword allows
you to list the function's components.

See the example in the :ref:`tutorial <tuto-func-vec-value>`.

.. _mod-func-ex:

------------------------------
Advanced examples
------------------------------

.. _mod-func-dag:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Building DAGs (directed acyclic graphs)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can create a DAG instead of an expression tree. This will result in a gain in performance.
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


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Applying a function with numerous arguments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

.. _mod-sys:

===================
Systems
===================

A *system* in IBEX is a set of constraints (equalities or inequalities) with, optionnaly, a goal function to minimize
and an initial domain for variables.
It corresponds to the usual concept of system in mathematical programming.
Here is an example of system:

   Minimize :math:`x+y,`

   :math:`x \in[-1,1], y\in[-1,1]`

   such that

     :math:`x^2+y^2\le1`  

     :math:`y\ge x^2`.


One is usually interested in solving the system while minimizing the criterion, if any.

We first present how constraints are represented.

.. _mod-sys-ctrs:

------------------------------
Constraints
------------------------------

In this section, we do not present *constraints* in their full generality
but *numerical constraints* (the ones you are the most likely interested in).

A numerical constraint in IBEX is either a relation like 
:math:`f(x)<0`, :math:`f(x)\le0`, :math:`f(x)=0`, :math:`f(x)\ge0` or :math:`f(x)>0`,
where *f* is a function as introduced in the previous section. If *f* is vector-valued, then
0 must be a vector.

Surprisingly, constraints do not play an important role in IBEX.
It sounds a little bit contraditory for a *constraint* programming library.
The point is that IBEX is rather a *contractor* programming library meaning that 
we build, apply and compose contractors rather than constraints directly.

As a programer, you may actually face two different situations.

Either you indeed want to use a constraint as a contractor in which case you
build a ``Ctc`` object with this constraint (the actual class depending
on the algorithm you chose, as explained in :ref:`the tutorial <tuto-ctc>` --by default, it is :ref:`ctc-fwd-bwd`--).
Either you need to do something else, say, like calculating the Jacobian matrix of the
function *f*. In this case, you just need to get a reference to this function 
and call ``jacobian``. In fact, all the information inherent to a constraint
(except the comparison operator of course) is contained in the underlying function so that there
is little specific code related to the constraint itself.

For these reasons, the only operations you actually do with a constraint is either
to read its field or wrap it into a contractor.

.. _mod-sys-ctrs-fields:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Class and Fields
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The class for representing a numerical constraint is ``NumConstraint``.
The first field in this class is a reference to the function:

  Function& f;


The second field is the comparison operator:

  CmpOp op;


``CmpOp`` is just an ``enum`` (integer) with the following values:

=======   ============
Op        def         
=======   ============
``LT``    :math:`<`   
``LEQ``   :math:`\le`
``EQ``    :math:`=`
``GEQ``   :math:`\ge`
``GT``    :math:`>`   
=======   ============

.. _mod-sys-ctrs-cpp:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Creating constraints (in C++)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To create a numerical constraint, you can build the function *f* first and
call the constructor of ``NumConstraint`` as in the following example.


.. code-block:: cpp

   Variable x;
   Function f(x,x+1);
   NumConstraint c(f,LEQ); // the constraint x+1<=0
	

But you can also write directly:


.. code-block:: cpp

   Variable x;
   NumConstraint c(x,x+1<=0);
	
which gives the same result. The only difference is that, in the second case,
the object ``c.f`` is "owned" (and destroyed) by the constraint whereas 
in the first case, ``c.f`` is only a reference to ``f``.

Note that the constant 0 is automatically interpreted as a vector (resp. matrix),
if the left-hand side expression is a vector (resp. matrix). However, it does not
work for other constants: you have to build the constant with the proper dimension,
e.g.,


.. code-block:: cpp

   Variable x(2);
   NumConstraint c(x,x=IntervalVector(2,1)); // the constraint x=(1,1)
   cout << "c=" << c << endl;
	
The display is::

  c=(_x_0-([1,1] ; [1,1]))=0


In case of several variables, the constructor of ``NumConstraint`` works as for functions.
Up to 6 variables can be passed as arguments:


.. code-block:: cpp

   Variable a,b,c,d,e,f,g;
   NumConstraint c(a,b,c,d,e,f,g,a+b+c+d+e+f+g<=1);
	

And if more variables are necessary, you need to build an ``Array<const ExprSymbol>`` first, like :ref:`here <mod-func-cpp>`.

.. _mod-sys-fields:

**Note:** There is currently the important restriction that inequalities can only be formed with real-valued (called "scalar") functions.
It could be possible, in theory, to write :math:`f(x)\le0` with f vector-valued by interpreting the operator componentwise but this
is not supporter by Ibex.

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


.. _mod-sys-transfo:

------------------------------
Transformation
------------------------------

We present in this section the different transformations that can be applied to a system.

.. _mod-sys-transfo-copy:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Copy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Normalization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Extended System
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Fritz-John (Khun-Tucker) conditions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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


.. _mod-sys-auxfunc:

------------------------------
Auxiliary functions
------------------------------

*(to be completed)*

.. _mod-sys-cpp:

------------------------------
Creating systems (in C++)
------------------------------

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

.. _mod-minibex:

===================
The Minibex syntax
===================

Entering a system programmatically is usually not very convenient.
You may prefer to separate the model of the problem from the algorithms
you use to solve it. In this way, you can run the same program with different
variants of your model without recompiling it each time.

IBEX provides such possibility. You can directly load a system from a (plain text) input file.

You may also prefer the Minibex syntax by itself to C++ operator overloading and use this syntax inside
your C++ code by initializing ``Function`` or ``NumConstraint`` objects with ``char*``.

Here is a simple example. Copy-paste the text above in a file named, say, ``problem.txt``. 
The syntax talks for itself::

  Variables
    x,y;

  Minimize
    x+y;

  Constraints
    x^2+y^2<=1;
  end

Then, in your C++ program, just write:


.. code-block:: cpp

   System sys("problem.txt");


and the system you get is exactly the same as in the previous example.

Next sections details the mini-language of these input files. 

.. _mod-minibex-struct:

------------------------------
Overall structure
------------------------------
First of all, the input file is a sequence of declaration blocks that must respect the following order:

-  (optional) constants
-  variables
-  (optional) auxiliary functions
-  (optional) goal function
-  constraints


Next paragraph gives the basic format of numbers and intervals.
The subsequent paragraphs detail each declaration blocks.

.. _mod-minibex-reals:

------------------------------
Real and Intervals
------------------------------
A real is represented with the usual English format, that is
with a dot separating the integral from the decimal part,
and, possibly, using scientific notation.

Here are some valid examples of reals in the syntax:

  0

  3.14159

  -0.0001

  1.001e-10

  +70.0000

An interval are two reals separated by a comma
and surrounded by square brackets. The special symbol
``oo`` (two consecutive "o") represents the infinity :math:`\infty`.
Note that, even with infinity bounds, the brackets
must be squared (and not parenthesis as it should be since the
bound is open). Here are some examples:

  [0,1]

  [0,+oo]

  [-oo,oo]

  [1.01e-02,1.02e-02]

.. _mod-minibex-constants:

------------------------------
Constants
------------------------------
Constants are all defined in the same declaration block, 
started with the ``Constants`` keyword.
A constant value can depends on other (previously defined) constants value. Example::

  Constants
    pi=3.14159;
    y=-1.0;
    z=sin(pi*y);

You can give a constant an interval enclosure rather than a single fixed value.
This interval will be embedded in all subsequent computations.
Following the previous example, we can give ``pi`` a valid enclosure as below.
We just have to replace "=" by "in"::

  Constants
    pi in [3.14159,3.14160];
    y=-1.0;
    z=sin(pi*y);

Constants can also be vectors, matrices or array of matrices.
You need to specify the dimensions of the constant in square brackets.
For instance ``x`` below is a column vector with 2 components, the first component is equal
to 0 and the second to 1::

 Constants
   x[2] = (0; 1);

Writing ``x[2]`` is equivalent to ``x[2][1]`` because a column vector is also a 2x1 matrix.
A row vector is a 1x2 matrix so a row vector has to be declared as follows. 
On the right side, note that we use commas instead of periods::

  Constants
    x[1][2] = (0, 1);


**important** remark. 
The reason why the syntax for declaring row vectors differs here from Matlab is that a 2-sized row vector surrounded
by brackets would conflict with an interval. So, do note confuse
``[0,1]`` with ``(0,1)``:

-  ``(0,1)`` is a 2-dimensional row vector of two reals, namely 0 and 1.
   This is **not** an open interval.
-  ``[0,1]`` is the 1-dimensional interval [0,1]. This is **not** a 2-dimensional row vector.

Of course, you can mix vector with intervals. For instance:
``([-oo,0];[0,+oo])`` is a column vector of 2 intervals, :math:`(-\infty,0]` and :math:`[0,+\infty)`.

Here is an example of matrix constant declaration::

  Constants
    M[3][2] = ((0 , 0) ; (0 , 1) ; (1 , 0));

This will create the constant matrix ``M`` with 3 rows and 2 columns equal to

.. math::
   \left(\begin{array}{cc}
   0 & 0 \\ 
   0 & 1 \\ 
   1 & 0 \\
   \end{array}\right).

You can also declare array of matrices::

  Constants
   c[2][2][3]=(((0,1,2); (3,4,5)) ; ((6,7,8); (9,10,11)));

It is possible to define up to three dimensional vectors, but not more.

When all the components of a multi-dimensional constant share the same interval, you
don't need to duplicate it on the right side. Here is an example of a 10x;10 matrix where all
components are [0,0]::

  Constants
   c[10][10] in [0,0];

Ibex intializes the 100 entries of the matrix ``c`` to :math:`[0,0]`.

Finally, the following table summarizes the possibility for declaring constants
through different examples.

+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x in [-oo,0]``              | declares a constant :math:`x\in(-\infty,0]`                                                                                            |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x in [0,1]``                | declares an constant :math:`x\in[0,1]`                                                                                                 | 
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x in [0,0]``                | declares a constant :math:`x\in[0,0]`                                                                                                  |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x = 0``                     | declares a real constant x equal to 0                                                                                                  |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x = 100*sin(0.1)``          | declares a constant x equal to 100*sin(0.1)                                                                                            |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x[10] in [-oo,0]``          | declares a \10-sized constant vector x,                                                                                                |
|                              | with each component :math:`x_i\in(-\infty,0]`                                                                                          |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x[2] in ([-oo,0];[0,+oo])`` | declares a 2-sized constant vector x with                                                                                              |
|                              | :math:`x_1\in(-\infty,0]` and :math:`x_2\in[0,+\infty)`                                                                                |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
|``x[3][3] in``                | declares a constrant matrix :math:`x\in\left(\begin{array}{ccc}[0,1] & 0 & 0 \\0 & [0,1] & 0 \\0 &  0 & [0,1] \\\end{array}\right)`.   |
|  ``(([0,1],0,0);``           |                                                                                                                                        |
|  ``(0,[0,1],0);``            |                                                                                                                                        |
|  ``(0,0,[0,1]))``            |                                                                                                                                        |
|                              |                                                                                                                                        |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
| ``x[10][5] in [0,1]``        | declares a matrix x with each entry :math:`x_{ij}\in[0,1]`.                                                                            |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
| ``x[2][10][5] in [0,1]``     | declares an array of two 10x5 matrices with each entry :math:`x_{ijk}\in[0,1]`.                                                        |
+------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+

.. _mod-minibex-func:

------------------------------
Functions
------------------------------

When the constraints involve the same expression repeatidly, it may be
convenient for you to put this expression once for all in a separate auxiliary
function and to call this function.

Assume for instance that your constraints intensively use the following expression

.. math::
  \sqrt{(x_a-x_b)^2+(y_a-y_b)^2)}

where :math:`x_a,\ldots y_b` are various sub-expressions, like in::

  sqrt((xA-1.0)^2+(yA-1.0)^2<=0;
  sqrt((xA-(xB+xC))^2+(yA-(yB+yC))^2=0;
  ...

You can declare the distance function as follows::

  function distance(xa,ya,xb,yb)
   return sqrt((xa-xb)^2+(ya-yb)^2;
  end


You will then be able to simplify the writing of constraints::

  distance(xA,1.0,yA,1.0)<=0;
  distance(xA,xB+xC,yA,yB+yC)=0;
  ...

As you may expect, this will result in the creation of a :ref:`Function <mod-func>` object that
you can access from your C++ program via the ``System`` class. See :ref:`auxiliary functions  <mod-sys-auxfunc>`.

A function can return a single value, a vector
or a matrix. Similarly, it can take real, vectors or matrix arguments.
You can also write some minimal "code" inside the function before
returning the final expression.

This code is however limited to be a sequence of assignments.

Let us now illustrate all this with a more sophisticated example.
We write below the function that calculates the rotation matrix
from the three Euler angles, :math:`\phi, \theta` and :math:`\psi` :

.. math::
   R : (\phi,\psi,\theta) \mapsto
   \left(\begin{array}{ccc}
   \cos(\theta)\cos(\psi) & -\cos(\phi)\sin(\psi)+\sin(\theta)\cos(\psi)\sin(\phi) & \sin(\psi)\sin(\phi)+\sin(\theta)\cos(\psi)\cos(\phi)\\
   \cos(\theta)\sin(\psi) & \cos(\psi)\cos(\phi)+\sin(\theta)\sin(\psi)\sin(\phi) & -\cos(\psi)\sin(\phi)+\sin(\theta)\cos(\phi)\sin(\psi)\\
   -\sin(\theta) & \cos(\theta)\sin(\phi) & \cos(\theta)\cos(\phi)
   \end{array}\right)

As you can see, there are many occurrences of the same subexpression
like :math:`\cos(\theta)` so a good idea for both readibility and (actually) efficiency
is to precalculate such pattern and put the result into an intermediate variable.

Here is the way we propose to define this function::


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
    ( (ctheta*cpsi, -cphi*spsi+stheta*cpsi*sphi, 
                     spsi*sphi+stheta*cpsi*cphi) ; 
      (ctheta*spsi, cpsi*cphi+stheta*spsi*sphi, 
                   -cpsi*sphi+stheta*cphi*spsi) ;
      (-stheta, ctheta*sphi, ctheta*cphi) );
  end


**Remark.** Introducing temporary variables like ``cphi`` amouts to build a DAG instead of
a tree for the function expression. It is also possible (and easy) to :ref:`build a DAG  <mod-func-dag>` when you directly create
a ``Function`` object in C++.

.. _mod-minibex-vars:

------------------------------
Variables (or arguments)
------------------------------

Variables are defined exactly in the same fashion as :ref:`constants <mod-minibex-constants>`.
It is possible to define up to three dimensional vectors, with an optional domain to initialize each
component with. The following examples are valid:

``x[10][5][4];``

``x[10][5][4] in [0,1];``

Whenever domains are not specified, they are set by default to :math:`(-\infty,+\infty)`.

.. _mod-minibex-ctrs:

------------------------------
Constraints
------------------------------

Constraints are simply written in sequence.
The sequence starts with the keword ``constraints`` and terminates with the keyword ``end``. 
They are a separated by semi-colon. Here is an example::


  Variables
    x in [0,oo];
  Constraints
    //you can use C++ comments
    x+y>=-1;
    x-y<=2;
  end

.. _mod-minibex-ctrs-index:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Indexing vector or matrix variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Use parenthesis like in Matlab.
Ex::

  Variables
    x[10][10] in [0,oo];
  Constraints
    x(1,1)=0;
  end

.. _mod-minibex-ctrs-loop:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Loops
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can resort to loops in a Matlab-like syntax to define constraints. Example::


  Variables
    x[10];

  Constraints
    for i=1:10;
      x(i) <= i;
    end
  end


.. _mod-minibex-cpp:

------------------------------
Some differences with C++
------------------------------

- Vectors indices are surrounded by parenthesis (not brackets),
- Indices start by 1 instead of 0,
- You have to use the "^" symbol (instead of ``sqr`` or ``pow``).


