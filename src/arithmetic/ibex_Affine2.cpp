/*
 * ibex_Affine2.cpp
 *
 *  Created on: 12 nov. 2012
 *      Author: nininjo
 */

#include "ibex_Affine2.h"

namespace ibex {


Affine2::Affine2() :
		 _n		(-1		),
		 _val	(NULL	),
		 _err	(Interval::ALL_REALS),
		 _actif	(false	),
		 _itv	(Interval::ALL_REALS) {
 }

Affine2::Affine2(int n) :
		_n		(n),
		_val	(new double[n + 1]),
		_err	(Interval::ALL_REALS),
		_actif	(true),
		_itv	(Interval::ALL_REALS) {
}

Affine2::Affine2(int n, int m, const Interval& itv) :
		_n		(n),
		_val	(new double[n + 1]),
		_err	(Interval::ZERO),
		_actif	(true),
		_itv	(itv) {

	_val[0] = itv.mid();
	for (int i = 1; i <= n; i++){
		_val[i] = 0.0;
	}

	if (m == 0) {
		_err += itv.rad();
	} else {
		_val[m] = itv.rad();
	}
}


Affine2::Affine2(int n, const double d) :
		_n		(n),
		_val	(new double[n + 1]),
		_err	(Interval::ZERO),
		_actif	(true),
		_itv	(Interval::ZERO) {

	_val[0] = d;
	for (int i = 1; i <= n; i++){
		_val[i] = 0.0;
	}
	_itv = d;
}

Affine2::Affine2(int n, const Interval & itv) :
		_n		(n),
		_val	(new double[n + 1]),
		_err	(itv.rad()),
		_actif	(true),
		_itv	(itv) {
	assert(n>=0);
	_val[0] = itv.mid();
	for (int i = 1; i <= n; i++){
		_val[i] = 0.0;
	}
}

Affine2::Affine2(const Affine2& x) :
		_n		(x.size()),
		_val	(new double[x.size() + 1]),
		_err	(x.err()),
		_actif	(x.isActif()),
		_itv	(x.itv()) {
	if (x.isActif()) {
		for (int i = 0; i <= x.size(); i++){
			_val[i] = x.val(i);
		}
	}
}

Affine2::Affine2(const Affine2& x, bool b) :
		_n		(x.size()),
		_val	(new double[x.size() + 1]),
		_err	(x.err()),
		_actif	(x.isActif()) {
	if (b) {
		_itv = -x.itv();
		if (x.isActif()) {
			for (int i = 0; i <= x.size(); i++){
				_val[i] = -x.val(i);
			}
		}
	} else {
		_itv = x.itv();
		if (x.isActif()) {
			for (int i = 0; i <= x.size(); i++){
				_val[i] = x.val(i);
			}
		}
	}
}


void Affine2::resize(int n) {
	assert(n>=0);
	if (n==_n) return;
	double* newVec=new double[n+1];
	int i=0;
	for (; (i<(_n+1)) && (i<(n+1)); i++) {
		newVec[i]=_val[i];
	}
	for (; i<n; i++){
		newVec[i]=0;
	}
	if (_val!=NULL) {// vec==NULL happens when default constructor is used (n==0)
		delete[] _val;
	}
	_n   = n;
	_val = newVec;
}

void Affine2::update_itv() {
	if (_actif && (_val!=NULL)) {
		Interval res(_val[0]);
		Interval pmOne(-1.0, 1.0);
		for (int i = 1; i <= _n; i++){
			res += (_val[i] * pmOne);
		}
		res += _err * pmOne;
		_itv &= res;
		_actif = !(_itv.is_empty());
	}
}

Affine2& Affine2::operator =(const Affine2& x) {
	if (this != &x) {
		if (_n!=x.size()) {
			if (_val!=NULL) { delete[] _val; }
			_n =x.size();
			_val = new double[x.size()+1];
		}
		_actif = x.isActif();
		_err = x.err();
		_itv = x.itv();
		if (_actif) {
			for (int i = 0; i <= x.size(); i++) {
				_val[i] = x.val(i);
			}
		}
	}
	return *this;
}

Affine2& Affine2::operator =(double d) {
	_actif = true;
	_err = Interval::ZERO;
	_itv = Interval(d);

	if ((_val==NULL)||(_n<0)) {
		_n = 0;
		_val = new double[1];
	}

	_val[0] = d;
	for (int i = 1; i <= _n; i++) {
		_val[i] = 0;
	}
	return *this;
}

Affine2& Affine2::operator =(const Interval& x) {
	_itv = x;
	_actif = (!x.is_empty());
	if (_actif) {
		if ((_val==NULL)||(_n<0)) {
			_n = 0;
			_val = new double[1];
		}
		_val[0] = x.mid();
		for (int i = 1; i <= _n; i++) {
			_val[i] = 0;
		}
		_err = x.rad();
	}
	return *this;
}



Affine2& Affine2::saxpy(double alpha, const Affine2& y, double beta,
		double ddelta, bool B1, bool B2, bool B3, bool B4) {
//std::cout << "saxpy " << alpha << " x " << *this << " + " << y << " + "<< beta << " +error " << ddelta << " / "<< B1 << B2 << B3 << B4 << std::endl;
	double temp, ttt, sss;
	int i;

	if (_actif) {
		if (B1) {  // multiply by a scalar alpha
			_itv *= alpha;
			if ((fabs(alpha)) < POS_INFINITY) {
				ttt= 0.0;
				sss= 0.0;
				for (i=0; i<=_n;i++) {
					_val[i] *= alpha;
					ttt += fabs(_val[i]);
					if (fabs(_val[i]<AF_EC())) {
						sss += fabs(_val[i]);
						_val[i] = 0.0;
					}
				}
				_err *= fabs(alpha);
				_err +=AF_EE()*(AF_EM()*ttt);
				_err += AF_EE()*sss;
			}
			else {
				_actif = false;
			}
		}

		if (B2) {  // add a affine2 form y

			_itv += y.itv();
			if (y.isActif()) {
				assert(this->size()==y.size());

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
			}
			else { // y is not a valid affine2 form. So we add y.itv() such as an interval
				_itv += y.itv();
				if (y.itv().is_unbounded()) {
					ttt=0.0;
					sss=0.0;
					double tmp_mid = y.itv().mid();
					temp = _val[0]+ tmp_mid;
					ttt = (fabs(_val[0])>fabs(tmp_mid))? fabs(_val[0]) :fabs(tmp_mid);
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
				_err += y.rad();
			}
		}
		if (B3) {  //add a constant beta
			_itv += beta;
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
				_actif = false;
			}
		}
		if (B4) {  // add an error  ddelta
			_itv += Interval(-ddelta,ddelta);
			if ((fabs(ddelta))<POS_INFINITY) {
				_err += ddelta;
			}
			else {
				_actif = false;
			}
		}

		update_itv();  // update _itv

		if (_actif) {
			for (i=0;i<=_n;i++) {
				_actif &= (fabs(_val[i])<POS_INFINITY);
			}
		}

	} else {
		if (B1) {  //scalar alpha
			_itv *= alpha;
		}
		if (B2) {  // add y
			_itv += y.itv();
		}
		if (B3) {  //constant beta
			_itv += beta;
		}
		if (B4) {  // error  delta
			_itv += Interval(-ddelta,ddelta);
		}
	}
	return *this;

}

Affine2& Affine2::operator *=(const Affine2& y) {


	if (_actif && (y.isActif())) {

		double Sx=0.0, Sy=0.0, Sxy=0.0, Sz=0.0, ttt=0.0, sss=0.0, ppp=0.0, tmp=0.0, xVal0=0.0;
		int i;
		double * xTmp;

		assert(_n==y.size());

		xTmp = new double[_n + 1];

		for (i = 1; i <= _n; i++) {
			ppp = _val[i] * y.val(i);
			ttt += fabs(ppp);
			ttt += (fabs(ppp) > fabs(Sx)) ? fabs(ppp) : fabs(Sx);
			ttt += (fabs(ppp) > fabs(Sxy)) ? fabs(ppp) : fabs(Sxy);
			Sz += ppp;
			Sxy += fabs(ppp);
			ttt += (fabs(_val[i]) > fabs(Sx)) ? fabs(_val[i]) : fabs(Sx);
			ttt += (fabs(y.val(i)) > fabs(Sy)) ? fabs(y.val(i)) : fabs(Sy);
			Sx += fabs(_val[i]);
			Sy += fabs(y.val(i));

		}

		xVal0 = _val[0];
		// RES = X%(0) * res
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
		//RES = Xtmp + X = Y%(0) * X + X%T(0) * Y - X%T(0)*Y%(0)
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
				fabs(_val[0]) : fabs(0.5 * Sz);

		_val[0] += 0.5 * Sz;

		_err = (fabs(y.val(0)) * _err + fabs(xVal0) * y.err() - 0.5 * Sxy)
				+ (_err + Sx) * (y.err() + Sy);
		_err += AF_EE() * (AF_EM() * ttt);
		_err += AF_EE() * sss;

		_itv *= y.itv();
		update_itv();

		if (_actif)  {
			for (i = 0; i <= _n; i++) {
				_actif &= (fabs(_val[i]) < POS_INFINITY);
			}
		}
		delete[] xTmp;
	} else {
		*this *= y.itv();
	}
	return *this;
}


Affine2& Affine2::sqr() {

	if (_actif) {

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

		_itv = pow(_itv,2);
		update_itv();

		if (_actif) {
			for (int i = 0; i <= _n; i++) {
				_actif &= (fabs(_val[i]) < POS_INFINITY);
			}
		}

	} else {
		_itv = pow(_itv,2);
	}
	return *this;
}

Affine2& Affine2::power(int n) {

	if (_itv.is_unbounded()) {
		_actif = false;
		_itv = pow(_itv, n);

	} else if (!_actif) {
		_itv = pow(_itv, n);

	} else {
		if (n == 0) {
			*this = Interval::ONE;
			return *this;

		} else if (n == 1) {
			return *this;

		} else if (n == 2) {
			return ((*this).sqr());

		} else if (n % 2 == 0) {

			double alpha, beta, ddelta, t1, t2;
			Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

			dmm = pow(_itv, n);
			if (_itv.diam() < AF_EC()) {
				alpha = 0.0;
				band = dmm;
			} else {
				alpha = (dmm.diam()
						/ (Interval(_itv.ub()) - Interval(_itv.lb()))).ub();
				if (alpha < 0)
					alpha = 0;
				//u = log(alpha);
				TEMP1 = dmm.lb() - alpha * (_itv.lb());
				TEMP2 = dmm.ub() - alpha * (_itv.ub());
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
			_itv &= dmm;


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
			xb0 = Interval(0.5) * (_itv.diam() * (x0) + _itv.lb() + _itv.ub());
			xb1 = Interval(0.5) * (_itv.diam() * (-x0) + _itv.lb() + _itv.ub());
			c0 = Interval(0.5) * (pow(xb0, n) + pow(xb1, n));
			c1 = x0 * pow(xb0, n) - x0 * pow(xb1, n);
			x0 = pow(_itv, n);

			TEMP1 = 2 * c1 / (_itv.diam());
			TEMP2 = c0 - c1 * ((_itv.lb() + _itv.ub()) / (_itv.diam()));

			alpha = TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n * Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();

			// compute the maximal error
			double max_error = 0.0;
			Interval period_0, nb_period;

			// compute the error at _itv.lb() and _itv.ub()
			max_error = (abs(
					pow(Interval(_itv.lb()), n)
							- (alpha * Interval(_itv.lb()) + beta))).ub();
			t1 = (abs(
					pow(Interval(_itv.ub()), n)
							- (alpha * Interval(_itv.ub()) + beta))).ub();
			if (t1 > max_error) max_error= t1 ;
			// u = (alpha/n)^(1/(n-1))
			TEMP2 = pow(Interval(alpha) / n, 1.0 / Interval(n - 1));
			if (!((TEMP2 & _itv).is_empty())) {
				t1 = (abs(pow(TEMP2, n) - (alpha * TEMP2 + beta))).ub();
				if (t1 > max_error) max_error= t1 ;
			}
			if (!(((-TEMP2) & _itv).is_empty())) {
				t1 = (abs(pow(-TEMP2, n) - (alpha * (-TEMP2) + beta))).ub();
				if (t1 > max_error) max_error= t1 ;
			}

			saxpy(alpha, Affine2(), beta, ddelta, true, false, true, true);
			saxpy(1.0, Affine2(), 0.0, max_error, false, false, false, true);
			_itv &= x0;
		}

		_actif &= (!(_itv.is_unbounded()) && !(_itv.is_empty()));

	}
	return *this;
}


Affine2& Affine2::linMinRange(affine2_expr num) {

	if (_actif) {
		double alpha, beta, ddelta, t1, t2;
		Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

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
			if ((_itv.ub() == POS_INFINITY) || (_itv.ub()<0) ) {
				_actif = false;
				_itv = sqrt(_itv);
			}
			else {
				if (_itv.lb()<0) {
					_itv = Interval(0.0,_itv.ub());
				}
				dmm = sqrt(_itv);
				if (_itv.diam()< AF_EC()) {
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
						TEMP1 = Interval(dmm.lb()) -alpha*_itv.lb();
						TEMP2 = Interval(dmm.ub()) -alpha*_itv.ub();
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
				_itv &= dmm;
			}
			break;
		}
		case AF_EXP : {

			if (_itv.is_unbounded())  {
				_actif = false;
				_itv = exp(_itv);
			}
			else {
				dmm = exp(_itv);
				if (_itv.diam()< AF_EC()) {
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
						TEMP1 = Interval(dmm.lb()) -alpha*_itv.lb();
						TEMP2 = Interval(dmm.ub()) -alpha*_itv.ub();
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
				_itv &= dmm;
			}

			break;
		}
		case AF_LOG : {

			if ((_itv.lb()<= 0) ||(_itv.ub()==POS_INFINITY) ) {
				_actif = false;
				_itv = log(_itv);
			}
			else {

				dmm = log(_itv);
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band =dmm;
				}
				else {
					alpha = (1.0/_itv).lb();  // compute the derivative
					if (alpha<=0) {
						alpha = 0.0;
						band = dmm;
					}
					else {
						TEMP1 = Interval(dmm.lb()) -alpha*_itv.lb();
						TEMP2 = Interval(dmm.ub()) -alpha*_itv.ub();
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
				_itv &= dmm;
			}

			break;
		}
		case AF_INV : {

			if (_itv.contains(0.0)||(_itv.is_unbounded()) ) {
				_actif = false;
				_itv = 1.0/_itv;
			}
			else {

				dmm = (1.0/abs(_itv));
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = ((-1.0)/pow(_itv,2)).ub(); // compute the derivative
					if (alpha<=0) {
						alpha = 0.0;
						band = dmm;
					}
					else {
						TEMP1 = Interval(dmm.lb()) -alpha*_itv.ub();
						TEMP2 = Interval(dmm.ub()) -alpha*_itv.lb();
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

				if (_itv.lb()<0.0) beta = -beta;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
				_itv &= ((dmm.lb()>= 0.0)? dmm : (-dmm));
			}
			break;
		}
		default :{
			linChebyshev(num);
			break;
		}
		}

	} else {
		switch (num) {
		case AF_SQRT :
			_itv = sqrt(_itv);
			break;
		case AF_EXP :
			_itv = exp(_itv);
			break;
		case AF_LOG :
			_itv = log(_itv);
			break;
		case AF_INV :
			_itv = 1.0/_itv;
			break;
		case AF_COS :
			_itv = cos(_itv);
			break;
		case AF_SIN :
			_itv = sin(_itv);
			break;
		case AF_TAN :
			_itv = tan(_itv);
			break;
		case AF_ABS :
			_itv = abs(_itv);
			break;
		case AF_ACOS :
			_itv = acos(_itv);
			break;
		case AF_ASIN :
			_itv = asin(_itv);
			break;
		case AF_ATAN :
			_itv = atan(_itv);
			break;
		case AF_COSH :
			_itv = cosh(_itv);
			break;
		case AF_SINH :
			_itv = sinh(_itv);
			break;
		case AF_TANH :
			_itv = tanh(_itv);
			break;
		default:
			ibex_error("Not implemented yet");
			break;
		}
	}
	_actif &= (!(_itv.is_unbounded()) && !(_itv.is_empty()));
	return *this;
}

Affine2& Affine2::linChebyshev(affine2_expr num) {

	if (_actif) {
		double alpha, beta, ddelta, t1, t2;
		Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

		switch (num) {
		/* alpha = diam(F(X))/diam(X)
		 * u = (f')^{-1}(alpha)
		 * d_a = f(inf(x)) -alpha*inf(X)
		 * d_b = f(sup(x)) -alpha*sup(x)
		 * d_min = min(d_a,d_b)
		 * d_max = f(u) - alpha*u
		 * beta = Interval(d_min,d_max).mid()
		 * zeta = Interval(d_min,d_max).rad()
		 */
		case AF_SQRT: {
			if ((_itv.ub() == POS_INFINITY) || (_itv.ub()<0) ) {
				_actif = false;
				_itv = sqrt(_itv);

			} else {
				if (_itv.lb()<0) {
					_itv = Interval(0.0,_itv.ub());
				}
				dmm = sqrt(_itv);
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = (dmm.diam()/(Interval(_itv.ub())-Interval(_itv.lb()))).lb();
					if (alpha<0) {
						alpha = 0;
						band = dmm;
					} else {
						//u = 1/(4*alpha^2);
						TEMP1 = Interval(dmm.lb())-alpha*(_itv.lb());
						TEMP2 = Interval(dmm.ub())-alpha*(_itv.ub());
						if (TEMP1.lb()>TEMP2.lb()) {
							band = Interval(TEMP2.lb(),(1.0/(2*Interval(alpha))).ub());
						} else {
							band = Interval(TEMP1.lb(),(1.0/(2*Interval(alpha))).ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
				_itv &= dmm;
			}

			break;
		}
		case AF_EXP : {

			if (_itv.is_unbounded()) {
				_actif = false;
				_itv = exp(_itv);

			} else {

				dmm = exp(_itv);
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = (dmm.diam()/(Interval(_itv.ub())-Interval(_itv.lb()))).ub();
					if (alpha<0) {
						alpha = 0;
						band =dmm;
					} else {
						//u = log(alpha);
						TEMP1 = Interval(dmm.lb())-alpha*(_itv.lb());
						TEMP2 = Interval(dmm.ub())-alpha*(_itv.ub());
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
				_itv &= dmm;
			}

			break;
		}
		case AF_LOG : {

			if ((_itv.lb()<= 0) ||(_itv.ub()==POS_INFINITY) ) {
				_actif = false;
				_itv = log(_itv);
			} else {

				dmm = log(_itv);
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				} else {
					alpha = (dmm.diam()/(Interval(_itv.ub())-Interval(_itv.lb()))).lb();
					if (alpha<0) {
						alpha = 0;
						band = dmm;
					} else {
						//u = 1/alpha;
						TEMP1 = Interval(dmm.lb())-alpha*(_itv.lb());
						TEMP2 = Interval(dmm.ub())-alpha*(_itv.ub());
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
				_itv &= dmm;
			}

			break;
		}
		case AF_INV : {

			if ((_itv.contains(0.0))||(_itv.is_unbounded()) ) {
				_actif = false;
				_itv = 1.0/_itv;
			}
			else {
				dmm = (1.0/abs(_itv));
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = -(dmm.diam()/(Interval(_itv.ub())-Interval(_itv.lb()))).ub();
					if (alpha>0) {
						alpha = 0;
						band = dmm;
					} else {
						//u = 1/sqrt(-alpha);
						TEMP1 = (1.0/Interval(_itv.lb()))-alpha*(_itv.lb());
						TEMP2 = (1.0/Interval(_itv.ub()))-alpha*(_itv.ub());
						if (TEMP1.ub()>TEMP2.ub()) {
							band = Interval(0.0,TEMP1.ub());
						}
						else {
							band = Interval(0.0,TEMP2.ub());
						}
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;
				if (_itv.lb()<0.0) beta = -beta;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
				_itv &= ((dmm.lb()>= 0.0)? dmm : (-dmm));
			}
			break;
		}
		case AF_COSH : {

			if (_itv.is_unbounded()) {
				_actif = false;
				_itv = cosh(_itv);
			}
			else {

				dmm = cosh(_itv);
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = ((cosh(Interval(_itv.ub()))-cosh(Interval(_itv.lb())))/(Interval(_itv.ub())-Interval(_itv.lb()))).ub();
					//u = asinh(alpha);
					TEMP1 = Interval(cosh(_itv.lb()))-alpha*(_itv.lb());
					TEMP2 = Interval(cosh(_itv.ub()))-alpha*(_itv.ub());
					if (TEMP1.ub()>TEMP2.ub()) {
						// cosh(asinh(alpha)) = sqrt(sqr(alpha)+1)
						band = Interval((sqrt(pow(Interval(alpha),2)+1)).lb(),TEMP1.ub());
					}
					else {
						band = Interval((sqrt(pow(Interval(alpha),2)+1)).lb(),TEMP2.ub());
					}
				}

				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);
				_itv &= dmm;
			}

			break;
		}
		case AF_ABS : {
			if (0<=_itv.lb()) {

			}
			else if (_itv.ub()<=0) {
				*this = Affine2(*this,true);
			}
			else if (_itv.is_unbounded() ) {
				_actif = false;
				_itv = abs(_itv);
			}
			else {

				dmm = abs(_itv);
				if (_itv.diam()< AF_EC()) {
					alpha = 0.0;
					band = dmm;
				}
				else {
					alpha = ((abs(Interval(_itv.ub()))-abs(Interval(_itv.lb())))/(Interval(_itv.ub())-Interval(_itv.lb()))).ub();
					if (alpha<0) alpha = 0;
					//u = log(alpha);
					TEMP1 = Interval(dmm.lb())-alpha*(_itv.lb());
					TEMP2 = Interval(dmm.ub())-alpha*(_itv.ub());
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
				_itv &= dmm;
			}

			break;
		}
	// now the other non-convex or concave function
	// trigo function
		case AF_TAN :
			if (tan(_itv).is_unbounded()) {
				_actif = false;
				_itv = tan(_itv);
				break;
			}
		case AF_COS :
		case AF_SIN : {
			if (_itv.diam()>=Interval::TWO_PI.lb()) {
				for (int i =0; i<=_n;i++)  { _val[i] = 0.0;}
				_err = 1;
				_itv = Interval(-1,1);
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
			xb0 = Interval(0.5)*(_itv.diam()*x0 +_itv.lb()+_itv.ub());
			xb1 = Interval(0.5)*(_itv.diam()*(-x0) +_itv.lb()+_itv.ub());

			switch (num) {
			case AF_COS :
				c0 = Interval(0.5)*(cos(xb0)+cos(xb1));
				c1 = x0*cos(xb0)-x0*cos(xb1);
				x0 = cos(_itv);
				break;
			case AF_SIN :
				c0 = Interval(0.5)*(sin(xb0)+sin(xb1));
				c1 = x0*sin(xb0)-x0*sin(xb1);
				x0 = sin(_itv);
				break;
			case AF_TAN :
				c0 = Interval(0.5)*(tan(xb0)+tan(xb1));
				c1 = x0*tan(xb0)-x0*tan(xb1);
				x0 = tan(_itv);
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}

			TEMP1 = 2*c1/(_itv.diam());
			TEMP2 = c0-c1*((_itv.lb()+_itv.ub())/(_itv.diam()));

			alpha= TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n*Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();

			// compute the maximal error
			double max_error= 0.0;
			Interval u,nb_period;

			// compute the error at _itv.lb() and _itv.ub() and compute the first point such as f'(u) = alpha
			switch (num) {
			case AF_COS :
				max_error = (abs(cos(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(cos(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				if (t1>max_error)  max_error= t1;
				u = asin(-Interval(alpha));
				nb_period = (_itv+Interval::HALF_PI) / Interval::PI;
				break;
			case AF_SIN :
				max_error = (abs(sin(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(sin(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				if (t1>max_error)  max_error= t1;
				u = acos(Interval(alpha));
				nb_period = (_itv) / Interval::PI;
				break;
			case AF_TAN :
				max_error = (abs(tan(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(tan(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				if (t1>max_error)  max_error= t1;
				u = acos(1/sqrt(Interval(alpha)));
				nb_period = (_itv) / Interval::PI;
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
					TEMP1 = (_itv & (i%2==0? (u + i*Interval::PI) : (i*Interval::PI - u)));
					if (!(TEMP1.is_empty())) { // check if maximize the error
						t1 = (abs(cos(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					i++;
				}
				break;
			case AF_SIN :
				while (i<=p2) { // looking for a point
					TEMP1 = (_itv & (i%2==0? (u + i*Interval::PI) : ((i+1)*Interval::PI - u)));
					if (!(TEMP1.is_empty())) {
						t1 = (abs(sin(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					i++;
				}
				break;
			case AF_TAN :
				while (i<=p2) { // looking for a point
					TEMP1 = (_itv & ( i*Interval::PI + u));
					if ((!(TEMP1.is_empty()))) {
						t1 = (abs(tan(TEMP1)-(alpha*TEMP1+beta))).ub();
						if (t1>max_error)  max_error= t1;
					}
					TEMP1 = (_itv & ( i*Interval::PI - u ));
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
			_itv &= x0;
			break;
		}
	// inverse function
		case AF_ACOS :
		case AF_ASIN :
			_itv &= Interval(-1,1);
			if (_itv.is_empty()) {
				_actif = false;
				break;
			}
		case AF_ATAN : //TODO to check
	// hyperbolic function
		case AF_TANH :
		case AF_SINH : {
			if (_itv.is_unbounded()) {
				_actif = false;
				switch(num) {
				case AF_TANH :
					_itv = tanh(_itv);
					break;
				case AF_SINH :
					_itv = sinh(_itv);
					break;
				case AF_ATAN :
					_itv = atan(_itv);
					break;
				default:
					ibex_error("this should not appear");
					break;
				}
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
			xb0 = Interval(0.5)*(_itv.diam()*x0 +_itv.lb()+_itv.ub());
			xb1 = Interval(0.5)*(_itv.diam()*(-x0) +_itv.lb()+_itv.ub());
			switch (num) {
			case AF_SINH :
				c0 = Interval(0.5)*(sinh(xb0)+sinh(xb1));
				c1 = x0*sinh(xb0)-x0*sinh(xb1);
				x0 = sinh(_itv);
				break;
			case AF_TANH :
				c0 = Interval(0.5)*(tanh(xb0)+tanh(xb1));
				c1 = x0*tanh(xb0)-x0*tanh(xb1);
				x0 = tanh(_itv);
				break;
			case AF_ATAN :
				c0 = Interval(0.5)*(atan(xb0)+atan(xb1));
				c1 = x0*atan(xb0)-x0*atan(xb1);
				x0 = atan(_itv);
				break;
			case AF_ACOS :
				c0 = Interval(0.5)*(acos(xb0)+acos(xb1));
				c1 = x0*acos(xb0)-x0*acos(xb1);
				x0 = acos(_itv);
				break;
			case AF_ASIN :
				c0 = Interval(0.5)*(asin(xb0)+asin(xb1));
				c1 = x0*asin(xb0)-x0*asin(xb1);
				x0 = asin(_itv);
				break;
			default:
				ibex_error("Not implemented yet");
				break;
			}
			TEMP1 = 2*c1/(_itv.diam());
			TEMP2 = c0-c1*((_itv.lb()+_itv.ub())/(_itv.diam()));

			alpha= TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n*Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();

			// compute the maximal error
			double max_error= 0.0;

			// compute the error at _itv.lb(), _itv.ub() and u such as f'(u) =alpha
			switch (num) {
			case AF_SINH :
				max_error = (abs(sinh(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(sinh(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				// u = acosh(alpha)
				TEMP2 = acosh(Interval(alpha));
				if (!((TEMP2 & _itv).is_empty())) {
					// sinh(acosh(x)) = sqrt(sqr(x)-1)
					t1 = (abs(sqrt(pow(Interval(alpha),2)-1)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & _itv).is_empty())) {
					// sinh(acosh(-x)) = -sqrt(sqr(x)-1)
					t1 = (abs((-sqrt(pow(Interval(alpha),2)-1))-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_TANH :
				max_error = (abs(tanh(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(tanh(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* tanh'(u)=alpha
				 * cosh(u)= -2/alpha -1
				 * u = +-acosh(-2/alpha -1)
				 */
				TEMP2 = acosh(-2/Interval(alpha) -1);
				if (!((TEMP2 & _itv).is_empty())) {
					t1 = (abs(tanh(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & _itv).is_empty())) {
					t1 = (abs(tanh(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_ATAN :
				max_error = (abs(atan(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(atan(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* atan'(u)=1/(u^2+1) = alpha
				 * u = +-sqrt(1/alpha -1)
				 */
				TEMP2 = sqrt(1/Interval(alpha)-1);
				if (!((TEMP2 & _itv).is_empty())) {
					t1 = (abs(atan(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & _itv).is_empty())) {
					t1 = (abs(atan(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_ACOS :
				max_error = (abs(acos(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(acos(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* acos'(u)=-1/sqrt(1-u^2) = alpha
				 * u = +-sqrt(1-1/(alpha^2))
				 */
				TEMP2 = sqrt(1-1/(pow(Interval(alpha),2)));
				if (!((TEMP2 & _itv).is_empty())) {
					t1 = (abs(acos(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & _itv).is_empty())) {
					t1 = (abs(acos(-TEMP2)-(alpha*(-TEMP2)+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				break;
			case AF_ASIN :
				max_error = (abs(asin(Interval(_itv.lb()))-(alpha*Interval(_itv.lb())+beta))).ub();
				t1 = (abs(asin(Interval(_itv.ub()))-(alpha*Interval(_itv.ub())+beta))).ub();
				max_error = (t1>max_error)? t1 : max_error;
				/* asin'(u)=1/sqrt(1-u^2) = alpha
				 * u = sqrt(1-1/(alpha^2))
				 */
				TEMP2 = sqrt(1/Interval(alpha)-1);
				if (!((TEMP2 & _itv).is_empty())) {
					// tanh(acosh(x)) = sqrt(sqr(x)-1) :x
					t1 = (abs(asin(TEMP2)-(alpha*TEMP2+beta))).ub();
					max_error = (t1>max_error)? t1 : max_error;
				}
				if (!(((-TEMP2) & _itv).is_empty())) {
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
			_itv &= x0;
			break;
		}
		default :
			ibex_error("Not implemented yet");
			break;
		}

	} else {
		switch (num) {
		case AF_SQRT :
			_itv = sqrt(_itv);
			break;
		case AF_EXP :
			_itv = exp(_itv);
			break;
		case AF_LOG :
			_itv = log(_itv);
			break;
		case AF_INV :
			_itv = 1.0/_itv;
			break;
		case AF_COS :
			_itv = cos(_itv);
			break;
		case AF_SIN :
			_itv = sin(_itv);
			break;
		case AF_TAN :
			_itv = tan(_itv);
			break;
		case AF_ABS :
			_itv = abs(_itv);
			break;
		case AF_ACOS :
			_itv = acos(_itv);
			break;
		case AF_ASIN :
			_itv = asin(_itv);
			break;
		case AF_ATAN :
			_itv = atan(_itv);
			break;
		case AF_COSH :
			_itv = cosh(_itv);
			break;
		case AF_SINH :
			_itv = sinh(_itv);
			break;
		case AF_TANH :
			_itv = tanh(_itv);
			break;
		default:
			ibex_error("Not implemented yet");
			break;
		}
	}
	_actif &= (!(_itv.is_unbounded()) && !(_itv.is_empty()));

	return *this;
}


std::ostream& operator <<(std::ostream& os, const Affine2& x) {
	if (x.is_empty()) {
		return os << "[ empty ]";
	} else {
		os << x.itv() << " : ";
		if (x.isActif()) {
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

Affine2 pow(const Affine2& x, int n) {
	if (n == 0)
		return Affine2(x.size(), Interval::ONE);
	else if (n == 1)
		return Affine2(x);
	else if (n == 2)
		return sqr(x);
	else
		return Affine2(x).power(n);
}


Affine2 pow(const Affine2& x, double d) {
	if ( ((int) (d)) == d) {
		return pow(x, (int) (d));
	} else
		return pow(x, Interval(d));
}


Affine2 root(const Affine2& x, int n) {
	Interval e = Interval::ONE;
	Affine2 y(x.size(), Interval::EMPTY_SET);
	e /= n >= 0 ? n : -n;
	if (n == 0)
		return Affine2(x.size(), Interval::ONE);
	else if (n == 1)
		return Affine2(x);
	else if (n % 2 == 0) {
		y = pow(x, e); // the negative part of x should be removed
	} else if (0 <= x.lb()) {
		y = pow(x, e);
	} else if (x.ub() <= 0) {
		y = -pow(-x, e);
	} else {
		// TODO do the root when x contains ZERO more properly
		//ibex_error("warning: Affine2 ROOT non completely well implemented yet");
		//		y=pow(x,e) |  // the negative part of x should be removed
		//	    (-pow(-x,e)); // the positive part of x should be removed
		Interval yitv = pow(x, e)| (-pow(-x,e));
		y = yitv;
	}

	return n >= 0 ? y : 1.0 / y;
}



Affine2 sign(const Affine2& x) {
	if (x.lb() > 0) {
		return Affine2(x.size(), 1.0);
	} else if (x.ub() < 0) {
		return Affine2(x.size(), -1.0);
	} else {
		return Affine2(x.size(), Interval(-1.0, 1.0));
	}
}
//TODO  to check
//============================= Proj operator =========================================

bool proj_add(const Interval& y, Affine2& x1, Affine2& x2) {
	bool b = proj_add(y, x1.ITV(), x2.ITV());
	if (b) {
		x1.setActif(false);
		x2.setActif(false);
	}
	return b;
}

bool proj_add(const Interval& y, Affine2& x1, Interval& x2) {
	bool b = proj_add(y, x1.ITV(), x2);
	if (b) {
		x1.setActif(false);
	}
	return b;
}

bool proj_sub(const Interval& y, Affine2& x1, Affine2& x2) {
	bool b = proj_sub(y, x1.ITV(), x2.ITV());
	if (b) {
		x1.setActif(false);
		x2.setActif(false);
	}
	return b;
}
bool proj_sub(const Interval& y, Affine2& x1, Interval& x2) {
	bool b = proj_sub(y, x1.ITV(), x2);
	if (b) {
		x1.setActif(false);
	}
	return b;
}
bool proj_sub(const Interval& y, Interval& x1, Affine2& x2) {
	bool b = proj_sub(y, x1, x2.ITV());
	if (b) {
		x2.setActif(false);
	}
	return b;
}

bool proj_mul(const Interval& y, Affine2& x1, Affine2& x2) {
	bool b = proj_mul(y, x1.ITV(), x2.ITV());
	if (b) {
		x1.setActif(false);
		x2.setActif(false);
	}
	return b;
}

bool proj_mul(const Interval& y, Affine2& x1, Interval& x2) {
	bool b = proj_mul(y, x1.ITV(), x2);
	if (b) {
		x1.setActif(false);
	}
	return b;
}

bool proj_div(const Interval& y, Affine2& x1, Affine2& x2) {
	bool b = proj_div(y, x1.ITV(), x2.ITV());
	if (b) {
		x1.setActif(false);
		x2.setActif(false);
	}
	return b;
}
bool proj_div(const Interval& y, Affine2& x1, Interval& x2) {
	bool b = proj_div(y, x1.ITV(), x2);
	if (b) {
		x1.setActif(false);
	}
	return b;
}
bool proj_div(const Interval& y, Interval& x1, Affine2& x2) {
	bool b = proj_div(y, x1, x2.ITV());
	if (b) {
		x2.setActif(false);
	}
	return b;
}

bool proj_sqrt(const Interval& y, Affine2& x) {
	bool b = proj_sqrt(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}


bool proj_sqr(const Interval& y, Affine2& x) {
	bool b = proj_sqr(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_pow(const Interval& y, int n, Affine2& x) {
	bool b = proj_pow(y, n, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_pow(const Interval& y, Affine2& x1, Affine2& x2) {
	bool b = proj_pow(y, x1.ITV(), x2.ITV());
	if (b) {
		x1.setActif(false);
		x2.setActif(false);
	}
	return b;
}
bool proj_pow(const Interval& y, Interval& x1, Affine2& x2) {
	bool b = proj_pow(y, x1, x2.ITV());
	if (b) {
		x2.setActif(false);
	}
	return b;
}
bool proj_pow(const Interval& y, Affine2& x1, Interval& x2) {
	bool b = proj_pow(y, x1.ITV(), x2);
	if (b) {
		x1.setActif(false);
	}
	return b;
}



bool proj_root(const Interval& y, int n, Affine2& x) {
	bool b = proj_root(y, n, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_exp(const Interval& y, Affine2& x) {
	bool b = proj_exp(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_log(const Interval& y, Affine2& x) {
	bool b = proj_log(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_cos(const Interval& y, Affine2& x) {
	bool b = proj_cos(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_sin(const Interval& y, Affine2& x) {
	bool b = proj_sin(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_tan(const Interval& y, Affine2& x) {
	bool b = proj_tan(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_acos(const Interval& y, Affine2& x) {
	bool b = proj_acos(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_asin(const Interval& y, Affine2& x) {
	bool b = proj_asin(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_atan(const Interval& y, Affine2& x) {
	bool b = proj_atan(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_cosh(const Interval& y, Affine2& x) {
	bool b = proj_cosh(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_sinh(const Interval& y, Affine2& x) {
	bool b = proj_sinh(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_tanh(const Interval& y, Affine2& x) {
	bool b = proj_tanh(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_abs(const Interval& y, Affine2& x) {
	bool b = proj_abs(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_sign(const Interval& y, Affine2& x) {
	bool b = proj_sign(y, x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}

bool proj_integer(Affine2& x) {
	bool b = proj_integer(x.ITV());
	if (b) {
		x.setActif(false);
	}
	return b;
}


}// end namespace ibex

