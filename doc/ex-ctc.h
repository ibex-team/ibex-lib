/** \page ex-ctc A simple SLAM example with contractor programming
 
\tableofcontents

\section intro Introduction

The goal of this example is to implement a simple contractor strategy 
for a SLAM problem with the IBEX library. SLAM means <i>simultaneous localization and map building</i>
and is a classical problem in mobile robotics.

We will see that contractor programmming with Ibex basically amounts to:
<ul>
<li> enter your mathematical model using the 
<a href="modeling.html#mod-func">Function</a> and 
<a href="modeling.html#mod-sys-ctrs">NumConstraint</a> classes
<li> build basic contractors (\c CtcFwdBwd in general) with respect to the equations
<li> apply operators to these contractors to yield new (more sophisticated) contractors
</ul>

The code we build here will eventually involve 5 different contrators:
<ul>
<li> \c CtcFwdBwd
<li> \c CtcCompo
<li> \c CtcFixPoint
<li> \c CtcQInter 
<li> \c CtcInverse.
</ul>

We shall implement a strategy that is similar to a predictor-corrector approach (like the Kalman filter for instance)
in the sense that we also use odometry and observation to reduce the uncertainty on the robot's position.
However, both information as considered on the same footing and there is no distinction such as \em prediction versus \em correction.
They are just contractors that can be used in any order and we will even calculate a fixpoint of them (so the strategy
is not a \em recursive filter).

\note For the sake of simplicity, we shall always use dynamic <i>allocation</i>:
\code
MyClass* x = new MyClass(...)
\endcode
just to avoid potential memory fault when pointing to temporary objects.
Of course, all these objects should be disallocated afterwards.

\subsection intro-download Download

<a href="../doc/slam.tar.gz">Download the full code</a>.

\subsection intro-prob-desc Problem description

The goal is to charaterize the trajectory of an autonomous robot by enclosing in a box its position x[t]
for each time step t=0...T.

<center>
\image html loc.png "SLAM"
\image latex loc.png "SLAM"
</center>

We have no direct information on its position (including the initial one) but the robot measures 
at each time step:
<ul>
<li> its distance from a set of N fixed beacons (&rarr; N measurements) as if it was equipped with a telemeter;
<li> its "speed" (delta) vector v[t]=x[t+1]-x[t].
</ul>

Each measurement is subject to uncertainty: <i>distances and speed vector</i>
but also the position of the beacons, that is supposed to be measured a priori.

Furthermore, we shall consider outliers.

First of all, let us assume that the measurements are all simulated in a seperate unit. 
The header file of this unit contains:

\snippet slam/data.h data

\section strat1 First strategy (no outlier)

First, we consider no outlier. A simple strategy consists in :
<ul>
<li> creating a contractor for each measurement,
<li> calling all these contractors in sequence (composition)
<li> performing a fixpoint loop
</ul>

Let us start by creating contractors for measurements, 
that is, those related to equations.

\subsection strat1-eq Entering equations and functions

A measurement is an equation. 

To enter an equation in Ibex, we use the <a href="modeling.html#mod-sys-ctrs">\c NumConstraint class</a>.
A \c NumConstraint object contains a mathematical condition,
or \c constraint.

To define a constraint mathematically, we must specify how many variables it relates
and in which order these variables must be taken.

That is why we need to create first some \c Variable objects.
But keep in mind that these objects are just a C++ trick for the only purpose of
declaring a constraint.

Once declared, a constraint is self-contained and depends on nothing else.

\e Example For creating the equations: 

<center>
forall t<T, x[t+1]-x[t]=v[t]
</center>

The corresponding code in Ibex is:
\code
  Variable x(T,2);  // create a Tx2 variable

  for (int t=0; t<T; t++) {
    if (t<T-1) {
      NumConstraint* c=new NumConstraint(x,x[t+1]-x[t]=v[t]);
      ...
    }
  }
\endcode

\note Here, \c v is not a variable but a constant (see \c data.h).

Sometimes, different constraints are based on the same pattern. It is then often convenient
to declare first a \c Function object.

\e Example. For distances constraints, we may first declare the distance function:

\snippet slam/slam1.cpp dist-decl

and then the equation for each time step and each beacon:
\code
    for (int t=0; t<T; t++) {
      for (int i=0; i<N; i++) {
      NumConstraint* c=new NumConstraint(
              x,dist(x[t],beacons[i])=d[t][i]);
      ...
      }
    }
\endcode


\subsection strat1-basic-ctc Creating basic contractors

We can create now contractors.

To create a contractor with respect to an equation we 
use the \c CtcFwdBwd class (stands for <i> forward-backward</i>).

\e Example. With the constraint x=1:

\code
		Variable x;
		NumConstraint* c=new NumConstraint(x,x=1);
		Ctc* ctc=new CtcFwdBwd(*c);
\endcode


\note The \c Ctc prefix indicates that this class is a contractor (i.e.,
it can be composed with other contractors). \c Ctc is also the name of
the generic contractor class.

\subsection strat1-comb Combining contractors

We are now ready to build our first strategy. We create all the contractors
and push them in a vector \c ctc (this vector will be necessary for the composition):

\snippet slam/slam1.cpp ctc-vec

Now, we can create the composition of all these contractors using \c CtcCompo
(the vector \c ctc being given in argument) and a fixpoint of the latter using \c CtcFixPoint. This gives:

\snippet slam/slam1.cpp ctc-fix

We are done. We just have to call the top-level contractor on the initial box.

\snippet slam/slam1.cpp ctc-contract

\subsection strat1-res Result

The execution shows that the final box contains the real trajectory.

\verbatim
  initial box =([0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10])

  final box =([8.592079632938807, 9.009246227143752] ; [0.4364101205434934, 0.8936036705218675] ; ... )
\endverbatim

The real positions are:

<center>
  x[0]=8.806965820867086 y[0]=0.6934996231894474
  x[1]=8.240950936914649 y[1]=1.517894644489497
  x[2]=8.553965973529273 y[2]=0.5681464742605957
  ...
</center>

\section strat2 Second strategy (with outliers)

We consider now that at most \c NB\_OUTLIERS outliers may occur for
each time step.

To contract rigorously despite of outliers, we must use the "q-intersection"
operator that basically consider all possible combinations of \c (N-NB\_OUTLIERS)
contractors among \c N:

Ibex provides the \c CtcQInter contractor. 

\subsection strat2-qinter Q-intersection

We must only place all the contractors
related to the same time step in another temporary vector (called \c cdist) 
and give this vector in argument of \c CtcQInter:

Let us see what happens if we do this.

Let us replace:
\code
if (t<T-1) {
      // Push the contractor corresponding to
      // the speed measurement at time t
      NumConstraint* c=new NumConstraint(x,x[t+1]-x[t]=v[t]);
      ctc.push_back(new CtcFwdBwd(*c));
    }
\endcode

by:

\snippet slam/slam2_tmp.cpp qinter

<b>Problem</b>: the program runs almost endlessly! ... Why?

... because the q-intersection
runs exponentially in the dimension of the input box, which is 2T.
  
Of course, the implementation should take advantage of the fact
that only 2 variables are actually impacted. But the current code
is not optimized in this way. 

Anyway, it is often necessary to apply a contractor strategy to
only a subset of variables (here, to the two components of \f$x_t\f$).
  
For this end, we will make use of the <b>inverse contractor</b>.

\subsection strat2-inverse Projection using the inverse contractor

Applying the q-intersection on the subset of variables x_t amounts to apply
the inverse of this contractor by the projection function x&rarr; x[t].

We replace:
\snippet slam/slam2_tmp.cpp qinter

By:

\snippet slam/slam2.cpp inv

And now, the program terminates instantaneously.
With \c NB_OUTLIERS set to 1, the dispaly shows a slightly larger box:

\verbatim
initial box =([0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10])

final box =([8.542599451371126, 9.032225305125761] ; [0.3807126686643456, 1.002241041162326] ; ...)
\endverbatim

\section strat3 Third strategy (how can this scale?)

The program we have proposed so far does not really scale. For example, setting T=200000 in \c data.cpp will make
the program run for a long time and crash after a memory overflow.  
We see now a more efficient variant. This variant, however, will be less concise. We will also partially lose the
elegancy of contractor programming. In particular, we will do ourselves the loop that compose the contractors
as time increases. But, after all, a programming language is always a compromise between efficiency and elegancy
so if you really look for efficiency, you should accept to sacrify a little bit of elegancy.


Let us first explain why the current program does not scale. In the program, we build a \c NumConstraint object for most of the contractors and each of these \c NumConstraint objects builds (silently) a \c Function object. 
For instance, by writing:
\code
   NumConstraint* c=new NumConstraint(xt,dist(xt,beacons[b])=d[t][b]);
\endcode

The following function is created somewhere
<center>
x &rarr; dist(xt,beacons[b])-d[t][b]
</center>

Now, you must be aware that the construction of \c Function objects is both time and memory consuming. The good point however is that, once built, these objects are fast to use (evaluation, gradient, etc.).

The \c CtcQInter objects are also costly because each contains a set of N references.

It is clear in our context that the we keep on creating the same contractors again and again, with only one of the parameters changing with time (in our previous example, d[t][b]). The key idea is to factorize all these contractors and create a ``parametrized'' contractor where the time is set dynamically. Let us start with the detection constraints.

\subsection strat3-detect Detection contractor

The following class declares a contractor for the detection of a given beacon. This is a handcrafted contractor so we need to create a new class that extends \c Ctc and implements the \c contract() function. Time (contrary to the beacon number) is not set at construction so that one instance of this contractor can be used
for any time:

\snippet slam3.cpp detection

\subsection strat3-speed Speed contractor

We do the same with the second set of time-dependant constraints, namely the "speed" or "delta" constraints between two consecutive time steps.

\snippet slam3.cpp speed

\subsection strat3-scan Scan contractor (q-intersection)

Again, we create a parametrized contractor for the q-intersection of the N detections occuring at a given time.
This set of measurements somehow forms a scanning of the environment so we name this contractor \c Scan:

\snippet slam3.cpp scan

\subsection strat3-traj Trajectory

We can create now the final contractor that calls 2T times an instance of the \c Scan and \c Speed contractors (there is only one instance of each). A call to \c set_time() precedes every call to \c contract(). Note that a fix-point would not be reasonable here.

\snippet slam3.cpp traj

*/
