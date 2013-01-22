

/*! \page guide Guide

\section sec_ent Entities

Usually, in CP, we consider variables, domains and constraints.

In \c ibex we consider in addition to variables other types of unknowns, namely: <em>interval constants</em>, <em>parameters</em> and <em>symbolic constants</em>. As an example, consider the following equation:

x=sin(pi/3),

where \a x and \a pi are two uncertain values whose domains are respectively (-oo,oo) and [3.141592,3.141593]. In terms of constraint modeling, \a x and \a pi do not have a different status since they are both a symbol with an attached domain.

But clearly, they must not be treated in the same way. The symbol \a pi is not a variable, that is, something we are looking for feasible values. It is rather as a \e constant, excpet that the attached value is not exactly known. We shall talk about an <em>interval constant</em>.

In ibex, <em>variables</em>, <em>interval constants</em>, <em>parameters</em> and <em>symbolic constants</em> are called \b entities. Variables are one particular type of entities.

As said above, all entities have the same status with regard to the constraints semantic: an entity is basically a leaf of the syntactical tree which is subject to uncertainty.

The point is that contractors and pavers must be able to distinguish which type of uncertainty is involved in a constraint, so that an appropriate treatment can be done. In the case of an interval constant such as [3.141592,3.141593], the best action is probably to do nothing (neither contract nor bisect it).

\subsection sec_eprs Parameters

Consider the following constraint:

\f[(P) \quad \exists p\in[5,10] \quad x^2+y^2=p \ \mbox{with} \ x~\in[0,10],\quad y~\in [0,10]. \f]

Of course, there may exist a specific contractor for the constraint C_p(x,y):

\f[C_p(x,y) \quad \exists p \quad x^2+y^2=p. \f]

or for a whole class of quantified constraints. 
But a typical situation is that only a contractor for \f$c(x,y,p): \quad x^2+y^2=p\f$ (i.e., with 3 variables and
no quantifier) is avalaible. If you want anyway to run a paver with this contractor to solve the initial problem, then you 
expect the entity p to be treated differently: 
<ol>
<li> contraction is authorized but not of prior importance
<li> bisection is forbidden
</ol>
Indeed, 

<b>Contraction is not of prior importance</b>, because a parameter is an entity for which values are somehow of ``no interest''. 
Indeed, consider a solution couple (x,y). In the way that the problem is stated, we are not really interested in the actual value of x^2+y^2.
We just want to be sure that this value belongs to [5,10]. Of course, if the system also provides this value we are even more happy.
But, this is not expected in the first place. If we do want to know what x^2+y^2 actually worths, it means that we are addressing the problem of
solving x^2+y^2=p, where p is a variable like the others. 

This distinction is crucial when the solver is looking for inner boxes. \\
An inner box is a compact representation of a continuum of solutions. By not considering
the dimensions of parameters in this representation, we get more chance to 
identify a box as an inner box.

If p is a parameter, every couple (x,y) in [6,9]x[0,1] is a solution since
for such couples we have 5<= x^2+y^2<=10. Then [6,9]x[0,1] can be detected as an inner 
box, and no further search need to be done in this box.

Inversely, if p is a variable, it is not possible to find an inner box [x]x[y]x[p]
except if [p] has a null diameter. Therefore, the same solution set than before will be
represented by a large number of small border boxes, which is time consuming.

\subsection sec_itv-cst Interval Constants

Interval constants have been introduced to distinguish modelization parameters from coefficients with a small uncertainty, because, clearly, they cannot be considered on the same level. If one has to insert Pi in an equation, this number will typically enter as an interval constant (no information on this entity has to be expected on return).

A typical example of interval constant : [3.141592,3.141593]

A interval constant is a real value that "should be fixed" but which is not because of a small uncertainty. Interval constants are mathematicaly equivalent to exist-param (they play the same role regarding a solution of the AE-system), but they are handled differently in practice. The three main distinctions are :

<ol>
   <li> Constants are never bisected nor filtered, their domains are not stored.
   <li> A constant cannot be multi-incident (several occurences in the system of equations). This means that each occurrence of the same constant in a system is independent. Let us illustrate this on one example: let PI be a constant and assign [3.141592,3.141593] to PI. If PI is used at different places in the equations, there is no point to correlate its occurrences (since the value taken by any of them will never be PI anyway).
   <li> Constants are considered as univ-params in most of the inner tests (for efficiency reasons).
</ol>

\note

<ol>
       <li> As a consequence, it is risky to launch a solver with a precision order of magnitude smaller than the constants diameter, except if you know the system is 0-dimensional or if inner boxes can be detected easily.
       <li> Interval constants could be narrowed, but they are not, essentially for efficiency reasons. Narrowing constants would imply storing their domains. As problis can include a very large number of constants, this would increase dramaticaly the size of the cell.
</ol>

\subsection sec_symb-cst Symbolic Constants

Interval constants represent coefficients we often need to control. Indeed, some coefficients are usually dependent to the "state" of a physical system, and they change as the state changes.

A symbolic constant is an interval constant that can be accessed by an identifier. The distinction between symbolic constants and interval constants only last for the syntaxical definition of a system. In particular, as soon as a system is being solved, each occurrence of the same symbolic constant is nothing but an independent interval constant (it is neither a variable nor a parameter).

\note All the remarks about interval constants carry over symbolic constants.

\section sec_env The Environment

\section sec_space Spaces



*/
