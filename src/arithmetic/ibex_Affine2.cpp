/* ============================================================================
 * I B E X - ibex_Affine2.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : Nov 12, 2012
 * Modify      : July 16, 2013
 * ---------------------------------------------------------------------------- */

#include "ibex_Affine2.h"
#include "ibex_Exception.h"


#ifdef _IBEX_AFFINE2_WITH_FAF1_
#include "ibex_Affine2_fAF1.cpp_"

#else
#if defined(_IBEX_AFFINE2_WITH_FAF2_) || defined(_IBEX_AFFINE2_WITH_FAF2_FMA_)
#include "ibex_Affine2_fAF2.cpp_"

#else
#if defined(_IBEX_AFFINE2_WITH_NO_)
#include "ibex_Affine2_No.cpp_"

#else
#if defined(_IBEX_AFFINE2_WITH_iAF_)
#include "ibex_Affine2_iAF.cpp_"

#endif
#endif
#endif
#endif

namespace ibex {



Affine2& Affine2::power(int n, const Interval itv) {
	//	std::cout << "in power "<<std::endl;
	bool b = (!(itv.is_empty()||itv.is_unbounded()));
	if (!b) {
		*this = pow(itv,n);
	} else if (!is_actif()) {
		*this = pow(itv,n);
	} else if (itv.diam()< AF_EC()) {
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

//			Affine2 tmp(*this);
//			((*this).power((n/2),itv)).sqr(pow(itv,n/2));

			double alpha, beta, ddelta, t1, t2;
			Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

			dmm = pow(itv, n);
			alpha = ((__builtin_powi(itv.ub(),n)-__builtin_powi(itv.lb(),n))/itv.diam());

			TEMP1 = Interval(dmm.lb()) - alpha * (itv.lb());
			TEMP2 = Interval(dmm.ub()) - alpha * (itv.ub());
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

			beta = band.mid();
			t1 = (beta - band).ub();
			t2 = (band - beta).ub();
			ddelta = (t1 > t2) ? t1 : t2;

			saxpy(alpha, Affine2(), beta, ddelta, true, false, true, true);
//


		} else {
//			Affine2 tmp(*this);
//			(*this).power((n-1),itv);
//			*this *=tmp;

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
			//  ddelta = evaluate the error at the bound and the points when f'(x)=alpha

			Interval x0, x1, xb0, xb1, fxb0, fxb1, c0, c1, TEMP1, TEMP2;
			double alpha, beta, ddelta, t1;

			x0 = 1.0 / sqrt(Interval(2));
			xb0 = Interval(0.5) * (itv.diam() * (x0) + itv.lb() + itv.ub());
			xb1 = Interval(0.5) * (itv.diam() * (-x0) + itv.lb() + itv.ub());
			c0 = Interval(0.5) * (pow(xb0, n) + pow(xb1, n));
			c1 = x0 * pow(xb0, n) - x0 * pow(xb1, n);

			TEMP1 = 2 * c1 / (itv.diam());
			TEMP2 = c0 - c1 * ((itv.lb() + itv.ub()) / (itv.diam()));

			alpha = TEMP1.mid();
			beta = TEMP2.mid();
			ddelta = ((_n * Interval(TEMP1.rad())) + Interval(TEMP2.rad())).ub();


			// compute the maximal error
			double max_error = 0.0;

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
//
		}

	}
	//	std::cout << "out power "<<std::endl;
	return *this;
}

