
.. _ibex-solve: 

**************************************
             IbexSolve
**************************************

This page describes the IbexSolve plugin, installed with the ``--with-solver`` option.


**The documentation is under construction.**


=================
Getting started
=================

IbexSolve is a end-user program that solves a system of nonlinear equations rigorously
(that is, it does not lose any solution and return each solution under the form of a 
small box enclosing the true value). It resorts to
a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that this program is based on a :ref:`generic solver <generic-solver>` that is much more customizable.

You can directly apply this solver on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the `Minibex syntax`_ and stored in an arborescence under ``plugins/solver/benchs/``.

.. _Minibex syntax: #func-minibex

Open a terminal, move to the ``bin`` subfolder and run IbexSolve with, for example, the problem named kolev36 located at the specified path::

  ~/ibex-2.6.0$ cd bin
  ~/ibex-2.6.0/bin$ ./ibexsolve ../plugins/solver/benchs/others/kolev36.bch
	
The following result should be displayed::

 ***************************** setup *****************************
   file loaded:		../plugins/solver/benchs/others/kolev36.bch
   output file:		../plugins/solver/benchs/others/kolev36.mnf
 *****************************************************************

 running............

 solving successful!

 number of inner boxes:		1
 number of boundary boxes:	0
 number of unknown boxes:	0
 number of pending boxes:	0
 cpu time used:			0.0640001s
 number of cells:		25

 results written in ../plugins/solver/benchs/others/kolev36.mnf

The number of "inner boxes" correspond to the number of solutions found (there is just one here). So see the solution, use the option ``-s``.

In the report, the "number of cells" correspond to the number of hypothesis (bisections) that was required to solve the problem.

.. _generic-solver:

================== 
Options
================== 

      -e<float>, --eps-min=<float>       Minimal width of output boxes. This is a
                                         criterion to _stop_ bisection: a
                                         non-validated box will not be larger
                                         than 'eps-min'. Default value is 1e-3.
      
      -E<float>, --eps-max=<float>       Maximal width of output boxes. This is a
                                         criterion to _force_ bisection: a
                                         validated box will not be larger than
                                         'eps-max' (unless there is no equality
                                         and it is fully inside inequalities).
                                         Default value is +oo (none)
      
      -t<float>, --timeout=<float>       Timeout (time in seconds). Default value
                                         is +oo (none).
      
      -i<filename>, --input=<filename>   Manifold input file. The file contains a
                                         (intermediate) description of the
                                         manifold with boxes in the MNF (binary)
                                         format.
      
      -o<filename>, --output=<filename>  Manifold output file. The file will
                                         contain the description of the manifold
                                         with boxes in the MNF (binary) format.
      
      --format                           Show the output text format.
      
      --bfs                              Perform breadth-first search (instead of
                                         depth-first search, by default)
      
      --txt                              Write the output manifold in a
                                         easy-to-parse text file. See --format
      
      --trace                            Activate trace. "Solutions" (output
                                         boxes) are displayed as and when they
                                         are found.
      
      --boundary=...                     Boundary test strength. Possible values are:
                                         
                                         - ``true``: always satisfied. Set by default
                                           for under constrained problems (0<m<n).
                                         - ``full-rank``: the gradients of all
                                           constraints (equalities and potentially
                                           activated inequalities) must be linearly
                                           independent.
                                         - ``half-ball``: (**not implemented yet**)
                                           the intersection of the box and the
                                           solution set is homeomorphic to a
                                           half-ball of R^n
                                         - ``false``: never satisfied. Set by default
                                           if m=0 or m=n (inequalities only/square
                                           systems)
                                         
      -s, --sols                         Display the "solutions" (output boxes)
                                         on the standard output.
      
      --random-seed=<float>              Random seed (useful for
                                         reproducibility). Default value is 1.
                                         
      -q, --quiet                        Print no report on the standard output.


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
