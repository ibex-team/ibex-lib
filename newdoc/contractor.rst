**************************************************
             Contractors  (*under construction*)
**************************************************

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

Propagation is another classical algorithm of constraint programming.

The basic idea is to calculate the fixpoint of a set of n contractors :math:`C_1\ldots,C_n`, that is:

.. math::
   (C_1\circ\ldots\circ C_n)^\infty

without calling a contractor when it is unecessary (see explanations given in the ref:`tutorial <tuto-propag>`).

The algorithm is implemented 


*(to be completed)*


.. _ctc-hc4:

------------------------------
HC4
------------------------------

A "constraint propagation" loop.

*(to be completed)*


*(to be completed)*

.. _ctc-inverse:

------------------------------
Inverse contractor
------------------------------


^^^^^^^^^^^^^^^^^^^^^^^^^^
The ``accumulate`` flag
^^^^^^^^^^^^^^^^^^^^^^^^^^

*(to be completed)*


.. _ctc-shaving:

------------------------------
Shaving
------------------------------

The shaving operator consists in calling a contractor C onto sub-parts ("slices") of the input box. If a slice is entirely eliminated by C,
the input box can be contracted by removing the slice from the box."

This operator can be viewed as a generalization of the SAC algorithm in discrete domains.

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

