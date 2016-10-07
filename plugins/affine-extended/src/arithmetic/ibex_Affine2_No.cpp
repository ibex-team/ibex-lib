/* ============================================================================
 * I B E X - Implementation of the Affine2Main<AF_No> class without rounding
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine2_No.h"
#include "ibex_Affine.h"

namespace ibex {


template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator=(const Interval& x) {

	if (x.is_empty()) {
		_n = -1;
		_elt._err = 0.0;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY && x.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._err = 0.0;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = x.lb();
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = x.ub();
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else  {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val	= new double[1];
		_elt._val[0] = x.mid();
		_elt._err	= x.rad();
	}
	return *this;
}



template<>
AffineMain<AF_No>::AffineMain() :
		 _n		(-2		),
		 _elt	(NULL	,POS_INFINITY)	{
 }

template<>
AffineMain<AF_No>::AffineMain(int n, int m, const Interval& itv) :
			_n 		(n),
			_elt	(NULL,0.0)
{
	assert((n>=0) && (m>=0) && (m<=n));
	if (!(itv.is_unbounded()||itv.is_empty())) {
		_elt._val	=new double[n + 1];
		_elt._val[0] = itv.mid();
		for (int i = 1; i <= n; i++){
			_elt._val[i] = 0.0;
		}

		if (m == 0) {
			_elt._err = itv.rad();
		} else {
			_elt._val[m] = itv.rad();
		}
	} else {
		*this = itv;
	}
}


template<>
AffineMain<AF_No>::AffineMain(const double d) :
			_n 		(0),
			_elt	(NULL, 0.0) {
	if (fabs(d)<POS_INFINITY) {
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE();
		_elt._val[0] = d;
	} else {
		_n=-1;
		_elt._err = d;
	}
}


template<>
AffineMain<AF_No>::AffineMain(const Interval & itv):
			_n 		(0),
			_elt	(NULL, 0.0) {

	if (itv.is_empty()) {
		_n = -1;
	} else if (itv.ub()>= POS_INFINITY && itv.lb()<= NEG_INFINITY ) {
		_n = -2;
	} else if (itv.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = itv.lb();
	} else if (itv.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = itv.ub();
	} else  {
		_n = 0;
		_elt._val	= new double[1];
		_elt._val[0] = itv.mid();
		_elt._err	= itv.rad();
	}
}


template<>
AffineMain<AF_No>::AffineMain(const AffineMain<AF_No>& x) :
		_n		(x._n),
		_elt	(NULL	 ,x._elt._err ) {
	if (is_actif()) {
		_elt._val =new double[x.size() + 1];
		for (int i = 0; i <= x.size(); i++){
			_elt._val[i] = x._elt._val[i];
		}
	}
}




template<>
double AffineMain<AF_No>::val(int i) const{
	assert((0<=i) && (i<=_n));
	return _elt._val[i];
}

template<>
double AffineMain<AF_No>::err() const{
	return _elt._err;
}


template<>
const Interval AffineMain<AF_No>::itv() const {

	if (is_actif()) {
		Interval res(_elt._val[0]);
		Interval pmOne(-1.0, 1.0);
		for (int i = 1; i <= _n; i++){
			res += (_elt._val[i] * pmOne);
		}
		res += _elt._err * pmOne;
		return res;
	} else if (_n==-1) {
		return Interval::EMPTY_SET;
	} else if (_n==-2) {
		return Interval::ALL_REALS;
	} else if (_n==-3) {
		return Interval(_elt._err,POS_INFINITY);
	} else  {  //if (_n==-4)
		return Interval(NEG_INFINITY,_elt._err);
	}

}


template<>
double AffineMain<AF_No>::mid() const{
	return (is_actif())? _elt._val[0] : itv().mid();
}


template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator=(const AffineMain<AF_No>& x) {
	if (this != &x) {
		_elt._err = x._elt._err;
		if (x.is_actif()) {
			if (_n!=x._n) {
				_n =x._n;
				if (_elt._val!=NULL) { delete[] _elt._val; }
				_elt._val = new double[_n+1];
			}
			if (_elt._val==NULL) _elt._val = new double[_n+1];

			for (int i = 0; i <= x.size(); i++) {
				_elt._val[i] = x._elt._val[i];
			}
		} else {
			_n = x._n;
			if (_elt._val != NULL) {
				delete[] _elt._val;
				_elt._val = NULL;
			}
		}
	}
	return *this;

}

template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator=(double d) {

	if (fabs(d)<POS_INFINITY) {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE();
		_elt._val[0] = d;
	} else {
		if (d>0) {
			_n = -3;
		} else {
			_n = -4;
		}
		_elt._err = d;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	}
	return *this;
}



/** \brief Return (-x) */
template<>
AffineMain<AF_No>& AffineMain<AF_No>::Aneg() {
	if (is_actif()) {
		for (int i = 0; i <= _n; i++) {
			_elt._val[i] = (-_elt._val[i]);
		}

	} else {
		switch(_n) {
		case -3 : {
			_elt._err=-_elt._err;
			_n = -4;
			break;
		}
		case -4 : {
			_elt._err= -_elt._err;
			_n = -3;
			break;
		}
		}
	}
	return *this;
}




