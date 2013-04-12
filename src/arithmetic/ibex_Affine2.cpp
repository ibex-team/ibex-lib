/* ============================================================================
 * I B E X - ibex_Affine2.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : Nov 12, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Affine2.h"
#include "ibex_Exception.h"
#include <stdio.h>

namespace ibex {


Affine2::Affine2() :
		 _n		(-1		),
		 _val	(NULL	),
		 _err	(Interval::ALL_REALS)	{
 }

Affine2::Affine2(int n, int m, const Interval& itv) :
			_n 		(n),
			_val	(NULL),
			_err	(Interval::ZERO)
		//,	_actif  (!(itv.is_unbounded()||itv.is_empty()))
{
	assert((n>=0) && (m>=0) && (m<=n));
	if (!(itv.is_unbounded()||itv.is_empty())) {
		_val	=new double[n + 1];
		_val[0] = itv.mid();
		for (int i = 1; i <= n; i++){
			_val[i] = 0.0;
		}

		if (m == 0) {
			_err = Interval(itv.rad());
		} else {
			_val[m] = itv.rad();
		}
	} else {
		_err = Interval(itv);
		_n = -1;
	}
}



Affine2::Affine2(const double d) :
			_n 		(0),
			_val	(NULL)  {
	if (fabs(d)<POS_INFINITY) {
		_val = new double[1];
		_err = 0.0; //abs(d)*AF_EE();
		_val[0] = d;
	} else {
		_err = Interval(d);
		_n = -1;
	}
}


Affine2::Affine2(const Interval & itv):
			_n 		(0),
			_val	(NULL) {
	if (!(itv.is_unbounded()||itv.is_empty())) {
		_err	=itv.rad();
		_val	=new double[1];
		_val[0] = itv.mid();
	} else {
		_err = itv;
		_n = -1;
	}
}


Affine2::Affine2(const Affine2& x) :
		_n		(x.size()),
		_val	(NULL	 ),
		_err	(x.err() ) {
	if (is_actif()) {
		_val =new double[x.size() + 1];
		for (int i = 0; i <= x.size(); i++){
			_val[i] = x.val(i);
		}
	}
}


const Interval Affine2::itv() const {
	if (is_actif()) {
		Interval res(_val[0]);
		Interval pmOne(-1.0, 1.0);
		for (int i = 1; i <= _n; i++){
			res += (_val[i] * pmOne);
		}
		res += _err * pmOne;
		return res;
	} else {
		return _err;
	}
}

Affine2& Affine2::operator=(const Affine2& x) {
	if (this != &x) {
		_err = x.err();
		if (x.is_actif()) {
			if (_n!=x.size()) {
				_n =x.size();
				if (_val!=NULL) { delete[] _val; }
				_val = new double[_n+1];
			}
			if (_val==NULL) _val = new double[_n+1];

			for (int i = 0; i <= x.size(); i++) {
				_val[i] = x.val(i);
			}
		} else {
			_n = -1;
			if (_val != NULL) {
				delete[] _val;
				_val = NULL;
			}
		}
	}
	return *this;

}

Affine2& Affine2::operator=(double d) {
	if (fabs(d)<POS_INFINITY) {
		_err = abs(d)*AF_EC();
		if (_n<0)  {
			_n = 0;
			if (_val!=NULL) { delete[] _val; }
			_val = new double[1];
		}
		if (_val==NULL) _val = new double[_n + 1];

		_val[0] = d;
		for (int i = 1; i <= _n; i++) {
			_val[i] = 0;
		}
	} else {
		_err = d;
		_n = -1;
		if (_val != NULL) {
			delete[] _val;
			_val = NULL;
		}
	}
	return *this;
}

Affine2& Affine2::operator=(const Interval& x) {
	if (!(x.is_unbounded()||x.is_empty())) {
		_err = x.rad();
		if (_n<0)  {
			_n = 0;
			if (_val!=NULL) { delete[] _val; }
			_val = new double[1];
		}
		if (_val==NULL) _val = new double[_n + 1];

		_val[0] = x.mid();
		for (int i = 1; i <= _n; i++) {
			_val[i] = 0;
		}
	} else {
		_err = x;
		_n = -1;
		if (_val != NULL) {
			delete[] _val;
			_val = NULL;
		}
	}

	return *this;
}



/** \brief Return (-x) */
Affine2 Affine2::operator-() const {
	Affine2 res;
	res._n = _n;
	res._err = _err;
	if (is_actif()) {
		res._val = new double[_n+1];
		for (int i = 0; i <= _n; i++) {
			res._val[i] = (-_val[i]);
		}

	}
	return res;
}




