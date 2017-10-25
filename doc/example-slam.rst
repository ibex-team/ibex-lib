
=======================================
A complete Example: SLAM with outliers
=======================================

.. _slam-intro:

---------------
Introduction
---------------

The goal of this example is to implement a simple contractor strategy 
for a SLAM problem with the IBEX library. SLAM means *simultaneous localization and map building*
and is a classical problem in mobile robotics.

We will see that contractor programmming with Ibex basically amounts to:

- enter your mathematical model using :ref:`mod-func` and :ref:`mod-sys-ctrs`;
- build basic contractors (``CtcFwdBwd`` in general) with respect to the equations;
- apply operators to these contractors to yield new (more sophisticated) contractors.

The code we build here will eventually involve 5 different contrators:

- ``CtcFwdBwd``
- ``CtcCompo``
- ``CtcFixPoint``
- ``CtcQInter`` 
- ``CtcInverse``.


We shall implement a strategy that is similar to a predictor-corrector approach (like the Kalman filter for instance)
in the sense that we also use odometry and observation to reduce the uncertainty on the robot's position.
However, both information as considered on the same footing and there is no distinction such as *prediction* versus *correction*.
They are just contractors that can be used in any order and we will even calculate a fixpoint of them (so the strategy
is not a *recursive* filter).

**note** For the sake of simplicity, we shall always use dynamic *allocation*:

.. code-block:: cpp

   MyClass* x = new MyClass(...)

just to avoid potential memory fault when pointing to temporary objects.
Of course, all these objects should be disallocated afterwards.

.. _slam-intro-download:

^^^^^^^^^^^^^^^^^^^^
Download
^^^^^^^^^^^^^^^^^^^^

The full code can be found under ``.../examples/slam`` (this subfolder is included in the Ibex package).

.. _slam-intro-prob-desc:

^^^^^^^^^^^^^^^^^^^^
Problem description
^^^^^^^^^^^^^^^^^^^^

The goal is to charaterize the trajectory of an autonomous robot by enclosing in a box its position x[t]
for each time step t=0...T.

.. image:: images/loc.png
   :width: 400 px

We have no direct information on its position (including the initial one) but the robot measures 
at each time step:

- its distance from a set of N fixed beacons (:math:`\rightarrow` N measurements) as if it was equipped with a telemeter;
- its "speed" (delta) vector v[t]=x[t+1]-x[t].


Each measurement is subject to uncertainty: *distances and speed vector*
but also the position of the beacons, that is supposed to be measured a priori.

Furthermore, we shall consider outliers.

First of all, let us assume that the measurements are all simulated in a seperate unit. 
The header file of this unit contains:

.. literalinclude:: ../examples/slam/data.h
   :language: cpp
   :start-after: [data]
   :end-before: [data]

.. _slam-strat1:

------------------------------
First strategy (no outlier)
------------------------------

First, we consider no outlier. A simple strategy consists in :

- creating a contractor for each measurement,
- calling all these contractors in sequence (composition)
- performing a fixpoint loop


Let us start by creating contractors for measurements, 
that is, those related to equations.

.. _slam-strat1-eq:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Entering equations and functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A measurement is an equation. 

To enter an equation in Ibex, we use the ``NumConstraint`` class (see :ref:`mod-sys-ctrs`).
A ``NumConstraint`` object contains a mathematical condition, or "constraint".

To define a constraint mathematically, we must specify how many variables it relates
and in which order these variables must be taken.

That is why we need to create first some ``Variable`` objects.
But keep in mind that these objects are just a C++ trick for the only purpose of
declaring a constraint.

Once declared, a constraint is self-contained and depends on nothing else.

*Example:* For creating the equations: 

.. math::

   \forall t<T, \ x[t+1]-x[t]=v[t]

The corresponding code in Ibex is:


.. code-block:: cpp

   Variable x(T,2);  // create a Tx2 variable

   for (int t=0; t<T; t++) {
     if (t<T-1) {
       NumConstraint* c=new NumConstraint(x,x[t+1]-x[t]=v[t]);
       ...
     } 
   }

**note** Here, ``v`` is not a variable but a constant (see ``data.h``).

Sometimes, different constraints are based on the same pattern. It is then often convenient
to declare first a ``Function`` object.

*Example:* For distance constraints, we may first declare the distance function:

