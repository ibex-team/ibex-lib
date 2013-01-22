
#include "IbexSystem.h"
#include "IbexSystemBuilder.h"

int main() {
 IbexSystem sys("ponts.txt");
 IbexSubSystemBuilder builder (sys);

 builder.set_entity (IBEX_VAR,29,IBEX_SYB);
 builder.add_all_ctr();

 IbexSystem sysblock (builder);
 sysblock.sync(sys);

 IbexBlockSet bs(30, 1);

 bs.set_blk(sysblock.var("A_y"), 0);
 IbexSystemArray decomp = sysblock.decompose(bs);
 cout << decomp[0] << endl;

 return 0;
}
