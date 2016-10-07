/* ============================================================================
 * I B E X - Implementation of the Affine2Main<AF_fAF1> class based on fAF version 1
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine2_fAF1.h"
#include "ibex_Affine.h"

namespace ibex {



template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator=(const Interval& x) {

	if (x.is_empty()) {
		_n = -1;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY && x.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = x;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	} else if (x.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = x;
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
AffineMain<AF_fAF1>::AffineMain() :
		 _n		(-2		),
		 _elt	(NULL	,Interval::ALL_REALS)	{
 }


template<>
AffineMain<AF_fAF1>::AffineMain(int n, int m, const Interval& itv) :
			_n 		(n),
			_elt	(NULL, 0.0)
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
AffineMain<AF_fAF1>::AffineMain(const double d) :
			_n 		(0),
			_elt	(NULL, 0.0) {
	if (fabs(d)<POS_INFINITY) {
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE;
		_elt._val[0] = d;
	} else {
        _n=-1;
        _elt._err = Interval(d);
	}
}


template<>
AffineMain<AF_fAF1>::AffineMain(const Interval & itv):
			_n 		(0),
			_elt	(NULL, 0.0) {

	if (itv.is_empty()) {
		_n = -1;
		_elt._err = itv;
	} else if (itv.ub()>= POS_INFINITY && itv.lb()<= NEG_INFINITY ) {
		_n = -2;
		_elt._err = itv;
	} else if (itv.ub()>= POS_INFINITY ) {
		_n = -3;
		_elt._err = itv;
	} else if (itv.lb()<= NEG_INFINITY ) {
		_n = -4;
		_elt._err = itv;
	} else  {
		_n = 0;
		_elt._val	= new double[1];
		_elt._val[0] = itv.mid();
		_elt._err	= itv.rad();
	}
}


template<>
AffineMain<AF_fAF1>::AffineMain(const AffineMain<AF_fAF1>& x) :
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
double AffineMain<AF_fAF1>::val(int i) const{
	assert((0<=i) && (i<=_n));
	return _elt._val[i];
}


template<>
double AffineMain<AF_fAF1>::err() const{
	return (fabs(_elt._err.lb())>fabs(_elt._err.ub()))? fabs(_elt._err.lb()) : fabs(_elt._err.ub());
}

template<>
const Interval AffineMain<AF_fAF1>::itv() const {

	if (is_actif()) {
		Interval res(_elt._val[0]);
		Interval pmOne(-1.0, 1.0);
		for (int i = 1; i <= _n; i++){
			res += (_elt._val[i] * pmOne);
		}
		res += _elt._err * pmOne;
		return res;
	} else {
		return _elt._err;
	}
}


template<>
double AffineMain<AF_fAF1>::mid() const{
	return (is_actif())? _elt._val[0] : itv().mid();
}



template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator=(const AffineMain<AF_fAF1>& x) {
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
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator=(double d) {

	if (fabs(d)<POS_INFINITY) {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE;
		_elt._val[0] = d;
	} else {
		_elt._err = d;
		_n = -1;
		if (_elt._val != NULL) {
			delete[] _elt._val;
			_elt._val = NULL;
		}
	}
	return *this;
}


/** \brief Return (-x) */
template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::Aneg() {
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
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator*=(double alpha) {
	double  ttt, sss;
	if (is_actif()) {
		if (alpha==0.0) {
			for (int i=0; i<=_n;i++) {
				_elt._val[i]=0;
			}
			_elt._err = 0;
		} else if ((fabs(alpha)) < POS_INFINITY) {
			ttt= 0.0;
			sss= 0.0;
			for (int i=0; i<=_n;i++) {
				_elt._val[i] *= alpha;
				ttt += fabs(_elt._val[i]);
				if (fabs(_elt._val[i])<AF_EC) {
					sss += fabs(_elt._val[i]);
					_elt._val[i] = 0.0;
				}
			}
			_elt._err *= fabs(alpha);
			_elt._err +=AF_EE*(AF_EM*ttt);
			_elt._err += AF_EE*sss;

			bool b = true;
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
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator+=(const AffineMain<AF_fAF1>& y) {
	double temp, ttt, sss;
	if (y.is_actif() && is_actif()) {
		if (_n==y.size()) {
			ttt=0.0;
			sss=0.0;
			for(int i=0;i<=_n;i++) {
				temp = _elt._val[i]+ y._elt._val[i];
				ttt += (fabs(_elt._val[i])>fabs(y._elt._val[i]))? fabs(_elt._val[i]) :fabs(y._elt._val[i]);
				if (fabs(temp)<AF_EC) {
					sss+= fabs(temp);
					_elt._val[i] = 0.0;
				}
				else {
					_elt._val[i]=temp;
				}
			}
			_elt._err += y._elt._err;
			_elt._err +=AF_EE*(AF_EM*ttt);
			_elt._err += AF_EE*sss;

			bool b = true;
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
		*this = itv() + y.itv();
	}
	return *this;

}



template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator+=(double beta) {
	double temp, ttt, sss;
	if (is_actif() && (fabs(beta))<POS_INFINITY) {
		ttt=0.0;
		sss=0.0;
		temp = _elt._val[0]+ beta;
		ttt = (fabs(_elt._val[0])>fabs(beta))? fabs(_elt._val[0]) :fabs(beta);
		if (fabs(temp)<AF_EC) {
			sss+= fabs(temp);
			_elt._val[0] = 0.0;
		}
		else {
			_elt._val[0]=temp;
		}
		_elt._err +=AF_EE*(AF_EM*ttt);
		_elt._err += AF_EE*sss;

		if (!(_elt._err.ub()<POS_INFINITY && (fabs(_elt._val[0])<POS_INFINITY))) { *this = Interval::ALL_REALS; }

	} else {
		*this = itv()+ beta;
	}
	return *this;

}


template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::inflate(double ddelta) {
	if (is_actif() && (fabs(ddelta))<POS_INFINITY) {
		_elt._err += fabs(ddelta);

		if (_elt._err.ub()==POS_INFINITY ) { *this = Interval::ALL_REALS; }
	} else {
		*this = itv()+Interval(-1,1)*ddelta;
	}
	return *this;

}





template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator*=(const AffineMain<AF_fAF1>& y) {
//	std::cout << "in *= "<<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			double Sx=0.0, Sy=0.0, Sxy=0.0, Sz=0.0, ttt=0.0, sss=0.0, ppp=0.0, tmp=0.0, xVal0=0.0;
			double * xTmp;

			xTmp = new double[_n + 1];

			for (int i = 1; i <= _n; i++) {
				ppp = _elt._val[i] * y._elt._val[i];
				ttt += fabs(ppp);
				ttt += (fabs(ppp) > fabs(Sz )) ? fabs(ppp) : fabs(Sz );
				ttt += (fabs(ppp) > fabs(Sxy)) ? fabs(ppp) : fabs(Sxy);
				Sz += ppp;
				Sxy += fabs(ppp);
				ttt += (fabs( _elt._val[i]) > fabs(Sx)) ? fabs( _elt._val[i]) : fabs(Sx);
				ttt += (fabs(y._elt._val[i]) > fabs(Sy)) ? fabs(y._elt._val[i]) : fabs(Sy);
				Sx += fabs(_elt._val[i]);
				Sy += fabs(y._elt._val[i]);

			}

			xVal0 = _elt._val[0];
			// RES = X%T(0) * res
			for (int i = 0; i <= _n; i++) {
				_elt._val[i] *= y._elt._val[0];
				ttt += fabs(_elt._val[i]);
				if (fabs(_elt._val[i]) < AF_EC) {
					sss += fabs(_elt._val[i]);
					_elt._val[i] = 0.0;
				}
			}
			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (int i = 1; i <= _n; i++) {
				xTmp[i] = xVal0 * y._elt._val[i];
				ttt += fabs(xTmp[i]);
				if (fabs(xTmp[i]) < AF_EC) {
					sss += fabs(xTmp[i]);
					xTmp[i] = 0.0;
				}
			}
			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (int i = 0; i <= _n; i++) {
				tmp = _elt._val[i] + xTmp[i];
				ttt += fabs(tmp);
				if (fabs(tmp) < AF_EC) {
					sss += fabs(tmp);
					_elt._val[i] = 0.0;
				} else
					_elt._val[i] = tmp;
			}

			ttt += fabs(0.5 * Sz);
			ttt += (fabs(_elt._val[0]) > fabs(0.5 * Sz)) ?
					fabs(_elt._val[0]) : fabs(0.5 * Sz)  ;

			_elt._val[0] += 0.5 * Sz;

			_elt._err = (fabs(y._elt._val[0]) * _elt._err + fabs(xVal0) * y._elt._err - 0.5 * Sxy)
								+ (_elt._err + Sx) * (y._elt._err + Sy);
			_elt._err += AF_EE * (AF_EM * ttt);
			_elt._err += AF_EE * sss;


			bool b = true;
			for (int i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) *this = Interval::ALL_REALS;

			delete[] xTmp;

		} else {
			if (_n>y.size()) {
				*this *=  AffineMain<AF_fAF1>(size(),0,y.itv());
			} else {
				Interval tmp1 = this->itv();
				*this = y;
				*this *= AffineMain<AF_fAF1>(size(),0,tmp1);
			}
		}

	} else { // y is not a valid affine2 form. So we add y.itv() such as an interval
		*this = itv() * y.itv();
	}
//	std::cout << "out *= "<<std::endl;

	return *this;
}


template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::operator*=(const Interval& y) {
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;

	} else {
		*this *= AffineMain<AF_fAF1>(size(),0,y);
	}
	return *this;
}



template<>
AffineMain<AF_fAF1>& AffineMain<AF_fAF1>::Asqr(const Interval& itv) {
//	std::cout << "in sqr "<<std::endl;

	bool b = (!(itv.is_empty()||itv.is_unbounded()));
	if (!b) {
		*this = pow(itv,2);
	} else if (!is_actif()) {
		*this = pow(itv,2);
	} else if (itv.diam() < AF_EC) {
		*this = pow(itv,2);
	} else  {  // _actif && b

		double Sx = 0, Sx2 = 0, ttt = 0, sss = 0, ppp = 0, x0 = 0;
		// compute the error
		for (int i = 1; i <= _n; i++) {
			ppp = _elt._val[i] * _elt._val[i];
			ttt += fabs(ppp) + (fabs(ppp) > fabs(Sx2) ? fabs(ppp) : fabs(Sx2));
			Sx2 += ppp;
			ttt += (fabs(_elt._val[i]) > fabs(Sx) ? fabs(_elt._val[i]) : fabs(Sx));
			Sx += fabs(_elt._val[i]);
		}
		// compute 2*_elt._val[0]*(*this)
		x0 = _elt._val[0];
		_elt._val[0] = x0 * x0;
		ttt += fabs(_elt._val[0]);
		if (fabs(_elt._val[0]) < AF_EC) {
			sss += fabs(_elt._val[0]);
			_elt._val[0] = 0;
		}
		// compute 2*_elt._val[0]*(*this)
		for (int i = 1; i <= _n; i++) {
			_elt._val[i] = (2 * x0) * _elt._val[i];
			ttt += fabs(_elt._val[i]);
			if (fabs(_elt._val[i]) < AF_EC) {
				sss += fabs(_elt._val[i]);
				_elt._val[i] = 0;
			}
		}

		ttt += fabs(0.5 * Sx2) +( (fabs(_elt._val[0]) > fabs(0.5 * Sx2)) ?
				fabs(_elt._val[0]) : fabs(0.5 * Sx2) );
		_elt._val[0] += 0.5 * Sx2;

		_elt._err = (2 * fabs(x0) * _elt._err - 0.5 * Sx2) + pow((_elt._err + Sx), 2);
		_elt._err += AF_EE * (AF_EM * ttt);
		_elt._err += AF_EE * sss;

		{
			bool b = true;
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
void AffineMain<AF_fAF1>::compact(double tol){
	for (int i=1;i<=_n;i++) {
		if (fabs(_elt._val[i])<tol) {
			_elt._err += fabs(_elt._val[i]);
			_elt._val[i] =0;
		}
	}
}



}// end namespace ibex