.. literalinclude:: ../examples/slam/slam1.cpp
   :language: cpp
   :start-after: dist-decl
   :end-before: dist-decl


and then the equation for each time step and each beacon:



.. code-block:: cpp
   
   for (int t=0; t<T; t++) {
     for (int i=0; i<N; i++) {
     NumConstraint* c=new NumConstraint(
             x,dist(x[t],beacons[i])=d[t][i]);
     ...
     }
   }

.. _slam-strat1-basic-ctc:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Creating basic contractors
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We can create now contractors.

To create a contractor with respect to an equation we 
use the ``CtcFwdBwd`` class (see :ref:`ctc-fwd-bwd`).

*Example:* With the constraint x=1:

.. code-block:: cpp
   
   Variable x;
   NumConstraint* c=new NumConstraint(x,x=1);
   Ctc* ctc=new CtcFwdBwd(*c);

**Note:** The ``Ctc`` prefix indicates that this class is a contractor (i.e.,
it can be composed with other contractors). ``Ctc`` is also the name of
the generic contractor class.

.. _slam-strat1-comb:

^^^^^^^^^^^^^^^^^^^^^^
Combining contractors
^^^^^^^^^^^^^^^^^^^^^^

We are now ready to build our first strategy. We create all the contractors
and push them in a vector ``ctc`` (this vector will be necessary for the composition):

.. literalinclude:: ../examples/slam/slam1.cpp
   :language: cpp
   :start-after: ctc-vec
   :end-before: ctc-vec

Now, we can create the composition of all these contractors using ``CtcCompo``
(the vector ``ctc`` being given in argument) and a fixpoint of the latter using ``CtcFixPoint``. This gives:

.. literalinclude:: ../examples/slam/slam1.cpp
   :language: cpp
   :start-after: ctc-fix
   :end-before: ctc-fix

We are done. We just have to call the top-level contractor on the initial box.

.. literalinclude:: ../examples/slam/slam1.cpp
   :language: cpp
   :start-after: ctc-contract
   :end-before: ctc-contract

.. _slam-strat1-res:

^^^^^^^^^^^^^^^^^^^^
Result
^^^^^^^^^^^^^^^^^^^^

The execution shows that the final box contains the real trajectory::

  initial box =([0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10])

  final box =([8.592079632938807, 9.009246227143752] ; [0.4364101205434934, 0.8936036705218675] ; ... )

The real positions are:

.. math::
   \begin{array}{lclc}  
   x_0= & 8.806965820867086 & y_0= & 0.6934996231894474\\
   x_1= & 8.240950936914649 & y_1= & 1.517894644489497\\
   x_2= & 8.553965973529273 & y_2= & 0.5681464742605957\\
   & \vdots & & \vdots
   \end{array}

.. _slam-strat2:

--------------------------------
Second strategy (with outliers)
--------------------------------

We consider now that at most NB_OUTLIERS outliers may occur for
each time step.

To contract rigorously despite of outliers, we must use the "q-intersection"
operator that basically consider all possible combinations of (N-NB_OUTLIERS)
contractors among N:

Ibex provides the ``CtcQInter`` contractor. 

.. _slam-strat2-qinter:

^^^^^^^^^^^^^^^^^^^^
Q-intersection
^^^^^^^^^^^^^^^^^^^^

We must only place all the contractors
related to the same time step in another temporary vector (called ``cdist)`` 
and give this vector in argument of ``CtcQInter:``

Let us see what happens if we do this.

Let us replace:

.. code-block:: cpp

   if (t<T-1) {
      // Push the contractor corresponding to
      // the speed measurement at time t
      NumConstraint* c=new NumConstraint(x,x[t+1]-x[t]=v[t]);
      ctc.push_back(new CtcFwdBwd(*c));
   }

by:


.. literalinclude:: ../examples/slam/slam2_tmp.cpp
   :language: cpp
   :start-after: qinter
   :end-before: qinter

**Problem**: the program runs almost endlessly! ... Why?

... because the q-intersection
runs exponentially in the dimension of the input box, which is 2T.
  
Of course, the implementation should take advantage of the fact
that only 2 variables are actually impacted. But the current code
is not optimized in this way. 

Anyway, it is often necessary to apply a contractor strategy to
only a subset of variables (here, to the two components of x[t]).
  
