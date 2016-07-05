/* ============================================================================
 * I B E X - Implementation of the Affine2Main<AF_sAF> class based on fAF version 2
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine2_sAF.h"
#include "ibex_Affine.h"


namespace ibex {



template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator=(const Interval& x) {

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
AffineMain<AF_sAF>::AffineMain() :
		 _n		(-2		),
		 _elt	(NULL	,POS_INFINITY)	{
 }

template<>
AffineMain<AF_sAF>::AffineMain(int n, int m, const Interval& itv) :
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
AffineMain<AF_sAF>::AffineMain(const double d) :
			_n 		(0),
			_elt	(NULL,0.0) {
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
AffineMain<AF_sAF>::AffineMain(const Interval & itv):
			_n 		(0),
			_elt	(NULL,0.0) {

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
AffineMain<AF_sAF>::AffineMain(const AffineMain<AF_sAF>& x) :
		_n		(x._n),
		_elt	(NULL	,x._elt._err ) {
	if (is_actif()) {
		_elt._val =new double[x.size() + 1];
		for (int i = 0; i <= x.size(); i++){
			_elt._val[i] = x._elt._val[i];
		}
	}
}



template<>
double AffineMain<AF_sAF>::val(int i) const{
	assert((0<=i) && (i<=_n));
	return _elt._val[i];
}

template<>
double AffineMain<AF_sAF>::err() const{
	return _elt._err;
}



template<>
const Interval AffineMain<AF_sAF>::itv() const {

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
double AffineMain<AF_sAF>::mid() const{
	return (is_actif())? _elt._val[0] : itv().mid();
}



template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator=(const AffineMain<AF_sAF>& x) {
	if (this != &x) {
		_elt._err = x._elt._err;
		if (x.is_actif()) {
			if (_n!=x.size()) {
				_n =x.size();
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
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator=(double d) {

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
AffineMain<AF_sAF>& AffineMain<AF_sAF>::Aneg() {
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
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator*=(double alpha) {
	double temp, ttt;
	if (is_actif()) {
		if (alpha==0.0) {
			for (int i=0; i<=_n;i++) {
				_elt._val[i]=0;
			}
			_elt._err = 0;
		} else if ((fabs(alpha)) < POS_INFINITY) {
			ttt= 0.0;
			for (int i=0; i<=_n;i++) {
				temp = _elt._val[i] * alpha;
				ttt = nextafter( ttt+ std::max(nextafter(temp,POS_INFINITY)-temp, temp - nextafter(temp,NEG_INFINITY) ), POS_INFINITY);
				_elt._val[i] = temp;
			}
			_elt._err = nextafter( ttt+ nextafter(fabs(alpha)*_elt._err,POS_INFINITY),POS_INFINITY);

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
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator+=(const AffineMain<AF_sAF>& y) {
	double temp, ttt;
	if (is_actif() && y.is_actif()) {
		if (_n==y.size()) {

			ttt=0.0;
			for(int i=0;i<=_n;i++) {
				temp = _elt._val[i] + y._elt._val[i];
				ttt = nextafter( ttt+ std::max(nextafter(temp,POS_INFINITY)-temp, temp - nextafter(temp,NEG_INFINITY) ), POS_INFINITY);
				_elt._val[i]=temp;
			}
			_elt._err = nextafter( ttt+ nextafter(_elt._err + y._elt._err,POS_INFINITY),POS_INFINITY);

			bool b = (_elt._err<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) *this = Interval::ALL_REALS;

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
		*this = itv()+y.itv();
	}
	return *this;
}



template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator+=(double beta) {
	if (is_actif() && (fabs(beta))<POS_INFINITY) {
		double temp, ttt;
		temp = _elt._val[0] + beta;
		ttt = std::max(nextafter(temp,POS_INFINITY)-temp, temp - nextafter(temp,NEG_INFINITY) );
		_elt._val[0]=temp;
		_elt._err = nextafter( ttt+ _elt._err ,POS_INFINITY);

		if (!(_elt._err<POS_INFINITY && (fabs(_elt._val[0])<POS_INFINITY))) { *this = Interval::ALL_REALS; }
	} else {
		*this = itv()+ beta;
	}
	return *this;

}


template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::inflate(double ddelta) {
	if (is_actif() && (fabs(ddelta))<POS_INFINITY) {
		_elt._err = nextafter( fabs(ddelta)+ _elt._err ,POS_INFINITY);
		if (_elt._err==POS_INFINITY) { *this = Interval::ALL_REALS; }
	} else {
		*this = itv()+ Interval(-1,1)*ddelta;
	}
	return *this;
}




template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator*=(const AffineMain<AF_sAF>& y) {
//	std::cout << "in *= "<< *this <<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			double Sx, Sy, Sxy, Sz, ttt, ppp, tmp, xVal0;
			double * xTmp;

			xTmp = new double[_n + 1];
			Sx=0.0; Sy=0.0; Sxy=0.0; Sz=0.0; ttt=0.0;  ppp=0.0; tmp=0.0; xVal0=0.0;

			for (int i = 1; i <= _n; i++) {
				ppp = _elt._val[i]*y._elt._val[i];
				ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);

				tmp = Sz + ppp;
				ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
				Sz = tmp;

				tmp = Sxy + fabs(ppp);
				ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
				Sxy = tmp;

				tmp = Sx + fabs(_elt._val[i]);
				ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
				Sx = tmp;

				tmp = Sy + fabs(y._elt._val[i]);
				ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
				Sy = tmp;

			}

			xVal0 = _elt._val[0];
			// RES = X%T(0) * res
			for (int i = 0; i <= _n; i++) {
				ppp = _elt._val[i] * y._elt._val[0];
				ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);
				_elt._val[i] = ppp;
			}

			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (int i = 1; i <= _n; i++) {
				ppp = xVal0 * y._elt._val[i];
				ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);
				xTmp[i] = ppp;
			}

			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (int i = 0; i <= _n; i++) {
				tmp = _elt._val[i]+ xTmp[i];
				ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
				_elt._val[i] = tmp;
			}

			ppp = 0.5 * Sz;
			ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);

			tmp = _elt._val[0] + ppp;
			ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
			_elt._val[0] = tmp;

			tmp = _elt._err+ Sx;
			ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);

			ppp = y._elt._err + Sy;
			ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);

			_elt._err =nextafter( nextafter(  nextafter( nextafter(
						ttt +
					nextafter(fabs(y._elt._val[0]) * _elt._err,POS_INFINITY),POS_INFINITY) +
					nextafter(fabs(xVal0) * y._elt._err,POS_INFINITY)  , POS_INFINITY)  +
					nextafter((tmp * ppp),POS_INFINITY)  , POS_INFINITY) +
					nextafter((-0.5) *  Sxy,POS_INFINITY)  , POS_INFINITY) ;

			bool b = (_elt._err<POS_INFINITY);
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) *this = Interval::ALL_REALS;

			delete[] xTmp;

		} else {
			if (_n>y.size()) {
				*this *= AffineMain<AF_sAF>(size(),0,y.itv());
			} else {
				Interval tmp1 = this->itv();
				*this = y;
				*this *=  AffineMain<AF_sAF>(size(),0,tmp1);
			}
		}

	} else { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this = itv()*y.itv();
	}

	return *this;
}



template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::operator*=(const Interval& y) {
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;

	} else {
		*this *= AffineMain<AF_sAF>(size(),0,y);
	}
	return *this;
}


template<>
AffineMain<AF_sAF>& AffineMain<AF_sAF>::Asqr(const Interval& itv) {
//	std::cout << "in sqr "<<std::endl;

	if (	(!is_actif())||
			itv.is_empty()||
			itv.is_unbounded()||
			(itv.diam() < AF_EC)  ) {
		*this = pow(itv,2);

	} else  {

		double Sx, Sx2, ttt, ppp, x0, tmp;
		Sx = 0; Sx2 = 0; ttt = 0;  ppp = 0; x0 = 0; tmp =0.0;

		// compute the error
		for (int i = 1; i <= _n; i++) {

			ppp = _elt._val[i] * _elt._val[i];
			ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);

			tmp = Sx2 + ppp;
			ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
			Sx2 = tmp;

			tmp = Sx + fabs(_elt._val[i]);
			ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
			Sx = tmp;

		}
		// compute 2*_elt._val[0]*(*this)
		x0 = _elt._val[0];

		ppp = x0 * x0;
		ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);
		_elt._val[0] = ppp;

		// compute 2*_elt._val[0]*(*this)
		for (int i = 1; i <= _n; i++) {
			ppp = (2*x0) * _elt._val[i];
			ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);
			_elt._val[i] = ppp;
		}

		ppp = 0.5 * Sx2;
		ttt = nextafter( ttt+ std::max(nextafter(ppp,POS_INFINITY)-ppp, ppp - nextafter(ppp,NEG_INFINITY) ), POS_INFINITY);

		tmp = _elt._val[0] + ppp;
		ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);
		_elt._val[0] = tmp;

		tmp = _elt._err + Sx;
		ttt = nextafter( ttt+ std::max(nextafter(tmp,POS_INFINITY)-tmp, tmp - nextafter(tmp,NEG_INFINITY) ), POS_INFINITY);

		_elt._err =nextafter( nextafter(  nextafter(
					ttt +
				nextafter(2*fabs(x0) * _elt._err  ,POS_INFINITY),POS_INFINITY) +
				nextafter((tmp * tmp)  ,POS_INFINITY)  , POS_INFINITY)  +
				nextafter((-0.5) *  Sx2 ,POS_INFINITY)  , POS_INFINITY) ;

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
void AffineMain<AF_sAF>::compact(double tol){
	for (int i=1;i<=_n;i++) {
		if (fabs(_elt._val[i])<tol) {
			_elt._err = nextafter( fabs(_elt._val[i])+ _elt._err ,POS_INFINITY);
			_elt._val[i] =0;
		}
	}
}

}// end namespace ibex