Affine2& Affine2::saxpy(double alpha, const Affine2& y, double beta, double ddelta, bool B1, bool B2, bool B3, bool B4) {
//std::cout << "saxpy IN " << alpha << " x " << *this << " + " << y << " + "<< beta << " +error " << ddelta << " / "<< B1 << B2 << B3 << B4 << std::endl;
	double temp, ttt, sss;
	int i;
//	std::cout << "in saxpy alpha=" << alpha  <<  "  beta= " <<  beta <<   "  delta = " << ddelta   << std::endl;
	if (is_actif()) {
		if (B1) {  // multiply by a scalar alpha
			if ((fabs(alpha)) < POS_INFINITY) {
				ttt= 0.0;
				sss= 0.0;
				for (i=0; i<=_n;i++) {
					_val[i] *= alpha;
					ttt += fabs(_val[i]);
					if (fabs(_val[i])<AF_EC()) {
						sss += fabs(_val[i]);
						_val[i] = 0.0;
					}
				}
				_err *= fabs(alpha);
				_err +=AF_EE()*(AF_EM()*ttt);
				_err += AF_EE()*sss;
			}
			else {
				_err = itv()*alpha;
				_n = -1;
				delete[] _val;
				_val = NULL;
			}
		}

		if (B2) {  // add a affine2 form y

			if (y.is_actif()) {
				if (_n==y.size()) {

					ttt=0.0;
					sss=0.0;
					for(i=0;i<=_n;i++) {
						temp = _val[i]+ y.val(i);
						ttt += (fabs(_val[i])>fabs(y.val(i)))? fabs(_val[i]) :fabs(y.val(i));
						if (fabs(temp)<AF_EC()) {
							sss+= fabs(temp);
							_val[i] = 0.0;
						}
						else {
							_val[i]=temp;
						}
					}
					_err += y.err();
					_err +=AF_EE()*(AF_EM()*ttt);
					_err += AF_EE()*sss;

				} else  {
					if (_n>y.size()) {
						return *this += y.itv();
					} else {
						Interval tmp1 = itv();
						*this = y;
						return *this += tmp1;
					}
				}
			}
			else { // y is not a valid affine2 form. So we add y.itv() such as an interval
				_err = itv()+y.itv();
				_n = -1;
				delete[] _val;
				_val = NULL;

			}
		}
		if (B3) {  //add a constant beta
			if ((fabs(beta))<POS_INFINITY) {
				ttt=0.0;
				sss=0.0;
				temp = _val[0]+ beta;
				ttt = (fabs(_val[0])>fabs(beta))? fabs(_val[0]) :fabs(beta);
				if (fabs(temp)<AF_EC()) {
					sss+= fabs(temp);
					_val[0] = 0.0;
				}
				else {
					_val[0]=temp;
				}
				_err +=AF_EE()*(AF_EM()*ttt);
				_err += AF_EE()*sss;
			}
			else {
				_err = itv()+beta;
				_n = -1;
				delete[] _val;
				_val = NULL;
			}
		}


		if (B4) {  // add an error  ddelta

			if ((fabs(ddelta))<POS_INFINITY) {
				_err += ddelta;
			}
			else {
				_err = itv()+Interval(-ddelta,ddelta);
				_n = -1;
				delete[] _val;
				_val = NULL;
			}
		}

		if (_val != NULL) {
			bool b = true;
			for (i=0;i<=_n;i++) {
				b &= (fabs(_val[i])<POS_INFINITY);
			}
			if (!b) {
				_err = Interval::ALL_REALS;
				_n = -1;
				delete[] _val;
				_val = NULL;
			}
		}

	} else {
		_n = -1;
		if (_val!=NULL) {
			delete[] _val;
			_val = NULL;
		}
		if (B1) {  //scalar alpha
			_err *= alpha;
		}
		if (B2) {  // add y
			_err += y.itv();
		}
		if (B3) {  //constant beta
			_err += beta;
		}
		if (B4) {  // error  delta
			_err += Interval(-ddelta,ddelta);
		}
	}
//	std::cout << " saxpy OUT x= "<< *this<<std::endl;
	return *this;

}

Affine2& Affine2::operator*=(const Affine2& y) {
//	std::cout << "in *= "<<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			double Sx=0.0, Sy=0.0, Sxy=0.0, Sz=0.0, ttt=0.0, sss=0.0, ppp=0.0, tmp=0.0, xVal0=0.0;
			int i;
			double * xTmp;



			xTmp = new double[_n + 1];

			for (i = 1; i <= _n; i++) {
				ppp = _val[i] * y.val(i);
				ttt += fabs(ppp);
				ttt += (fabs(ppp) > fabs(Sx )) ? fabs(ppp) : fabs(Sx );
				ttt += (fabs(ppp) > fabs(Sxy)) ? fabs(ppp) : fabs(Sxy);
				Sz += ppp;
				Sxy += fabs(ppp);
				ttt += (fabs( _val[i]) > fabs(Sx)) ? fabs( _val[i]) : fabs(Sx);
				ttt += (fabs(y.val(i)) > fabs(Sy)) ? fabs(y.val(i)) : fabs(Sy);
				Sx += fabs(_val[i]);
				Sy += fabs(y.val(i));

			}

			xVal0 = _val[0];
			// RES = X%T(0) * res
			for (i = 0; i <= _n; i++) {
				_val[i] *= y.val(0);
				ttt += fabs(_val[i]);
				if (fabs(_val[i]) < AF_EC()) {
					sss += fabs(_val[i]);
					_val[i] = 0.0;
				}
			}
			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (i = 1; i <= _n; i++) {
				xTmp[i] = xVal0 * y.val(i);
				ttt += fabs(xTmp[i]);
				if (fabs(xTmp[i]) < AF_EC()) {
					sss += fabs(xTmp[i]);
					xTmp[i] = 0.0;
				}
			}
			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (i = 0; i <= _n; i++) {
				tmp = _val[i] + xTmp[i];
				ttt += fabs(tmp);
				if (fabs(tmp) < AF_EC()) {
					sss += fabs(tmp);
					_val[i] = 0.0;
				} else
					_val[i] = tmp;
			}

			ttt += fabs(0.5 * Sz);
			ttt += (fabs(_val[0]) > fabs(0.5 * Sz)) ?
					fabs(_val[0]) : fabs(0.5 * Sz)  ;

			_val[0] += 0.5 * Sz;

			_err = (fabs(y.val(0)) * _err + fabs(xVal0) * y.err() - 0.5 * Sxy)
						+ (_err + Sx) * (y.err() + Sy);
			_err += AF_EE() * (AF_EM() * ttt);
			_err += AF_EE() * sss;

			{
				bool b = true;
				for (i=0;i<=_n;i++) {
					b &= (fabs(_val[i])<POS_INFINITY);
				}
				if (!b) {
					_err = Interval::ALL_REALS;
					_n = -1;
					delete[] _val;
					_val = NULL;
				}
			}
			delete[] xTmp;

		} else {
			if (size()>y.size()) {
				*this *= y.itv();
			} else {
				Interval tmp1 = itv();
				*this = y;
				*this *= tmp1;
			}
		}


	} else {
		_err = itv()*y.itv();
		_n = -1;
		if (_val!=NULL) {
			delete[] _val;
			_val = NULL;
		}
	}
