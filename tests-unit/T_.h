
#ifndef _T_H
#define _T_H

#include "Interval.h"

extern int failed;
extern int N;
extern int _nber;
extern const char* _class;
extern const char* _meth;

void _test(bool mustbetrue);

bool approx(REAL value, REAL ref, REAL delta);

bool approxI(const INTERVAL& x, REAL lb, REAL ub, REAL delta);

#endif
