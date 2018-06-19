/*
 * ibex_Id.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: gilles
 */

#include <atomic>

using namespace std;

namespace {

atomic_long id_count(0);

}

namespace ibex {

long next_id() {
	return id_count++;
}

}