For this end, we will make use of the :ref:`ctc-inverse`.

.. _slam-strat2-inverse:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Projection using the inverse contractor
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Applying the q-intersection on the subset of variables x_t amounts to apply
the inverse of this contractor by the projection function :math:`x\mapsto x[t]`.

We replace:

.. literalinclude:: ../examples/slam/slam2_tmp.cpp
   :language: cpp
   :start-after: qinter
   :end-before: qinter

By:

.. literalinclude:: ../examples/slam/slam2.cpp
   :language: cpp
   :start-after: inv
   :end-before: inv

And now, the program terminates instantaneously.
With ``NB_OUTLIERS`` set to 1, the dispaly shows a slightly larger box::

  initial box =([0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10] ; [0, 10])

  final box =([8.542599451371126, 9.032225305125761] ; [0.3807126686643456, 1.002241041162326] ; ...)

.. _slam-strat3:

---------------------------------------
Third strategy (how can this scale?)
---------------------------------------

The program we have proposed so far does not really scale. For example, setting T=200000 in ``data.cpp`` will make
the program run for a long time and crash after a memory overflow.  
We see now a more efficient variant. This variant, however, will be less concise. We will also partially lose the
elegancy of contractor programming. In particular, we will do ourselves the loop that compose the contractors
as time increases. But, after all, a programming language is always a compromise between efficiency and elegancy
so if you really look for efficiency, you should accept to sacrify a little bit of elegancy.


Let us first explain why the current program does not scale. In the program, we build a ``NumConstraint`` object for most of the contractors and each of these ``NumConstraint`` objects builds (silently) a ``Function`` object. 
For instance, by writing:

.. code-block:: cpp

   NumConstraint* c=new NumConstraint(xt,dist(xt,beacons[b])=d[t][b]);

The following function is created somewhere

.. math::
   x \mapsto dist(x[t],beacons[b])-d[t][b]

Now, you must be aware that the construction of ``Function`` objects is both time and memory consuming. The good point however is that, once built, these objects are fast to use (evaluation, gradient, etc.).

The ``CtcQInter`` objects are also costly because each contains a set of N references.

It is clear in our context that the we keep on creating the same contractors again and again, with only one of the parameters changing with time (in our previous example, d[t][b]). The key idea is to factorize all these contractors and create a "parametrized" contractor where the time is set dynamically. Let us start with the detection constraints.

.. _slam-strat3-detect:

^^^^^^^^^^^^^^^^^^^^^
Detection contractor
^^^^^^^^^^^^^^^^^^^^^

The following class declares a contractor for the detection of a given beacon. This is a handcrafted contractor so we need to create a new class that extends ``Ctc`` and implements the ``contract()`` function. Time (contrary to the beacon number) is not set at construction so that one instance of this contractor can be used
for any time:

.. literalinclude:: ../examples/slam/slam3.cpp
   :language: cpp
   :start-after: [detection]
   :end-before: [detection]

.. _slam-strat3-speed:

^^^^^^^^^^^^^^^^^^^^
Speed contractor
^^^^^^^^^^^^^^^^^^^^

We do the same with the second set of time-dependant constraints, namely the "speed" or "delta" constraints between two consecutive time steps.

.. literalinclude:: ../examples/slam/slam3.cpp
   :language: cpp
   :start-after: [speed]
   :end-before: [speed]

.. _slam-strat3-scan:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Scan contractor (q-intersection)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Again, we create a parametrized contractor for the q-intersection of the N detections occuring at a given time.
This set of measurements somehow forms a scanning of the environment so we name this contractor ``Scan:``

.. literalinclude:: ../examples/slam/slam3.cpp
   :language: cpp
   :start-after: [scan]
   :end-before: [scan]

.. _slam-strat3-traj:

^^^^^^^^^^^^^^^^^^^^
Trajectory
^^^^^^^^^^^^^^^^^^^^

We can create now the final contractor that calls 2T times an instance of the ``Scan`` and ``Speed`` contractors (there is only one instance of each). A call to ``set_time()`` precedes every call to ``contract()``. Note that a fix-point would not be reasonable here.

.. literalinclude:: ../examples/slam/slam3.cpp
   :language: cpp
   :start-after: [traj]
   :end-before: [traj]