template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator*=(double alpha) {
	if (is_actif()) {
		if (alpha==0.0) {
			for (int i=0; i<=_n;i++) {
				_elt._val[i]=0;
			}
			_elt._err = 0;
		} else if ((fabs(alpha)) < POS_INFINITY) {
			for (int i=0; i<=_n;i++) {
				_elt._val[i] *= alpha;
			}

			_elt._err *= fabs(alpha);

			bool b = (_elt._err<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) *this = Interval::ALL_REALS;

		} else {
			*this = itv()*alpha;
		}
	} else {
		*this = itv()* alpha;
	}
	return *this;
}


template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator+=(const AffineMain<AF_No>& y) {
	if (is_actif() && y.is_actif()) {
		if (_n==y.size()) {

			for(int i=0;i<=_n;i++) {
				_elt._val[i] +=y._elt._val[i];
			}

			_elt._err += y._elt._err;

			bool b = (_elt._err<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) * this = Interval::ALL_REALS;

		} else  {
			if (_n>y.size()) {
				*this += y.itv();
			} else {
				Interval tmp1 = itv();
				*this = y;
				*this += tmp1;
			}
		}
	} else if (is_actif()) { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this += y.itv();
	} else if (y.is_actif()) {
		Interval tmp = itv();
		*this = y;
		*this += tmp;
	} else {
		*this =  itv() +y.itv();
	}

	return *this;

}


template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator+=(double beta) {
	if (is_actif() && (fabs(beta))<POS_INFINITY) {
		_elt._val[0] += beta;
		if (fabs(_elt._val[0])==POS_INFINITY) { *this = Interval::ALL_REALS; }

	} else {
		*this = itv()+ beta;
	}
	return *this;
}


template<>
AffineMain<AF_No>& AffineMain<AF_No>::inflate(double ddelta) {
	if (is_actif() && (fabs(ddelta))<POS_INFINITY) {
		_elt._err += fabs(ddelta);
		if (_elt._err==POS_INFINITY ) { *this = Interval::ALL_REALS; }

	} else {
		*this = itv()+Interval(-1,1)*ddelta;
	}
	return *this;
}



template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator*=(const AffineMain<AF_No>& y) {
//	std::cout << "in *= "<< *this <<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			double Sx, Sy, Sxy, Sz, xVal0,tmp;
			int i;
			double * xTmp;

			xTmp = new double[_n + 1];
			Sx=0.0; Sy=0.0; Sxy=0.0; Sz=0.0; xVal0=0.0; tmp =0.0;

			for (i = 1; i <= _n; i++) {

				tmp =_elt._val[i]*y._elt._val[i];
				Sz += tmp;
				Sxy += fabs(tmp);
				Sx += fabs(_elt._val[i]);
				Sy += fabs(y._elt._val[i]);

			}

			xVal0 = _elt._val[0];
			// RES = X%T(0) * res
			for (i = 0; i <= _n; i++) {
				_elt._val[i] *= y._elt._val[0];
			}

			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (i = 1; i <= _n; i++) {
				xTmp[i] = xVal0*y._elt._val[i];
			}

			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (i = 0; i <= _n; i++) {
				_elt._val[i] += xTmp[i];
			}

			_elt._val[0] += 0.5 *Sz ;
			_elt._err = fabs(y._elt._val[0]) * _elt._err + fabs(xVal0) * y._elt._err +(_elt._err+Sx)*(y._elt._err+Sy)- 0.5 *  Sxy;


			{
				bool b = (_elt._err<POS_INFINITY);
				for (i=0;i<=_n;i++) {
					b &= (fabs(_elt._val[i])<POS_INFINITY);
				}
				if (!b) {
					*this = Interval::ALL_REALS;
				}
			}
			delete[] xTmp;

		} else {
			if (_n>y.size()) {
				*this *=  AffineMain<AF_No>(size(),0,y.itv());
			} else {
				Interval tmp1 = this->itv();
				*this = y;
				*this *=AffineMain<AF_No>(size(),0,tmp1);
			}
		}

	} else { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this = itv() * y.itv();
	}
