**************************************************
             Contractors
**************************************************

------------------------------
Introduction
------------------------------

The concept of *contractor* is directly inspired from the ubiquituous concept of *filtering algorithm* in constraint programming.

Given a constraint c relating a set of variables x, an algorithm C is called a filtering algorithm if, given a box (and more 
generally a "domain" for x, i.e., a set of potential tuples variable can be assigned to):

.. math::

   C([x])\subseteq [x] \ \wedge  \ \forall x \in [x], \ c(x) \Rightarrow x \in C([x]).

This relation means that:

#. Filtering gives a sub-domain of the input domain [x];
#. The resulting subdomain C([x]) contains all the feasible points with respect to the constraint c. No solution is "lost".

This illustrated in the next picture. The constraint c (i.e., the set of points satisfying c) is represented by a green shape.

.. figure:: filtering.png
   :width: 300 px
   :align: center

Constraint programming is the context where this concept has been formalized and intensively used. But constraint programming is by no means 
the only paradigm where algorithms complying with this definition have been developed.
The most significant example is interval analysis, where a Newton-like iteration has been developed since the 1960's :ref:`[Moore 1966] <Moore66>` that corresponds
exactly to a filtering algorithm in the case of a constraint c of the following form:

.. math::
   c(x) \iff f(x)=0

where f is any (non-linear) differentiable function from :math:`\mathbb{R}^n\to\mathbb{R}^n`. The most famous variant of this Newton iteration is probably the Hansen-Sengupta 
algorithm :ref:`[Hansen & Sengupta 1980] <Hansen80>`. 

Another important example is in picture processing, where there exist algorithms able to reduce the size of a picture to some *region of interest*.
This kind of alorithm is today implemented in almost every digital cameras, for an automatic focus adjustment.

.. figure:: ladybug.png
   :width: 400 px
   :align: center

In this example, the constraint c is: 

  c(x) :math:`\iff` x *belongs to a region of interest*

Here, it is clear that the constraint is more a consequence of the algorithm than in the other way around.
This last example suggests the next definition. 

An algorithm C is called *a contractor* if:

.. math::
  \begin{array}{ll}
  \forall [x]\in \mathbb{IR}^{n},\mathcal{C}([x])\subseteq [x] & \text{(contraction)} \\ 
  \left(x\in [x], \mathcal{C}(\{x\})= \{x\} \right) \Rightarrow x\in \mathcal{C}([x]) & \text{(consistency)} \\
  C(\{x\} )=\emptyset
  \Leftrightarrow \left( \exists \varepsilon >0,\forall [x]\subseteq
  B\left(x,\varepsilon \right) ,\mathcal{C}([x])=\emptyset
  \right)  & \text{(continuity)}
  \end{array}

- The first condition is the same as before. 
- The second condition, the one related to the underlying constraint, has been dropped.
  In fact, the constraint c has been replaced by the set of *insensitive* points, those satisfying :math:`C(\{x\})=\{x\}`. 
  So the constraint still exists, but implicitely. With this in mind, it is clear now the second condition here 
  states again that "no solution is lost".
- The last condition is important for some convergence issues only.

Withdrawing the link to the constraint from the definition forces one to view the contractor as a pure function:

.. math::
   C: \mathbb{IR}^n \to \mathbb{IR}^n,

where :math:`\mathbb{IR}` denotes the set of real intervals. "Pure" means that the execution of the contractor does not depend on
a context and does not produce side effects.
In the former definition of *filtering algorithm*, the operator was depending on constraints and, in practice, constraints are external objects sharing
some structures representing domains of variables. This means that the execution was depending on the context and producing side effects.
This centralized architecture is often met in discrete constraint programming. It allows the implementation of many code optimization techniques,
but at a price of a huge programming complexity. 

In contrast, *contractor programming* is so simple that anyone can build a solver in a few lines.
Here is the interface for contractors. As one can see, it could not be more minimalist:

.. code-block:: cpp

   class Ctc {
     public:

     // Build a contractor on nb_var variables.
     Ctc(int nb_var);

     // Performs contraction. 
     // This is the only function that must be implemented in a subclass of Ctc.
     // The box in argument is contracted in-place (in-out argument).
     virtual void contract(IntervalVector& box)=0;

   };

