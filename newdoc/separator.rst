**************************************************
             Sets and Separators
**************************************************

.. _set:

===============================
Sets
===============================

Ibex provides a structure for representing sets of :math:`\mathbb{R}^n` with boxes and performing operations on sets diretly.
Another possible name for such a set would be a *paving*.

**Note:** This part of the library is recent and under active development. It will be enriched with new functionalities
in future releases.

The structure is organized as a binary tree. Each leaf represents a box with a *status* that indicates whether
the box is inside the set, outside the set or potentially crossing the boundary of the set.
The status is a boolean interval (``BoolInterval``) which possible values are either ``YES`` (*inside*), ``NO`` (*outside*)
or ``MAYBE`` (*boundary*). This is depicted in the figure below:

.. figure:: set.png
   :width: 450 px
   :align: center
   
   
A set is an instance of ``Set`` and can either be initially defined as :math:`\mathbb{R}^n` itself or a specific box:

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-init-C
   :end-before: set-init-C


------------------------------
File operations
------------------------------

You can save a set into a file and load a set from a file.

To save into a file named "set-example":

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-save-C
   :end-before: set-save-C
   
To load a set from a file, use the constructor with string argument:

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-load-C
   :end-before: set-load-C

.. _set-explore:

------------------------------
Exploring and plotting sets
------------------------------

The internal structure of sets is not intended to be handled directly by users (and may actually change with time).
If you want to explore a set, you can use the ``SetVisitor`` class (following the
`visitor design pattern`_) . This class must implement a ``visit_leaf``
function that will be automatically called for every leaf of the set (it can also optionnaly implement a ``visit_node``
function that is called for every intermediate node in the tree).

.. _visitor design pattern : http://en.wikipedia.org/wiki/Visitor_pattern

A typical usage of a set visitor is for listing or plotting the set and we shall illustrate the mechanism for such usage.

The ``visit_leaf`` function  must take in argument the information that characterizes a leaf, namely, a box (``IntervalVector``) and a status
(``BoolInterval``). Here is an example that simply dispaly the box and the status of a leaf in the standard output:

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-visit-1-C
   :end-before: set-visit-1-C
   
Now the following code load a set from a file an list all the boxes inside:


.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-visit-2-C
   :end-before: set-visit-2-C
   
The result is:


+----------------------------------------------------------+--------------------------------------+
| .. literalinclude:: ../examples/doc-separator.txt        | .. figure:: set-visit.png            |
|   :start-after:  set-visit-2-O                           |    :width: 300 px                    |
|   :end-before:   set-visit-2-O                           |    :align: center                    |
+----------------------------------------------------------+--------------------------------------+
+  **Visiting the set with** ``ToPlot``                    | **Visiting the set with** ``ToVibes``|
+----------------------------------------------------------+--------------------------------------+

We will now show how to plot a set calculated by Ibex with :ref:`Vibes <lab_vibes>`.

We create a new class that implements the ``visit_leaf`` function as follows:

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-visit-3-C
   :end-before: set-visit-3-C
   
The main code is similar:

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-visit-4-C
   :end-before: set-visit-4-C
   
And the result is the picture above.

------------------------------
Intersection
------------------------------

A set can be intersected with another set that can either be explicit (of type ``Set``) or implicit.
A separator is an example of implicit set. In this case, the separator is recursively applied on
the set until some precision is reached (size of boundary boxes).

Let us first see intersection with a separator.
In the next example, we intersect the set [0,1]x[0,1] with a separator corresponding to the set
:math:`\|x\|\le 5`.

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-sep-C
   :end-before: set-sep-C

The result is the following picture.

.. figure:: set-sep.png
   :width: 300 px
   :align: center

Now, the intersection between two sets is obtained with the ``&=`` operator.
In the next example, we create two sets from separators, one being a circle or radius 5, 
the other being the complementary of a circle of radius 4. The result of the intersection gives a ring.


.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-inter-C
   :end-before: set-inter-C

The result is the following picture.

.. figure:: set-inter.png
   :width: 300 px
   :align: center

**Note:** It should be emphasized that it is always better to handle sets explicitly on last resort.
This means that, in this example, it would have been more efficient to create one set from the intersection
of the two separators.
   
------------------------------
Union
------------------------------
The union works similarly. In the next example, we create two sets, one being a circle centered on the origin
and the other the circle centered on the point (5,0).
Then we perform the union of the two sets.

.. literalinclude:: ../examples/doc-separator.cpp    
   :language: cpp
   :start-after: set-union-C
   :end-before: set-union-C

The result is the following picture.

.. figure:: set-union.png
   :width: 300 px
   :align: center


===============================
Separators
===============================

------------------------------
Introduction
------------------------------

