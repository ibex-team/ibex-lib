#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <set>
using namespace std;
using namespace ibex;




 int main(int argc, char** argv) {


	int inliers=0;
	ifstream input("inliers.txt");
	int in;
	while (input >> in)
	  {
	   
	    if (in)
	      cout << inliers << " " ;
	    inliers++;
	  }
 }
	 