//	std::cout << "out *= "<<std::endl;

	return *this;
}


Affine2& Affine2::operator*=(const Interval& y) {
	if (is_actif()&& (!(y.is_empty()||y.is_unbounded()))) {
		double  ttt=0.0, sss=0.0,  yVal0=0.0;
		int i;

		yVal0 = y.mid();
		// RES = X%(0) * res
		for (i = 0; i <= _n; i++) {
			_val[i] *= yVal0;
			ttt += fabs(_val[i]);
			if (fabs(_val[i]) < AF_EC()) {
				sss += fabs(_val[i]);
				_val[i] = 0.0;
			}
		}

		_err *= (fabs(yVal0)+Interval(y.rad()));
		_err += AF_EE() * (AF_EM() * ttt);
		_err += AF_EE() * sss;

		{
			bool b = true;
			for (i=0;i<=_n;i++) {
				b &= (fabs(_val[i])<POS_INFINITY);
			}
			if (!b) {
				_err = Interval::ALL_REALS;
				_n = -1;
				delete[] _val;
				_val = NULL;
			}
		}

	} else {
		_err = itv()*y;
		_n = -1;
		if (_val != NULL) {
			delete[] _val;
			_val = NULL;
		}
	}
	return *this;
}


Affine2& Affine2::sqr(const Interval itv) {
//	std::cout << "in sqr "<<std::endl;
	bool b = (!(itv.is_empty()||itv.is_unbounded()));
	if (!b) {
		_err = pow(itv,2);
		_n = -1;
		if (_val !=NULL) {
			delete[] _val;
			_val = NULL;
		}
	} else if (!is_actif()) {
		*this = pow(itv,2);
	} else  {  // _actif && b

		double Sx = 0, Sx2 = 0, ttt = 0, sss = 0, ppp = 0, x0 = 0;
		// compute the error
		for (int i = 1; i <= _n; i++) {
			ppp = _val[i] * _val[i];
			ttt += fabs(ppp) + (fabs(ppp) > fabs(Sx2) ? fabs(ppp) : fabs(Sx2));
			Sx2 += ppp;
			ttt += (fabs(_val[i]) > fabs(Sx) ? fabs(_val[i]) : fabs(Sx));
			Sx += fabs(_val[i]);
		}
		// compute 2*_val[0]*(*this)
		x0 = _val[0];
		_val[0] = x0 * x0;
		ttt += fabs(_val[0]);
		if (fabs(_val[0]) < AF_EC()) {
			sss += fabs(_val[0]);
			_val[0] = 0;
		}
		// compute 2*_val[0]*(*this)
		for (int i = 1; i <= _n; i++) {
			_val[i] = (2 * x0) * _val[i];
			ttt += fabs(_val[i]);
			if (fabs(_val[i]) < AF_EC()) {
				sss += fabs(_val[i]);
				_val[i] = 0;
			}
		}

		ttt += fabs(0.5 * Sx2) +( (fabs(_val[0]) > fabs(0.5 * Sx2)) ?
				fabs(_val[0]) : fabs(0.5 * Sx2) );
		_val[0] += 0.5 * Sx2;

		_err = (2 * fabs(x0) * _err - 0.5 * Sx2) + pow((_err + Sx), 2);
		_err += AF_EE() * (AF_EM() * ttt);
		_err += AF_EE() * sss;

		{
			bool b = true;
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_val[i])<POS_INFINITY);
			}
			if (!b) {
				_err = Interval::ALL_REALS;
				_n = -1;
				delete[] _val;
				_val = NULL;
			}
		}

	}
//	std::cout << "out sqr "<<std::endl;
	return *this;
}


