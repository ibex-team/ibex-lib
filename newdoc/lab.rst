
Do it Yourself!
================================

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

Lab n°1: Set image
------------------

The complete code can be found here: ``examples/lab/lab1.cpp``.

**Introduction**

The goal of this first lab is to calculate the image of a box by a function using interval arithmetic:

If we denote by f the function and [x] the initial box, then the set S
to calculate is:

.. math::

   S := \{ f(x), x\in[x] \}.
   
Applying directly an :ref:`interval evaluation <tuto-func-eval>`
of the function f to [x] will give a single box that only represents an enclosure of S.

To fight with the wrapping effect, we will split [x] into smaller boxes and evaluate
the function with every little box as argument. This will result in a better description
of the set that will eventually converge to S as the size of the boxes tend to zero.

This will consist in three tasks:

- creating the function f
- creating the initial box [x] and splitting it into small boxes
- evaluating the function of every boxes
- plotting the results


**Question 1**

Create in the ``main`` the function

.. math::
   
   f:(x,y)\in\mathbb{R}^2\mapsto \begin{pmatrix}\sin(x+y)\\\cos(x+0.9\times y)\end{pmatrix}.
   
.. hidden-code-block:: cpp
   :label: show/hide solution
  
   Variable x,y;
   Function f(x,y,Return(sin(x+y),cos(x+0.9*y)));
		
**Question 2**

Create the box ([x],[y])=([0,6],[0,6]) and split each dimension into n slices, where n is a constant.

.. hidden-code-block:: cpp
   :label: show/hide solution

   IntervalVector box(2,Interval(0,6));

   // size of the "slice" on each dimension (x and y)
   double deltax=box[0].diam()/n;
   double deltay=box[1].diam()/n;

   for (int i=0; i<n; i++)
     for (int j=0; j<n; j++) {
       IntervalVector box2(2);
       box2[0]=Interval(box[0].lb()+i*deltax,box[0].lb()+(i+1)*deltax);
       box2[1]=Interval(box[1].lb()+j*deltay,box[1].lb()+(j+1)*deltay);
      
     }
   }
   
   
**Question 3**

Evaluate the function on each box and plot the result with Vibes.


.. hidden-code-block:: cpp
   :label: show/hide solution
   
   for ... 
     for ... {
       ...
       IntervalVector y=f.eval_vector(box2);
       // "b[b]" means that both the border and the interior of the box
       // is painted in blue
       vibes::drawBox(y[0].lb(), y[0].ub(), y[1].lb(), y[1].ub(), "b[b]");
     }
   }
   
   
**Question 4**

Compare the result with n=15, n=80 and n=500.

You should obtain the following pictures:


   +--------------------------+------------------------+--------------------------+
   | .. image:: lab1-15.png   | .. image:: lab1-80.png | .. image:: lab1-500.png  |
   |    :scale: 30%           |    :scale: 30%         |    :scale: 30%           |
   +--------------------------+------------------------+--------------------------+
   | n=15                     | n=80                   | n=500                    |
   +--------------------------+------------------------+--------------------------+
   

Lab n°2: Set inversion (basic)
----------------------------------



Lab n°3: Set inversion (with contractors)
-------------------------------------------

