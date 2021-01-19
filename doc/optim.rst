
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
it minimizes a (nonlinear) objective function under (nonlinear) inequality and equation constraints:

.. math::

	{\mbox Minimize} \ f(x)
	
	{\mbox s.t.} \ h(x)=0 \wedge g(x)\leq 0.

In the previous formula, *f* is a real-valued function, but *h* and *g* can be multivariate.
The right-hand sides 0 are therefore vectors and the equation and inequality sign applies component-wise. 

This notation convention is kept for the whole document.


.. note::
    
   IbexOpt resorts to a unique black-box strategy (whatever the input problem is) and with a very limited 
   number of parameters. Needless to say, this strategy is a kind of compromise and not the best one for a given problem.

   If you are a C++ programmer, the design of the Ibex library allows you to customize IbexOpt or even build your own optimizer from high-level algorithmic bricks.
   See the developer guide.

You can directly apply this optimizer on one of the benchmark problems 
distributed with Ibex. 
The benchmarks are all written in the :ref:`Minibex syntax <mod-minibex>` and stored under ``benchs/optim/``.
If you compare the Minibex syntax of these files with the ones given to :ref:`IbexSolve <solver>`, you will see that a ``minimize``
keyword has appeared.

.. _Minibex syntax: #func-minibex

Open a terminal (move to the ``bin`` subfolder of Ibex if it is installed locally) and run IbexOpt with, for example, the problem named ex3_1_3 located at the specified path::

  $ ./ibexopt [ibex-lib-path]/benchs/optim/easy/ex3_1_3.bch
	
The following result should be displayed::

 running............

 optimization successful!

 f* in	[-310.011436309,-309.999999842]
	    (best bound)

 x* =	(4.99999999901 ; 1.00000000001 ; 5 ; 9.99996530027e-10 ; 5 ; 10)
	    (best feasible point)

 relative precision on f*:	3.6890471184e-05 [passed] 
 absolute precision on f*:	0.0114364679579
 cpu time used:			0.00209500000001s
 number of cells:		4

The program has proved that the minimum of the objective lies in a small interval enclosing -310. It also gives
a point close to (5 ; 1 ; 5 ; 0 ; 5 ; 10) which satisfies the constraints and for which
the value taken by the objective function is inside this interval. The process took less than 0.003 seconds.
The number of "cells" is also reported. This number basically corresponds to the total number of hypothesis (bisections) that was 
required to solve the problem. It gives an indication of its complexity. 


.. _optim-output:

============================
The output of IbexOpt 
============================

.. _ULPs : https://en.wikipedia.org/wiki/Unit_in_the_last_place

We denote now x\* the global minimizer and f\*=f(x\*) the global minimum of a NLP.

IbexOpt can be run in two different *modes* and the precise meaning of the output depends on the chosen mode.

- In the **relaxed** mode (the default one), the NLP which is solved is actually
  
  .. math::

     {\mbox Minimize} \ f(x)
	
     {\mbox s.t.} \ |h(x)|\leq \varepsilon_h  \wedge g(x)\leq 0.
	
  where :math:`\varepsilon_h` is a parameter. By default it is set to 1e-8 but the user can fix it differently
  using the ``--eps-h`` option. We will call this NLP the *relaxed NLP*. Notice that it only contains inequalities.
  
  IbexOpt returns then a vector x (not a box) and an interval [y]. It is proven that:
  
    1. [y] is an enclosure of f\* (the global minimum of the relaxed NLP) respecting one :ref:`precision criterion <optim-obj-prec>`.
    2. x is feasible, i.e., satisfies all the inequalities 
    3. f(x) belongs to [y].

  A consequence of these properties is that x is "almost" a global minimum in the sense that it is a feasible point which image
  f(x) is close to the real minimum f\* (according to one :ref:`precision criterion <optim-obj-prec>`); but x may not be close to 
  x\* itself. It can even be arbitrarily far away, although this happens in practice only on pathological cases.
  
- In the **rigor** mode, IbexOpt solves the original NLP, with strict equations.
  
  It returns a box [x] and and interval [y]. It is proven that:
  
    1. [y] is an enclosure of f\* (the global minimum of the NLP) respecting the :ref:`precision criteria <optim-obj-prec>`.
    2. [x] contains at least one feasible point, i.e., a point x which satisfies all the constraints (equations and inequalities)
    3. for all x in [x], f(x) belongs to [y].
    
  The same observation can be made as for the relaxed case: it is proven that [x] contains a point that is "almost" a global
  minimum (according to one :ref:`precision criterion <optim-obj-prec>`) but there is no information about the distance between
  x and x\*.
  
  A further remark is about the size of [x]. We have not explicitly mentioned that it has to be small. Indeed, in theory,
  we can't bound its size. But, in practice, if IbexOpt succeeds, the size of this box is very tiny, just about a few `ULPs`_.
  Notice also that conditions 1 and 3 together somehow also impose [x] to be small. But, again, this is true only in practice (consider for instance
  a constant objective function as a counter-example).
  