That's all. Another advantage of removing the constraint from the definition is that it makes natural the cooperation of heterogenous contractors (would they be linked internally to a numerical constraint, a picture processing algorithm, a combinatorial problem, etc.).

The good news is that some important constraint programming techniques like :ref:`propagation <ctc-propag>`, :ref:`shaving <ctc-shaving>` or :ref:`constructive disjunction <ctc-acid>` can actually be 
generalized to contractors. They don't intrinsically need the concept of constraint.

These operators all take a set of contractors as input and produce a new (more sophisticated) contractor. 
The design of a solver simply amouts to the composition of such operators. All these operators form a little functionnal language, where contractors are first-class citizens.
This is what is called contractor programming :ref:`[Chabert & Jaulin 2009] <Chabert09a>`.

We present in this chapter the basic or "numerical" contractors (built from a constraint, etc.) and the operators.

.. _ctc-fwd-bwd:

------------------------------
Forward-Backward
------------------------------

Forward-backward (also known as HC4Revise) is a classical algorithm in constraint programming for 
contracting quickly with respect to an equality or inequality. See, e.g.,
:ref:`[Benhamou & Granvilliers 2006] <Benhamou06>`, :ref:`[Benhamou et al. 1999] <Benhamou99>`,
:ref:`[Collavizza 1998] <Collavizza98>`. However, the more occurrences of variables in the expression
of the (in)equality, the less accurate the contraction.
Hence, this contractor is often used as an "atomic" contractor embedded in an higher-level operator like :ref:`ctc-propag` or :ref:`ctc-shaving`.

 
The algorithm works in two steps. The **forward step** apply :ref:`itv-arith` to each operator of the :ref:`function <mod-func>` expression, from
the leaves of the expression (variable domains) upto the root node. 

This is illustrated in the next picture with the constraint :math:`(x-y)^2-z=0` with :math:`x\in[0,10], \ y\in[0,4]` and :math:`z\in[9,16]`:

.. figure:: fwd.png
   :width: 400 px
   :align: center
   
   *Forward step*

The **backward step** sets the interval associated to the root node to [0,0] (imposes constraint satisfaction) and, then, apply :ref:`itv-bwd-arith` from the root downto the leaves:

.. figure:: bwd.png
   :width: 400 px
   :align: center
 
   *Backward step*

This contractor can either be built with a :ref:`NumConstraint <mod-sys-ctrs>` object or directly with a function f. In the latter case, the constraint f=0 is implicitely considered.


See **examples in the** :ref:`tutorial <tuto-fwd-bwd>`.


.. _ctc-propag:

------------------------------
Propagation
------------------------------

Propagation :ref:`[Bessiere 2006] <Bessiere06>` is another classical algorithm of constraint programming.

The basic idea is to calculate the fixpoint of a set of n contractors :math:`C_1\ldots,C_n`, that is:

.. math::
   (C_1\circ\ldots\circ C_n)^\infty

without calling a contractor when it is unecessary (as it is explained in the :ref:`tutorial <tuto-propag>`).

Let us first introduce for a contractor C two sets of indices: the *input* and *output* dimensions of C:

.. math::

  \exists [x]\in\mathbb{IR}^n,\ [y]\in\mathbb{IR}^n \ (\forall j\!\neq\!i \ [x]_j\!=\![y]_j) \wedge C([x])\!\neq\!C([y]) \Longrightarrow i\in input(C);

  \exists [x]\in\mathbb{IR}^n,\ C([x])_i\neq [x]_i \Longrightarrow i\in output(C).

Basically, *input(C)* contains the variables that **potentially impacts** the result of the contractor while *ouput(C)*
contains the variables that are **potentially impacted by** the contractor.

We will explain further how this information is set in Ibex.

The propagation works as follows. It creates and maintain a set of *active* contractors :math:`\mathcal{A}` (called "agenda").
The agenda is initialized to the full set:

.. math::

   \mathcal{A}:=\{C_1,\ldots,C_n\};

And the algorithm is:

#. Pop a contractor C from :math:`\mathcal{A}`
#. Perform contraction: :math:`[x]:=C([x])`. 
#. If the contraction was effective, push into  :math:`\mathcal{A}` all the contractors C' such that :math:`input(C')\cap output(C)\neq\emptyset`
#. Return to step 1 until  :math:`\mathcal{A}=\emptyset`.

**Note:** The algorithm could be improved by not pushing again in the agenda a contractor C that is idempotent (*under development*).

