/** \page modeling Modeling
 
\tableofcontents

\section mod-intro Introduction

The purpose of this chapter is to show how to create and manipulate
objects corresponding to the mathematical concepts of
\e variable, \e function, \e constraint and \e system.

We talk about \e modeling as these objects mathematically model the concrete problem you are faced to.

\subsection mod-into-cpp-vs-minibex C++ versus Minibex

There are two possible alternatives for modeling.
You can either:
<ul>
<li> write C++ code. Variables, functions, constraints
and systems are C++ object that you declare yourself
and build by calling the constructors of the corresponding classes
<li> write all these basic mathematic data in a text file, following
the (very intuitive) Minibex syntax. All these data are loaded
simultaneously and stored in a single \c System object.
</ul>

In both cases, you will access and use the data in the same way.
For instance, you will calculate the interval derivative of a function
by the same code, would it be created in your C++ program or loaded
from a Minibex file.

The chapter is organized as follows: we present
each concept (variable, function, etc.) in turn and
each time explain how objects are created in C++.

All the Minibex syntax is given afterwards, in a separate \link mod-minibex section \endlink.
 
\subsection mod-intro-mean What we mean by ``variable'' and ``function''

Let us rule out a potential ambiguity.

Since we are in the C++ programming language, the term \e variable and
\e function already refers to something precise. For instance, the following
piece of code introduces a \e function \c sum and a \e variable \c x:
\code
int sum(int x, int y) { 
  return x+y;
}

int x=2;
\endcode
The variable \c x may represent, say, the balance of a bank account.
The account number is what we call the \e semantic of \c x, that is, what \c x is supposed
to represent in the user's mind. So, on one side,
we have <em>what we write</em>, that is, a program with variables and functions,
 and on the other side, <em>what we represent</em>, that is, concepts 
like a bank account.

With IBEX, we write programs to represent mathematical concepts
that are also called \e variables and \e functions.
The mapping <i>(x,y)&rarr;sin(x+y)</i> is an example of function that
we want to represent. It shall not be confused with the function \c sum
above.

To avoid ambiguity, we shall talk about \e mathematical
variables (resp. functions) versus \e program variables (resp. functions).
We will also use italic symbol like \e x to denote a mathematical variable
and postscript symbols like \c x for program variables.
In most of our discussions, variables and functions will refer
to the mathematical objects so that the mathematical meaning will be the implicit one. 

\subsection mod-intro-arg-vs-var Arguments versus variables

A (mathematical) variable does not necessarily represent a single real value.
It can also be a vector, a matrix or an array-of-matrices.
One can, e.g., build the following function 
\f[\begin{array}{cccc}
f: & \mathbb{R}^2\times\mathbb{R}^3 & \to & \mathbb{R}\\
   &  (x,y) & \mapsto & x_1\times y_1+x_2\times y_2 - x_3
\end{array}.\f]

In this case, \e x and \e y are vector variables with 2 and 3 components respectively.

We see, at this point, that the term \e variable becomes ambiguous.
For instance, if I say that the function \e f takes 2 variables, we don't really know if
it means that the function takes two arguments (that might be vectors or matrices) or if
the total input size is a vector of \f$\mathbb{R}^2\f$.

For this reason, from now on, we will call \b argument the formal parameters
or input symbols the function has been defined with and \b variable a component of the
latters. 

Hence, the function \e f in the previous paragraphs has two arguments, \e x and \e y
and 5 variables \f$x_1\f$, \f$x_2\f$, \f$y_1\f$, \f$y_2\f$ and \f$y_3\f$.

Note that, as a consequence, variables are always real-valued.

\section mod-func Functions

Mathematical functions are represented by objects of the class \c Function.

These objects are very easy to build. You can either build them in \link mod-func-cpp C++\endlink
or in \link mod-minibex Minibex \endlink.

\subsection mod-func-arg Arguments

Before telling you which class represents the arguments of a function, let us say first that
this class does not play a big role.
Indeed, the only purpose of declaring an argument \e x in IBEX is
for building a function right after, like <i>f:x&rarr;x+1</i>.
Functions play, in contrast, a big role.

In other words, \e x is nothing but a syntaxic leaf in the expression 
of the function. In particular, an argument is not a slot for
representing domain.
E.g, if you want to calculate the range of \e f for \e x &isin; [0,1],
you just call a (program) function \c eval with a plain box in argument.
It's just as if \e f was the function that takes one argument and
increment it, whatever the name of this argument is.

Once \e f has been built, we can almost say that \e x is no longer useful.
Arguments must be seen only as temporary objects, in the process of function construction.

Before going on, let us slightly moderate this point.
We have assumed here that, as a user of IBEX the operations you are interested in are: \e evaluate \e f on a box, 
calculate \e f' on a box, solve \e f(x)=0 and so on. All these operations can be qualified as numerical: they take
intervals and return intervals. You don't need to deal again with the expression of the function, once built.
But if you need to handle, for any reason, the symbolic form of the function then you have to inspect the syntax
and arguments appear again.

\subsubsection mod-func-arg-dim Dimensions and ordering

We have said in the previous paragraph that an argument \e x can actually represent
n variables \f$x_1,\ldots,x_n\f$. So each argument has some associated information about
its dimension(s). 

Let us consider again this function: 
\f[\begin{array}{cccc}
f: & \mathbb{R}^2\times\mathbb{R}^3 & \to & \mathbb{R}\\
   &  (x,y) & \mapsto & x_1\times y_1+x_2\times y_2 - x_3
\end{array}.\f]

From the user standpoint, the function \e f (once built) is "flattened" (or "serialized") to a mapping from \f$\mathbb{R}^5\f$ to \f$\mathbb{R}\f$.
Each C++ function (eval, etc.) expects a 5-dimensional box as parameter.

The way intervals are mapped to the variables components follows a straightforward ordering:
everytime we call a (program) function of \e f with the box \f$[b]=([b]_1,\ldots,[b]_5)\f$ in argument, we simply enforce
\f[x\in[b]_1\times[b]_2 \quad \mbox{and} \quad y\in[b]_3\times[b]_4\times[b]_5.\f]

If you don't want to create functions in C++, you can move now to \link mod-func-op function operations \endlink.

\subsubsection mod-func-arg-fiels Class name and fields

As we have just said, arguments are just symbols in expression. For this reason,
they are represented by a class named \c ExprSymbol.
In fact, there is also another class we introduced for convenience, called \c Variable.
It is, of course, a very confusing name from the programer's viewpoint since a \c Variable
does actually not represent \link mod-intro-arg-vs-var a \e variable but an \e argument \endlink. However, from the user's viewpoint,
this distinction is not visible and "variable" is more meaningful than "arguments".
Anyway, the programer never has to deal with a \c Variable object. Without going further into details, the \c Variable
class must be seen as a kind of "macro" that generates \c ExprSymbol objects. 
This macro is only useful if you \link mod-func-arg-cpp build arguments in C++ \endlink.

Once built, an argument is always typed \c ExprSymbol.

If \c x is an \c ExprSymbol object, you can obtain the information about its dimensions via \c x.dim.
The \c dim field is of type \c Dim, a class that simply contains 3 integers (one for each dimension, see
the API for further details).

Finally, an argument also has a name, that is only useful for displaying. It is a 
regular C string (\c char*) stored in the field \c name.

\subsubsection mod-func-arg-cpp Creating arguments (in C++)

The following piece of code creates an argument \c x and prints it.

\code
  Variable x;
  cout << x << endl;
\endcode

The first instruction creates a (program) variable \c x. It is initialized by default, since
nothing is given here to the constructor.
By default, the argument is real (or \e scalar), meaning it is not a vector nor a matrix. 
Furthermore, the argument has a name that is automatically
generated. Of course, the name of the argument does not necessarily correspond to the name of the 
program variable.
For instance, \c x is the name of a C++ variable but the corresponding argument is named \e _x_0.
The second instruction prints the name of the argument on the standard output:

\code
_x_0
\endcode

It is possible to rename arguments, see below.

\subsubsection mod-func-arg-vec-cpp Creating vector and matrix arguments (in C++)

To create a n-dimensional vector argument, just
give the number n as a parameter to the constructor:

\code
  Variable y(3);   // creates a 3-dimensional vector
\endcode

To create a m&times;n matrix, give m (number of rows) and n (number of columns) as parameters:

\code
  Variable z(2,3);   // creates a 2*3-dimensional matrix
\endcode

We can go like this up to 3 dimensional arrays:

\code
  Variable t(2,3,4);   // creates a 2*3*4-dimensional array
\endcode

\subsubsection mod-func-arg-rename Renaming arguments

Usually, you don't really care about the names of arguments since you handle
program variables in your code.
However, if you want a more user-friendly display, you can specify
the name of the argument as a last parameter to the constructor.

In the following example, we create a scalar, a vector and a matrix argument each
time with a chosen name.

\code
  Variable x("x");     // creates a real argument named "x"
  Variable y(3,"y");   // creates a vector argument named "y"
  Variable z(2,3,"z"); // creates a matrix argument named "z"
  cout << x << " " << y << " " << z << endl;
\endcode

Now, the display is:
\code
x y z
\endcode


\subsection mod-func-op Operations

Various operations can be performed with a function. We detail below the main ones.

\subsubsection mod-func-op-fwd Evaluation (forward computation)

See the <a href="tutorial.html#func-eval">tutorial</a>.

\subsubsection mod-func-op-bwd Backward
	
One of the main feature of Ibex is the ability to <i>contract</i> a box representing the domain of a variable
x with respect to the constraint that f(x) belongs to a restricted input range [y]. 
The range [y] can be any constant (real value, interval, inteval vector, etc.).
Rigorously, given two intervals [x] and [y], the contraction gives a new interval [z] such that
\f[ \forall x\in[x], \quad f(x)\in[y] \Longrightarrow x \in[z] \subseteq [x] \f]

One way to do this is by using the famous <em>forward-backward</em> (alias \c HC4Revise). 
It is quick since it runs in linear time w.r.t. the size
of the constraint syntax and optimal when arguments have all one occurrence
in this syntax.
This algorithm does not return a new interval [z] but contract the input interval [x] which is therefore 
an input-output argument.

In the following snippet we require the function sin(x+y) to take the value -1 (a degenerated interval).
With an initial box (x,y)=[1,2],[3,4], we obtain the result that (x,y) must lie in the subdomain 
([1, 1.7123] ; [3, 3.7124]). 

\snippet tutorial.cpp func-bwd

One can indeed check that the resulting box is a consistent narrowing
of the initial one.

\subsubsection mod-func-op-grad Gradient

Consider <i>f:(x,y)&arr; x&times;y</i>.
The first and most simple way of calculating the gradient is:
\code
  double init_xy[][2] = { {1,2}, {3,4} };
  IntervalVector box(2,init_xy);
  cout << "gradient=" << f.gradient(box) << endl;
\endcode

Since \f$\frac{\partial{f}}{\partial{x}}=y\f$ and \f$\frac{\partial{f}}{\partial{y}}=x\f$ we get:
\verbatim
gradient=([3,4] ; [1,2])
\endverbatim

In this first variant, the returned vector is a new object created each time the function is called.
When we have to compute many times different values of the gradient for the same function, we can
also build a vector once for all and ask the \c gradient to store the result in this slot:

\code
  IntervalVector g(4);
  f.gradient(box,g);
  cout << "gradient=" << g << endl;
\endcode

\subsubsection mod-func-op-jac Jacobian and Hansen's matrix

The interval Jacobian matrix of a function \f$ f \f$ on a box \f$[x]\f$ is
\f[J=\left(\begin{array}{ccc}
\frac{\partial{f_1}}{\partial{x_1}}([x]) & \ldots & \frac{\partial{f_1}}{\partial{x_n}}([x])\\
\vdots \\
\frac{\partial{f_m}}{\partial{x_1}}([x]) & \ldots & \frac{\partial{f_m}}{\partial{x_n}}([x])\\
\end{array}\right)\f]

The interval Jacobian matrix is obtained exactly as for the gradient.
Just write:
\code
  f.jacobian(box)
\endcode
to get an \c IntervalMatrix
containing an enclosure of the Jacobian matrix of \e f on the box in argument.

There is also a variant where the matrix is passed as parameter
(as for the gradient) in order to avoid allocating memory for the calculated matrix:
\code 
  f.jacobian(box,J)
\endcode

You can also compute with IBEX the ``Hansen matrix''. This matrix
is another \e slope matrix, thiner than the interval Jacobian (but slower to be calculated).
It is, for example, used inside the interval Newton operator.
The Hansen matrix corresponds to the following matrix, where \f$(x_1,\ldots,x_n)\f$ denotes
the midvector of \f$[ix]\f$.

\f[\left(\begin{array}{cccc}
\frac{\partial{f_1}}{\partial{x_1}}([ix]_1,x_2,\ldots,x_n) & 
\frac{\partial{f_1}}{\partial{x_2}}([ix]_1,[ix]_2,\ldots,x_n) & 
\ldots & 
\frac{\partial{f_1}}{\partial{x_n}}([ix]_1,[ix]_2,\ldots,[ix]_n) \\
\vdots & \\
\frac{\partial{f_m}}{\partial{x_1}}([ix]_1,x_2,\ldots,x_n) & 
\frac{\partial{f_n}}{\partial{x_2}}([ix]_1,[ix]_2,\ldots,x_n) & 
\ldots & 
\frac{\partial{f_m}}{\partial{x_n}}([ix]_1,[ix]_2,\ldots,[ix]_n) \\
\end{array}\right)\f]

Here is an example:
\snippet modeling.cpp func-hansen

The display is:
\verbatim
Hansen matrix:
(([3, 6] ; [1, 4])
(<2.25, 2.25> ; [2, 8]))
\endverbatim


\subsection mod-func-cpp Creating functions (in C++)

The following piece of code creates the function
<i>(x,y)&rarr; sin(x+y)</i>:

\snippet modeling.cpp func-cpp1

The display is:
\code
_f_0:(x,y)->sin((x+y))
\endcode
%_f_0:(_x_0,_x_1)->sin((_x_0+_x_1))

You can directly give up to 6 variables in argument of the \c Function constructor:

\snippet modeling.cpp func-cpp2

If more than 6 variables are needed, you need to build an intermediate array for collecting the arguments.
More precisely, this intermediate object is an <tt>Array<const ExprSymbol></tt>. The usage is summarized below. In this
example, we have 7 variables. But instead of creating the function
\f[x\mapsto x_1+\ldots+x_7\f]
with one argument (a vector with 7 components), we decide to create the function
\f[(x_1,\ldots,x_7)\mapsto x_1+\ldots+x_7.\f]
with 7 arguments (7 scalar variables).
\code
Variable x[7]; // not to be confused with x(7)
Array<const ExprSymbol> vars(7);
for (int i=0; i<7; i++)
  vars.set_ref(i,x[i]);
Function f(vars, x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]);
cout << f << endl;
\endcode	

The display is:
\verbatim
_f_1:(_x_0,_x_1,_x_2,_x_3,_x_4,_x_5,_x_6)->
     ((((((_x_0+_x_1)+_x_2)+_x_3)+_x_4)+_x_5)+_x_6)
\endverbatim

\subsubsection mod-func-cpp-renaming Renaming functions

By default, function names are also generated. But you can also set your own function name, as the last parameter of the constructor:
\code
Function f(x,y,sin(x+y),"f");
\endcode

\subsubsection mod-func-cpp-symbols Allowed symbols

The following symbols are allowed in expressions:
\verbatim
sign, min, max,
sqr, sqrt, exp, log, pow, 
cos, sin, tan, acos, asin, atan,
cosh, sinh, tanh, acosh, asinh, atanh
atan2
\endverbatim

Power symbols \c ^ are not allowed. You must
either use <tt>pow(x,y)</tt>, or simply <tt>sqr(x)</tt> for the square function.

Here is an example of the distance function between (\c xa,\c ya) and
(\c xb,\c yb):

\snippet modeling.cpp func-cpp-symbols

\subsubsection mod-func-cpp-vecargs Functions with vector arguments

If arguments are vectors, you can refer to the component
of an argument using square brackets. Indices start by 0,
following the convention of the C language.

We rewrite here the previous distance function using 2-dimensional
arguments \c a and \c b instead:

\snippet modeling.cpp func-cpp-vec-args

\subsubsection mod-func-cpp-compo Composition

You can compose functions. Each argument of the called function can be substitued
by an argument of the calling function, a subexpression or a constant value.

See the example in the <a href="tutorial.html#func-compo">tutorial</a>.

\subsubsection mod-func-cpp-vecvalued Vector-valued functions

To define a vector-valued function, the \c Return keword allows
you to list the function's components.

See the example in the <a href="tutorial.html#func-vec-value">tutorial</a>.

\subsubsection mod-func-dag Building DAGs (directed acyclic graphs)

You can create a DAG instead of an expression tree. This will result in a gain in performance.
For that, you need to handle references of shared subexpressions with variables types \c const ExprNode&.

In the following example we create the function :
\f[f:x\mapsto (\cos(x)+1)^2, \cos(x)+1)^3)\f]
and we want the subexpression cos(x)+1 to be shared:

\snippet modeling.cpp func-dag

\subsection mod-func-ex Advanced examples

\subsubsection mod-func-ex-iterated-sum Iterated sum

Let us build a function that returns the sum of the square of \c N variables, where \c N is some constant.

The only difficulty is that we cannot assign references in C++, so we need to use pointers to
\c (const ExprNode) instead:

\snippet modeling.cpp func-iterated-sum

The display is:
\verbatim
f:(x)->(((((((((x[0]^2+x[1]^2)+x[2]^2)+x[3]^2)+x[4]^2)+x[5]^2)+x[6]^2)+x[7]^2)+x[8]^2)+x[9]^2)
\endverbatim

\section mod-sys Systems

A \e system in IBEX is a set of constraints (equalities or inequalities) with, optionnaly, a goal function to minimize
and an initial domain for variables.
It corresponds to the usual concept of system in mathematical programming.
Here is an example of system:

<center>
Minimize x+y<br>
with x &isin;[-1,1], y&isin;[-1,1]<br>
such that<br>
x^2+y^2<=1 and y\gex^2.<br>
</center>

One is usually interested in solving the system while minimizing the criterion, if any.

We first present how constraints are represented.

\subsection mod-sys-ctrs Constraints

In this section, we do not present \e constraints in their full generality
but <em>numerical constraints</em> (the ones you are the most likely interested in).

A numerical constraint in IBEX is either a relation like 
\f$f(x)<0\f$, \f$f(x)\le0\f$, \f$f(x)=0\f$, \f$f(x)\ge0\f$ or \f$f(x)>0\f$,
where \e f is a function as introduced in the previous section. If \e f is vector-valued, then
0 must be a vector.

Surprisingly, constraints do not play an important role in IBEX.
It sounds a little bit contraditory for a \e constraint programming library.
The point is that IBEX is rather a \e contractor programming library meaning that 
we build, apply and compose contractors rather than constraints directly.

As a programer, you may actually face two different situations.

Either you indeed want to use a constraint as a contractor in which case you
build a \c Ctc object with this constraint (the actual class depending
on the algorithm you chose, as explained in \link ctc the tutorial\endlink --by default, it is \c HC4Revise--).
Either you need to do something else, say, like calculating the Jacobian matrix of the
function \e f. In this case, you just need to get a reference to this function 
and call \c jacobian. In fact, all the information inherent to a constraint
(except the comparison operator of course) is contained in the underlying function so that there
is little specific code related to the constraint itself.

For these reasons, the only operations you actually do with a constraint is either
to read its field or wrap it into a contractor.

\subsubsection mod-sys-ctrs-fields Class and Fields

The class for representing a numerical constraint is \c NumConstraint.
The first field in this class is a reference to the function:
\code
  Function& f;
\endcode

The second field is the comparison operator:
\code
  CmpOp op;
\endcode

\c CmpOp is just an \c enum (integer) with the following values

<table >
<tr><td> \c LT </td><td> \f$<\f$ </td><td> \c LEQ </td><td> \f$\le\f$</td></tr>
<tr><td> \c EQ </td><td> =</td><td></tr>
<tr><td> \c GEQ </td><td> \f$\ge\f$ </td><td> \c GT </td><td> \f$>\f$</td></tr>
</table>

\subsubsection mod-sys-ctrs-cpp Creating constraints (in C++)

To create a numerical constraint, you can build the function \e f first and
call the constructor of \c NumConstraint as in the following example.

\code
Variable x;
Function f(x,x+1);
NumConstraint c(f,LEQ); // the constraint x+1<=0
\endcode	

But you can also write directly:
\code
Variable x;
NumConstraint c(x,x+1<=0);
\endcode	
which gives the same result. The only difference is that, in the second case,
the object \c c.f is ``owned'' (and destroyed) by the constraint whereas 
in the first case, \c c.f is only a reference to \c f.

Note that the constant 0 is automatically interpreted as a vector (resp. matrix),
if the left-hand side expression is a vector (resp. matrix). However, it does not
work for other constants: you have to build the constant with the proper dimension,
e.g.,
\code
Variable x(2);
NumConstraint c(x,x=IntervalVector(2,1)); // the constraint x=(1,1)
cout << "c=" << c << endl;
\endcode	

The display is:
\verbatim
c=(_x_0-([1,1] ; [1,1]))=0
\endverbatim

In case of several variables, the constructor of \c NumConstraint works as for functions.
Up to 6 variables can be passed as arguments:
\code
Variable a,b,c,d,e,f,g;
NumConstraint c(a,b,c,d,e,f,g,a+b+c+d+e+f+g<=1);
\endcode	

And if more variables are necessary, you need to build an <tt>Array<const ExprSymbol></tt> first, like \link mod-func-cpp here \endlink.

\subsection mod-sys-fields Systems fields

A system is not as simple as a collection of \e any constraints because
each constraint must exactly relates the same set of arguments. And this set must
also coincide with that of the goal function.
Many algorithms of IBEX are based on this assumption.
This is why they requires a system as argument (and not just an array of constraints).
This makes systems a central concept in IBEX.

A system is an object of the \c System class. This object is made of several fields
that are detailed below.

<ul>
<li> <tt>const int \c nb_var</tt>: the total number of variables or, in other words, the
\e size of the problem. This number is basically the sum of all arguments' components. For instance,
if one declares an argument \e x with 10 components and an argument \e y with 5, the value of this field
will be 15.<br>
<li> <tt>const int nb_ctr</tt>: the number of constraints.<br>
<li> <tt>Function* goal</tt>: a pointer to the goal function. If there is no goal function, this
pointer is \c NULL.<br>
<li> <tt>Function f</tt>: the (usually vector-valued) function representing the constraints. 
For instance, if one defines three constraints: \f$x+y\leq0\f$, \f$x-y=1\f$ and \f$x-y\geq0\f$, the function f will be 
\f$(x,y)\mapsto (x+y,x-y-1,x-y)\f$. Note that the constraints are automatically transformed so that the right side 
is 0 but, however, without changing the comparison sign. It is however possible to \link mod-sys-transfo-normalize normalize\endlink a system so that
all inequalities are defined with the \f$\le\f$ sign (see ).
<li> <tt>IntervalVector box</tt>: when a system is \link mod-minibex loaded from a file \endlink,
a initial box can be specified. It is contained in this field.
<li> <tt>Array<NumConstraint> ctrs</tt>: the array of constraints. The \c Array class of IBEX can
be used as a regular C array.
</ul>

\subsection mod-sys-transfo Transformation
<b>(under construction)</b>
\subsubsection mod-sys-transfo-copy Copy
<b>(under construction)</b>
\subsubsection mod-sys-transfo-normalize Normalization
<b>(under construction)</b>
\subsubsection mod-sys-transfo-extend Transforming the goal into a constraint
<b>(under construction)</b>
\subsubsection mod-sys-transfo-fritz-john Fritz-John (Khun-Tucker) conditions
<b>(under construction)</b>
\subsection mod-sys-auxfunc Auxiliary functions
<b>(under construction)</b>
\subsection mod-sys-cpp Creating systems (in C++)

The first alternative for creating a system is to do it programmatically, that is, directly in your C++ program.
Creating a system in C++ resorts to a temporary object called a <em>system factory</em>. The task is done in a few simple steps:
<ul>
<li> declare a new system factory (an object of \c SystemFactory)
<li> add arguments in the factory using \c add_var.
<li> (optional) add the expression of the goal function using \c add_goal
<li> add the constraints using \c add_ctr
<li> create the system simply by passing the factory to the constructor of \c System
</ul>

Here is an example:

\code
  Variable x,y;

  SystemFactory fac;
  fac.add_var(x);
  fac.add_var(y);
  fac.add_goal(x+y);
  fac.add_ctr(sqr(x)+sqr(y)<=1);

  System sys(fac);
\endcode

If you compare the declaration of the constraint here with the examples given \link mod-sys-ctrs-cpp here\endlink,
you notice that we do not list here the arguments before writing <tt>sqr(x)+sqr(y)<=1</tt>.
The reason is simply that, as said above, the goal function and the constraints in a system
share all the same list of arguments. This list is defined via \c add_var once for all.

\section mod-minibex The Minibex syntax

Entering a system programmatically is usually not very convenient.
You may prefer to separate the model of the problem from the algorithms
you use to solve it. In this way, you can run the same program with different
variants of your model without recompiling it each time.

IBEX provides such possibility. You can directly load a system from a (plain text) input file.

Here is a simple example. Copy-paste the text above in a file named, say, <tt>problem.txt</tt>. 
The syntax talks for itself:

<div class="minibex">\verbatim
Variables
  x,y;

Minimize
  x+y;

Constraints
  x^2+y^2<=1;
end
\endverbatim</div>

Then, in your C++ program, just write:

\code
System sys("problem.txt");
\endcode

and the system you get is exactly the same as in the previous example.

Next sections details the mini-language of these input files. 

\subsection mod-minibex-struct Overall structure
First of all, the input file is a sequence of declaration blocks that must respect the following order:
<ul>
<li> (optional) constants
<li> variables
<li> (optional) auxiliary functions
<li> (optional) goal function
<li> constraints
</ul>

Next paragraph gives the basic format of numbers and intervals.
The subsequent paragraphs detail each declaration blocks.

\subsection mod-minibex-reals Real and Intervals
A real is represented with the usual English format, that is
with a dot separating the integral from the decimal part,
and, possibly, using scientific notation.

Here are some valid examples of reals in the syntax:
\verbatim
0
3.14159
-0.0001
1.001e-10
+70.0000
\endverbatim
 
An interval are two reals separated by a comma
and surrounded by square brackets. The special symbol
\c oo (two consecutive "o") represents the infinity \f$\infty\f$.
Note that, even with infinity bounds, the brackets
must be squared (and not parenthesis as it should be since the
bound is open). Here are some examples:

\verbatim
[0,1]
[0,+oo]
[-oo,oo]
[1.01e-02,1.02e-02]
\endverbatim

\subsection mod-minibex-constants Constants
Constants are all defined in the same declaration block, 
started with the \c Constants keyword.
A constant value can depends on other (previously defined) constants value. Example:

<div class="minibex">\verbatim
Constants
  pi=3.14159;
  y=-1.0;
  z=sin(pi*y);
\endverbatim</div> 

You can give a constant an interval enclosure rather than a single fixed value.
This interval will be embedded in all subsequent computations.
Following the previous example, we can give \c pi a valid enclosure as below.
We just have to replace "=" by "in".


<div class="minibex">\verbatim
Constants
  pi in [3.14159,3.14160];
  y=-1.0;
  z=sin(pi*y);
\endverbatim</div>

Constants can also be vectors, matrices or array of matrices.
You need to specify the dimensions of the constant in square brackets.
For instance \c x below is a column vector with 2 components, the first component is equal
to 0 and the second to 1:
<div class="minibex">\verbatim
Constants
x[2] = (0; 1);
\endverbatim</div>
Writing \c x[2] is equivalent to \c x[2][1] because a column vector is also
a 2&times;1 matrix.
A row vector is a 1&times;2 matrix so a row vector has to be declared as follows. 
On the right side, note that we use commas instead of periods:
<div class="minibex">\verbatim
Constants
x[1][2] = (0, 1);
\endverbatim</div>

\b important remark. 
The reason why the syntax for declaring row vectors differs here from Matlab is that a 2-sized row vector surrounded
by brackets would conflict with an interval. So, do note confuse
\c [0,1] with \c (0,1):
<ul>
<li> \c (0,1) is a 2-dimensional row vector of two reals, namely 0 and 1.
This is \b not an open interval.
<li> \c [0,1] is the 1-dimensional interval [0,1]. This is \b not a 2-dimensional row vector.
</ul>

Of course, you mix vector with intervals. For instance:
\c ([-oo,0];[0,+oo]) is a column vector of 2 intervals, \f$(-\infty,0]\f$ and \f$[0,+\infty)\f$.

Here is an example of matrix constant declaration: 
<div class="minibex">\verbatim
Constants
M[3][2] = ((0 , 0) ; (0 , 1) ; (1 , 0));
\endverbatim</div>
This will create the constant matrix \c M with 3 rows and 2 columns equal to
\f[\left(\begin{array}{cc}
0 & 0 \\ 
0 & 1 \\ 
1 & 0 \\
\end{array}\right)\f]

You can also declare array of matrices:
<div class="minibex">\verbatim
Constants
 c[2][2][3]=(((0,1,2); (3,4,5)) ; ((6,7,8); (9,10,11)));
\endverbatim</div>
It is possible to define up to three dimensional vectors, but not more.

When all the components of a multi-dimensional constant share the same interval, you
don't need to duplicate it on the right side. Here is an example of a 10&times;10 matrix where all
components are [0,0]:
<div class="minibex">\verbatim
Constants
 c[10][10] in [0,0];
\endverbatim</div>
Ibex intializes the 100 entries of the matrix \c c to \f$[0,0]\f$.

Finally, the following table summarizes the possibility for declaring constants
through different examples.

<table>
<tr><td><tt> x in [-oo,0]</tt> </td><td> declares a constant \c x \f$\in(-\infty,0]\f$ </td></tr>
<tr><td><tt> x in [0,1]</tt> </td>  <td> declares an constant \c x &isin;[0,1] </td></tr>
<tr><td><tt> x in [0,0]</tt> </td>  <td> declares a constant \c x &isin;[0,0] </td></tr>
<tr><td><tt> x = 0</tt> </td>       <td> declares a real constant \c x equal to 0 </td></tr>
<tr><td><tt> x = 100*sin(0.1)</tt></td><td> declares a constant \c x equal to 100*sin(0.1) </td></tr>
<tr><td><tt> x[10] in [-oo,0]</tt></td><td> declares a \10-sized constant vector \c x, with each component <tt> x(i)</tt> \f$\in(-\infty,0]\f$ </td></tr>
<tr><td><tt> x[2] in ([-oo,0];[0,+oo])</tt> </td><td> declares a 2-sized constant vector \c x with <tt> x(1)</tt>\f$\in(-\infty,0]\f$ and <tt> x(2) &isin; \f$[0,+\infty)\f$ </td></tr>
<tr>
<td><tt> x[3][3] in<br>
  (([0,1],0,0);
  (0,[0,1],0);
  (0,0,[0,1]))</tt>
</td>
</tt> </td>
<td>declares a constrant matrix \c x 
\f$\in
\left(\begin{array}{ccc}
[0,1] & 0 & 0\\
0 & [0,1] & 0\\
0 &  0 & [0,1] \\
\end{array}\right)\f$.</td></tr>
<tr><td><tt> x[10][5] in [0,1]</tt> </td><td> declares a matrix \c x with each entry <tt> x(i,j)</tt> &isin;[0,1].</td></tr>
<tr><td><tt> x[2][10][5] in [0,1]</tt> </td><td> declares an array of 2 \f 10x5 matrices with each entry <tt> x(i,j,k)</tt> &isin;[0,1].</td></tr>
</table>

\subsection mod-minibex-func Functions

When the constraints involve the same expression repeatidly, it may be
convenient for you to put this expression once for all in a separate auxiliary
function and to call this function.

Assume for instance that your constraints intensively use the following expression
\f[\sqrt{(x_a-x_b)^2+(y_a-y_b)^2)}\f]
where \f$x_a,\ldots y_b\f$ are various sub-expressions, like in:
<div class="minibex">\verbatim
sqrt((xA-1.0)^2+(yA-1.0)^2<=0;
sqrt((xA-(xB+xC))^2+(yA-(yB+yC))^2=0;
...
\endverbatim</div>

You can declare the distance function as follows.
<div class="minibex">\verbatim
function distance(xa,ya,xb,yb)
 return sqrt((xa-xb)^2+(ya-yb)^2;
end
\endverbatim</div>

You will then be able to simplify the writing of constraints:
<div class="minibex">\verbatim
distance(xA,1.0,yA,1.0)<=0;
distance(xA,xB+xC,yA,yB+yC)=0;
...
\endverbatim</div>

As you may expect, this will result in the creation of
a \link mod-func \c Function \endlink object that
you can access from your C++ program via the \c System class.
See \link mod-sys-auxfunc auxiliary functions \endlink.

A function can return a single value, a vector
or a matrix. Similarly, it can take real, vectors or matrix arguments.
You can also write some minimal ``code'' inside the function before
returning the final expression.
%assigning the final expression to the return variable. 
This code is however limited to be a sequence of assignments.

Let us now illustrate all this with a more sophisticated example.
We write below the function that calculates the rotation matrix
from the three Euler angles, \f$\phi\f$, \f$\theta\f$ and \f$\psi\f$ :

\f[R : (\phi,\psi,\theta) \mapsto
\left(\begin{array}{ccc}
\cos(\theta)\cos(\psi) & -\cos(\phi)\sin(\psi)+\sin(\theta)\cos(\psi)\sin(\phi) & \sin(\psi)\sin(\phi)+\sin(\theta)\cos(\psi)\cos(\phi)\\
\cos(\theta)\sin(\psi) & \cos(\psi)\cos(\phi)+\sin(\theta)\sin(\psi)\sin(\phi) & -\cos(\psi)\sin(\phi)+\sin(\theta)\cos(\phi)\sin(\psi)\\
-\sin(\theta) & \cos(\theta)\sin(\phi) & \cos(\theta)\cos(\phi);
\end{array}\right)
\f]

As you can see, there are many occurrences of the same subexpression
like \f$\cos(\theta)\f$ so a good idea for both readibility and (actually) efficiency
is to precalculate such pattern and put the result into an intermediate variable.

Here is the way we propose to define this function:

<div class="minibex">\verbatim
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
\endverbatim</div>

\b Remark. Introducing temporary variables like \c cphi amouts to build a DAG instead of
a tree for the function expression. It is also possible (and easy) to \link mod-func-dag build a DAG \endlink when you directly create
a \c Function object in C++.

\subsection mod-minibex-vars Variables (or arguments)

Variables are defined exactly in the same fashion as \link mod-minibex-constants constants\endlink.
It is possible to define up to three dimensional vectors, with an optional domain to initialize each
component with. The following examples are valid:

<tt> x[10][5][4];</tt>

<tt> x[10][5][4] in [0,1];</tt>

Whenever domains are not specified, they are set by default to \f$(-\infty,+\infty)\f$.

\subsection mod-minibex-ctrs Constraints

Constraints are simply written in sequence.
The sequence starts with the keword \c constraints and terminates with the keyword \c end. 
They are a separated by semi-colon. Here is an example:

<div class="minibex">\verbatim
Variables
  x in [0,oo];
Constraints
  //you can use C++ comments
  x+y>=-1;
  x-y<=2;
end
\endverbatim</div>

\subsubsection mod-minibex-ctrs-loop Loops

You can resort to loops in a Matlab-like syntax to define constraints. Example:

<div class="minibex">\verbatim
Variables
  x[10];

Constraints
  for i=1:10;
    x(i) <= i;
  end
end
\endverbatim</div>

\subsection mod-minibex-cpp Some differences with C++

<ul>
<li> Vectors are surrounded by parenthesis (not brackets)
<li> Indices start by \f$1\f$ instead of \f$0\f$
<li> You can use the "^" symbol
</ul>
*/
