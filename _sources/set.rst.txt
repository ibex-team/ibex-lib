.. _set:

**************************************************
             Sets
**************************************************

**Note:** This part of the library is recent and under active development. It will be enriched with new functionalities
in future releases.

------------------------------
Introduction
------------------------------

Ibex provides a structure for representing sets of :math:`\mathbb{R}^n` with boxes and performing operations on sets diretly.
Another possible name for such a set would be a *paving*.

The structure is organized as a binary tree. Each leaf represents a box with a *status* that indicates whether
the box is inside the set, outside the set or potentially crossing the boundary of the set.
The status is a boolean interval (``BoolInterval``) which possible values are either ``YES`` (*inside*), ``NO`` (*outside*)
or ``MAYBE`` (*boundary*). This is depicted in the figure below:

.. figure:: images/set.png
   :width: 450 px
   :align: center
   
   **Example of "set"**, obtained from the constraint :math:`\|(x,y)\|\le5`.


.. _lab_vibes:

---------------------------
A graphical tool: Vibes
---------------------------

.. _Vibes: http://enstabretagnerobotics.github.io/VIBES/

In all the examples proposed below, we will display boxes (rectangles) to have a visual rendering of the computations. 

In the solutions, we propose to use `Vibes`_ but you can easily adapt the code to use your favorite graphical tool.

If you want to use Vibes alos, here is a few tips (valid for Linux and MacOS). 

First, install Vibes::

  gunzip  VIBES-XXX.tar.gz
  tar xvf VIBES-XXX.tar
  cd VIBES-XXX/viewer
  cmake .
  make 
  cd ../..
  
Vibes is based on a client-server approach, the boxes are sent by the client program through a pipe 
and plot by another program (the viewer). 

To run the viewer::

  VIBES-XXX/viewer/VIBes-viewer&

