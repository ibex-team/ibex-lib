
.. _ibex-solve: 

**************************************
              Tutorial (IbexSolve)
**************************************

.. _generic-solver:

================== 
The generic solver
================== 

The generic solver is a classical branch and prune algorithm that interleaves contraction and branching (bisection) until
boxes get sufficiently small. It takes three operators in input:

#. a **contractor** 
       
    Operator that contracts boxes by removing non-solution points. It should be emphasized that the solver only sees the
    contraction operator, not the underlying system (of equations or inequalities). This is one of the reason why it is
    said to be "generic". In particular, one can introduce contractors of other kind, that are not based on mathematical 
    expressions.

#. a **bisector**           

    Operator that splits a box. The box is bisected providing it is large enough, so this operator takes also a *precision* parameter. 
    If the box is too small, the solver will not continue the search and backtrack. See :ref:`strategy-bisectors` for more details. 

#. a **cell buffer**
  
    Operator that manages the list of pending boxes (a *cell* is a box with a little bit of extra information used by the search).
    See :ref:`strategy-cell-buffers` for more details.

Our next example creates a solver for the intersection of two circles of radius :math:`d`, 
one centered on :math:`(0,0)` and the other in :math:`(1,0)`. 

To this end we first create a vector-valued function:

.. math::
	(x,y) \mapsto \begin{pmatrix} x^2+y^2-d \\ (x-1)^2+y^2-d \end{pmatrix}

Then, we build two contractors; a `forward-backward`_ contractor and (because the system is square),
an `interval Newton`_ contractor.		


We chose as bisection operator the *round-robin* operator, that splits each component in turn.
The precision of the solver is set to 1e-7.

Finally, the cell buffer is a stack, which leads to a depth-first search.

.. warning:: 
   This solver is simple and not very efficient. The :ref:`IbexOpt <ibex-opt>` plugin comes with a 
   default solver that implements a more sophisticated strategy.

.. literalinclude:: ../examples/doc-tutorial.cpp 
   :language: cpp
   :start-after: strat-basic-solver
   :end-before: strat-basic-solver


=================
