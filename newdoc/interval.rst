*************************************************************
              Interval Computations (*under construction*)
*************************************************************



============================
Set-membership operations
============================

The following operations are valid for ``Interval'', ``IntervalVector`` and ``IntervalMatrix``:

-----------------------------------------

========================================

    bool is_subset(const Interval& x) const;

    bool is_strict_subset(const Interval& x) const;

    bool is_superset(const Interval& x) const;

    bool is_strict_superset(const Interval& x) const;

    bool contains(double d) const;

    bool strictly_contains(double d) const;

    bool is_disjoint(const Interval &x) const;

    bool is_empty() const;



===================
Backward arithmetic
===================