A separator is an operator that performs two independent and
complementary contractions. The separator is associated with
a set (noted :math:`\mathcal{S}`) and the first contraction (called "inner")
removes points inside :math:`\mathcal{S}`. The second contraction (called
"outer") removes points outside S. See :ref:`[Jaulin & Desrochers 2014] <Jaulin14>`.

In concrete terms, given a box :math:`[\mathbf{x}]`, the separator produces two sub-boxes
:math:`[\mathbf{x}_{in}]` and :math:`[\mathbf{x}_{out}]` that verify:

.. math::

  \begin{array}{l}
  ([\mathbf{x}] \setminus [\mathbf{x}_{in}]) \subset S \\
  ([\mathbf{x}] \setminus [\mathbf{x}_{out}]) \cap S = \emptyset
  \end{array}

For efficiency reasons, the *separate(...)* function takes
two input-output arguments, <x_in> and <x_out>, each containing
initially a copy of the box :math:`[\mathbf{x}]`.

The first and more natural way to build a separator is to do it from a set implicitly defined by a constraint.
See :ref:`sep-ctr`.

Since a separator can be viewed a as pair of contractors, another natural way to build a separator is from two complementary contractors.
See :ref:`sep-ctc-pair`.

A separator is however not necessarily built this way.
A separator can also be built from a contractor and a predicate. In this case, the contractor is assumed to work with respect to the boundary of a set (that is, it removes both inner and outer points) and the predicate is assumed to state if a given box is either inside, outside or crossing the boundary of the same set. See :ref:`sep-boundary`.

.. _sep-ctr:

Separator from a constraint
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When the set :math:`\mathcal{S}` corresponds to an inequality f(x)<0, a separator with respect to the set :math:`\mathcal{S}` can therefore be automatically derived using forward / backward techniques for both contractions. This is what the ``SepFwdBwd`` class stands for.
The outer (resp. inner) contractor is simply a forward-backward with respect to f<0 (resp. f>=0).

.. literalinclude:: ../examples/doc-separator.cpp
   :language: cpp
   :start-after: sep-fwd-bwd-C
   :end-before:  sep-fwd-bwd-C

The result is:

.. literalinclude:: ../examples/doc-separator.txt
   :start-after:  sep-fwd-bwd-O
   :end-before:   sep-fwd-bwd-O


.. _sep-ctc-pair:

Separator from complementary contractors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If two complementary contractors :math:`\mathcal{C}_{in}` and  :math:`\mathcal{C}_{out}` 
are available, the separator can build using the ``SepCtcPair`` class.

**(to be completed)**

.. code-block:: cpp

	Ctc c_in = ...
	Ctc c_out = ...
	// Build a separator from two complementary contractors
	SepCtcPair s(c_in, c_out);
    

.. _sep-boundary:

Boundary-Based Separator
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sometimes, we only have contractors with respect to the boundary of a set :math:`\mathcal{S}`. Isolating the inner and the outer contractor is not possible, or not easy. It is still possible to build a separator in this case, providing that we can also test whether a point belongs to the set or not. 

Let us consider a contractor C w.r.t. the boundary of a set :math:`\mathcal{S}`. The main idea behind the separator is, first, to contract the input box using C and, second, to test for each box in :math:`\neg C` if it belongs to :math:`\mathcal{S}` or not.

The test is performed for a given box by picking randomly one point and calling a *predicate*. A predicate is an object of a class extending ``Pdc``. It must implements a method ``test(IntervalVector&)`` that returns a boolean interval, that is, either ``YES``, ``NO`` or ``MAYBE`` (see ``BoolInterval``). In the case of a separator, the predicate must be an operator T such that:

.. math::
	\begin{array}{cccl}
		T: & \mathbb{IR}^n & \longrightarrow & \{yes, no, maybe\}\\
		&\mathbf{[x]} & \longmapsto & 
			\left\{
				\begin{array}{ll}
					yes & \text{ if } \mathbf{[x]} \subseteq \mathcal{S} \\
					no & \text{ if } \mathbf{[x]}\cap \mathcal{S}=\emptyset \\ 
                                        maybe & \text{otherwise}
				\end{array}
			\right.
	\end{array}	

A separator is built from a contractor and a predicate using the ``SepBoundaryCtc`` class.

**Note:** the predicate is called by ``SepBoundaryCtc`` only with points (degenerated boxes) but the interface for ``Pdc`` has been made to deal with a more general situation.

The :ref:`separator for the constraint "points in polygon" <sep-polygon>` is an illustraction of this type of separator.

As an illustration of the concept, we build here a separator for an inequality using ``SepBoundaryCtc``:

.. literalinclude:: ../examples/doc-separator.cpp
   :language: cpp
   :start-after: sep-boundary-C
   :end-before:  sep-boundary-C

The result is:

.. literalinclude:: ../examples/doc-separator.txt
   :start-after:  sep-boundary-O
   :end-before:   sep-boundary-O
---------------------------------
Separator Algebra
---------------------------------

The Separator algebra is a direct extension of the set algebra. 
E.g., the intersection of two separators w.r.t :math:`\mathcal{S}_1` and :math:`\mathcal{S}_2` 
is a separator w.r.t. :math:`\mathcal{S}_1\cap\mathcal{S}_2`.

Here are the available operations and the way they are performed. 
Separators are viewed as pair of contractors denoted :math:`\mathcal{S}_i = (\mathcal{S}_i^{in}, \mathcal{S}_i^{out})`.

.. math::
	\begin{array}{cccc}
	\overline{\mathcal{S}} & = & ( \mathcal{S}^{\text{out}},\mathcal{S}^{\text{in}} ) & \text{(Negation)} \\
	\mathcal{S}_{1}\cap \mathcal{S}_{2} & = & ( \mathcal{S}_{1}^{\text{in}%
	}\cup \mathcal{S}_{2}^{\text{in}},\mathcal{S}_{1}^{\text{out}}\cap \mathcal{S%
	}_{2}^{\text{out}})  & \text{(intersection)} \\ 
	\mathcal{S}_{1}\cup \mathcal{S}_{2} & = & ( \mathcal{S}_{1}^{\text{in}%
	}\cap \mathcal{S}_{2}^{\text{in}},\mathcal{S}_{1}^{\text{out}}\cup \mathcal{S%
	}_{2}^{\text{out}})  & \text{(union)} \\ 
	\bigcap\limits^{\{q\}}\mathcal{S}_{i} & = & (
	\bigcap\limits^{\{m-q-1\}}\mathcal{S}_{i}^{\text{in}},\bigcap\limits^{\{q%
	\}}\mathcal{S}_{i}^{\text{out}})  & \text{(relaxed intersection)} \\ 
	\mathcal{S}_{1}\setminus \mathcal{S}_{2} & = & \mathcal{S}_{1}\cap 
	\overline{\mathcal{S}_{2}}. & \text{(difference)}%
	\end{array}

The following example shows how to combine separator for finding the union and intersection of 3 rings.

**(to be completed)**

.. code-block:: cpp

    // define the center of circle
    double ax[] = {3,7,-3};
    double ay[] = {4,3,7};
    double dist[] = {3,6,6};


     Variable x,y;

     // Rings definitions
     Function f1(x,y,sqrt(sqr(x-ax[0]) + sqr(y-ay[0])));
     SepFwdBwd S1(f1,dist[0]);

     Function f2(x,y,sqrt(sqr(x-ax[1]) + sqr(y-ay[1])));
     SepFwdBwd S2(f2,dist[1]);

     Function f3(x,y,sqrt(sqr(x-ax[2]) + sqr(y-ay[2])));
     SepFwdBwd S3(f3,dist[2]);

     // Negation of separator
     SepNot S4(S3);

     // union of separators
     Array<Sep> AS(S1,S2,S3);
     SepUnion SUL = SepUnion(AS); 			// Union from an array of separators
     SepUnion SU2 = SepUnion(S1,S2);		// Union from two separators
     SepUnion SU3 = SepUnion(S1,S2,S3);

     // intersection of separators
     SepInter SIL = SepInter(AS);
     SepInter SI2 = SepInter(S1,S2);
     SepInter SI3 = SepInter(S1,S2,S4);



-------------------------------
Separator for a Polygon
-------------------------------

**(under construction)**

Contractor for a Segment
~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``CtcSegment`` class allows to contract a box w.r.t. a segment (in the plane), that is, w.r.t. to the constraint

.. math::

  \mathbf{x}\in\left[\mathbf{a},\mathbf{b}\right]
 
where :math:`\mathbf{a}\in\mathbb{R}^2,\mathbf{b}\in\mathbb{R}^2`.

The contractor works by consider the following equivalent constraints :

.. math::

	\left\{ 
	\begin{array}{c}
	\det \left( \mathbf{b} - \mathbf{a}, \mathbf{a} - \mathbf{x}  \right) =0 \\
	\min \left( \mathbf{a},\mathbf{b}\right) \leq \mathbf{x}\leq \max \left( 
	\mathbf{a},\mathbf{b}\right) .%
	\end{array}%
	\right.

the *min* and the *max* being interpreted componentwise.

*Remark*: The first constraint is an equality :math:`\mathbf{f}(x) = 0` and the associated contractor will contract only on the boundary. We only have an approximation of :math:`\mathbb{X}^+` because there is no inner part.

.. _sep-polygon:

Point Inside a Polygon 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We consider an oriented polygon :math:`\mathcal{P}`, convex or not, without self interaction, composed of N segments. The boundary :math:`\partial\mathcal{P}` of the polygon satisfies the following constraint:

.. math::

	\partial\mathcal{P} = \left\{ \mathbf{m} \in \mathbb{R}^2 ,\ 
		\exists i \in [\![1,N]\!], \mathbf{m} \in \left[\mathbf{a}_i, 
		\mathbf{b}_i \right]   \right\}

Let's us take  :math:`\mathcal{C}_{a_i,b_i}` as a contractor for the segment  :math:`\left[\mathbf{a_i},\mathbf{b_i}\right]`, the contractor for  :math:`\partial\mathcal{P}` is:

.. math::

	\mathcal{C}_{\partial\mathcal{P}} = \bigcup\limits^{N}_{i=1} \mathcal{C}_{a_i,b_i}

*Remark*: Because the union of minimal contractor is minimal, :math:`\mathcal{C}_{\partial\mathcal{P}}` is a minimal contractor for the border of the polygon :math:`\mathcal{P}`.

To identify which part is inside and outside we use a test based on the *Winding Number* which represents the total number of times that curve travels counterclockwise around the point. The winding number depends on the orientation of the curve, and is negative if the curve travels around the point clockwise.
Let us take a polygon :math:`\mathcal{P}` with vertices's :math:`V_1, V_2, \dots, V_n = V_1` and :math:`\mathbf{m}` a point not on the border of P. The Winding Number is defined by:

.. math::

	\mathbf{wn}(\mathbf{m},P) = \frac{1}{2\pi}\sum_{i=1}^{n}\theta_i = \frac{1}{2\pi}\sum_{i=1}^{n} \arccos\left(\frac{(V_i - 	\mathbf{m}).(V_{i+1}-\mathbf{m})}{\| (V_i - \mathbf{m})\| \|(V_{i+1} - \mathbf{m})\|}\right)


+-------------------------------+-----------------------------------+
| *m is inside P*               |  *m is outside P*                 |
|                               |                                   |
| .. image:: T_PointInPoly.png  |  .. image:: T_PointOutPoly.png    |
|    :width: 300 px             |     :width: 300 px                |
|                               |                                   |
+-------------------------------+-----------------------------------+

So, if m is outside P we will have :math:`\mathbf{wn}(\mathbf{m},P) = 0`, otherwise if :math:`\mathbf{m}` is inside, :math:`\mathbf{wn}(\mathbf{m},P) = 1`.

The class implementing a separator for a polygon is ``SepPolygon``.

**(to be completed)**

Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Let :math:`\mathcal{S}_P` be the polygon described in figure ... 

The following snippet shows how to build the associated separator and make operations with:

**(to be completed)**

.. code-block:: cpp
	
	// Polygone convex
	vector<double> walls_xa,walls_xb,walls_ya,walls_yb;

	walls_xa.push_back(6);  walls_ya.push_back(-6);   walls_xb.push_back(7);  walls_yb.push_back(9);
	walls_xa.push_back(7);  walls_ya.push_back(9);   walls_xb.push_back(0);  walls_yb.push_back(5);
	walls_xa.push_back(0);  walls_ya.push_back(5);    walls_xb.push_back(-9); walls_yb.push_back(8);
	walls_xa.push_back(-9); walls_ya.push_back(8);    walls_xb.push_back(-8); walls_yb.push_back(-9);
	walls_xa.push_back(-8); walls_ya.push_back(-9);   walls_xb.push_back(6);  walls_yb.push_back(-6);

	SepPolygon S1(walls_xa, walls_ya, walls_xb, walls_yb);

	// Make a hole inside the first one
	vector<double> walls_xa2,walls_xb2,walls_ya2,walls_yb2;
	walls_xa2.push_back(-2); walls_ya2.push_back(3);   walls_xb2.push_back(3.5);  walls_yb2.push_back(2);
	walls_xa2.push_back(3.5); walls_ya2.push_back(2);   walls_xb2.push_back(3);  walls_yb2.push_back(-4);
	walls_xa2.push_back(3); walls_ya2.push_back(-4);   walls_xb2.push_back(-3);  walls_yb2.push_back(-3);
	walls_xa2.push_back(-3); walls_ya2.push_back(-3);   walls_xb2.push_back(-2);  walls_yb2.push_back(3);

	SepPolygon S2(walls_xa2, walls_ya2, walls_xb2, walls_yb2);
    SepNot S3(S2);

    // Separator for the polygon with a hole in it
    SepInter S(S1, S3);

Using a paver, the previous separator will produce the following figure :

.. figure:: Poly.png
   :width: 400 px
   :align: center

   *Point inside the polygon are in dark gray.*
