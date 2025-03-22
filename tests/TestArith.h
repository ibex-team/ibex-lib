/* ============================================================================
 * I B E X - Test of interval operations
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * Last Update : Oct 29, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_ARITH_H__
#define __TEST_ARITH_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "ibex_Interval.h"
#include "utils.h"

using namespace ibex;

class TestArith : public CppUnit::TestFixture {
public:

	void testMe() {
		setUp();
	
		tan_issue248();

		float01();
		float02();
		float03();
		float04();
		float05();
		float06();
		float07();
		float08();
		float09();
	/*	float10(); disabled temporarily due to bug between Gaol and elibc (used by Travis CI) */


		minus01();
		minus02();
		minus03();
		minus04();

		add01();
		add02();
		add03();
		add04();
		add05();
		add06();
		add07();
		add08();
		add09();
		add10();
		add11();
		add12();
		add13();
		add14();
		add15();
		add16();
		add17();
		add18();
		add19();

		mul01();
		mul02();
		mul03();
		mul04();
		mul05();
		mul06();
		mul07();
		mul08();
		mul09();
		mul10();
		mul11();
		mul12();
		mul13();
		mul14();
		mul15();
		mul16();
		mul17();
		mul18();
		mul19();

		mulMM01();

		div01();
		div02();
		div03();
		div04();
		div05();
		div06();
		div07();
		div08();
		div09();
		div10();
		div11();
		div12();
		div13();
		div14();
		div15();
		div16();
		div17();

		div2_01();
		div2_02();
		div2_03();
		div2_04();
		div2_05();
		div2_06();
		div2_07();
		div2_08();
		div2_09();
		div2_10();
		div2_11();
		div2_12();
		div2_13();

		log01();
		log02();
		log03();
		log04();
		log05();
		log06();
		log07();
		log08();
		log09();
		log10();
		log11();

		exp01();
		exp02();
		exp03();
		exp04();
		exp05();
		exp06();
		exp07();
		exp08();
		exp09();

		sin01();
		sin02();
		sin03();
		sin04();
		sin05();
		sin06();
		sin07();
		sin08();
		sin09();
		sin10();
		sin11();
		sin12();
		sin13();

		tan01();
		tan02();
		tan03();
		tan04();
		tan05();

		pow01();
		pow02();
		pow03();
		pow04();
		pow05();
		pow06();
		pow07();
		pow08();
		pow09();
		pow10();
		pow11();
		pow12();
		pow13();

		root01();
		root02();
		root03();
		root04();

		sqrt01();
		sqrt02();
		sqrt03();
		sqrt04();
		sqrt05();

		sinh01();
		sinh02();
		sinh03();
		sinh04();
		sinh05();
		sinh06();
		sinh07();

		cosh01();
		cosh02();
		cosh03();
		cosh04();
		cosh05();
		cosh06();
		cosh07();

		atan2_01();
		atan2_02();
		atan2_03();
		atan2_04();
		atan2_05();
		atan2_06();
		atan2_07();
		atan2_08();
		atan2_09();
		atan2_10();
		atan2_11();
		atan2_12();
		atan2_13();
		atan2_14();
		atan2_15();

		floor01();
		floor02();
		floor03();
		floor04();
		floor05();
		floor06();
		floor07();
		floor08();

		ceil01();
		ceil02();
		ceil03();
		ceil04();
		ceil05();
		ceil06();
		ceil07();
		ceil08();

		integer01();
		integer02();
		integer03();
		integer04();
		integer05();
		integer06();
		integer07();
		integer08();

		bwd_mul01();
		bwd_mul02();
		bwd_mul03();
		bwd_mul04();
		bwd_mul05();

		bwd_mulVV01();
		bwd_mulMV01();


		bwd_div01();
		bwd_div02();
		bwd_div03();
		bwd_div04();
		bwd_div05();
		bwd_div06();
		bwd_div07();
		bwd_div08();
		bwd_div09();

		bwd_maxmin01();
		bwd_maxmin02();
		bwd_maxmin03();
		bwd_maxmin04();
		bwd_maxmin05();

		bwd_sqr01();
		bwd_sqr02();
		bwd_sqr03();
		bwd_sqr04();
		bwd_sqr05();
		bwd_sqr06();
		bwd_sqr07();

		bwd_log01();
		bwd_log02();
		bwd_log03();
		bwd_log04();
		bwd_log05();
		bwd_log06();
		bwd_log07();
		bwd_log08();

		bwd_sin01();
		bwd_sin02();
		bwd_sin03();
		bwd_sin04();
		bwd_sin05();
		bwd_sin06();
		bwd_sin07();
		bwd_sin08();
		bwd_sin09();
		bwd_sin10();
		bwd_sin11();
		bwd_sin12();
		bwd_sin13();
		bwd_sin14();
		bwd_sin15();
		bwd_sin16();
		bwd_sin17();
		bwd_sin18();
		bwd_sin19();
		bwd_sin20();
		bwd_sin21();
		bwd_sin22();
		bwd_sin23();
		bwd_sin24();

		bwd_pow01();
		bwd_pow02();
		bwd_pow03();
		bwd_pow04();
		bwd_pow05();
		bwd_pow06();
		bwd_pow07();
		bwd_pow08();
		bwd_pow09();
		bwd_pow10();
		bwd_pow11();
		bwd_pow12();
		bwd_pow13();
		bwd_pow14();
		bwd_pow15();
		bwd_pow16();
		bwd_pow17();

	// ===== temporarily commented ===============
				bwd_atan2_01();
		bwd_atan2_02();
		bwd_atan2_03();
		bwd_atan2_04();
		bwd_atan2_05();
		bwd_atan2_06();
		bwd_atan2_07();
		bwd_atan2_08();
		bwd_atan2_09(); // commented waiting for an optimal contractor.
		bwd_atan2_10();
		bwd_atan2_issue134();

		bwd_sqrt01();
		bwd_sqrt02();
		bwd_sqrt03();
		bwd_sqrt04();
		bwd_sqrt05();
		bwd_sqrt06();
		bwd_sqrt07();

		bwd_atan01();
		bwd_atan02();
		bwd_atan03();
		bwd_atan04();
		bwd_atan05();
		bwd_atan06();
		bwd_atan07();
		bwd_atan08();

		bwd_add01();
		bwd_add02();
		bwd_add03();
		bwd_add04();
		bwd_add05();

		bwd_sub01();
		bwd_sub02();
		bwd_sub03();
		bwd_sub04();
		bwd_sub05();

		bwd_imod_01();
		bwd_imod_02();
		bwd_imod_03();
		bwd_imod_04();
		bwd_imod_05();
		bwd_imod_06();
		bwd_imod_07();
		bwd_imod_08();

		bwd_floor_01();
		bwd_floor_02();
		bwd_floor_03();
		bwd_floor_04();
		bwd_floor_05();
		bwd_floor_06();
		bwd_floor_07();
		bwd_floor_08();
		bwd_floor_09();
		bwd_floor_10();
		bwd_floor_11();
		bwd_floor_12();
		bwd_floor_13();

		bwd_ceil_01();
		bwd_ceil_02();
		bwd_ceil_03();
		bwd_ceil_04();
		bwd_ceil_05();
		bwd_ceil_06();
		bwd_ceil_07();
		bwd_ceil_08();
		bwd_ceil_09();
		bwd_ceil_10();
		bwd_ceil_11();
		bwd_ceil_12();
		bwd_ceil_13();

		tearDown();
	}