To illustrate how the propagation can be used and the benefit it provides, we compare in the next example the number of times contractors
(that we chose to be forward-backward) are called in a simple fixpoint loop with the number obtained in a propagation loop.

To this aim, we need to increment a counter each time a forward-backward is called. The easiest way to do this is simply to create a
subclass of ``CtcFwdBwd`` that just call the parent contraction function and increments a global counter (static variable named ``count``).

Here is the class:


.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-propag-1-C
   :end-before:  ctc-propag-1-C

Now, we load a set of constraints from a file. We chose on purpose a large but very sparse problem (makes propagation advantageous) and
create our "counting" contractor for each constraint:

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-propag-2-C
   :end-before:  ctc-propag-2-C


^^^^^^^^^^^^^^^^^^^^^^^^^^
Fixpoint ratio
^^^^^^^^^^^^^^^^^^^^^^^^^^

The two contractors (``CtFixPoint`` and ``CtcPropag``) take also as argument a "fixpoint ratio". 
The principle is that if a contraction does not remove more that

   (ratio :math:`\times` the diameter of a variable domain), 

then this reduction is not propagated. The default value is 0.01 in the case of propagation, 0.1 in the case of fixpoint.

**Warning:** In theory (and sometimes in practice), the fixpoint ratio gives no information on the "distance"
between the real fixpoint and the one we calculate.

Here we fix the ratio to 1e-03 for both:

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-propag-3-C
   :end-before:  ctc-propag-3-C

We can finally build the two strategies, call them on the same box and observe the number of calls.
We also check that the final boxes are equal, up to the precision required.

With a fix point:

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-propag-4-C
   :end-before:  ctc-propag-4-C

With a propagation:

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-propag-5-C
   :end-before:  ctc-propag-5-C

The display is:

.. literalinclude:: ../examples/doc-contractor.txt
   :language: cpp
   :start-after:  ctc-propag-O
   :end-before:   ctc-propag-O

^^^^^^^^^^^^^^^^^^^^^^^^^^
Input and Output variables
^^^^^^^^^^^^^^^^^^^^^^^^^^

The input variables (the ones that potentially impacts the contractor) and the
output variables (the ones that are potentially impacted) are two lists of variables stored under the form of "bitsets".

A biset is nothing but an (efficiently structured) list of integers. 

These bitsets are the two following fields of the``Ctc`` class:

.. code-block:: cpp

  	/**
	 * The input variables (NULL pointer means "unspecified")
	 */
	BitSet* input;

	/**
	 * The output variables NULL pointer means "unspecified")
	 */
	BitSet* output;

These fields are not built by default. One reason is for allowing the distinction between an empty bitset and *no bitset* (information not provided).
The other is that, in some applications, the number of variables is too large so that one prefers not to build these data structures even if they
are very compacted.

To show the usage of these bitsets and their impact on propagation, we consider the same example as before.
Let us now force the input/output bitsets of each contractors to contain every variable:

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-input-output-C
   :end-before:  ctc-input-output-C

We observe now that the fixpoint with ``CtcPropag`` is reached with as many iterations as without ``CtcPropag``:

.. literalinclude:: ../examples/doc-contractor.txt
   :start-after:  ctc-input-output-O
   :end-before:   ctc-input-output-O

If you build a contractor from scratch (not inheriting a built-in contractor like we have just done), don't forget to
create the bitsets before using them with ``add/remove``. 

Here is a final example. Imagine that we have implemented a contraction algorithm for the following constraint (over 100 variables):

.. math::

   \forall i, 0\le i \le 49, \quad  x[2\times i]>0 \Longrightarrow x[2\times i+1]=0.
	
The the input (resp. output) variables is the set of even (resp. odd) numbers. Here is how the initialization could be done:

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-input-output-2-C
   :end-before:  ctc-input-output-2-C


^^^^^^^^^^^^^^^^^^^^^^^^^^
The ``accumulate`` flag
^^^^^^^^^^^^^^^^^^^^^^^^^^

The accumate flag is a subtle tuning parameter you may ignore on first reading.

As you know now, one annoyance with continuous variables is that we have to stop the fixpoint before it is actually reached,
which means that unsignificant contractions are not propagated.

Now, to measure the significance of a contractor, we look at the intervals after contraction and compare them to
the intervals just before contraction. 

