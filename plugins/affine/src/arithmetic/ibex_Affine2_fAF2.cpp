/* ============================================================================
 * I B E X - Implementation of the Affine2Main<AF_fAF2> class based on fAF version 2
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 16, 2013
 * ---------------------------------------------------------------------------- */
#include "ibex_Affine2_fAF2.h"
#include "ibex_Affine2.h"


namespace ibex {



template<>
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::operator=(const Interval& x) {

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
Affine2Main<AF_fAF2>::Affine2Main() :
		 _n		(-2		),
		 _elt	(NULL	,POS_INFINITY)	{
 }

template<>
Affine2Main<AF_fAF2>::Affine2Main(int n, int m, const Interval& itv) :
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
Affine2Main<AF_fAF2>::Affine2Main(const double d) :
			_n 		(0),
			_elt	(NULL,0.0) {
	if (fabs(d)<POS_INFINITY) {
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE;
		_elt._val[0] = d;
	} else {
		_n=-1;
		_elt._err = d;
	}
}


template<>
Affine2Main<AF_fAF2>::Affine2Main(const Interval & itv):
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
Affine2Main<AF_fAF2>::Affine2Main(const Affine2Main<AF_fAF2>& x) :
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
double Affine2Main<AF_fAF2>::val(int i) const{
	assert((0<=i) && (i<=_n));
	return _elt._val[i];
}

template<>
double Affine2Main<AF_fAF2>::err() const{
	return _elt._err;
}



template<>
const Interval Affine2Main<AF_fAF2>::itv() const {

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
double Affine2Main<AF_fAF2>::mid() const{
	return (is_actif())? _elt._val[0] : itv().mid();
}



template<>
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::operator=(const Affine2Main<AF_fAF2>& x) {
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
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::operator=(double d) {

	if (fabs(d)<POS_INFINITY) {
		if (_elt._val!=NULL) { delete[] _elt._val; }
		_n = 0;
		_elt._val = new double[1];
		_elt._err = 0.0; //abs(d)*AF_EE;
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
Affine2Main<AF_fAF2> Affine2Main<AF_fAF2>::operator-() const {
	Affine2Main<AF_fAF2> res;
	res._n = _n;
	res._elt._err = _elt._err;
	if (is_actif()) {
		res._elt._val = new double[_n+1];
		for (int i = 0; i <= _n; i++) {
			res._elt._val[i] = (-_elt._val[i]);
		}

	}
	return res;
}



template<>
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::saxpy(double alpha, const Affine2Main<AF_fAF2>& y, double beta, double ddelta, bool B1, bool B2, bool B3, bool B4) {
//std::cout << "saxpy IN " << alpha << " x " << *this << " + " << y << " + "<< beta << " +error " << ddelta << " / "<< B1 << B2 << B3 << B4 << std::endl;
	double temp, ttt, sss, eee;
	int i;
//	std::cout << "in saxpy alpha=" << alpha  <<  "  beta= " <<  beta <<   "  delta = " << ddelta   << std::endl;
	if (is_actif()) {
		if (B1) {  // multiply by a scalar alpha
			if (alpha==0.0) {
				for (i=0; i<=_n;i++) {
					_elt._val[i]=0;
				}
				_elt._err = 0;
			}
			else if ((fabs(alpha)) < POS_INFINITY) {
				ttt= 0.0;
				sss= 0.0;
				for (i=0; i<=_n;i++) {
					eee = _elt.twoProd(_elt._val[i], alpha, &temp);
					_elt._val[i] = temp;
					ttt = (1+2*AF_EM)*(ttt+fabs(eee));
					if (fabs(_elt._val[i])<AF_EC) {
						sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[i]));
						_elt._val[i] = 0.0;
					}
				}

//				_elt._err = (1+2*AF_EM)*((1+2*AF_EM)*fabs(alpha)*_elt._err+AF_EE*AF_EM*ttt + AF_EE*sss);
				_elt._err = (1+2*AF_EM)*(
						((1+2*AF_EM)*fabs(alpha)*_elt._err) +
						((AF_EE*ttt) +
						(AF_EE*sss))
						);

			}
			else {
				*this = itv()*alpha;
			}
		}

		if (B2) {  // add a affine2 form y

			if (y.is_actif()) {
				if (_n==y.size()) {

					ttt=0.0;
					sss=0.0;
					for(i=0;i<=_n;i++) {
						eee = _elt.twoSum(_elt._val[i], y._elt._val[i], &temp);
						ttt = (1+2*AF_EM)*(ttt+fabs(eee));
						if (fabs(temp)<AF_EC) {
							sss = (1+2*AF_EM)*(sss+ fabs(temp));
							_elt._val[i] = 0.0;
						}
						else {
							_elt._val[i]=temp;
						}
					}
//					_elt._err = (1+2*AF_EM)*((_elt._err+y._elt._err+ (AF_EE*(AF_EM*ttt)+AF_EE*sss));
					_elt._err = (1+2*AF_EM)*(
							(_elt._err+y._elt._err) +
							((AF_EE*(ttt)) +
							(AF_EE*sss))
							);

				} else  {
					if (_n>y.size()) {
						*this += y.itv();
					} else {
						Interval tmp1 = itv();
						*this = y;
						*this += tmp1;
					}
				}
			}
			else { // y is not a valid affine2 form. So we add y.itv() such as an interval
				*this = itv()+y.itv();
			}
		}
		if (B3) {  //add a constant beta
			if ((fabs(beta))<POS_INFINITY) {
				ttt=0.0;
				sss=0.0;
				eee = _elt.twoSum(_elt._val[0],beta,&temp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				if (fabs(temp)<AF_EC) {
					sss = (1+2*AF_EM)*(sss+fabs(temp));
					_elt._val[0] = 0.0;
				}
				else {
					_elt._val[0]=temp;
				}
//				_elt._err = (1+2*AF_EM)*(_elt._err+ (AF_EE*(AF_EM*ttt)+AF_EE*sss));
				_elt._err = (1+2*AF_EM)*(
						_elt._err +
						(AF_EE*(ttt)+
						AF_EE*sss)
						);

			}
			else {
				*this = itv()+beta;
			}
		}

		if (B4) {  // add an error  ddelta

			if ((fabs(ddelta))<POS_INFINITY) {
				ttt=0.0;
				sss=0.0;
				eee = _elt.twoSum(_elt._err,fabs(ddelta), &temp);
				ttt = (1+2*AF_EM)*(fabs(eee));
				if (fabs(temp)<AF_EC) {
					sss = (1+2*AF_EM)*(fabs(temp));
					temp =0;
				}
//				_elt._err = (1+2*AF_EM)*(temp+ (AF_EE*(AF_EM*ttt)));;
				_elt._err = (1+2*AF_EM)*(
						temp +
						(AF_EE*(ttt) +
						AF_EE*sss)
						);

			}
			else {
				*this = itv()+Interval(-1,1)*ddelta;
			}
		}

		if (_elt._val != NULL) {
			bool b = (_elt._err<POS_INFINITY);
			for (i=0;i<=_n;i++) {
				b &= (fabs(_elt._val[i])<POS_INFINITY);
			}
			if (!b) {
				*this = Interval::ALL_REALS;
			}
		}

	} else {
		if (B1) {  //scalar alpha
			*this = itv()* alpha;
		}
		if (B2) {  // add y
			*this = itv()+ y.itv();
		}
		if (B3) {  //constant beta
			*this = itv()+ beta;
		}
		if (B4) {  // error  delta
			*this = itv()+Interval(-1,1)*ddelta;
		}
	}
//	std::cout << " saxpy OUT x= "<< *this<<std::endl;
	return *this;

}




template<>
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::operator*=(const Affine2Main<AF_fAF2>& y) {
//	std::cout << "in *= "<< *this <<std::endl;

	if (is_actif() && (y.is_actif())) {

		if (_n==y.size()) {
			double Sx, Sy, Sxy, Sz, ttt, sss, ppp, tmp, xVal0, eee;
			int i;
			double * xTmp;

			xTmp = new double[_n + 1];
			Sx=0.0; Sy=0.0; Sxy=0.0; Sz=0.0; ttt=0.0; sss=0.0; ppp=0.0; tmp=0.0; xVal0=0.0; eee=0.0;

			for (i = 1; i <= _n; i++) {
				eee = _elt.twoProd(_elt._val[i],y._elt._val[i], &ppp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));

				eee = _elt.twoSum(Sz,ppp, &tmp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				Sz = tmp;

				if (fabs(Sz) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(Sz));
					Sz = 0.0;
				}

				eee = _elt.twoSum(Sxy,fabs(ppp), &tmp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				Sxy = tmp;

				if (fabs(Sxy) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(Sxy));
					Sxy = 0.0;
				}

				eee = _elt.twoSum(Sx,fabs(_elt._val[i]), &tmp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				Sx = tmp;

				if (fabs(Sx) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(Sx));
					Sx = 0.0;
				}
				eee = _elt.twoSum(Sy,fabs(y._elt._val[i]), &tmp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				Sy = tmp;

				if (fabs(Sy) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(Sy));
					Sy = 0.0;
				}

			}

			xVal0 = _elt._val[0];
			// RES = X%T(0) * res
			for (i = 0; i <= _n; i++) {
				eee = _elt.twoProd(_elt._val[i],y._elt._val[0], &ppp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				_elt._val[i] = ppp;

				if (fabs(_elt._val[i]) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[i]));
					_elt._val[i] = 0.0;
				}
			}

			// Xtmp = X%T(0) * Y
			xTmp[0] = 0.0;
			for (i = 1; i <= _n; i++) {
				eee = _elt.twoProd(xVal0,y._elt._val[i], &ppp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				xTmp[i] = ppp;

				if (fabs(xTmp[i]) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(xTmp[i]));
					xTmp[i] = 0.0;
				}

			}

			//RES =  RES + Xtmp = ( Y%(0) * X ) + ( X%T(0) * Y - X%T(0)*Y%(0) )
			for (i = 0; i <= _n; i++) {

				eee = _elt.twoSum(_elt._val[i],xTmp[i], &tmp);
				ttt = (1+2*AF_EM)*(ttt+fabs(eee));
				_elt._val[i] = tmp;

				if (fabs(_elt._val[i]) < AF_EC) {
					sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[i]));
					_elt._val[i] = 0.0;
				}

			}

			eee = _elt.twoProd(0.5,Sz, &ppp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));

			eee = _elt.twoSum(_elt._val[0],ppp, &tmp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));
			_elt._val[0] = tmp;

			if (fabs(_elt._val[0]) < AF_EC) {
				sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[0]));
				_elt._val[0] = 0.0;
			}

			eee = _elt.twoSum(_elt._err,Sx, &tmp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));

			eee = _elt.twoSum(y._elt._err,Sy, &ppp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));


			_elt._err = (1+ 2*AF_EM) * (
					((1+ 2*AF_EM) *fabs(y._elt._val[0]) * _elt._err)  +
					((1+ 2*AF_EM) *fabs(xVal0) * y._elt._err)  +
					((1+ 2*AF_EM) *(tmp * ppp)) +
					((1- 2*AF_EM) *(-0.5) *  Sxy)  +
//					(AF_EE * (AF_EM * ttt))  +
					(AF_EE * (ttt))  +
					(AF_EE * sss)
					);

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
				*this *= Affine2Main<AF_fAF2>(size(),0,y.itv());
			} else {
				Interval tmp1 = this->itv();
				*this = y;
				*this *= Affine2Main<AF_fAF2>(size(),0,tmp1);
			}
		}


	} else {
		*this = itv()*y.itv();
	}