//TODO debut linChebyshev
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

	// Particular case
	bool b = (!(res_itv.is_empty()||res_itv.is_unbounded()));
	if (!b) {
		*this = res_itv;
	} else if ((!is_actif())||(itv.diam()<AF_EC())) {
		*this = res_itv;
	}  else  { // _actif && b


		double alpha, beta, ddelta, t1, t2;
		Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0);

		switch (num) {
		// alpha = (F(sup(x)) - F(inf(x)))/diam(X)
		// u = (f')^{-1}(alpha)
		// d_a = f(inf(x)) -alpha*inf(X)
		// d_b = f(sup(x)) -alpha*sup(x)
		// d_min = min(d_a,d_b)
		// d_max = f(u) - alpha*u
		// beta = Interval(d_min,d_max).mid()
		// zeta = Interval(d_min,d_max).rad()

		case AF_SQRT: {
			Interval itv2;
			if (itv.lb()<0) {
				itv2 = Interval(0.0,itv.ub());
			} else {
				itv2 = itv;
			}
			dmm = sqrt(itv2);
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

			beta = band.mid();
			t1 = (beta -band).ub();
			t2 = (band-beta).ub();
			ddelta = (t1>t2)? t1 : t2;

			saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);


			break;
		}
		case AF_EXP : {


			if (itv.is_unbounded()) {
				*this = res_itv;

			} else {

				dmm = res_itv;
				//alpha = (dmm.diam()/(Interval(itv.ub())-Interval(itv.lb()))).ub();
				alpha = (dmm.diam()/itv.diam());
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


				beta = band.mid();
				t1 = (beta -band).ub();
				t2 = (band-beta).ub();
				ddelta = (t1>t2)? t1 : t2;

				saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			}

			break;
		}
		case AF_LOG : {
			dmm = res_itv;
			//alpha = (dmm.diam()/(Interval(itv.ub())-Interval(itv.lb()))).lb();
			alpha = (dmm.diam()/itv.diam());
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


			beta = band.mid();
			t1 = (beta -band).ub();
			t2 = (band -beta).ub();
			ddelta = (t1>t2)? t1 : t2;

			saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);

			break;
		}
		case AF_INV : {
			if (itv.is_unbounded()) {
				*this = res_itv;
			}
			else {
				dmm = (1.0/abs(itv));
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


			dmm = res_itv;
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


			beta = band.mid();
			t1 = (beta -band).ub();
			t2 = (band-beta).ub();
			ddelta = (t1>t2)? t1 : t2;

			saxpy(alpha, Affine2(), beta, ddelta, true,false,true,true);


			break;
		}
		case AF_ABS : {
			if (0<=itv.lb()) {
				return *this;
			}
			else if (itv.ub()<=0) {
				return *this = -Affine2(*this);
			}
			else {

				dmm = res_itv;
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
		case AF_COS :
		case AF_SIN : {
			if (itv.diam()>=Interval::TWO_PI.lb()) {
				*this = Interval(-1,1);
				break;
			}
			//  pour _itv = [a,b]
			// x0 = 1/sqrt(2)
			// x1= - x0
			// xb0 = 0.5*((b-a)*x0 +(a+b))
			// xb1 = 0.5*((b-a)*x1 +(a+b))
			// c0 = 0.5 (f(xb0)+f(xb1))
			// c1 = x0*f(xb0)+x1*f(xb1)
			// alpha = 2*c1/(b-a)
			// beta = c0-c1*(a+b)/(b-a)
			//  old : ddelta = (b-a)^2 * f''(_itv)/16
			//  ddelta = evaluate the error at the bound and the points when f'(x)=alpha

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

	// inverse trigonometric function and
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
			//  pour _itv = [a,b]
			// x0 = 1/sqrt(2)
			// x1= - x0
			// xb0 = 0.5*((b-a)*x0 +(a+b))
			// xb1 = 0.5*((b-a)*x1 +(a+b))
			// c0 = 0.5 (f(xb0)+f(xb1))
			// c1 = x0*f(xb0)+x1*f(xb1)
			// alpha = 2*c1/(b-a)
			// beta = c0-c1*(a+b)/(b-a)
			//  old : ddelta = (b-a)^2 * f''(_itv)/16
			//  ddelta = evaluate the error at the bound and the points when f'(x)=alpha

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




/*
//TODO debut linMinRange  NOT USE !!!!
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
		*this = res_itv;
	} else if (!is_actif()) {
		*this =res_itv;


	} else  {  // _actif && b
		double alpha, beta, ddelta, t1, t2;
		Interval dmm(0.0), TEMP1(0.0), TEMP2(0.0), band(0.0), itv2;

		switch (num) {
		/// for _itv = [a,b]
		 //if f increase,
		 // alpha = f'(a)
		 // band = Interval(a- alpha*a , b-alpha*b)
		 //if f decrease
		 // alpha = f'(b)
		 // band = Interval(a- alpha*b , b-alpha*a)
		 //
		 // beta = band.mid()
		 //  ddelta = band.rad()
		 //

		case AF_SQRT: {
			//if ((itv.ub() == POS_INFINITY) || (itv.ub()<0) ) {
			//	_err = sqrt(itv);
			//	_actif = false;
			//	delete[] _val;
			//	_val = NULL;
			//}
			//else {
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

			//if ((itv.lb()<= 0) ||(itv.ub()==POS_INFINITY) ) {
			//	_err = log(itv);
			//	_actif = false;
			//	delete[] _val;
			//	_val = NULL;
			//}
			//else {

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




*/



}// end namespace ibex













