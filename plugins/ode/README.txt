

=============== Lyapunov plugin ========================

Only the executable "find-level-set" is installed by default
on the "bin" folder of IBEX. 

To run the "academic benchmark" of the paper:

1- set the PKG_CONFIG_PATH variable:

   $ export PKG_CONFIG_PATH=[path-to-ibex]/share/pkgconfig

2- compile the programs (from this directory)

   $ make

4- run the program. Example:

   $ ./dicho-scalable examples/scalable/Scalable2_ 2 10 1e-3 10


