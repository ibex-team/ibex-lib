.. _mod-sys-ctrs:

*****************************************************
              Constraints
*****************************************************

In this section, we do not present *constraints* in their full generality
but *numerical constraints* (the ones you are the most likely interested in).

A numerical constraint in IBEX is either a relation like 
:math:`f(x)<0`, :math:`f(x)\le0`, :math:`f(x)=0`, :math:`f(x)\ge0` or :math:`f(x)>0`,
where *f* is a function as introduced in the previous section. If *f* is vector-valued, then
0 must be a vector.

Surprisingly, constraints do not play an important role in IBEX.
It sounds a little bit contraditory for a *constraint* programming library.
The point is that IBEX is rather a *contractor* programming library meaning that 
we build, apply and compose contractors rather than constraints directly.

As a programer, you may actually face two different situations.

Either you indeed want to use a constraint as a contractor in which case you
build a ``Ctc`` object with this constraint (the actual class depending
on the algorithm you chose, as explained in :ref:`the tutorial <tuto-ctc>` --by default, it is :ref:`ctc-fwd-bwd`--).
Either you need to do something else, say, like calculating the Jacobian matrix of the
function *f*. In this case, you just need to get a reference to this function 
and call ``jacobian``. In fact, all the information inherent to a constraint
(except the comparison operator of course) is contained in the underlying function so that there
is little specific code related to the constraint itself.

For these reasons, the only operations you actually do with a constraint is either
to read its field or wrap it into a contractor.

.. _mod-sys-ctrs-fields:

----------------------
Class and Fields
----------------------

The class for representing a numerical constraint is ``NumConstraint``.
The first field in this class is a reference to the function:

  Function& f;


The second field is the comparison operator:

  CmpOp op;


``CmpOp`` is just an ``enum`` (integer) with the following values:

=======   ============
Op        def         
=======   ============
``LT``    :math:`<`   
``LEQ``   :math:`\le`
``EQ``    :math:`=`
``GEQ``   :math:`\ge`
``GT``    :math:`>`   
=======   ============

.. _mod-sys-ctrs-cpp:

--------------------------------------------
Creating constraints (in C++)
--------------------------------------------

To create a numerical constraint, you can build the function *f* first and
call the constructor of ``NumConstraint`` as in the following example.


.. code-block:: cpp

   Variable x;
   Function f(x,x+1);
   NumConstraint c(f,LEQ); // the constraint x+1<=0
	

But you can also write directly:


.. code-block:: cpp

   Variable x;
   NumConstraint c(x,x+1<=0);
	
which gives the same result. The only difference is that, in the second case,
the object ``c.f`` is "owned" (and destroyed) by the constraint whereas 
in the first case, ``c.f`` is only a reference to ``f``.

Note that the constant 0 is automatically interpreted as a vector (resp. matrix),
if the left-hand side expression is a vector (resp. matrix). However, it does not
work for other constants: you have to build the constant with the proper dimension,
e.g.,


.. code-block:: cpp

   Variable x(2);
   NumConstraint c(x,x=IntervalVector(2,1)); // the constraint x=(1,1)
   cout << "c=" << c << endl;
	
The display is::

  c=(_x_0-([1,1] ; [1,1]))=0


In case of several variables, the constructor of ``NumConstraint`` works as for functions.
Up to 6 variables can be passed as arguments:


.. code-block:: cpp

   Variable a,b,c,d,e,f,g;
   NumConstraint c(a,b,c,d,e,f,g,a+b+c+d+e+f+g<=1);
	

And if more variables are necessary, you need to build an ``Array<const ExprSymbol>`` first, like :ref:`here <mod-func-cpp>`.

.. _mod-sys-fields:

**Note:** There is currently the important restriction that inequalities can only be formed with real-valued (called "scalar") functions.
It could be possible, in theory, to write :math:`f(x)\le0` with f vector-valued by interpreting the operator componentwise but this
is not supporter by Ibex.
