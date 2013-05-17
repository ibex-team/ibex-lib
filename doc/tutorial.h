

/** \page tutorial Tutorial
 
\tableofcontents

\section start Getting started


\subsection start-run-solver Run the default solver
A couple of example programs are compiled with Ibex and installed under the
\c /__build__/examples/ subfolder.

One of them is the <i>default solver</i> that solves a system of nonlinear equations rigorously
(that is, it does not lose any solution and return each solution under the form of a 
small box enclosing the true value). It is called the "default" solver because is resorts to
a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that Ibex also supplies a "generic" solver that is much more customizable.

You can directly apply this default solver on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the <a href="#func-minibex">Minibex syntax</a> and stored 
in an arborescence under \c benchs.

Open a terminal and move to the examples subfolder:
\code
~/Ibex/ibex-2.0/$ cd __build__/examples
~/Ibex/ibex-2.0/__build__/examples/$
\endcode

Now, run the default solver with, for example, the problem named kolev36 located at the specified path:
\code
~/Ibex/ibex-2.0/__build__/examples/$./defaultsolver ../../benchs/benchs-satisfaction/benchlib2/kolev36.bch 1e-07 10
\endcode

The second argument is the precision required on solution (1e-07). The third and last argument
is the time limit in seconds (10 seconds).

The following result should be displayed:
\verbatim
load file ../../benchs/benchs-satisfaction/benchlib2/kolev36.bch.
sol 1 nb_cells 6 ([0.1173165676349034, 0.1173165676349242] ;
                  [0.4999999999996189, 0.5000000000003763] ;
                  [0.8826834323643938, 0.8826834323657756] ;
                  [-0.2071067811866001, -0.2071067811865033] ;
                  [1.207106781186472, 1.207106781186622] ;
                  [-2.000000000000119, -1.999999999999871])
number of solutions=1
cpu time used=0.08000500000000002s.
number of cells=26
\endverbatim

The first simply says that the file has been successfuly loaded.
The second line (broken here into 6 lines for clarity) details the first solution found. There are 6 variables so 6 intervals are displayed.

The third line is the total number of solutions found (there is just one here).
The two last lines report the CPU time and the number of hypothesis (bisections) that was required to solve the problem.

\subsection start-run-optim Run the default optimizer

Similarly to the default solver, a default optimizer is installed with Ibex.
This program minimizes a (nonlinear) objective function under (nonlinear) inequality constraints.
Let us execute this optimizer with the problem ex3_1_3 got from the Coconut library. If you compare
the Minibex syntax of this benchmark with that of the previous example, you will see that a "minimize"
keyword has appeared.

\code{.txt}
~/Ibex/ibex-2.0/__build__/examples/$./defaultoptimizer ../../benchs/benchs-optim/coconutbenchmark-library1/ex3_1_3.bch 1e-07 1e-07 10
\endcode

As you see, the optimizer requires 3 arguments, besides the name of the benchmark.
The extra argument corresponds to the precision on the objective (both relative and absolute). Here, this precision is also set to 1e-07.

The following result should be displayed:
\verbatim
 best bound in: [-310,-309.999999984]
 Relative precision obtained on objective function: 5.23566394568e-11  [passed]  1e-07
 Absolute precision obtained on objective function: 1.62305582307e-08  [passed]  1e-07
 best feasible point (4.9999999999 ; 1 ; 5 ; 1.53748558193e-10 ; 5 ; 10)
 cpu time used 0.048003s.
 number of cells 78
\endverbatim

The program has proved that the minimum of the objective lies in [-310,-309.999999984]. It also gives
a point (4.9999999999 ; 1 ; 5 ; 1.53748558193e-10 ; 5 ; 10) which satisfies the constraints and for which
the value taken by the objective function is inside this interval.

\subsection start-main Start a program

To write a program with Ibex, use the following canvas:

\code
#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

// write your own code here

}
\endcode
You can execute by yourself all the code snippets of this tutorial, using this canvas.

To compile a program, the easiest way is to copy-paste the \c makefile
of the \c examples subfolder of Ibex.
See also the <a href="../../index.php?page=download#examples" target="_parent">guidelines</a>.

\subsection start-call-solver Call the default solver from C++

You can call the default solver and get the solutions from C++.
Two objects must be built: the first represents the problem (or "system"), the second
the solver itself. Then, we just run the solver. Here is a simple example:
\snippet tutorial.cpp start-call-solver

<a href="uploads/api/classibex_1_1DefaultSolver.html" target="_blank">See the API</a>.

\subsection start-call-optim Call the default optimizer from C++

Calling the default optimizer is as simple as for the default solver.
The loaded system must simply correspond to an optimization problem. The default optimizer
is an object of the class \c DefaultOptimizer.
Once the optimizer has been executed(), the main information is stored in three fields, 
where f is the objective:
<ul>
<li>\c loup ("lo-up") is the lowest upper bound known for min(f).
<li>\c uplo ("up-lo") is the uppest lower bound known for min(f).
<li>\c loup_point is the vector for which the value taken by f is less or
equal to the loup.
</ul>
Example:
\snippet tutorial.cpp start-call-optim


\section Basic Interval computations


\subsection basic-create-itv Creating intervals
Here are examples of intervals
\snippet tutorial.cpp basic-create-itv

\subsection basic-op-itv Operation between intervals
C++ operator overloading allows you to calculate the sum of two
intervals by using directly the "+" symbol:
\snippet tutorial.cpp basic-op-itv
You can use the other operators similarly (\c -,\c *,\c /).


\subsection basic-func-itv Applying a function to an interval
All the elementary functions can be applied to intervals, and composed in an arbitrarly way:
\snippet tutorial.cpp basic-func-itv

\subsection basic-vec Interval vectors
You can create an interval vector by using an intermediate array of n*2\c double, representing the lower and
uppoer bounds of each components.
The first argument of the constructor of \c IntervalVector in this case is the dimension (here, 3),
the second the array of \c double.
\snippet tutorial.cpp basic-create-vec1
You can also create an interval vector by duplicating a given interval or simply create the empty interval vector.
\snippet tutorial.cpp basic-create-vec2

\subsection basic-mat Interval matrices
Interval matrices can be created in a similar way. However, since we cannot build 3-dimensional arrays in C++,
all the bounds must be set in a single n*2 array representing the matrix row by row (and n is the total number of entries
of the matrix). The two first arguments of the constructor are the number of rows and columns respectively. The last one
is the array of \c double.
Here is an example of a 3x3 matrix:
\snippet tutorial.cpp basic-create-mat

\subsection basic-mat-vec Operations between matrices and vectors
You can use the usual operations of linear algebra between matrices and vectors (<em>sum of vectors,
transpose of vectors, sum of matrices, left multiplication of a matrix by a scalar, etc.</em>).

\snippet tutorial.cpp basic-mat-vec

\subsection basic-mid-rad Midpoint, radius, magnitude, etc.
These usual properties can be obtained for intervals. They are also all extended to interval vectors or matrices
componentwise. For instance, the radius of an interval matrix is the (real) matrix of the radii.

As a consequence, Ibex also has classes to handle real (versus interval) vectors and matrices.
Mathematical Operations (like the sum) can also be applied to these objects but, of course, using this times
floating-point arithmetic (not interval).

\snippet tutorial.cpp basic-mid-rad

\section func Functions

\subsection func-create Creating functions
To create your own function, you must first build \e variables. As before, the expression of the function
is then obtained using C++ operator overloading:
\snippet tutorial.cpp basic-func-create

\subsection func-create-cst Constants inside functions
You can insert interval constants in the expresion of a function.
For instance, if you want to create the function sin(2*x), just write:
\snippet tutorial.cpp basic-func-create-cst1

Assume now that the function to be created is sin(π*x). It is still possible to
use a \c double representing approximately &pi; but to keep
numerical reliability, it is required in this case to use an interval constant enclosing
&pi;. Next function must be seen as a ``thick'' function rigorously encloses sin(π*x):
\snippet tutorial.cpp basic-func-create-cst2

\subsection func-vec-arg Functions with vector arguments
Arguments of a function are not necessarily scalar variables. They can also be vectors
or matrices. In the following example, we build the distance function:
\f[dist:(a,b)->\|a-b\|]\f]

where a and b are 2-dimensional vectors.
\snippet tutorial.cpp func-vec-arg

\note \link func-eval Evaluation \endlink of a thick function will necessarily result in an interval with non-null diameter, even if the argument is reduced to a point.


\subsection func-compo Composing functions
You can compose functions to build new functions. We build here the function that maps a vector x
to its distance with a constant point (1,2). To this end, we first define a generic distance function 
dist(a,b) as above.
\snippet tutorial.cpp func-compo

\subsection func-vec-value Vector-valued functions

In line with the previous example, we define now the function that
associates to a vector x its distance with two different points:

\f[f:x->( \|x-(1,1)\| , \|x-(0,0)\| )\f]
\snippet tutorial.cpp func-vec-value

\note The \c Return keyword is only necessary when the output of a function is a vector (or a matrix).

\subsection func-mat-value Matrix-valued functions
You can also create functions that return matrices.
Here is an example of a function from R to R^(2x2) where:
\f[f: x \mapsto ( (2\times x, -x) ; (-x,3\times x) )\f]

\snippet tutorial.cpp func-mat-value

The boolean value \c true given here to the two embedded \c Return
means that, each time, the two components must be put in rows, and not in column as it is by default.
In contrast, the enclosing \c Return keeps the default behaviour since the two rows are
put in column in order to form a 2x2 matrix.

\subsection func-minibex Using the Minibex syntax

To create sophisticated functions we advice you to use an intermediate "minibex" input file as follows instead of
embedding the function directly in your C++ program.
The previous example can be written in a plain text file:
\code{.txt}
function f(x)
return ((2*x,-x);(-x,3*x));
end
\endcode
Save this file under the name "myfunction.txt". Now, you can load this function in your C++ program:
\code{.pp}
Function f("myfunction.txt");
\endcode

\subsection func-minibex-inter Minibex syntax with intermediate variables
When several occurrences of the same subexpression occur in a function, it
is a good idea for readibility (and, actually, efficiency) to put
this subexpression into intermediate variables.

The following example is the function thar returns the rotation matrix
from the three Euler angles. In this function an experssion like cos(phi)
occurs several times.

\code{.txt}
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
\endcode

\subsection func-eval-float Evaluation over floating-point numbers
Given input \c double values x, you can obtain a rigorous inclusion of f(x) either using
\c eval, \c eval_vector or \c eval_matrix.
These functions return interval enclosures of the true result.

These functions are presented below in a more general setting where the inputs are intervals as well.

So, to get the image by f of fixed floating-point values, simply create degenerated intervals in the
next examples.

\subsection func-eval Interval evaluation
The interval evaluation of f is the image of the given input interval vector [x] by f, this range being noted by f([x]):

\f[ f([x]) := \{ f(x), x\in[x] \}\f]

Let us start with a real-valued function f with scalar arguments:

\snippet tutorial.cpp func-eval

The sine function is not monotonic on [4,6] and actually reaches its
minimum at 3&pi;/2.

Note that the \c eval takes an \c IntervalVector
as argument, even if there is only one variable. So, in the latter case, you have to build a vector
reduced to a single component.

We consider now a vector-valued function.
Since the return type of an evaluation is not anymore an \c Interval but
an \c IntervalVector, we have to use a method with a different 
signature, namely, \c eval_vector:

\snippet tutorial.cpp func-eval-vec

Finally, for a matrix-valued function, the evaluation is obtained via \c eval_matrix.
We assume again that the following matrix-valued function
\f[ f: x \mapsto ( (2\times x, -x) ; (-x,3\times x) ) \f]
has been written in a "minibex" input file (see above).

\snippet tutorial.cpp func-eval-mat

\subsection func-grad Interval gradient
For a scalar-valued function, you can get an interval enclosure of the gradient:

\snippet tutorial.cpp func-grad

\subsection func-jac Interval Jacobian matrix
For a vector-valued function, you can get an interval enclosure of the Jacobian matrix:

\snippet tutorial.cpp func-jac

\subsection func-bwd Backward (or contraction)
One of the main feature of Ibex is the ability to <i>contract</i> a box representing the domain of a variable
x with respect to the constraint that f(x) belongs to a restricted input range [y]. 
Rigorously, given two intervals [x] and [y], the contraction gives a new interval [z] such that
\f[ \forall x\in[x], \quad f(x)\in[y] \Longrightarrow x \in[z] \subseteq [x] \f]

One way to do this is by using the famous \e backward algorithm. 
This algorithm does not return a new interval [z] but contract the input interval [x] which is therefore 
an input-output argument.

In the following snippet we require the function sin(x+y) to take the value -1 (a degenerated interval).
With an initial box (x,y)=[1,2],[3,4], we obtain the result that (x,y) must lie in the subdomain 
([1, 1.7123] ; [3, 3.7124]).

\snippet tutorial.cpp func-bwd

\section ctc Contractors

\subsection ctc-what What is a contractor programming?
The key idea behind <em>contractor programming</em> is to abstract the algorithm
from the underlying constraint and to view it a function "C":

\f[C: \mathbb{IR}^n \to \mathbb{IR}^n \ \mbox{such that} \ C([x])\subseteq[x],\f]
where \f$\mathbb{IR}\f$ denotes the set of real intervals.

In other word, we take as primary concept the \e operational definition of a constraint.

In this way, operators (like the intersection and the others below) can be extended to contractors. 

Since contractors implicitly represent sets, the fundamental advantage of extending operations
to contractors is that we actually extend these operations to sets. 

All contractors in Ibex are algorithms represented by different classes. 
See the <a href="http://en.wikipedia.org/wiki/Strategy_pattern" target="_blank">strategy pattern</a> for more information on this design choice.
Classes representing contractors are prefixed by \c Ctc.

\subsection ctc-fwd-bwd Forward-Backward
The standard way to contract with respect to a constraint is by using the <em>forward-bacwkard</em> algorithm.
The corresponding class is \c CtcFwdBwd.

A constraint has to be built first using the \c NumConstraint class.
In the following piece of code, we build a forward-backward contractor with respect to x+y=z.

\snippet tutorial.cpp ctc-fwd-bwd

Of course, the expression of a constraint can involve a previously defined function.
Furthermore, if the constraint is simply "f=0", where f is a \c Function object, it is not 
necessary in this case to build an intermediate \c NumConstraint object. 
One can directly give the function f that has to be nullify to \c CtcFwdBwd. 
In the next example, we consider the problem of finding the point which distance from
both (0,0) and (1,1) is sqrt(2)/2. The solution is (0.5,0.5).

\snippet tutorial.cpp ctc-fwd-bwd2

Of course, the result is rather crude. Remember that the purpose of \c CtcFwdBwd 
is to contract \e quickly with respect to \e any numerical constraint: it is widely applicable and takes
a time that is only proportional to the expression size. In the other hand, it is not accurate in general.

\subsection ctc-fixpoint Fixpoint

The fixpoint operator applies a contractor C iteratively:
\f[fixpoint(C): [x] \mapsto C(\ldots C([x])\ldots),\f]
while the "gain" is more than the given \c ratio. More precisely, the "gain"
is the relative Hausdorff distance between the input box [x] and the output box C([x]) but, often, you can ignore 
the precise meaning of this gain and just consider that the procedure will loop until the contracted box
will roughly differ "by ratio" from the input one.

Let us now follow the previous example. As said, the solution is (0.5,0.5). We can see that simply embedding
the \c CtcFwdBwd contractor in a fixpoint loop (with a \c ratio
set to 0.1) gives a box with sharp bounds.

\snippet tutorial.cpp ctc-fixpoint

\subsection ctc-inter-union Intersection, union & composition
Given two of more contractors, we can apply the two logical operators <i>union</i> and <i>intersection</i>:
\f[union(C_1,\ldots,C_n): [x] \mapsto C_1([x]) \cup\ldots\cup C_n([x]).\f]
\f[inter(C_1,\ldots,C_n): [x] \mapsto C_1([x]) \cap\ldots\cap C_n([x]).\f]

However, the latter operation is barely used and usually replaced by the \e composition:
\f[compo(C_1,\ldots,C_n): [x] \mapsto C_n(\ldots(C_1([x])\ldots).\f]

Indeed, one can see that the composition amounts to the same logical operation (the intersection of each contractor's set), 
but in a more efficient way since we take advantage of the contraction performed by C1,...,C(i-1) when contracting with Ci.
In contrast, the intersection operator calls each contractor independently on the same initial box.

The corresponding classes are \c CtcUnion and \c CtcCompo.

As a rule of thumb, use \c CtcUnion for the union of two contractors and \c CtcCompo for the intersection.
Here is an example with the union:
\snippet tutorial.cpp ctc-union
Here is an example with the intersection (composition):
\snippet tutorial.cpp ctc-inter

\subsection ctc-newton Interval Newton
When a function is "square" (the dimension is the same as the codimension, i.e., \f$f:\mathbb{R}^n\to\mathbb{R}^n\f$), you can contract a 
box with respect to the constraint f(x)=0 using the interval Newton iteration. You just have to build a
\c CtcNewton object with the function and call \c contract.
This operator can give extremly accurate bounds proving that the input box is already "sufficiently" small (that is, 
"inside the convergence basin" of Newton's iteration). In the following example, we give a box that encloses the
solution (1,0) with a radius of 10^-3. Newton's iteration contracts this box downto the maximal precision:

\snippet tutorial.cpp ctc-newton

\subsection ctc-propag Propagation
The <i>propagation</i> operator calculates the fixpoint of (the composition of) n contractors by using a more 
sophisticated ("incremental") strategy than a simple loop.
So, semantically, the propagation operator can be defined as follows:
\f[propagation(C_1,\ldots,C_n):=fixpoint(compo(C_1,\ldots,C_n)).\f]
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
<b>(to be completed)</b>

\subsection ctc-acid Acid & 3BCid
<b>(to be completed)</b>

\subsection ctc-lin-rel Linear Relaxation
<b>(to be completed)</b>

\subsection ctc-qinter Q-Intersection (robustness w.r.t. outliers)
The Q-intersection is typically used in a context where we have a set of contractors that result
from measurements (each measurement enforces a constraint), some of which can be incorrect.
If we are sure that at least q measurements are correct (which amounts to say that the number of
outliers is bounded by N-q) then we can contract the box in a robust way, by calculating the union
of the boxes resulting from the contraction with all combinaisons of q contractors among N.
Mathematicaly, with (i1 , . . . , iq) ranging over the set of all q distinct indices between 0 and N-1:
\f[q-inter(C_1,\ldots,C_n,q):=union(\ldots,inter(Ci1,\ldots,Ciq),\ldots)\f]

Here is a simple example inspired from parameter estimation.
We assume a point (x,y) has to be localized. We measure 4 distances "bD" from 6 (approximately known) points (bX,bY).
 Each position bX, bY and each distance bD has an uncertainty [-0.1,0.1]. We also know there may be at most one outlier.

The solution point is: x=6.32193 y=5.49908

First of all, let us enter the coordinates of the points (bX,bY) and the distances. This data will simulate our measurements.
\snippet tutorial.cpp ctc-qinter1
define the measurement intervals (with uncertainty taken into account)
\snippet tutorial.cpp ctc-qinter2
Now, we artificially introduce an outlier by shifting the interval for one measurement (here, x position n°5) by a
large value:
\snippet tutorial.cpp ctc-qinter3
Now, all our simulated data is set up. We just have to define the contractors.
We first declare the distance function and then 6 contractors corresponding to
the distance with each (bX,bY):
\snippet tutorial.cpp ctc-qinter4
We can contract now a box with the q-intersection of these contractors:
\snippet tutorial.cpp ctc-qinter5
The displayed result is ([3.9667, 7.2381] ; [4.5389, 8.1479]). Of course, we can do better by calculating a fixpoint of the q-intersection:
\snippet tutorial.cpp ctc-qinter6
The displayed result is ([5.9277, 6.8836] ; [5.0914, 5.7996]) which, indeed, better encloses the solution point x=6.32193 y=5.49908.

\subsection ctc-own Build your own contractor
To create a contractor, you just have to declare a class that extends \c Ctc and create:
<ul>
<li>a constructor that calls the constructor of \c Ctc. The latter
requires an integer argument: the dimension of the box to be contracted (this dimension must indeed be fixed at construction
time, this is a restriction imposed by Ibex).
<li>a function \c contract that takes a reference to a box (\c IntervalVector&)
and contracts it. The function returns \c void.
</ul>
In the following example, we create a contractor that simply divides by two the radius of each component.
\snippet tutorial.cpp ctc-own1
Then, if we create this contractor and applies it several time to the same box, we can observe the expected result:
\snippet tutorial.cpp ctc-own2
This contractor can now be combined with the ones built-in. For instance, we can decide to calculate the fixpoint.  Then, the result is a small box enclosing (0.5,0.5,0.5):
\snippet tutorial.cpp ctc-own3

\section strat Strategies

\subsection strat-def-solver The default solver <b>(under construction)</b>
\subsection strat-gen-solver The generic solver <b>(under construction)</b>
\subsection strat-def-optim The default optimizer <b>(under construction)</b>
\subsection strat-gen-optim The generic optimizer <b>(under construction)</b>
\subsection strat-sivia The default SIVIA <b>(under construction)</b>

\section ex Complete examples
\subsection ex-sivia Your own SIVIA <b>(under construction)</b>
\subsection ex-solver Your own solver <b>(under construction)</b>
\subsection ex-param Parameter estimation<b>(under construction)</b>
\subsection ex-slam Robot localization<b>(under construction)</b>

*/
