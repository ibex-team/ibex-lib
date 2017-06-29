/* ============================================================================
 * I B E X - TestAgenda
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2017
 * ---------------------------------------------------------------------------- */

#include "TestAgenda.h"
#include "ibex_Agenda.h"
#include "utils.h"
#include <float.h>

using namespace std;

void TestAgenda::test01() {
	Agenda a(10);
	a.push(1);
	a.push(4);
	a.push(1);
	a.push(0);
	a.push(7);
	int i=a.first();
	CPPUNIT_ASSERT(i==1);
	i=a.next(i);
	CPPUNIT_ASSERT(i==4);
	i=a.next(i);
	CPPUNIT_ASSERT(i==0);
	i=a.next(i);
	CPPUNIT_ASSERT(i==7);
	i=a.next(i);
	CPPUNIT_ASSERT(i==a.end());
}

void TestAgenda::copy() {
	Agenda a1(10);
	a1.push(1);
	a1.push(4);
	a1.push(1);
	a1.push(0);
	a1.push(7);
	Agenda a(a1);
	int i=a.first();
	CPPUNIT_ASSERT(i==1);
	i=a.next(i);
	CPPUNIT_ASSERT(i==4);
	i=a.next(i);
	CPPUNIT_ASSERT(i==0);
	i=a.next(i);
	CPPUNIT_ASSERT(i==7);
	i=a.next(i);
	CPPUNIT_ASSERT(i==a.end());
}

void TestAgenda::swap() {
	Agenda a1(10);
	a1.push(1);
	a1.push(4);
	a1.push(1);
	a1.push(0);
	a1.push(7);
	Agenda a(a1,true);
	int i=a.first();
	CPPUNIT_ASSERT(i==7);
	i=a.next(i);
	CPPUNIT_ASSERT(i==0);
	i=a.next(i);
	CPPUNIT_ASSERT(i==4);
	i=a.next(i);
	CPPUNIT_ASSERT(i==1);
	i=a.next(i);
	CPPUNIT_ASSERT(i==a.end());
}

void TestAgenda::pop01() {
	Agenda a(10);
	a.push(1);
	a.push(4);
	a.push(1);
	a.push(0);
	a.push(7);
	int i;
	CPPUNIT_ASSERT(!a.empty());
	a.pop(i);
	CPPUNIT_ASSERT(i==1);
	CPPUNIT_ASSERT(!a.empty());
	a.pop(i);
	CPPUNIT_ASSERT(i==4);
	CPPUNIT_ASSERT(!a.empty());
	a.pop(i);
	a.push(1);
	CPPUNIT_ASSERT(i==0);
	CPPUNIT_ASSERT(!a.empty());
	a.pop(i);
	CPPUNIT_ASSERT(i==7);
	CPPUNIT_ASSERT(!a.empty());
	a.pop(i);
	CPPUNIT_ASSERT(i==1);
	CPPUNIT_ASSERT(a.empty());
}

void TestAgenda::push01() {
	Agenda a(10);
	a.push(1);
	a.push(4);
	a.push(0);
	a.push(7);

	Agenda a2(10);
	a2.push(2);
	a2.push(7);
	a2.push(3);
	a2.push(4);
	a2.push(6);

	a.push(a2);
	int i=a.first();
	CPPUNIT_ASSERT((i==1));
	CPPUNIT_ASSERT((i=a.next(i))==4);
	CPPUNIT_ASSERT((i=a.next(i))==0);
	CPPUNIT_ASSERT((i=a.next(i))==7);
	CPPUNIT_ASSERT((i=a.next(i))==2);
	CPPUNIT_ASSERT((i=a.next(i))==3);
	CPPUNIT_ASSERT((i=a.next(i))==6);
	CPPUNIT_ASSERT(((i=a.next(i))==a.end()));
}