//	std::cout << "out *= "<< *this<<std::endl;

	return *this;
}



template<>
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::operator*=(const Interval& y) {
	if (	(!is_actif())||
			y.is_empty()||
			y.is_unbounded() ) {
		*this = itv()*y;

	} else {
		*this *= Affine2Main<AF_fAF2>(size(),0,y);
	}
	return *this;
}





template<>
Affine2Main<AF_fAF2>& Affine2Main<AF_fAF2>::sqr(const Interval itv) {
//	std::cout << "in sqr "<<std::endl;

	bool b = (!(itv.is_empty()||itv.is_unbounded()));
	if (	(!is_actif())||
			itv.is_empty()||
			itv.is_unbounded()||
			(itv.diam() < AF_EC)  ) {
		*this = pow(itv,2);

	} else  {

		double Sx, Sx2, ttt, sss, ppp, x0, eee,tmp;
		Sx = 0; Sx2 = 0; ttt = 0; sss = 0; ppp = 0; x0 = 0; eee =0.0; tmp =0.0;

		// compute the error
		for (int i = 1; i <= _n; i++) {

			eee = _elt.twoProd(_elt._val[i],_elt._val[i], &ppp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));

			eee = _elt.twoSum(Sx2,ppp, &tmp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));
			Sx2 = tmp;

			if (fabs(Sx2) < AF_EC) {
				sss = (1+2*AF_EM)*(sss+ fabs(Sx2));
				Sx2 = 0.0;
			}

			eee = _elt.twoSum(Sx,fabs(_elt._val[i]), &tmp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));
			Sx = tmp;

			if (fabs(Sx) < AF_EC) {
				sss = (1+2*AF_EM)*(sss+ fabs(Sx));
				Sx = 0.0;
			}

		}
		// compute 2*_elt._val[0]*(*this)
		x0 = _elt._val[0];

		eee = _elt.twoProd(x0,x0, &ppp);
		ttt = (1+2*AF_EM)*(ttt+fabs(eee));
		_elt._val[0] = ppp;

		if (fabs(_elt._val[0]) < AF_EC) {
			sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[0]));
			_elt._val[0] = 0.0;
		}

		// compute 2*_elt._val[0]*(*this)
		for (int i = 1; i <= _n; i++) {

			eee = _elt.twoProd((2*x0),_elt._val[i], &ppp);
			ttt = (1+2*AF_EM)*(ttt+fabs(eee));
			_elt._val[i] = ppp;

			if (fabs(_elt._val[i]) < AF_EC) {
				sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[i]));
				_elt._val[i] = 0.0;
			}

		}

		eee = _elt.twoProd(0.5,Sx2, &ppp);
		ttt = (1+2*AF_EM)*(ttt+fabs(eee));

		eee = _elt.twoSum(_elt._val[0],ppp, &tmp);
		ttt = (1+2*AF_EM)*(ttt+fabs(eee));
		_elt._val[0] = tmp;

		if (fabs(_elt._val[0]) < AF_EC) {
			sss = (1+2*AF_EM)*(sss+ fabs(_elt._val[0]));
			_elt._val[0] = 0.0;
		}

		eee = _elt.twoSum(_elt._err,Sx, &tmp);
		ttt = (1+2*AF_EM)*(ttt+fabs(eee));

		_elt._err = (1+ 2*AF_EM) * (
				((1+ 2*AF_EM) *2*fabs(x0) * _elt._err)  +
				((1+ 2*AF_EM) *(tmp * tmp)) +
				((1- 2*AF_EM) *(-0.5) *  Sx2)  +
//					(AF_EE * (AF_EM * ttt))  +
				(AF_EE * (ttt))  +
				(AF_EE * sss)
				);

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
void Affine2Main<AF_fAF2>::compact(double tol){
	for (int i=1;i<=_n;i++) {
		if (fabs(_elt._val[i])<tol) {
			double temp=0.0;
			double sss=0.0;
			double eee = _elt.twoSum(_elt._err,fabs(_elt._val[i]), &temp);
			double ttt = (1+2*AF_EM)*(fabs(eee));
			if (fabs(temp)<AF_EC) {
				sss = (1+2*AF_EM)*(fabs(temp));
				temp =0;
			}
//			_elt._err = (1+2*AF_EM)*(temp+ (AF_EE*(AF_EM*ttt)));;
			_elt._err = (1+2*AF_EM)*(
					temp +
					(AF_EE*(ttt) +
							AF_EE*sss)
			);

			_elt._val[i] =0;
		}
	}
}



}// end namespace ibex