One little risk with this strategy is when a lot of unsignificant contractions gradually contracts domains to a point
where the overall contraction has become significant. The propagation algorithm may stop despite of this significant contraction.

The ``accumulate`` flag avoids this by comparing not with the intervals just before the current contraction, but the intervals
obtained after the last significant one. The drawback, however, is that all the unsignificant contractions are cumulated
and attributed to the current contraction, whence a little loss of efficiency.

To set the accumulate flag, just write:

.. code-block:: cpp

   CtcPropag propag(...);
   propag.accumulate = true;

Often, in practice, setting the ``accumulate`` flag results in a sligthly better contraction with a little more time.

.. _ctc-hc4:

------------------------------
HC4
------------------------------

HC4 is the classical "constraint propagation" loop that we found in the literature.
It allows to contract with respect to a :ref:`system <mod-sys>` of constraints.

In Ibex, the ``CtcHC4`` contractor is simply a direct specialization of ``CtcPropag`` (the :ref:`propagation contractor <ctc-propag>`).

The contractors that are propated are nothing but the default (:ref:`ctc-fwd-bwd`) contractors associated to every constraint of the system.

Here is an example:

.. literalinclude:: ../examples/doc-contractor.cpp 
   :language: cpp
   :start-after: ctc-hc4-C
   :end-before: ctc-hc4-C

And the result:

.. literalinclude:: ../examples/doc-contractor.txt 
   :start-after: ctc-hc4-O
   :end-before: ctc-hc4-O

.. _ctc-inverse:

------------------------------
Inverse contractor
------------------------------
Given 

- a function :math:`f:\mathbb{R}^n\to\mathbb{R}^m`
- a contractor :math:`C:\mathbb{IR}^m\to\mathbb{IR}^m`

The **inverse** of C by f is a contractor from :math:`\mathbb{IR}^n\to\mathbb{IR}^n` denoted by :math:`f^{-1}(C)` that satisfies: 

.. math::

  \forall [x]\in\mathbb{IR}^n, \quad \Bigl(f^{-1}(C)\Bigr)[x] \supseteq \{ x \in[x], \exists y\in C(f([x])) \}.

To illustrate this we shall consider the function

.. math::

   t\mapsto (\cos(t),\sin(t))

and a contractor with respect to the constraint

.. math::
  
   x\ge0, y\ge 0  

.. literalinclude:: ../examples/doc-contractor.cpp
   :language: cpp
   :start-after: ctc-inv-C
   :end-before:  ctc-inv-C

This gives:

.. literalinclude:: ../examples/doc-contractor.txt
   :language: cpp
   :start-after: ctc-inv-O
   :end-before:  ctc-inv-O


.. _ctc-shaving:

------------------------------
Shaving
------------------------------

The shaving operator consists in calling a contractor C onto sub-parts ("slices") of the input box. If a slice is entirely eliminated by C,
the input box can be contracted by removing the slice from the box.

This operator can be viewed as a generalization of the SAC algorithm in discrete domains :ref:`[Bessiere and Debruyne 2004] <Bessiere04>`.
The concept with continuous constraint was first introduced in :ref:`[Lhomme 1993] <Lhomme93>` with the "3B" algorithm. In this paper, the 
sub-contractor C was :ref:`ctc-HC4`.


**(to be completed)**


+-------------------------+--------------------------+-------------------------+--------------------------+
| .. image:: shaving0.png |  .. image:: shaving1.png | .. image:: shaving2.png |  .. image:: shaving3.png |
|                         |                          |                         |                          |
|initial box              |                          |                         |                          |
+-------------------------+--------------------------+-------------------------+--------------------------+
| .. image:: shaving4.png | .. image:: shaving5.png  | .. image:: shaving6.png | .. image:: shaving7.png  |
+-------------------------+--------------------------+-------------------------+--------------------------+

.. image:: shaving8.png
   :align: center

.. _ctc-acid:

------------------------------ 
Acid & 3BCid
------------------------------

*(to be completed)*

.. _ctc-polytope-hull:

------------------------------
Polytope Hull
------------------------------

*(to be completed)*

.. _ctc-linear-relax:

------------------------------
Linear Relaxations
------------------------------

*(to be completed)*


.. _ctc-xnewton:

------------------------------
X-Newton
------------------------------


*(to be completed)*

------------------------------
Exists and ForAll
------------------------------

*(to be completed)*

