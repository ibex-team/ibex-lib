
.. _optim: 

**************************************
              IbexOpt
**************************************

This documentation is for using IbexOpt with the command prompt.

To use IbexOpt in C++ or program your own global optimizer with Ibex, see the :ref:`programmer guide <optim-prog>`.


.. _optim-run-default:

=================
Getting started
=================

IbexOpt is a end-user program that solves a standard NLP problem (non-linear programming), i.e.,
it minimizes a (nonlinear) objective function under (nonlinear) inequality and equality constraints:

.. math::

	{\mbox Minimize} \ f(x)
	
	{\mbox s.t.} \ h(x)=0 \wedge g(x)\leq 0.
	
IbexOpt resorts to a unique black-box strategy (whatever the input problem is) and with a very limited 
number of parameters. Needless to say, this strategy is a kind of compromise and not the 
best one for a given problem.

Note that this program is based on the :ref:`generic optimizer <optim-generic>`, a C++ class
that allows to build a more customizable optimizer.

You can directly apply this optimizer on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the :ref:`Minibex syntax <mod-minibex>` and stored in an arborescence under ``plugins/optim/benchs/``.
If you compare the Minibex syntax of these files with the ones given to IbexSolve, you will see that a "minimize"
keyword has appeared.

.. _Minibex syntax: #func-minibex

Open a terminal, move to the ``bin`` subfolder and run IbexSolve with, for example, the problem named ex3_1_3 located at the specified path::

  ~/ibex-2.8.7$ cd bin
  ~/ibex-2.8.7/bin$ ./ibexopt ../plugins/optim/benchs/easy/ex3_1_3.bch
	
The following result should be displayed::


 ************************ setup ************************
   file loaded:	../plugins/optim/benchs/easy/ex3_1_3.bch
 *******************************************************

 running............

  optimization successful!

  best bound in: [-310.309999984,-309.999999984]
  relative precision obtained on objective function: 0.001  [passed]
  absolute precision obtained on objective function: 0.309999999985  [failed]
  best feasible point: (4.9999999999 ; 1 ; 5 ; 0 ; 5 ; 10)
  cpu time used: 0.00400000000001s.
  number of cells: 1

The program has proved that the minimum of the objective lies in a small interval enclosing -310. It also gives
a point close to (5 ; 1 ; 5 ; 0 ; 5 ; 10) which satisfies the constraints and for which
the value taken by the objective function is inside this interval. The process took less than 0.005 seconds.


.. _optim-return:

================== 
Return status
================== 

When the optimizer terminates, the following possible status are:

- **success**:    
              An enclosure of the minimum respecting the precision requirements (``--a`` and ``--r``)
              has been found as well as a global minimizer .
              In standard mode (without ``--rigor``), equalities are relaxed and the global minimizer is
              a point x* satisfying
              :math:`-\varepsilon_h\leq h(^*)\leq\varepsilon_h`. In rigor mode (``--rigor``), the
              global minimizer is a box  :math:`[x^*]` such that, for some x* inside we do have :math:`h(x^*)=0`.
              In both cases, for the (explicit or implicit) point x*, f(x*) is also sufficiently closed
              to the real global minimum, according to the precision criteria.
- **infeasible**: 
              This return status actually corresponds to two different situations. Either the constraints
              are not satisfiable (that is, there is not point x simultaneously satisfying all equalities
              and inequalities) or the feasible points are all outside the definition domain of the
              objective funnction f.
- **no feasible point found**:
              The optimizer could not be able to find a feasible point. This status typically arises
              if you control the precision of the bisection (``--eps-x``). Indeed, it may happen, in this case,
              that the search stops and no box explored was enough bisected to find a feasible point inside. So the
              search is over but the problem was not solved. It may also arise when an inequality
              is actually an equality (e.g., :math:`x^2\leq 0`), because in non-rigor mode, neither a relaxation nor
              an equality satisfaction proof is enforced in this case.
- **unbounded objective**:
              The optimizer could not find a lower bound of the minimum. This means that the objective is very
              likely to be unbounded. 
- **time out**:  
              The time specified with ``-t`` is reached. Note that this time is only for the solving process itself and
              does not count for the system loading step. This means that if the system (the Minibex file) is very big, 
              you may actually wait longer.
- **unreached precision**:
              This status happens when the search is over but the enclosure on the minimum does not respect the
              precision requirements (``--a`` and ``--r``). It is a similar but slightly better situation than 
              when the status is ``no feasible point found``. The difference is that some feasible points have 
              been found but some part of the search space could not be processed (neither rejected nor proven
              as containing a solution), preventing a good minimum enclosure. An example is when minimizing x
              under the constraint :math:`x^2(x-1)(x-2)\leq0` in non-rigor mode. Feasible points in the interval
              [1,2] are quickly found so that the loup is quickly set to 1. But the lower bound is stuck to 0
              as :math:`x^2\leq 0` contains a solution (0) which is not found. The problem does not happen in 
              rigor mode.


.. _optim-options:

================== 
Options
================== 

+--------------------------------------+------------------------------------------------------------------------------+
| -r<*float*>, --rel-eps-f=<*float*>   |  Relative precision on the objective. Default value is 1e-3.                 |
+--------------------------------------+------------------------------------------------------------------------------+                                        
| -a<*float*>, --abs-eps-f=<*float*>   | Absolute precision on the objective. Default value is 1e-7.                  |
+--------------------------------------+------------------------------------------------------------------------------+
| --eps-h=<*float*>                    | Equality relaxation value. Default value is 1e-8.                            |
+--------------------------------------+------------------------------------------------------------------------------+
| -t<*float*>, --timeout=<*float*>     | Timeout (time in seconds). Default value is +oo.                             |
+--------------------------------------+------------------------------------------------------------------------------+
| --random-seed=<*float*>              | Random seed (useful for reproducibility). Default value is 1.                |
+--------------------------------------+------------------------------------------------------------------------------+
| --eps-x=<*float*>                    | Precision on the variable (**Deprecated**). Default value is 0.              |
+--------------------------------------+------------------------------------------------------------------------------+
| --initial-loup=<*float*>             | Initial "loup" (a priori known upper bound).                                 |
+--------------------------------------+------------------------------------------------------------------------------+
| --rigor                              | Activate rigor mode (certify feasibility of equalities).                     |
+--------------------------------------+------------------------------------------------------------------------------+
| --trace                              | Activate trace. Updates of loup/uplo are printed while minimizing.           |
+--------------------------------------+------------------------------------------------------------------------------+
