
.. _mod-minibex:

=====================
The Minibex Language
=====================



.. _mod-into-cpp-vs-minibex:

------------------------------
Introduction
------------------------------

There are three possible alternatives for modeling.

- First, you can write C++ code. Variables, functions, constraints
  and systems are C++ object that you declare yourself
  and build by calling the constructors of the corresponding classes

- Entering mathematic formulas programmatically is usually not very convenient.
  You may prefer to separate the model of the problem from the algorithms
  you use to solve it. In this way, you can run the same program with different
  variants of your model without recompiling it each time.
  IBEX provides such possibility. You can directly load a :ref:`function <mod-func>`, a :ref:`constraint <mod-sys-ctrs>` or a :ref:`system <mod-sys>` from a (plain text) input file, following the (very intuitive) Minibex syntax. 
 
- However, files I/O operations are not always welcome. The third possibility is a kind of compromise.
  You can initialize a ``Function`` or ``NumConstraint`` objects with a string (``char*``)
  that contains the Minibex code. The syntax is exactly the same (see examples in the tutorial).

In all cases, you will access and use the data in the same way.
For instance, you will calculate the interval derivative of a function
by the same code, would it be created in your C++ program or loaded
from a Minibex file.

Here are simple examples where the syntax talks for itself.

------------------------------
Examples
------------------------------

.. _mod-func-minibex:

^^^^^^^^^^^^^^^^^^^^^^^^
Function
^^^^^^^^^^^^^^^^^^^^^^^^

Copy-paste the text below in a file named, say, ``function.txt``::

  function f(x)
    return x+y;
  end

Then, in your C++ program, just write:


.. code-block:: cpp

   Function f("function.txt");

and the function you get is (x,y)->x+y.


^^^^^^^^^^^^^^^^^^^^^^^^
Constraint
^^^^^^^^^^^^^^^^^^^^^^^^

Copy-paste the text below in a file named, say, ``constraint.txt``::

  Variables
    x,y;

  Constraints
    x^2+y^2<=1;
  end

Then, in your C++ program, just write:


.. code-block:: cpp

   NumConstraint ctr("constraint.txt");


and the constraint you get is x^2+y^2<=1. Notice that the keyword ``constraints`` has an "s" at the end because the Minibex syntax
allows several constraints declaration. 
If you load a constraint from a Minibex file that contains several constraints, only the first one is considered.


^^^^^^^^^^^^^^^^^^^^^^^^
System
^^^^^^^^^^^^^^^^^^^^^^^^

Copy-paste the text below in a file named, say, ``system.txt``::

  Variables
    x in [-1,1];
    y in [-1,1];

  Minimize
    x+y;

  Constraints
    x^2+y^2<=1;
  end

Then, in your C++ program, just write:


.. code-block:: cpp

   System sys("system.txt");


and the system you get is:

   Minimize :math:`x+y,`

   :math:`x \in[-1,1], y\in[-1,1]`

   such that

     :math:`x^2+y^2\le1`  

     :math:`y\ge x^2`.


Next sections details the mini-language of these input files. 

.. _mod-minibex-struct:

------------------------------
Overall structure
------------------------------
First of all, the input file is a sequence of declaration blocks that must respect the following order:

-  constants
-  variables
-  auxiliary functions
-  goal function
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

There is a predefined interval constant ``pi`` representing :math:`\pi`.

.. _mod-minibex-constants:

------------------------------
Constants
------------------------------
Constants are all defined in the same declaration block, 
started with the ``Constants`` keyword. This block is always optionnal.

A constant value can depends on other (previously defined) constants value. Example::

  Constants
    e=0.5772156649;
    y=-1.0;
    z=sin(pi*y);

You can give a constant an interval enclosure rather than a single fixed value.
This interval will be embedded in all subsequent computations.
Following the previous example, we can give ``e`` a valid enclosure as below.
We just have to replace "=" by "in"::

  Constants
    e in [0.577215664,0.577215665];
    y=e+1;

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


.. _mod-minibex-vars:

------------------------------
Variables
------------------------------

Variables need to be declared in two situations:

- when you create a function. In this case, the variables are the arguments of the function and
  they are only visible inside the body of the function. Let us call them *local variables*.
  Here is an example::
  
    function f(x)  // x is a local variable
          ...
    end
  
- when you create a constraint or a system of constraints. In this case, the variables are declared
  globally in a specific block and shared by all the constraints. Let us call them *global variables*::
  
    variables
       x;          // x is a global variable
       ...
    
Note that global variables are not visible inside the (auxiliary) functions and conversely. So there is no possible confusion between the
global and the local variables.
  
Local and global variables can be vectors and matrices. Declaring vector and matrix variables follow exactly the
same rules as for vector and matrix :ref:`constants <mod-minibex-constants>`. Example::
 
  function f1(x[3])  // x is a vector of 3 components
    ...
  end

It is possible to define up to three dimensional vectors.

Global variables can also be given a domain to initialize each
component with. The following examples are valid::

   variables
   
     x[10][5][4];
     y[10][5][4] in [0,1];

Whenever domains are not specified, they are set by default to :math:`(-\infty,+\infty)`.

.. _mod-minibex-expr:

------------------------------
Expressions
------------------------------

The expressions are built by applying operators on constants and variables.

In the following, we assume that:

- e, e1, e2,... are expressions
- real-cst is a constant expression (not involving variables)
- int-cst is a constant integer expression
- func is the name of an auxiliary function (see below)

You can use parenthesis and any space characters inside the expression, including new line. 

Operators for real-valued expressions are:

=======================  =============================================
-e                       opposite
e1+e2                    sum
e1-e2                    subtraction
e1*e2                    multiplication
e1/e2                    division
e1^e2                    power
e^int-cst                power (note: faster than previous op.)
max(e1,e2,...)           max
min(e1,e2,...)           min
atan2(e1,e2)             atan2
sign(e)                  sign of e
abs(e)                   absolute value
exp(e)                   exponential
ln(e)                    neperian logarithm
sqrt(e)                  square root
cos(e)                   cosine
sin(e)                   sine
tan(e)                   tangent
acos(e)                  inverse cosine
asin(e)                  inverse sine
atan(e)                  inverse tangent  
cosh(e)                  hyperbolic cosine
sinh(e)                  hyperbolic sine
tanh(e)                  hyperbolic tangent
acosh(e)                 inverse hyperbolic cosine
asinh(e)                 inverse hyperbolic sine
atanh(e)                 inverse hyperbolic tangent
func(e1,e2,...)          apply the function "func" to the arguments
(e1,e2,...)              create a row vector of expressions
(e1;e2;...)              create a column vector of expressions
=======================  =============================================

Operators for vector/matrix-valued expressions are:

=======================  ==============================================================================
e'                       transposition (like in Matlab)
-e                       opposite
e1+e2                    sum
e1-e2                    subtraction
e1*e2                    matrix-vector multiplication or dot/Hadamard product
e(int-cst)               get the ith component of a vector or the ith row of a matrix
e(int-cst,int-cst)       get the (i,j)th entry of a matrix expression
(e1,e2,...)              create a matrix from column vectors
(e1;e2;...)              create a matrix from row vectors
=======================  ==============================================================================

So, indexing vector or matrix variables follow Matlab convention and, remember, indices start from 1.

Ex::

  Variables
    x[10][10] in [0,oo];
  Constraints
    x(1,1)=0;
  end

.. _mod-minibex-cpp:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Some differences with C++
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


- Vectors indices are surrounded by parenthesis (not brackets),
- Indices start by 1 instead of 0,
- You have to use the "^" symbol (instead of ``sqr`` or ``pow``).




.. _mod-minibex-func:

------------------------------
Functions
------------------------------

A function declared in a Minibex file may have two different usage.
 
- You need to handle this function in your C++ program.
  In this case, your Minibex file should only contain that function.
  The file can then be loaded with the appropriate :ref:`constructor <mod-func-minibex>` of the Function class.

- You have several constraints that involve the same expression repeatidly. Then, it may be
  convenient for you to put this expression once for all in a separate 
  function and to invoke this function inside the constraints expressions. We shall talk in
  this case about *auxiliary functions*.

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

