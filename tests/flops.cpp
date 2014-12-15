// ============================================================================
// program copy-pasted from here:
// https://www.daniweb.com/software-development/cpp/threads/391888/wrote-code-that-measures-flops-would-be-nice-if-someone-could-review-it
// ============================================================================
#include <iostream>
#include <string>
#include <ctime>
#include <stdint.h>

// how many times the loops are run, the higher this number
// is, the longer it takes, but the more accurate it gets.
// max is 4294967295 (cause I'm using uint32_t)
//#define LOOP_REPS 4294967295
const uint32_t LOOP_REPS=100000000; //4294967295;

using namespace std;

int main(int argc, char *argv[]) {
	cout.setf(ios_base::fixed); // shows decimals in the output
	//cout << "loop_reps: " << LOOP_REPS << endl;
	// reference loop
	clock_t rl_start = clock();
	// loop index is volatile so that the empty loop isn't optimized away
	for(volatile uint32_t rl_index = 0; rl_index < LOOP_REPS; ++rl_index) {
		// empty loop - just to calculate how much time an empty loop needs
	}
	clock_t rl_end = clock();
	double rl_time = difftime(rl_end, rl_start) / CLOCKS_PER_SEC;
	// output the time the reference loop took
	cout << "cl_time: " << rl_time << endl;
	// flops loop
	volatile float a = 1.5;
	volatile float b = 1.6;
	clock_t fl_start = clock();
	for(volatile uint32_t fl_index = 0; fl_index < LOOP_REPS; ++fl_index) {
		a *= b; // multiplication operation
		b += a; // addition operation
	}
	clock_t fl_end = clock();
	double fl_time = difftime(fl_end, fl_start) / CLOCKS_PER_SEC;
	unsigned long flops = LOOP_REPS / ((fl_time - rl_time) / 2);
	cout << "fl_time: " << fl_time << endl;
	cout << "flops: " << flops << endl;
}
