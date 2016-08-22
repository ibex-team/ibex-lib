#include "ibex_Random.h"
#include <stdlib.h>

bool ibex::RNG::srand(){ ::srand(::rand()+10);  return true; }

bool ibex::RNG::srand(unsigned long s){  ::srand(s); return true; }

uint32_t ibex::RNG::rand () { return ::rand(); }