private:
	/* test:
	 * =======
	 * previous_float(double x)
	 * next_float(double x)
	 */
	void float01();
	void float02();
	void float03();
	void float04();
	void float05();
	void float06();
	void float07();
	void float08();
	void float09();
	void float10();

	/* test: operator&=(const Interval& x) */
	void minus01();
	void minus02();
	void minus03();
	void minus04();

	/* test:
	 * =======
	 *   operator+=(double d)
	 *   operator+=(const Interval& x)
	 *   operator+(const Interval& x, double d)
	 *   operator+(double d,const Interval& x)
	 *   operator+(const Interval& x1, const Interval& x2)
	 *   operator-=(double d)
	 *   operator-=(const Interval& x)
	 *   operator-(const Interval& x, double d)
	 *   operator-(double d,const Interval& x)
	 *   operator-(const Interval& x1, const Interval& x2)
	 */
	void add01();
	void add02();
	void add03();
	void add04();
	void add05();
	void add06();
	void add07();
	void add08();
	void add09();
	void add10();
	void add11();
	void add12();
	void add13();
	void add14();
	void add15();
	void add16();
	void add17();
	void add18();
	void add19();

	/* test:
	 * =======
	 *   operator*=(double d)
	 *   operator*=(const Interval& x)
	 *   operator*(const Interval& x, double d)
	 *   operator*(double d,const Interval& x)
	 *   operator*(const Interval& x1, const Interval& x2)
	 */
	void mul01();
	void mul02();
	void mul03();
	void mul04();
	void mul05();
	void mul06();
	void mul07();
	void mul08();
	void mul09();
	void mul10();
	void mul11();
	void mul12();
	void mul13();
	void mul14();
	void mul15();
	void mul16();
	void mul17();
	void mul18();
	void mul19();

	void mulMM01();

	/* test:
	 * =======
	 *   operator/=(double d)
	 *   operator/=(const Interval& x)
	 *   operator/(const Interval& x, double d)
	 *   operator/(double d,const Interval& x)
	 *   operator/(const Interval& x1, const Interval& x2)
	 */

	void div01();
	void div02();
	void div03();
	void div04();
	void div05();
	void div06();
	void div07();
	void div08();
	void div09();
	void div10();
	void div11();
	void div12();
	void div13();
	void div14();
	void div15();
	void div16();
	void div17();

	/* test:
	 * =========
	 *  div2_inter(const Interval& x, const Interval& y, Interval& out2)
	 *  div2_inter(const Interval& x, const Interval& y)
	 *  div2(const Interval& x, const Interval& y, Interval& out1, Interval& out2)
	 */
	void div2_01();
	void div2_02();
	void div2_03();
	void div2_04();
	void div2_05();
	void div2_06();
	void div2_07();
	void div2_08();
	void div2_09();
	void div2_10();
	void div2_11();
	void div2_12();
	void div2_13();

	void log01();
	void log02();
	void log03();
	void log04();
	void log05();
	void log06();
	void log07();
	void log08();
	void log09();
	void log10();
	void log11();

	void exp01();
	void exp02();
	void exp03();
	void exp04();
	void exp05();
	void exp06();
	void exp07();
	void exp08();
	void exp09();

	/* test:
	 * sin
	 * cos
	 */
	void sin01();
	void sin02();
	void sin03();
	void sin04();
	void sin05();
	void sin06();
	void sin07();
	void sin08();
	void sin09();
	void sin10();
	void sin11();
	void sin12();
	void sin13();

	void tan01();
	void tan02();
	void tan03();
	void tan04();
	void tan05();
	void tan06();
	void tan07();

	void pow01();
	void pow02();
	void pow03();
	void pow04();
	void pow05();
	void pow06();
	void pow07();
	void pow08();
	void pow09();
	void pow10();
	void pow11();
	void pow12();
	void pow13();

	void root01();
	void root02();
	void root03();
	void root04();

	/* test: sqrt(const Interval& x); */
	void sqrt01();
	void sqrt02();
	void sqrt03();
	void sqrt04();
	void sqrt05();
	void sqrt06();

	void sinh01();
	void sinh02();
	void sinh03();
	void sinh04();
	void sinh05();
	void sinh06();
	void sinh07();

	void cosh01();
	void cosh02();
	void cosh03();
	void cosh04();
	void cosh05();
	void cosh06();
	void cosh07();

	void atan2_01();
	void atan2_02();
	void atan2_03();
	void atan2_04();
	void atan2_05();
	void atan2_06();
	void atan2_07();
	void atan2_08();
	void atan2_09();
	void atan2_10();
	void atan2_11();
	void atan2_12();
	void atan2_13();
	void atan2_14();
	void atan2_15();

	void floor01();
	void floor02();
	void floor03();
	void floor04();
	void floor05();
	void floor06();
	void floor07();
	void floor08();

	void ceil01();
	void ceil02();
	void ceil03();
	void ceil04();
	void ceil05();
	void ceil06();
	void ceil07();
	void ceil08();

	void integer01();
	void integer02();
	void integer03();
	void integer04();
	void integer05();
	void integer06();
	void integer07();
	void integer08();

	/* test: bwd_mul */
	void bwd_mul01();
	void bwd_mul02();
	void bwd_mul03();
	void bwd_mul04();
	void bwd_mul05();

	void bwd_mulVV01();
	void bwd_mulMV01();

	/* test: bwd_div */
	void bwd_div01();
	void bwd_div02();
	void bwd_div03();
	void bwd_div04();
	void bwd_div05();
	void bwd_div06();
	void bwd_div07();
	void bwd_div08();
	void bwd_div09();

	void bwd_maxmin01();
	void bwd_maxmin02();
	void bwd_maxmin03();
	void bwd_maxmin04();
	void bwd_maxmin05();

	/* test: bwd_sqrt(const Interval& y, Interval& x) */
	void bwd_sqr01();
	void bwd_sqr02();
	void bwd_sqr03();
	void bwd_sqr04();
	void bwd_sqr05();
	void bwd_sqr06();
	void bwd_sqr07();

	void bwd_log01();
	void bwd_log02();
	void bwd_log03();
	void bwd_log04();
	void bwd_log05();
	void bwd_log06();
	void bwd_log07();
	void bwd_log08();

	/* test: bwd_sin & bwd_cos */
	void bwd_sin01();
	void bwd_sin02();
	void bwd_sin03();
	void bwd_sin04();
	void bwd_sin05();
	void bwd_sin06();
	void bwd_sin07();
	void bwd_sin08();
	void bwd_sin09();
	void bwd_sin10();
	void bwd_sin11();
	void bwd_sin12();
	void bwd_sin13();
	void bwd_sin14();
	void bwd_sin15();
	void bwd_sin16();
	void bwd_sin17();
	void bwd_sin18();
	void bwd_sin19();
	void bwd_sin20();
	void bwd_sin21();
	void bwd_sin22();
	void bwd_sin23();
	void bwd_sin24();

	void bwd_pow01();
	void bwd_pow02();
	void bwd_pow03();
	void bwd_pow04();
	void bwd_pow05();
	void bwd_pow06();
	void bwd_pow07();
	void bwd_pow08();
	void bwd_pow09();
	void bwd_pow10();
	void bwd_pow11();
	void bwd_pow12();
	void bwd_pow13();
	void bwd_pow14();
	void bwd_pow15();
	void bwd_pow16();
	void bwd_pow17();

	void bwd_atan2_01();
	void bwd_atan2_02();
	void bwd_atan2_03();
	void bwd_atan2_04();
	void bwd_atan2_05();
	void bwd_atan2_06();
	void bwd_atan2_07();
	void bwd_atan2_08();
	void bwd_atan2_09();
	void bwd_atan2_10();
	void bwd_atan2_issue134();

	void bwd_sqrt01();
	void bwd_sqrt02();
	void bwd_sqrt03();
	void bwd_sqrt04();
	void bwd_sqrt05();
	void bwd_sqrt06();
	void bwd_sqrt07();

	void bwd_atan01();
	void bwd_atan02();
	void bwd_atan03();
	void bwd_atan04();
	void bwd_atan05();
	void bwd_atan06();
	void bwd_atan07();
	void bwd_atan08();

	void bwd_add01();
	void bwd_add02();
	void bwd_add03();
	void bwd_add04();
	void bwd_add05();

	void bwd_sub01();
	void bwd_sub02();
	void bwd_sub03();
	void bwd_sub04();
	void bwd_sub05();

	void bwd_imod_01();
	void bwd_imod_02();
	void bwd_imod_03();
	void bwd_imod_04();
	void bwd_imod_05();
	void bwd_imod_06();
	void bwd_imod_07();
	void bwd_imod_08();

	void bwd_floor_01();
	void bwd_floor_02();
	void bwd_floor_03();
	void bwd_floor_04();
	void bwd_floor_05();
	void bwd_floor_06();
	void bwd_floor_07();
	void bwd_floor_08();
	void bwd_floor_09();
	void bwd_floor_10();
	void bwd_floor_11();
	void bwd_floor_12();
	void bwd_floor_13();

	void bwd_ceil_01();
	void bwd_ceil_02();
	void bwd_ceil_03();
	void bwd_ceil_04();
	void bwd_ceil_05();
	void bwd_ceil_06();
	void bwd_ceil_07();
	void bwd_ceil_08();
	void bwd_ceil_09();
	void bwd_ceil_10();
	void bwd_ceil_11();
	void bwd_ceil_12();
	void bwd_ceil_13();

	void tan_issue248();


	void check_add_scal(const Interval& x, double z, const Interval& y_expected);
	void check_add(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_mul_scal(const Interval& x, double z, const Interval& y_expected);
	void check_mul(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div_scal(const Interval& x, double z, const Interval& y_expected);
	void check_div(const Interval& x, const Interval& z, const Interval& y_expected);
	void check_div2(const Interval& x, const Interval& y, const Interval& out1, const Interval& out2);
	void check_sinh(const Interval& x);
	void check_cosh(const Interval& x);
	void check_trigo(const Interval& x, const Interval& y);

	void check_pow(const Interval& x, int p, const Interval& y_expected);
	bool checkbwd_mul(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkbwd_div(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	void checkbwd_max(const Interval& z,  const Interval& x, const Interval& y, const Interval& x_expected, const Interval& y_expected);
	void checkbwd_trigo(const Interval& y, const Interval& xbefore, const Interval& xafter);
	void checkbwd_pow(const Interval& y, const Interval& xbefore, const Interval& xafter, int p);
	bool checkbwd_atan2(const Interval& a, const Interval& y_bef, const Interval& x_bef,
								const Interval& y_aft, const Interval& x_aft);
	bool checkbwd_atan(const Interval& y, const Interval& xbefore, const Interval& xafter);
	bool checkbwd_add(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	bool checkbwd_sub(const Interval& y, const Interval& x1_before, const Interval& x2_before, const Interval& x1_after, const Interval& x2_after);
	bool checkbwd_imod(const double& period, const Interval& x_bef, const Interval& y_bef,
								const Interval& x_aft, const Interval& y_aft);

	bool checkbwd_floor(const Interval& y, const Interval& xbefore, const Interval& x_after_expected);
	bool checkbwd_ceil(const Interval& y, const Interval& xbefore, const Interval& x_after_expected);

};




#endif // __TEST_ARITH__