Affine2& Affine2::power(int n, const Interval itv) {
//	std::cout << "in power "<<std::endl;
	bool b = (!(itv.is_empty()||itv.is_unbounded()));
	if (!b) {
		_err = pow(itv,n);
		_n = -1;
		if (_val !=NULL) {
			delete[] _val;
			_val = NULL;
		}

	} else if (!is_actif()) {
		*this = pow(itv,n);

	} else {
		if (n == 0) {
			*this = Interval::ONE;
			return *this;

		} else if (n == 1) {
			return *this;

		} else if (n == 2) {
			return ((*this).sqr(itv));

		} else if (n % 2 == 0) {

			double alpha, beta, ddelta, t1, t2;
			Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

			dmm = pow(itv, n);
			if (itv.diam() < AF_EC()) {
				alpha = 0.0;
				band = dmm;
			} else {
				alpha = (dmm.diam()
						/ (Interval(itv.ub()) - Interval(itv.lb()))).ub();
				if (alpha < 0)
					alpha = 0;
				//u = log(alpha);
				TEMP1 = dmm.lb() - alpha * (itv.lb());
				TEMP2 = dmm.ub() - alpha * (itv.ub());
				// u = (alpha/n)^(1/(n-1))
				if (TEMP1.ub() > TEMP2.ub()) {
					TEMP2 = Interval(alpha) / n;
					band = Interval(
							((1 - n) * TEMP2 * (root(TEMP2, n - 1))).lb(),
							TEMP1.ub());
				} else {
					TEMP1 = Interval(alpha) / n;
					band = Interval(
							((1 - n) * TEMP1 * (root(TEMP1, n - 1))).lb(),
							TEMP2.ub());
				}
			}

			beta = band.mid();
			t1 = (beta - band).ub();
			t2 = (band - beta).ub();
			ddelta = (t1 > t2) ? t1 : t2;

			saxpy(alpha, Affine2(), beta, ddelta, true, false, true, true);

		} else {
			/*  for _itv = [a,b]
			 * x0 = 1/sqrt(2)
			 * x1= - x0
			 * xb0 = 0.5*((b-a)*x0 +(a+b))
			 * xb1 = 0.5*((b-a)*x1 +(a+b))
			 * c0 = 0.5 (f(xb0)+f(xb1))
			 * c1 = x0*f(xb0)+x1*f(xb1)
			 * alpha = 2*c1/(b-a)
			 * beta = c0-c1*(a+b)/(b-a)
			 *  old : ddelta = (b-a)^2 * f''(_itv)/16
			 *  ddelta = evaluate the error at the bound and the points when f'(x)=alpha
			 */
			Interval x0, x1, xb0, xb1, fxb0, fxb1, c0, c1, TEMP1, TEMP2;
			double alpha, beta, ddelta, t1;

			x0 = 1.0 / sqrt(Interval(2));
			xb0 = Interval(0.5) * (itv.diam() * (x0) + itv.lb() + itv.ub());
			xb1 = Interval(0.5) * (itv.diam() * (-x0) + itv.lb() + itv.ub());
			c0 = Interval(0.5) * (pow(xb0, n) + pow(xb1, n));
			c1 = x0 * pow(xb0, n) - x0 * pow(xb1, n);
			x0 = pow(itv, n);

			TEMP1 = 2 * c1 / (itv.diam());
			TEMP2 = c0 - c1 * ((itv.lb() + itv.ub()) / (itv.diam()));

			alpha = TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n * Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();

			// compute the maximal error
			double max_error = 0.0;
			Interval period_0, nb_period;

			// compute the error at _itv.lb() and _itv.ub()
			max_error = (abs(
					pow(Interval(itv.lb()), n)
							- (alpha * Interval(itv.lb()) + beta))).ub();
			t1 = (abs(
					pow(Interval(itv.ub()), n)
							- (alpha * Interval(itv.ub()) + beta))).ub();
			if (t1 > max_error) max_error= t1 ;
			// u = (alpha/n)^(1/(n-1))
			TEMP2 = pow(Interval(alpha) / n, 1.0 / Interval(n - 1));
			if (!((TEMP2 & itv).is_empty())) {
				t1 = (abs(pow(TEMP2, n) - (alpha * TEMP2 + beta))).ub();
				if (t1 > max_error) max_error= t1 ;
			}
			if (!(((-TEMP2) & itv).is_empty())) {
				t1 = (abs(pow(-TEMP2, n) - (alpha * (-TEMP2) + beta))).ub();
				if (t1 > max_error) max_error= t1 ;
			}

			saxpy(alpha, Affine2(), beta, ddelta, true, false, true, true);
			saxpy(1.0, Affine2(), 0.0, max_error, false, false, false, true);

		}

	}
//	std::cout << "out power "<<std::endl;
	return *this;
}