//	std::cout << "out *= "<< *this<<std::endl;

	return *this;
}



template<>
AffineMain<AF_No>& AffineMain<AF_No>::operator*=(const Interval& y) {
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;

	} else {
		*this *= AffineMain<AF_No>(size(),0,y);
	}
	return *this;
}


template<>
AffineMain<AF_No>& AffineMain<AF_No>::Asqr(const Interval& itv) {
//	std::cout << "in sqr "<<std::endl;

	if (	(!is_actif())||
			itv.is_empty()||
			itv.is_unbounded()||
			(itv.diam() < AF_EC)  ) {
		*this = pow(itv,2);

	} else  {

		double Sx, Sx2, x0;
		Sx = 0; Sx2 = 0;  x0 = 0;

		// compute the error
		for (int i = 1; i <= _n; i++) {
			Sx2 += _elt._val[i]*_elt._val[i];
			Sx += fabs(_elt._val[i]);
		}
		// compute 2*_elt._val[0]*(*this)
		x0 = _elt._val[0];
		_elt._val[0] = x0*x0;

		// compute 2*_elt._val[0]*(*this)
		for (int i = 1; i <= _n; i++) {
			_elt._val[i] *= (2*x0);
		}
		_elt._val[0] += 0.5 *Sx2;

		_elt._err = 2*fabs(x0) * _elt._err-0.5 *Sx2  + __builtin_powi((_elt._err+Sx),2);


		{
			bool b = (_elt._err<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) {
				*this = Interval::ALL_REALS;
			}
		}

	}

//	std::cout << "out sqr "<<std::endl;
	return *this;
}







template<>
AffineMain<AF_No>& AffineMain<AF_No>::Apow(int n, const Interval& itv) {
	//	std::cout << "in power "<<std::endl;

	// Particular case
	if (itv.is_empty()||itv.is_unbounded()) {
		*this = pow(itv,n);
	} else if (!is_actif()) {
		*this = pow(itv,n);
	} else if (itv.diam()< AF_EC) {
		*this = pow(itv,n);
	} else {

	// General Case
		if (n == 0) {
			*this = Interval::ONE;

		} else if (n == 1) {
			// Nothing to do
		} else if (n == 2) {
			sqr(itv);

		} else if (n % 2 == 0) {
			// alpha = (F(sup(x)) - F(inf(x)))/diam(X)
			// u = (f')^{-1}(alpha)
			// d_a = f(inf(x)) -alpha*inf(X)
			// d_b = f(sup(x)) -alpha*sup(x)
			// d_min = min(d_a,d_b)
			// d_max = f(u) - alpha*u
			// beta = Interval(d_min,d_max).mid()
			// zeta = Interval(d_min,d_max).rad()
			double alpha,  TEMP1, TEMP2;
			Interval  dmm(0.0), band(0.0);

			dmm = pow(itv, n);
			alpha = (( ::pow(itv.ub(),n)- ::pow(itv.lb(),n))/itv.diam());

			TEMP1 = (dmm.lb()) - alpha * (itv.lb());
			TEMP2 = (dmm.ub()) - alpha * (itv.ub());
			// u = (alpha/n)^(1/(n-1))
			if (TEMP1 > TEMP2) {
				TEMP2 = alpha/ n;
				band = Interval(
						((1 - n) * TEMP2 * ( ::pow(TEMP2, 1.0/(n - 1)))),
						TEMP1);
			} else {
				TEMP1 = alpha/ n;
				band = Interval(
						((1 - n) * TEMP1 * ( ::pow(TEMP1, 1.0/(n - 1)))),
						TEMP2);
			}

			*this *= alpha;
			*this += band.mid();
			this->inflate(band.rad());

		} else {
			// for _itv = [a,b]
			// x0 = 1/sqrt(2)
			// x1= - x0
			// xb0 = 0.5*((b-a)*x0 +(a+b))
			// xb1 = 0.5*((b-a)*x1 +(a+b))
			// c0 = 0.5 (f(xb0)+f(xb1))
			// c1 = x0*f(xb0)+x1*f(xb1)
			// alpha = 2*c1/(b-a)
			// beta = c0-c1*(a+b)/(b-a)
			//  old : ddelta = (b-a)^2 * f''(_itv)/16
			//  new : ddelta = evaluate the error at the bound and the points when f'(x)=alpha

			double alpha, beta, ddelta, t1, x0, xb0, xb1, fxb0, fxb1, c0, c1, TEMP2;

			x0  = 1.0 /  ::sqrt(2.);
			xb0 = (0.5) * (itv.diam() * ( x0) + itv.lb() + itv.ub());
			xb1 = (0.5) * (itv.diam() * (-x0) + itv.lb() + itv.ub());
			fxb0 =  ::pow(xb0, n);
			fxb1 =  ::pow(xb1, n);
			c0 = (0.5) * (fxb0 + fxb1);
			c1 = x0 * fxb0 - x0 * fxb1;

			alpha = 2 * c1 / (itv.diam());
			beta  = c0 - c1 * ((itv.lb() + itv.ub()) / (itv.diam()));
			//ddelta = ((_n * Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub(); //

			// compute the maximal error

			// compute the error at _itv.lb() and _itv.ub()
			ddelta = ( fabs( ::pow(itv.lb(), n)- (alpha * (itv.lb()) + beta)));
			t1     = ( fabs( ::pow(itv.ub(), n)- (alpha * (itv.ub()) + beta)));
			if (t1 > ddelta) ddelta= t1 ;
			// u = (alpha/n)^(1/(n-1))
			TEMP2 =  ::pow(alpha / n, 1.0 / n - 1);
			if (((itv.contains(TEMP2)))) {
				t1 = ( fabs( ::pow(TEMP2, n) - (alpha * TEMP2 + beta)));
				if (t1 > ddelta) ddelta= t1 ;
			}
			if (((itv.contains(-TEMP2)))) {
				t1 = ( fabs( ::pow(-TEMP2, n) - (alpha * (-TEMP2) + beta)));
				if (t1 > ddelta) ddelta= t1 ;
			}

			*this *= alpha;
			*this += beta;
			this->inflate(ddelta);
		}

	}
	//	std::cout << "out power "<<std::endl;
	return *this;
}