Note that for a problem without equations, the relaxed and rigor modes are the same.

We advice to rather use the relaxed mode, should you have to set a very small precision :math:`\varepsilon_h`.
The rigor mode is useful only if strict satisfaction of equations are required.
It can take longer and may sometimes fails were the relaxed mode succeeds, typically because of redundancies; see
the :ref:`FAQ <optim-faq>`. 


.. _optim-obj-prec:

============================
Objective precision criteria
============================

Remind that IbexOpt returns an interval [y] which encloses f\* and a feasible point x (surrounded by a tiny box in rigor mode) such 
that :math:`f(x)\in[y]`.

We note :math:`y^-` and :math:`y^+` the lower and upper bounds of [y].

There are two precision criteria for the objective: an absolute and a relative one.
IbexSolve stops if one of the criterion is fulfilled (not both).

The **absolute** precision :math:`\varepsilon_{f}^{abs}` can be set with the ``--abs-eps-f`` (or ``-a`` in short) option.
The default value is 1e-7.

This criterion is fulfilled when :math:`y^+\leq y^- + \varepsilon_{f}^{abs}`.
This therefore ensures

.. math::
   |f(x)-f^*|\leq \varepsilon_f^{abs}.

The **relative** precision :math:`\varepsilon_{f}^{rel}` can be set with the ``--rel-eps-f`` (or ``-r`` in short) option.
The default value is 1e-3.

There are several cases:

- If [y] contains 0 or has an infinite bound, the criterion is not satisfied.
- If :math:`y^->0`, the criterion is satisfied if :math:`y^+ \leq (1+\varepsilon_{f}^{rel}) y^-`.
- If :math:`y^+<0`, the criterion is satisfied if :math:`y^- \geq (1+\varepsilon_{f}^{rel}) y^+`.

This therefore ensures

.. math::
   |f(x)-f^*|\leq \varepsilon_f^{rel}|f^*|.

.. _optim-return:

============================
Return status
============================

When the optimizer terminates, the following possible status are:

- **success**:  
              An enclosure of the minimum respecting one of the precision requirements (``--a`` or ``--r``)
              has been found as well as a pseudo-global minimizer, as explained :ref:`above <optim-output>`.
              In standard mode (without ``--rigor``), equations are relaxed and the global minimizer is
              a point x satisfying
              :math:`-\varepsilon_h\leq h(x)\leq\varepsilon_h`. In rigor mode (``--rigor``), the
              global minimizer is a box  :math:`[x]` such that, for some x inside we do have :math:`h(x)=0`.
              In both cases, for the (explicit or implicit) point x, f(x) is also sufficiently closed
              to the real global minimum, according to the precision criteria.
- **infeasible**: 
              This return status actually corresponds to two different situations. Either the constraints
              are not feasible (that is, there is not point x simultaneously satisfying all equations
              and inequalities) or the feasible points are all outside the definition domain of the
              objective function f.
- **no feasible point found**:
              The optimizer could not be able to find a feasible point. This status typically arises
              if you control the precision of the bisection (``--eps-x``). Indeed, it may happen, in this case,
              that the search stops and no box explored was enough bisected to find a feasible point inside. So the
              search is over but the problem was not solved. It may also arise when an inequality
              is actually an equation (e.g., :math:`x^2\leq 0`), because in non-rigor mode, neither a relaxation nor
              an equation satisfaction proof is enforced in this case.
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
| --simpl=...                          | Expression simplification level. Possible values are:                        |
|                                      |                                                                              |
|                                      | - 0: no simplification at all (fast).                                        |
|                                      | - 1: basic simplifications (fairly fast).                                    |
|                                      |      E.g. x+1+1 --> x+2                                                      |
|                                      | - 2: more advanced simplifications without developing (can be slow).         |
|                                      |      E.g. x*x + x^2 --> 2x^2.                                                |
|                                      |      Note that the DAG structure can be lost.                                |
|                                      | - 3: simplifications with full polynomial developing (can blow up!).         |
|                                      |      E.g. x*(x-1) + x --> x^2.                                               |
|                                      |      Note that the DAG structure can be lost.                                |
|                                      |                                                                              |
|                                      | Default value is : 1.                                                        |
+--------------------------------------+------------------------------------------------------------------------------+
| --random-seed=<*float*>              | Random seed (useful for reproducibility). Default value is 1.                |
+--------------------------------------+------------------------------------------------------------------------------+
| --eps-x=<*float*>                    | Precision on the variable (**Deprecated**). Default value is 0.              |
+--------------------------------------+------------------------------------------------------------------------------+
| --initial-loup=<*float*>             | Initial "loup" (a priori known upper bound).                                 |
+--------------------------------------+------------------------------------------------------------------------------+
| -i<*filename*>, --input=<*filename*> | COV input file. The file contains optimization data in the COV (binary)      |
|                                      | format.                                                                      |
+--------------------------------------+------------------------------------------------------------------------------+      
| -o<*filename*>, --output=<*filename*>| COV output file. The file will contain the optimization data in the COV      |
|                                      | (binary) format. See --format                                                |
|                                      |                                                                              |
+--------------------------------------+------------------------------------------------------------------------------+
| --rigor                              | Activate rigor mode (certify feasibility of equations).                      |
+--------------------------------------+------------------------------------------------------------------------------+
| --kkt                                | Activate contractor based on Kuhn-Tucker conditions (rigor mode only).       | 
+--------------------------------------+------------------------------------------------------------------------------+
| --trace                              | Activate trace. Updates of loup/uplo are printed while minimizing.           |
+--------------------------------------+------------------------------------------------------------------------------+
| -q, --quiet                          | Print no report on the standard output.                                      |
+--------------------------------------+------------------------------------------------------------------------------+