Affine2& Affine2::linMinRange(affine2_expr num, const Interval itv) {

	Interval res_itv;
	switch (num) {
	case AF_SQRT :
		res_itv = sqrt(itv);
		break;
	case AF_EXP :
		res_itv = exp(itv);
		break;
	case AF_LOG :
		res_itv = log(itv);
		break;
	case AF_INV :
		res_itv = 1.0/itv;
		break;
	case AF_COS :
		res_itv = cos(itv);
		break;
	case AF_SIN :
		res_itv = sin(itv);
		break;
	case AF_TAN :
		res_itv = tan(itv);
		break;
	case AF_ABS :
		res_itv = abs(itv);
		break;
	case AF_ACOS :
		res_itv = acos(itv);
		break;
	case AF_ASIN :
		res_itv = asin(itv);
		break;
	case AF_ATAN :
		res_itv = atan(itv);
		break;
	case AF_COSH :
		res_itv = cosh(itv);
		break;
	case AF_SINH :
		res_itv = sinh(itv);
		break;
	case AF_TANH :
		res_itv = tanh(itv);
		break;
	default:
		ibex_error("Not implemented yet");
		break;
	}


	bool b = (!(res_itv.is_empty()||res_itv.is_unbounded()));
	if (!b) {
		_err = res_itv;
		_n = -1;
		if (_val !=NULL) {
			delete[] _val;
			_val = NULL;
		}
	} else if (!is_actif()) {
		*this =res_itv;


	} else  {  // _actif && b
		double alpha, beta, ddelta, t1, t2;
		Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0), itv2;

		switch (num) {
		/*  for _itv = [a,b]
		 * if f increase,
		 *  alpha = f'(a)
		 *  band = Interval(a- alpha*a , b-alpha*b)
		 * if f decrease
		 *  alpha = f'(b)
		 *  band = Interval(a- alpha*b , b-alpha*a)
		 *
		 *  beta = band.mid()
		 *  ddelta = band.rad()
		 */

		case AF_SQRT: {
			/*if ((itv.ub() == POS_INFINITY) || (itv.ub()<0) ) {
				_err = sqrt(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;
			}
			else { */
				if (itv.lb()<0) {
					itv2 = Interval(0.0,itv.ub());
				} else {
					itv2 =itv;
				}

				dmm = sqrt(itv2);
				if (itv2.diam()< AF_EC()) {
					alpha = 0.0;
					band =dmm;
				}
				else {
					alpha = (1.0/(2.0*dmm)).lb();  // compute the derivative
					if (alpha<=0) {
						alpha = 0.0;
						band = dmm;
					}
					else {
						TEMP1 = Interval(dmm.lb()) -alpha*itv2.lb();
						TEMP2 = Interval(dmm.ub()) -alpha*itv2.ub();
						if (TEMP1.lb()>TEMP2.ub()) {
							band = Interval(TEMP2.lb(),TEMP1.ub());
							// normally this case never happen
						}
						else {
							band = Interval(TEMP1.lb(),TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			//}
			break;
		}
		case AF_EXP : {

			if (itv.lb()<=NEG_INFINITY)  {
				*this = res_itv;
			}
			else {
				dmm = res_itv;
				if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band =dmm;
				}
				else {
					alpha = dmm.lb();  // compute the derivative
					if (alpha<=0) {
						alpha = 0.0;
						band = dmm;
					}
					else {
						TEMP1 = Interval(dmm.lb()) -alpha*itv.lb();
						TEMP2 = Interval(dmm.ub()) -alpha*itv.ub();
						if (TEMP1.lb()>TEMP2.ub()) {
							band = Interval(TEMP2.lb(),TEMP1.ub());
							// normally this case never happen
						}
						else {
							band = Interval(TEMP1.lb(),TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
			}

			break;
		}
		case AF_LOG : {

			/*if ((itv.lb()<= 0) ||(itv.ub()==POS_INFINITY) ) {
				_err = log(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;
			}
			else {*/

				dmm = res_itv;
				if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band =dmm;
				}
				else {
					alpha = (1.0/itv).lb();  // compute the derivative
					if (alpha<=0) {
						alpha = 0.0;
						band = dmm;
					}
					else {
						TEMP1 = Interval(dmm.lb()) -alpha*itv.lb();
						TEMP2 = Interval(dmm.ub()) -alpha*itv.ub();
						if (TEMP1.lb()>TEMP2.ub()) {
							band = Interval(TEMP2.lb(),TEMP1.ub());
							// normally this case never happen
						}
						else {
							band = Interval(TEMP1.lb(),TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			//}

			break;
		}
		case AF_INV : {

			if (itv.is_unbounded()) {
				*this= res_itv;
			}
			else {

				dmm = (1.0/abs(itv));
				if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = ((-1.0)/pow(itv,2)).ub(); // compute the derivative
					if (alpha<=0) {
						alpha = 0.0;
						band = dmm;
					}
					else {
						TEMP1 = Interval(dmm.lb()) -alpha*itv.ub();
						TEMP2 = Interval(dmm.ub()) -alpha*itv.lb();
						if (TEMP1.lb()>TEMP2.ub()) {
							band = Interval(TEMP2.lb(),TEMP1.ub());
							// normally this case never happens
						}
						else {
							band = Interval(TEMP1.lb(),TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				if (itv.lb()<0.0) beta = -beta;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			}
			break;
		}
		default :{
			linChebyshev(num, itv);
			break;
		}
		}

	}

	return *this;
}

Affine2& Affine2::linChebyshev(affine2_expr num, const Interval itv) {
	//  std::cout << "linChebyshev IN itv= "<<itv << " x =  "<< *this << num<< std::endl;

	Interval res_itv;
	switch (num) {
	case AF_SQRT :
		res_itv = sqrt(itv);
		break;
	case AF_EXP :
		res_itv = exp(itv);
		break;
	case AF_LOG :
		res_itv = log(itv);
		break;
	case AF_INV :
		res_itv = 1.0/itv;
		break;
	case AF_COS :
		res_itv = cos(itv);
		break;
	case AF_SIN :
		res_itv = sin(itv);
		break;
	case AF_TAN :
		res_itv = tan(itv);
		break;
	case AF_ABS :
		res_itv = abs(itv);
		break;
	case AF_ACOS :
		res_itv = acos(itv);
		break;
	case AF_ASIN :
		res_itv = asin(itv);
		break;
	case AF_ATAN :
		res_itv = atan(itv);
		break;
	case AF_COSH :
		res_itv = cosh(itv);
		break;
	case AF_SINH :
		res_itv = sinh(itv);
		break;
	case AF_TANH :
		res_itv = tanh(itv);
		break;
	default: {
		ibex_error("Not implemented yet");
		break;
	}
	}


	bool b = (!(res_itv.is_empty()||res_itv.is_unbounded()));
	if (!b) {
		_err = res_itv;
		_n = -1;
		if (_val !=NULL) {
			delete[] _val;
			_val = NULL;
		}
	} else if ((!is_actif())||(itv.diam()<AF_EC())) {
		*this =res_itv;
	}  else  { // _actif && b

		double alpha, beta, ddelta, t1, t2;
		Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

		switch (num) {
		/* alpha = (F(sup(x)) - F(inf(x)))/diam(X)
		 * u = (f')^{-1}(alpha)
		 * d_a = f(inf(x)) -alpha*inf(X)
		 * d_b = f(sup(x)) -alpha*sup(x)
		 * d_min = min(d_a,d_b)
		 * d_max = f(u) - alpha*u
		 * beta = Interval(d_min,d_max).mid()
		 * zeta = Interval(d_min,d_max).rad()
		 */
		case AF_SQRT: {
			/*if ((itv.ub() == POS_INFINITY) || (itv.ub()<0) ) {
				_err = sqrt(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;

			} else {*/
				Interval itv2;
				if (itv.lb()<0) {
					itv2 = Interval(0.0,itv.ub());
				} else {
					itv2 = itv;
				}
				dmm = sqrt(itv2);
			/*	if (itv2.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else */ {
					alpha = (dmm.diam()/(Interval(itv2.ub())-Interval(itv2.lb()))).lb();
					if (alpha<0) {
						alpha = 0;
						band = dmm;
					} else {
						//u = 1/(4*alpha^2);
						TEMP1 = Interval(dmm.lb())-alpha*(itv2.lb());
						TEMP2 = Interval(dmm.ub())-alpha*(itv2.ub());
						if (TEMP1.lb()>TEMP2.lb()) {
							band = Interval(TEMP2.lb(),(1.0/(4*Interval(alpha))).ub());
						} else {
							band = Interval(TEMP1.lb(),(1.0/(4*Interval(alpha))).ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			//}

			break;
		}
		case AF_EXP : {

			if (itv.is_unbounded()) {
				*this = res_itv;

			} else {

				dmm = res_itv;
				/*if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else */{
					alpha = (dmm.diam()/(Interval(itv.ub())-Interval(itv.lb()))).ub();
					if (alpha<0) {
						alpha = 0;
						band =dmm;
					} else {
						//u = log(alpha);
						TEMP1 = Interval(dmm.lb())-alpha*(itv.lb());
						TEMP2 = Interval(dmm.ub())-alpha*(itv.ub());
						if (TEMP1.ub()>TEMP2.ub()) {
							band = Interval((alpha*(1-log(Interval(alpha)))).lb(),TEMP1.ub());
						} else {
							band = Interval((alpha*(1-log(Interval(alpha)))).lb(),TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			}

			break;
		}
		case AF_LOG : {

		/*	if ((itv.lb()<= 0) ||(itv.ub()==POS_INFINITY) ) {
				_err = log(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;
			} else {*/

				dmm = res_itv;
				/*if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				} else */{
					alpha = (dmm.diam()/(Interval(itv.ub())-Interval(itv.lb()))).lb();
					if (alpha<0) {
						alpha = 0;
						band = dmm;
					} else {
						//u = 1/alpha;
						TEMP1 = Interval(dmm.lb())-alpha*(itv.lb());
						TEMP2 = Interval(dmm.ub())-alpha*(itv.ub());
						if (TEMP1.lb()>TEMP2.lb()) {
							band = Interval(TEMP2.lb(),(-log(Interval(alpha))-1).ub());
						}
						else {
							band = Interval(TEMP1.lb(),(-log(Interval(alpha))-1).ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			//}

			break;
		}
		case AF_INV : {
			if (itv.is_unbounded()) {
				*this = res_itv;
			}
			else {
				dmm = (1.0/abs(itv));
				/*if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else */ {
					alpha = -(dmm.diam()/(Interval(itv.ub())-Interval(itv.lb()))).lb();
					if (alpha>0) {
						alpha = 0;
						band = dmm;
					} else {
						//u = 1/sqrt(-alpha);
						TEMP1 = (1.0/Interval(abs(itv).lb()))-alpha*(abs(itv).lb());
						TEMP2 = (1.0/Interval(abs(itv).ub()))-alpha*(abs(itv).ub());
						if (TEMP1.ub()>TEMP2.ub()) {
							band = Interval((2*sqrt(-Interval(alpha))).lb(),TEMP1.ub());
						}
						else {
							band = Interval((2*sqrt(-Interval(alpha))).lb(),TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;
				if (itv.lb()<0.0) beta = -beta;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			}
			break;
		}
		case AF_COSH : {

			/*if (itv.is_unbounded()) {
				_err = cosh(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;
			}
			else { */

				dmm = res_itv;
				/*if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else */ {
					alpha = ((cosh(Interval(itv.ub()))-cosh(Interval(itv.lb())))/(Interval(itv.ub())-Interval(itv.lb()))).lb();
					//u = asinh(alpha);
					TEMP1 = Interval(cosh(itv.lb()))-alpha*(itv.lb());
					TEMP2 = Interval(cosh(itv.ub()))-alpha*(itv.ub());
					if (TEMP1.ub()>TEMP2.ub()) {
						// cosh(asinh(alpha)) = sqrt(sqr(alpha)+1)
						band = Interval((sqrt(pow(Interval(alpha),2)+1)-alpha*asinh(Interval(alpha))).lb(),TEMP1.ub());
					}
					else {
						band = Interval((sqrt(pow(Interval(alpha),2)+1)-alpha*asinh(Interval(alpha))).lb(),TEMP2.ub());
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			//}

			break;
		}
		case AF_ABS : {
			if (0<=itv.lb()) {
				return *this;
			}
			else if (itv.ub()<=0) {
				return *this = -Affine2(*this);
			}
		/*	else if (itv.is_unbounded() ) {
				_err = abs(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;
			}*/
			else {

				dmm = res_itv;
				/*if (itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else */{
					alpha = ((abs(Interval(itv.ub()))-abs(Interval(itv.lb())))/(Interval(itv.ub())-Interval(itv.lb()))).ub();
					if (alpha<0) alpha = 0;
					//u = log(alpha);
					TEMP1 = Interval(dmm.lb())-alpha*(itv.lb());
					TEMP2 = Interval(dmm.ub())-alpha*(itv.ub());
					if (TEMP1.ub()>TEMP2.ub()) {
						// u = 0
						band = Interval(0.0,TEMP1.ub());
					}
					else {
						band = Interval(0.0,TEMP2.ub());
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			}

			break;
		}
	// now the other non-convex or concave function
	// trigo function
		case AF_TAN :
			/*if (tan(itv).is_unbounded()) {
				_err = tan(itv);
				_actif = false;
				delete[] _val;
				_val = NULL;
				break;
			}*/
		case AF_COS :
		case AF_SIN : {
			if (itv.diam()>=Interval::TWO_PI.lb()) {
				for (int i =0; i<=_n;i++)  { _val[i] = 0.0;}
				_err = 1.0;
				break;
			}
			/*  pour _itv = [a,b]
			 * x0 = 1/sqrt(2)
			 * x1= - x0
			 * xb0 = 0.5*((b-a)*x0 +(a+b))
			 * xb1 = 0.5*((b-a)*x1 +(a+b))
			 * c0 = 0.5 (f(xb0)+f(xb1))
			 * c1 = x0*f(xb0)+x1*f(xb1)
			 * alpha = 2*c1/(b-a)
			 * beta = c0-c1*(a+b)/(b-a)
			 *  old : ddelta = (b-a)^2 * f''(_itv)/16
			 *  ddelta = evaluate the error at the bound and the points when f'(x)=alpha
			 */

			Interval x0,x1,xb0,xb1,fxb0,fxb1,c0,c1;

			x0 = 1.0/sqrt(Interval(2));
			xb0 = Interval(0.5)*(itv.diam()*x0 +itv.lb()+itv.ub());
			xb1 = Interval(0.5)*(itv.diam()*(-x0) +itv.lb()+itv.ub());

			switch (num) {
			case AF_COS :
				c0 = Interval(0.5)*(cos(xb0)+cos(xb1));
				c1 = x0*cos(xb0)-x0*cos(xb1);
				x0 = cos(itv);
				break;
			case AF_SIN :
				c0 = Interval(0.5)*(sin(xb0)+sin(xb1));
				c1 = x0*sin(xb0)-x0*sin(xb1);
				x0 = sin(itv);
				break;
			case AF_TAN :
				c0 = Interval(0.5)*(tan(xb0)+tan(xb1));
				c1 = x0*tan(xb0)-x0*tan(xb1);
				x0 = tan(itv);
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}

			TEMP1 = 2*c1/(itv.diam());
			TEMP2 = c0-c1*((itv.lb()+itv.ub())/(itv.diam()));

			alpha= TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n*Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();

			// compute the maximal error
			double max_error= 0.0;
			Interval u,nb_period;

			// compute the error at _itv.lb() and _itv.ub() and compute the first point such as f'(u) = alpha
			switch (num) {
			case AF_COS :
				max_error = (abs(cos(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(cos(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				if (t1>max_error)  max_error= t1;
				u = asin(-Interval(alpha));
				nb_period = (itv+Interval::HALF_PI) / Interval::PI;
				break;
			case AF_SIN :
				max_error = (abs(sin(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(sin(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				if (t1>max_error)  max_error= t1;
				u = acos(Interval(alpha));
				nb_period = (itv) / Interval::PI;
				break;
			case AF_TAN :
				max_error = (abs(tan(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(tan(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				if (t1>max_error)  max_error= t1;
				u = acos(1/sqrt(Interval(alpha)));
				nb_period = (itv) / Interval::PI;
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}

			// evaluate the error at the point such as f'(u) = alpha

			int p1 = ((int) nb_period.lb())-2;
			int p2 = ((int) nb_period.ub())+2;

			int i = p1;
			switch(num) {
			case AF_COS :
				while (i<=p2) { // looking for a point
					TEMP1 = (itv & (i%2==0? (u + i*Interval::PI) : (i*Interval::PI - u)));
					if (!(TEMP1.is_empty())) { // check if maximize the error
						t1 = (abs(cos(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					i++;
				}
				break;
			case AF_SIN :
				while (i<=p2) { // looking for a point
					TEMP1 = (itv & (i%2==0? (u + i*Interval::PI) : ((i+1)*Interval::PI - u)));
					if (!(TEMP1.is_empty())) {
						t1 = (abs(sin(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					i++;
				}
				break;
			case AF_TAN :
				while (i<=p2) { // looking for a point
					TEMP1 = (itv & ( i*Interval::PI + u));
					if ((!(TEMP1.is_empty()))) {
						t1 = (abs(tan(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					TEMP1 = (itv & ( i*Interval::PI - u ));
					if ((!(TEMP1.is_empty()))) {
						t1 = (abs(tan(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					i++;
				}
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}


			saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
			saxpy(1.0, Affine2(), 0.0, max_error, false,false,false,true);

			break;
		}
	// inverse function and
	// hyperbolic function

		case AF_ACOS :
		case AF_ASIN :
			if ((itv.lb() < (-1))||(itv.ub() > 1)) {
				return linChebyshev(num,(itv & Interval(-1,1)));
			}
		case AF_TANH :
		case AF_ATAN :
			if (itv.is_unbounded() ) {
				return *this = res_itv ;
			}
		case AF_SINH : {
			/*if (itv.is_unbounded()|| itv.is_empty()) {
				_actif = false;
				delete[] _val;
				_val = NULL;
				switch(num) {
				case AF_ACOS :
					_err = acos(itv);
					break;
				case AF_ASIN :
					_err = asin(itv);
					break;
				case AF_ATAN :
					_err = atan(itv);
					break;
				case AF_TANH :
					_err = tanh(itv);
					break;
				case AF_SINH :
					_err = sinh(itv);
					break;
				default:
					ibex_error("this should not appear");
					break;
				}
				break;
			} */
			/*  pour _itv = [a,b]
			 * x0 = 1/sqrt(2)
			 * x1= - x0
			 * xb0 = 0.5*((b-a)*x0 +(a+b))
			 * xb1 = 0.5*((b-a)*x1 +(a+b))
			 * c0 = 0.5 (f(xb0)+f(xb1))
			 * c1 = x0*f(xb0)+x1*f(xb1)
			 * alpha = 2*c1/(b-a)
			 * beta = c0-c1*(a+b)/(b-a)
			 *  old : ddelta = (b-a)^2 * f''(_itv)/16
			 *  ddelta = evaluate the error at the bound and the points when f'(x)=alpha
			 */
			Interval x0,x1,xb0,xb1,fxb0,fxb1,c0,c1;

			x0 = 1.0/sqrt(Interval(2));
			xb0 = Interval(0.5)*(itv.diam()*x0 +itv.lb()+itv.ub());
			xb1 = Interval(0.5)*(itv.diam()*(-x0) +itv.lb()+itv.ub());
			switch (num) {
			case AF_SINH :
				c0 = Interval(0.5)*(sinh(xb0)+sinh(xb1));
				c1 = x0*sinh(xb0)-x0*sinh(xb1);
				x0 = sinh(itv);
				break;
			case AF_TANH :
				c0 = Interval(0.5)*(tanh(xb0)+tanh(xb1));
				c1 = x0*tanh(xb0)-x0*tanh(xb1);
				x0 = tanh(itv);
				break;
			case AF_ATAN :
				c0 = Interval(0.5)*(atan(xb0)+atan(xb1));
				c1 = x0*atan(xb0)-x0*atan(xb1);
				x0 = atan(itv);
				break;
			case AF_ACOS :
				c0 = Interval(0.5)*(acos(xb0)+acos(xb1));
				c1 = x0*acos(xb0)-x0*acos(xb1);
				x0 = acos(itv);
				break;
			case AF_ASIN :
				c0 = Interval(0.5)*(asin(xb0)+asin(xb1));
				c1 = x0*asin(xb0)-x0*asin(xb1);
				x0 = asin(itv);
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}
			TEMP1 = 2*c1/(itv.diam());
			TEMP2 = c0-c1*((itv.lb()+itv.ub())/(itv.diam()));

			alpha= TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n*Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();

			// compute the maximal error
			double max_error= 0.0;

			// compute the error at _itv.lb(), _itv.ub() and u such as f'(u) =alpha
			switch (num) {
			case AF_SINH :
				max_error = (abs(sinh(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(sinh(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				// u = acosh(alpha)
				TEMP2 = acosh(Interval(alpha));
				if (!((TEMP2 & itv).is_empty())) {
					// sinh(acosh(x)) = sqrt(sqr(x)-1)
					t1 = (abs(sqrt(pow(Interval(alpha),2)-1)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & itv).is_empty())) {
					// sinh(acosh(-x)) = -sqrt(sqr(x)-1)
					t1 = (abs((-sqrt(pow(Interval(alpha),2)-1))-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_TANH :
				max_error = (abs(tanh(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(tanh(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* tanh'(u)=alpha
				 * cosh(u)= -2/alpha -1
				 * u = +-acosh(-2/alpha -1)
				 */
				TEMP2 = acosh(-2/Interval(alpha) -1);
				if (!((TEMP2 & itv).is_empty())) {
					t1 = (abs(tanh(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & itv).is_empty())) {
					t1 = (abs(tanh(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_ATAN :
				max_error = (abs(atan(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(atan(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* atan'(u)=1/(u^2+1) = alpha
				 * u = +-sqrt(1/alpha -1)
				 */
				TEMP2 = sqrt(1/Interval(alpha)-1);
				if (!((TEMP2 & itv).is_empty())) {
					t1 = (abs(atan(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & itv).is_empty())) {
					t1 = (abs(atan(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_ACOS :
				max_error = (abs(acos(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(acos(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* acos'(u)=-1/sqrt(1-u^2) = alpha
				 * u = +-sqrt(1-1/(alpha^2))
				 */
				TEMP2 = sqrt(1-1/(pow(Interval(alpha),2)));
				if (!((TEMP2 & itv).is_empty())) {
					t1 = (abs(acos(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & itv).is_empty())) {
					t1 = (abs(acos(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_ASIN :
				max_error = (abs(asin(Interval(itv.lb()))-(alpha*Interval(itv.lb())+beta))).ub();
				t1 = (abs(asin(Interval(itv.ub()))-(alpha*Interval(itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* asin'(u)=1/sqrt(1-u^2) = alpha
				 * u = sqrt(1-1/(alpha^2))
				 */
				TEMP2 = sqrt(1/Interval(alpha)-1);
				if (!((TEMP2 & itv).is_empty())) {
					// tanh(acosh(x)) = sqrt(sqr(x)-1) :x
					t1 = (abs(asin(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & itv).is_empty())) {
					// sinh(acosh(-x)) = -sqrt(sqr(x)-1)
					t1 = (abs(asin(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}

			saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
			saxpy(1.0, Affine2(), 0.0, max_error, false,false,false,true);

			break;
		}
		default : {
			ibex_error("Not implemented yet");
			break;
		}
		}

	}

//std::cout << "linChebyshev OUT x =  "<< *this << num<< std::endl;
	return *this;
}


std::ostream& operator<<(std::ostream& os, const Affine2& x) {
	{
		os << x.itv() << " : ";
		if (x.is_actif()) {
			os << x.val(0);
			for (int i = 1; i <= x.size(); i++) {
				os << " + " << x.val(i) << " eps_" << i;
			}
			os << " + " << x.err() << "x[-1,1] ";
		} else {
			os << "Affine2 form not Activate ";
		}
		return os;

	}
}

Affine2 pow(const Affine2& x, int n, const Interval itv) { 	// std::cout << "in pow "<<std::endl;
	if (n == 0)
		return Affine2(1.0);
	else if (n == 1)
		return Affine2(x);
	else if (n == 2)
		return sqr(x,itv);
	else if (n<0)
		return inv(Affine2(x).power(-n,itv),pow(itv,-n));
	else
		return Affine2(x).power(n,itv);
}


Affine2 pow(const Affine2& x, double d, const Interval itv) {
	if ( ((int) (d)) == d) {
		return pow(x, (int) (d),itv);
	} else if (d<0)
		return inv(pow(x,Interval(-d), itv),pow(itv,-d));
	else
		return pow(x, Interval(d),itv);
}


Affine2 root(const Affine2& x, int n, const Interval itv) {

	if (x.is_empty()) return Affine2(Interval::EMPTY_SET);
	else if (n==0) return Affine2(Interval::ONE);
	else if (n==1) return x;
	else if (n<0) return inv(root(x,-n,itv),root(itv,-n));
	else if (x.is_degenerated()) return Affine2(pow(Interval(x.val(0)),1.0/n));
	else if (n % 2 == 0) return pow(x, Interval::ONE/n,itv); // the negative part of x should be removed
	else if (0 <= itv.lb()) return  pow(x, Interval::ONE/n,itv);
	else if (itv.ub() <= 0) return  -pow(-x, Interval::ONE/n,-itv);
	else {
		// TODO do the root when x contains ZERO more properly
		//ibex_error("warning: Affine2 ROOT non completely well implemented yet");
		//		y=pow(x,e) |  // the negative part of x should be removed
		//	    (-pow(-x,e)); // the positive part of x should be removed
		return Affine2( pow(x, Interval::ONE/n,itv)| (-pow(-x,Interval::ONE/n,-itv)));  // BE CAREFULL the result of this union is an INTERVAL, so y lost all its affine form
	}

}



Affine2 sign(const Affine2& x, const Interval itv) {
	if (itv.lb() > 0) {
		return Affine2(1.0);
	} else if (itv.ub() < 0) {
		return Affine2(-1.0);
	} else {
		return Affine2(Interval(-1.0, 1.0));
	}
}

}// end namespace ibex