template<>
void AffineMain<AF_No>::compact(double tol){
	for (int i=1;i<=_n;i++) {
		if (fabs(_elt._val[i])<tol) {
			_elt._err += fabs(_elt._val[i]);
			_elt._val[i] =0;
		}
	}
}


}// end namespace ibex






//
//// debut linChebyshev
//template<>
//AffineMain<AF_No>& AffineMain<AF_No>::linChebyshev(Affine2_expr num, const Interval& itv) {
//	//  std::cout << "linChebyshev IN itv= "<<itv << " x =  "<< *this << num<< std::endl;
//
//	Interval res_itv;
//	switch (num) {
//	case AF_SQRT :
//		res_itv = sqrt(itv);
//		break;
//	case AF_EXP :
//		res_itv = exp(itv);
//		break;
//	case AF_LOG :
//		res_itv = log(itv);
//		break;
//	case AF_INV :
//		res_itv = 1.0/itv;
//		break;
//	case AF_COS :
//		res_itv = cos(itv);
//		break;
//	case AF_SIN :
//		res_itv = sin(itv);
//		break;
//	case AF_TAN :
//		res_itv = tan(itv);
//		break;
//	case AF_ABS :
//		res_itv = abs(itv);
//		break;
//	case AF_ACOS :
//		res_itv = acos(itv);
//		break;
//	case AF_ASIN :
//		res_itv = asin(itv);
//		break;
//	case AF_ATAN :
//		res_itv = atan(itv);
//		break;
//	case AF_COSH :
//		res_itv = cosh(itv);
//		break;
//	case AF_SINH :
//		res_itv = sinh(itv);
//		break;
//	case AF_TANH :
//		res_itv = tanh(itv);
//		break;
//	default: {
//		ibex_error("Not implemented yet");
//		break;
//	}
//	}
//
//	// Particular case
//	if (res_itv.is_empty() || res_itv.is_unbounded()) {
//		*this = res_itv;
//	} else if ((!is_actif())||(itv.diam()<AF_EC)) {
//		*this = res_itv;
//	}  else  {
//
//	// General case
//		double alpha, beta, ddelta, TEMP1, TEMP2, t1, t2;
//		Interval dmm(0.0), band(0.0);
//
//		switch (num) {
//		// alpha = (F(sup(x)) - F(inf(x)))/diam(X)
//		// u = (f')^{-1}(alpha)
//		// d_a = f(inf(x)) -alpha*inf(X)
//		// d_b = f(sup(x)) -alpha*sup(x)
//		// d_min = min(d_a,d_b)
//		// d_max = f(u) - alpha*u
//		// beta = Interval(d_min,d_max).mid()
//		// zeta = Interval(d_min,d_max).rad()
//
//		case AF_SQRT: {
//			Interval itv2;
//			if (itv.lb()<0) {
//				itv2 = Interval(0.0,itv.ub());
//			} else {
//				itv2 = itv;
//			}
//			dmm = sqrt(itv2);
//			alpha = dmm.diam()/itv2.diam();
//
//			//u = 1/(4*alpha^2);
//			TEMP1 = dmm.lb()-alpha*(itv2.lb());
//			TEMP2 = dmm.ub()-alpha*(itv2.ub());
//			if (TEMP1>TEMP2) {
//				band = Interval(TEMP2,(1.0/(4*(alpha))));
//			} else {
//				band = Interval(TEMP1,(1.0/(4*(alpha))));
//			}
//
//			saxpy(alpha, AffineMain<AF_No>(), band.mid(), band.diam()/2, true,false,true,true);
//			break;
//		}
//		case AF_EXP : {
//
//			if (itv.is_unbounded()) {
//				*this = res_itv;
//			} else {
//
//				dmm = res_itv;
//				alpha = dmm.diam()/itv.diam();
//
//				//u = log(alpha);
//				TEMP1 = dmm.lb()-alpha*(itv.lb());
//				TEMP2 = dmm.ub()-alpha*(itv.ub());
//				if (TEMP1>TEMP2) {
//					band = Interval((alpha*(1- ::log((alpha)))),TEMP1);
//				} else {
//					band = Interval((alpha*(1- ::log((alpha)))),TEMP2);
//				}
//
//				saxpy(alpha, AffineMain<AF_No>(), band.mid(), band.diam()/2, true,false,true,true);
//			}
//
//			break;
//		}
//		case AF_LOG : {
//			dmm = res_itv;
//			alpha = dmm.diam()/itv.diam();
//
//			//u = 1/alpha;
//			TEMP1 = dmm.lb()-alpha*(itv.lb());
//			TEMP2 = dmm.ub()-alpha*(itv.ub());
//			if (TEMP1>TEMP2) {
//				band = Interval(TEMP2,(- ::log((alpha))-1));
//			}
//			else {
//				band = Interval(TEMP1,(- ::log((alpha))-1));
//			}
//
//			saxpy(alpha, AffineMain<AF_No>(), band.mid(), band.diam()/2, true,false,true,true);
//			break;
//		}
//		case AF_INV : {
//			if (itv.is_unbounded()) {
//				*this = res_itv;
//			}
//			else {
//				dmm = (1.0/abs(itv));
//				alpha = -(dmm.diam()/itv.diam());
//
//				//u = 1/sqrt(-alpha);
//				TEMP1 = (1.0/(abs(itv).lb()))-alpha*(abs(itv).lb());
//				TEMP2 = (1.0/(abs(itv).ub()))-alpha*(abs(itv).ub());
//				if (TEMP1>TEMP2) {
//					band = Interval((2* ::sqrt(-(alpha))),TEMP1);
//				}
//				else {
//					band = Interval((2* ::sqrt(-(alpha))),TEMP2);
//				}
//
//				beta = band.mid();
//
//				if (itv.lb()<0.0) beta = -beta;
//				saxpy(alpha, AffineMain<AF_No>(), beta, band.diam()/2, true,false,true,true);
//			}
//			break;
//		}
//		case AF_COSH : {
//
//			dmm = res_itv;
//			alpha = (( ::cosh((itv.ub()))- ::cosh((itv.lb())))/itv.diam());
//
//			//u = asinh(alpha);
//			TEMP1 =  ::cosh((itv.lb()))-alpha*(itv.lb());
//			TEMP2 =  ::cosh((itv.ub()))-alpha*(itv.ub());
//			if (TEMP1>TEMP2) {
//				// cosh(asinh(alpha)) = sqrt(sqr(alpha)+1)
//				band = Interval(( ::sqrt( ::pow((alpha),2)+1)-alpha* ::asinh(alpha)),TEMP1);
//			}
//			else {
//				band = Interval(( ::sqrt( ::pow(alpha,2)+1)-alpha* ::asinh(alpha)),TEMP2);
//			}
//			saxpy(alpha, AffineMain<AF_No>(), band.mid(), band.diam()/2, true,false,true,true);
//			break;
//		}
//		case AF_ABS : {
//			if (0<=itv.lb()) {
//				break;
//			}
//			else if (itv.ub()<=0) {
//				*this = (-AffineMain<AF_No>(*this));
//			}
//			else {
//				dmm = res_itv;
//				alpha = ((abs((itv.ub()))-abs((itv.lb())))/itv.diam());
//
//				TEMP1 = dmm.lb()-alpha*(itv.lb());
//				TEMP2 = dmm.ub()-alpha*(itv.ub());
//				if (TEMP1>TEMP2) {
//					// u = 0
//					band = Interval(0.0,TEMP1);
//				}
//				else {
//					band = Interval(0.0,TEMP2);
//				}
//
//				saxpy(alpha, AffineMain<AF_No>(), band.mid(), band.diam()/2, true,false,true,true);
//			}
//			break;
//		}
//	// now the other non-convex or concave function
//	// trigo function
//		case AF_TAN :
//		case AF_COS :
//		case AF_SIN : {
//			if (itv.diam()>=Interval::TWO_PI.lb()) {
//				*this = Interval(-1,1);
//				break;
//			}
//			//  pour _itv = [a,b]
//			// x0 = 1/sqrt(2)
//			// x1= - x0
//			// xb0 = 0.5*((b-a)*x0 +(a+b))
//			// xb1 = 0.5*((b-a)*x1 +(a+b))
//			// c0 = 0.5 (f(xb0)+f(xb1))
//			// c1 = x0*f(xb0)+x1*f(xb1)
//			// alpha = 2*c1/(b-a)
//			// beta = c0-c1*(a+b)/(b-a)
//			//  old : ddelta = (b-a)^2 * f''(_itv)/16
//			//  new : ddelta = evaluate the error at the bound and the points when f'(x)=alpha
//
//			double x0,xb0,xb1,fxb0,fxb1,c0,c1;
//
//			x0 = 1.0/ ::sqrt(2.);
//			xb0 = (0.5)*(itv.diam()*x0 +itv.lb()+itv.ub());
//			xb1 = (0.5)*(itv.diam()*(-x0) +itv.lb()+itv.ub());
//
//			switch (num) {
//			case AF_COS :
//				fxb0 =  ::cos(xb0);
//				fxb1 =  ::cos(xb1);
//				break;
//			case AF_SIN :
//				fxb0 =  ::sin(xb0);
//				fxb1 =  ::sin(xb1);
//				break;
//			case AF_TAN :
//				fxb0 =  ::tan(xb0);
//				fxb1 =  ::tan(xb1);
//				break;
//			default:
//				ibex_error("Not implemented yet");
//				break;
//			}
//
//			c0 = (0.5)*(fxb0+fxb1);
//			c1 = x0*fxb0-x0*fxb1;
//
//			alpha  = 2*c1/(itv.diam());
//			beta   = c0-c1*((itv.lb()+itv.ub())/(itv.diam()));
//			//ddelta = ((size()*Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();
//
//			// compute the maximal error
//			ddelta= 0.0;
//			Interval nb_period;
//			double u;
//
//			// compute the error at _itv.lb() and _itv.ub() and compute the first point such as f'(u) = alpha
//			switch (num) {
//			case AF_COS :
//				ddelta = ( fabs( ::cos((itv.lb()))-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::cos((itv.ub()))-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				u =  ::asin(-(alpha));
//				nb_period = (itv+Interval::HALF_PI.ub()) / Interval::PI.lb();
//				break;
//			case AF_SIN :
//				ddelta = ( fabs( ::sin((itv.lb()))-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::sin((itv.ub()))-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				u =  ::acos((alpha));
//				nb_period = (itv) / Interval::PI.lb();
//				break;
//			case AF_TAN :
//				ddelta = ( fabs( ::tan((itv.lb()))-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::tan((itv.ub()))-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				u =  ::acos(1/ ::sqrt((alpha)));
//				nb_period = (itv) / Interval::PI.lb();
//				break;
//			default:
//				ibex_error("Not implemented yet");
//				break;
//			}
//
//			// evaluate the error at the point such as f'(u) = alpha
//			int p1 = ((int) nb_period.lb())-2;
//			int p2 = ((int) nb_period.ub())+2;
//
//			int i = p1;
//			switch(num) {
//			case AF_COS :
//				while (i<=p2) { // looking for a point
//					TEMP1 = (i%2==0? (u + i*Interval::PI.lb()) : (i*Interval::PI.lb() - u));
//					if ((itv.contains(TEMP1))) { // check if maximize the error
//						t1 = ( fabs( ::cos(TEMP1)-(alpha*TEMP1+beta)));
//						if (t1>ddelta)  ddelta= t1;
//					}
//					i++;
//				}
//				break;
//			case AF_SIN :
//				while (i<=p2) { // looking for a point
//					TEMP1 =  (i%2==0? (u + i*Interval::PI.lb()) : ((i+1)*Interval::PI.lb() - u));
//					if (itv.contains(TEMP1)) {
//						t1 = ( fabs( ::sin(TEMP1)-(alpha*TEMP1+beta)));
//						if (t1>ddelta)  ddelta= t1;
//					}
//					i++;
//				}
//				break;
//			case AF_TAN :
//				while (i<=p2) { // looking for a point
//					TEMP1 = ( ( i*Interval::PI.lb() + u));
//					if (((itv.contains(TEMP1)))) {
//						t1 = ( fabs( ::tan(TEMP1)-(alpha*TEMP1+beta)));
//						if (t1>ddelta)  ddelta= t1;
//					}
//					TEMP1 = ( ( i*Interval::PI.lb() - u ));
//					if (itv.contains(TEMP1)) {
//						t1 = ( fabs( ::tan(TEMP1)-(alpha*TEMP1+beta)));
//						if (t1>ddelta)  ddelta= t1;
//					}
//					i++;
//				}
//				break;
//			default:
//				ibex_error("Not implemented yet");
//				break;
//			}
//
//			saxpy(alpha, AffineMain<AF_No>(), beta, ddelta, true,false,true,true);
//			break;
//		}
//
//	// inverse trigonometric function and
//	// hyperbolic function
//		case AF_ACOS :
//		case AF_ASIN :
//			// additional particular case
//			if ((itv.lb() < (-1))||(itv.ub() > 1)) {
//				AffineMain<AF_No>::linChebyshev(num,(itv & Interval(-1,1)));
//				break;
//			}
//		case AF_TANH :
//		case AF_ATAN :
//			// additional particular case
//			if (itv.is_unbounded() ) {
//				*this = res_itv ;
//				break;
//			}
//		case AF_SINH : {
//			//  pour _itv = [a,b]
//			// x0 = 1/sqrt(2)
//			// x1= - x0
//			// xb0 = 0.5*((b-a)*x0 +(a+b))
//			// xb1 = 0.5*((b-a)*x1 +(a+b))
//			// c0 = 0.5 (f(xb0)+f(xb1))
//			// c1 = x0*f(xb0)+x1*f(xb1)
//			// alpha = 2*c1/(b-a)
//			// beta = c0-c1*(a+b)/(b-a)
//			//  old : ddelta = (b-a)^2 * f''(_itv)/16
//			//  new : ddelta = evaluate the error at the bound and the points when f'(x)=alpha
//
//			double x0,xb0,xb1,fxb0,fxb1,c0,c1;
//
//			x0 = 1.0/ ::sqrt(2.);
//			xb0 = (0.5)*(itv.diam()*  x0  +itv.lb()+itv.ub());
//			xb1 = (0.5)*(itv.diam()*(-x0) +itv.lb()+itv.ub());
//			switch (num) {
//			case AF_SINH :
//				fxb0 =  ::sinh(xb0);
//				fxb1 =  ::sinh(xb1);
//				break;
//			case AF_TANH :
//				fxb0 =  ::tanh(xb0);
//				fxb1 =  ::tanh(xb1);
//				break;
//			case AF_ATAN :
//				fxb0 =  ::atan(xb0);
//				fxb1 =  ::atan(xb1);
//				break;
//			case AF_ACOS :
//				fxb0 =  ::acos(xb0);
//				fxb1 =  ::acos(xb1);
//				break;
//			case AF_ASIN :
//				fxb0 =  ::asin(xb0);
//				fxb1 =  ::asin(xb1);
//				break;
//			default:
//				ibex_error("Not implemented yet");
//				break;
//			}
//
//			c0 = (0.5)*(fxb0+fxb1);
//			c1 = x0*fxb0-x0*fxb1;
//
//			alpha  = 2*c1/(itv.diam());
//			beta   = c0-c1*((itv.lb()+itv.ub())/(itv.diam()));
//			//ddelta = ((_n*Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();
//
//			// compute the maximal error
//			ddelta= 0.0;
//
//			// compute the error at _itv.lb(), _itv.ub() and u such as f'(u) =alpha
//			switch (num) {
//			case AF_SINH :
//				ddelta =  fabs( ::sinh((itv.lb()))-(alpha*(itv.lb())+beta));
//				t1     =  fabs( ::sinh((itv.ub()))-(alpha*(itv.ub())+beta));
//				if (t1>ddelta)  ddelta= t1;
//				// u = acosh(alpha)
//				TEMP2 = ::acosh((alpha));
//				if (((itv.contains(TEMP2)))) {
//					// sinh(acosh(x)) = sqrt(sqr(x)-1)
//					t1 = ( fabs( ::sqrt( ::pow(alpha,2)-1)-(alpha*TEMP2+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				if (((itv.contains(-TEMP2)))) {
//					// sinh(acosh(-x)) = -sqrt(sqr(x)-1)
//					t1 = ( fabs((- ::sqrt( ::pow(alpha,2)-1))-(alpha*(-TEMP2)+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				break;
//			case AF_TANH :
//				ddelta = ( fabs( ::tanh((itv.lb()))-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::tanh((itv.ub()))-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				// tanh'(u)=alpha
//				// cosh(u)= -2/alpha -1
//				// u = +-acosh(-2/alpha -1)
//				TEMP2 = ::acosh(-2/alpha -1);
//				if (((itv.contains(TEMP2)))) {
//					t1 = ( fabs( ::tanh(TEMP2)-(alpha*TEMP2+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				if (((itv.contains(-TEMP2)))) {
//					t1 = ( fabs( ::tanh(-TEMP2)-(alpha*(-TEMP2)+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				break;
//			case AF_ATAN :
//				ddelta = ( fabs( ::atan((itv.lb()))-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::atan((itv.ub()))-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				// atan'(u)=1/(u^2+1) = alpha
//				// u = +-sqrt(1/alpha -1)
//				TEMP2 =  ::sqrt(1/alpha-1);
//				if (((itv.contains(TEMP2)))) {
//					t1 = ( fabs( ::atan(TEMP2)-(alpha*TEMP2+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				if (((itv.contains(-TEMP2)))) {
//					t1 = ( fabs( ::atan(-TEMP2)-(alpha*(-TEMP2)+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				break;
//			case AF_ACOS :
//				ddelta = ( fabs( ::acos(itv.lb())-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::acos(itv.ub())-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				// acos'(u)=-1/sqrt(1-u^2) = alpha
//				// u = +-sqrt(1-1/(alpha^2))
//				TEMP2 =  ::sqrt(1-1/( ::pow(alpha,2)));
//				if (((itv.contains(TEMP2)))) {
//					t1 = ( fabs( ::acos(TEMP2)-(alpha*TEMP2+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				if (((itv.contains(-TEMP2)))) {
//					t1 = ( fabs( ::acos(-TEMP2)-(alpha*(-TEMP2)+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				break;
//			case AF_ASIN :
//				ddelta = ( fabs( ::asin((itv.lb()))-(alpha*(itv.lb())+beta)));
//				t1     = ( fabs( ::asin((itv.ub()))-(alpha*(itv.ub())+beta)));
//				if (t1>ddelta)  ddelta= t1;
//				// asin'(u)=1/sqrt(1-u^2) = alpha
//				// u = sqrt(1-1/(alpha^2))
//				TEMP2 =  ::sqrt(1/(alpha)-1);
//				if (((itv.contains(TEMP2)))) {
//					// tanh(acosh(x)) = sqrt(sqr(x)-1) :x
//					t1 = ( fabs( ::asin(TEMP2)-(alpha*TEMP2+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				if (((itv.contains(-TEMP2)))) {
//					// sinh(acosh(-x)) = -sqrt(sqr(x)-1)
//					t1 = ( fabs( ::asin(-TEMP2)-(alpha*(-TEMP2)+beta)));
//					if (t1>ddelta)  ddelta= t1;
//				}
//				break;
//			default:
//				ibex_error("Not implemented yet");
//				break;
//			}
//
//			saxpy(alpha, AffineMain<AF_No>(), beta, ddelta, true,false,true,true);
//			break;
//		}
//		default : {
//			ibex_error("Not implemented yet");
//			break;
//		}
//		}
//
//	}
////std::cout << "linChebyshev OUT x =  "<< *this << num<< std::endl;
//	return *this;
//}