.. _optim-faq:

====================
FAQ
====================

---------------------------------------
warning: too many active constraints
---------------------------------------

 When I run IbexOpt in rigor mode, the program never ends and prints the following warning repeatedly:

   warning: too many active constraints, cannot prove feasibility -> loup lost!
 

.. topic:: Answer

   This means that, very likely, your problem has a redundant equation. It is impossible to certify feasibility 
   in this case. So, as IbexOpt never finds a feasible point, the upper bound on the objective is 
   stuck to infinity and the program runs forever.

   The only solution in this case (beside using relaxed mode) is to revise your model. A redundant equation
   is somehow a modeling issue.


-------------------------------------------------------
The problem is solved in relaxed but not rigor mode
-------------------------------------------------------

.. _LICQ : https://en.wikipedia.org/wiki/Karush%E2%80%93Kuhn%E2%80%93Tucker_conditions#Regularity_conditions_(or_constraint_qualifications)

 **Short answer** : likely, there is a redundant constraint in your problem. Try to fix it.
 
 **Long answer**

  The rigor mode relies on a numerical algorithm (an interval variant of the Newton iteration) that requires
  linear independence constraint qualification, in short `LICQ`_.

  In particular, the LICQ condition cannot be fulfilled with redundant constraints. And most of the time, this is what happens.
  
  When the number of active constraints exceed the number of variables, IbexOpt detects it and displays the warning `too many active constraints` (see
  the previous FAQ).
  So, in this case, you have an hint that your problem has a redundancy. The warning message must appear repeatedly though, otherwise, the warning may
  only reflect a local singularity. Typically, we may have two inequalities that get tangential in some region (while being distant from each other elsewhere). 

  But if the number of active constraints is less than the number of variables, there is no way for IbexOpt to detect a redundancy. Because IbexOpt is a  numerical tool dedicated to
  non-linear problems, it can only observe in a given box that the LICQ condition is not respected. It cannot conclude that there is a
  singularity at the model level, and even less, a redundancy. But, again, most of the time, there is.

  This situation happens in some of the benchmark problems supplied with IbexOpt (see the files under `benchs/optim`):
  **sambal**, **harker**, **immun** and **ex7_3_5**.

  We suggest the following strategy for analyzing your problem:
  
  1. Try to see if there is an obvious redundancy in the equations of your model. If not:
  
  2. Calculate the pseudo-global minimum in relaxed mode with a tight relaxation parameter (ex: ``eps-h=1e-14``). See which inequalities and bounds are active at 
     this pseudo-minimum. Then try to see if there is a redundancy among all the active constraints
     (including equations of course). If not:
  
  3. Calculate the jacobian of these active constraints and check that it is not full-rank. This just confirms that the LICQ conditions are not satisfied, hence
     a normal behavior of IbexOpt (if the matrix is full-rank, please, submit a bug report!)
  
  4. Run again IbexSolve in rigor mode and activate the trace (``--trace``).
     Two situation occurs:
     
     - some feasible points (aka *loup*) are found (they appear on the screen as they are found). This means that your problem has a local singularity right at the
       global minimum! Your are not lucky: this is something that happens rarely in practice, and nothing proves that there is a redundancy (i.e., that you
       can fix this problem easily).
     
     - feasible points seem never to be found by IbexOpt. Then, your problem has probably a redundancy. Try to fix it. 
  

  Let us apply this strategy to the aforementioned problems:

  - **sambal**: step 1: there are 8 linear equations and the rank of the matrix is 7. So one constraint is redundant. Of course, in this particular case of
    linear constraints, IbexOpt could easily detect the redundancy itself. Such test has not been implemented yes (IbexOpt is rather dedicated to nonlinear problems).
  - **harker**: step 1: there are 20 linear constraints and the rank of the matrix is 18. Same remark as above. 
  - **immun** : step 2: there is `x16^2` in the objective function, so the bound constraint `x16=0` is active at the minimum. Constraint `-x10=-x16` enforces then `x10=0`, which 
    is also a bound constraint. So we have 3 constraints: `-x10-x16=0`, `x16=0` and `x10=0` with one redundant.
  - **ex7_3_5**: step 4: this case is more complicated. We suspect again a redundancy because IbexOpt never finds feasible points (and the jacobian is not full-rank at the pseudo-global minimum).