Now, copy the client API in the folder of your C++ program::

  cp VIBES-XXX/client-api/C++/src/* [my-folder]

In the client program, include the Vibes API:

.. code-block:: cpp

   #include "ibex.h"
   #include "vibes.cpp"

   using namespace std;
   using namespace ibex;

Then, connect to the server with the following instructions.

.. code-block:: cpp

   int main() {
     vibes::beginDrawing ();
     vibes::newFigure("....");
     ...

And disconnect before your program terminantes:

.. code-block:: cpp

   vibes::endDrawing();

Finally, to plot a box ([a,b],[c,d]) just call:

.. code-block:: cpp

   vibes::drawBox(a, b, c, d, "...");

What is between the double quotes is the color code of the box. For instance, "b[r]" will paint the box in red and the contour in blue.

.. _set-creation:

----------------
Set Creation
----------------

A set is an instance of ``Set`` and can either be initially defined as :math:`\mathbb{R}^n` itself or a specific box:

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-init-1-C
   :end-before: set-init-1-C

It can also be initialized from a constraint or a system of constraints.
In this case, the set is built by performing some computations recursively, until some precision is reached. The second parameter given to the
constructor controls this precision. 

**Note**: the computation performed recursively is an application of the :ref:`forward-backward separator <sep-ctr>` associated to the 
constraint.

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-init-2-C
   :end-before: set-init-2-C

The result is the following picture, obtained with Vibes. We explain how we have generated this picture in the next section.

.. figure:: images/set-sep.png
   :width: 300 px
   :align: center
   
.. _set-explore:

------------------------------
Set Exploration 
------------------------------

The internal structure of sets is not intended to be handled directly by users (and may actually change with time).
If you want to explore a set, you can use the ``SetVisitor`` class (following the
`visitor design pattern`_) . This class must implement a ``visit_leaf``
function that will be automatically called for every leaf of the set. It can also optionnaly implement a ``visit_node``
function that is called for every intermediate node in the tree, and tell wether or not to visit children of the current
node by returning true (default) or false.

.. _visitor design pattern : http://en.wikipedia.org/wiki/Visitor_pattern

A typical usage of a set visitor is for listing or plotting the set and we shall illustrate the mechanism for such usage.

The ``visit_leaf`` function  must take in argument the information that characterizes a leaf, namely, a box (``IntervalVector``) and a status
(``BoolInterval``). Here is an example that simply display the box and the status of a leaf in the standard output:

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-visit-1-C
   :end-before: set-visit-1-C
   
Now the following code :ref:`loads a set <set-file>` from a file an lists all the boxes inside:


.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-visit-2-C
   :end-before: set-visit-2-C
   
The result is:

+----------------------------------------------------+---------------------------------------------+
| .. literalinclude:: ../examples/doc-set.txt        | .. figure:: images/set-visit.png            |
|   :start-after:  set-visit-2-O                     |    :width: 300 px                           |
|   :end-before:   set-visit-2-O                     |    :align: center                           |
+----------------------------------------------------+---------------------------------------------+
+  **Visiting the set with** ``ToConsole``           | **Visiting the set with** ``ToVibes``       |
+----------------------------------------------------+---------------------------------------------+

.. _set-plot:


We will now show how to plot a set calculated by Ibex with :ref:`Vibes <lab_vibes>`.

We create a new class that implements the ``visit_leaf`` function as follows:

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-visit-3-C
   :end-before: set-visit-3-C
   
The main code is similar:

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-visit-4-C
   :end-before: set-visit-4-C
   
And the result is the picture above.

.. _set-file:

------------------------------
File operations
------------------------------

You can save a set into a file and load a set from a file.

To save into a file named "set-example":

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-save-C
   :end-before: set-save-C
   
To load a set from a file, use the constructor with string argument:

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-load-C
   :end-before: set-load-C

.. _set-inter:

------------------------------
Set Intersection
------------------------------

A set can be intersected with another set that can either be explicit (of type ``Set``) or implicit.
A contractor and a separator are examples of implicit sets. In this case, we talk about 
:ref:`set contraction <set-contract>` rather than intersection (but, conceptually, this is two
equivalent terms when dealing with sets).

We consider here intersection of two explicits sets.

The intersection between two sets is obtained with the ``&=`` operator.
In the next example, we create two sets from separators, one being a circle or radius 5, 
the other being the complementary of a circle of radius 4. The result of the intersection gives a ring.

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-inter-C
   :end-before: set-inter-C

The result is the following picture.

.. figure:: images/set-inter.png
   :width: 300 px
   :align: center

**Note:** It should be emphasized that it is always better to handle sets explicitly on last resort.
This means that, in this example, it would have been more efficient to create one set from the conjunction
of the two constraints.
   
------------------------------
Set Union
------------------------------

The union works similarly. In the next example, we create two sets, one being a circle centered on the origin
and the other the circle centered on the point (5,0).
Then we perform the union of the two sets.

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-union-C
   :end-before: set-union-C

The result is the following picture.


.. figure:: images/set-union.png
   :width: 300 px
   :align: center

.. _set-contract:

------------------------------
Set Contraction
------------------------------

A :ref:`separator <sep>` can be used to contract a set.

The operator is recursively applied on
the set until some precision is reached (size of boundary boxes).

We illustrate this by calculating again the "ring" but in a different way.
We have already shown how to calculate a ring:

- by giving a conjunction of the constraints to the :ref:`constructor of Set <set-creation>`
- by computing two sets separately, one for each constraint, and then by performing :ref:`an intersection <set-inter>`

We now create a set for one of the constraint and then contracts this set with the other constraint.

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-contract-1-C
   :end-before: set-contract-1-C

.. _set-interval:

-----------------
Set Intervals
-----------------

A set interval [S] (or i-set) :ref:`[Jaulin 2012] <Jaulin12>` is the given of two sets :math:`(S_1,S_2)` 
that represent a lower and upper bound (with respect to the inclusion order)
of an unkown set S:

.. math::

   S_1 \subseteq S \subseteq S_2.

A possible notation (that we use in the code below) for the set interval [S] is: :math:`[S_1,S_2]`.
  
A set interval can be explicitly represented by an instance of the ``SetInterval`` class.
It can also be implicitly represented by a separator. Let us explain how.
A separator S have been used so far to represent a "simple" set (not a set interval)
by two complementary contractions :math:`C_1` and :math:`C_1`, being respectively for the inner and outer part.
This means that the set associated to the separator can be seen as the following
degenerated set interval:

.. math::

   set(S) = [set(C_1),~^c{set(C_2)}].

where set(C) designates the set associated to C (the insensitive points).

Now, it is possible to change the status of either the inner or outer contraction to
the special value ``MAYBE``. This means that the contracted part is not inside or
outside the set but potentially inside either one. If we change this way the
status of :math:`C_1`, the separator is now associated to the set interval:

.. math::

   set(S) = [\emptyset,~^c{set(C_2)}].
   
If we change the status of :math:`C_2`, we obtain:

.. math::

   set(S) = [set(C_1),\mathbb{R}^n].

The next example illustrates the use of separators to contract a set interval with the
following information:

- the set is enclosed in the circle centered on the origin and of radius 2
- the set encloses n little circles centered on different points and or radius 1 

A set interval can be visited exactly like a set and we have used the same ``ToVibes``
class as above for producing the picture below.

.. literalinclude:: ../examples/doc-set.cpp    
   :language: cpp
   :start-after: set-interval-C
   :end-before: set-interval-C


.. figure:: images/set-interval.png
   :width: 300 px
   :align: center
