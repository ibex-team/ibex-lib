
class TestQuimper {

    public static void main(String[] args) {

	Quimper q = new Quimper("ring.qpr");
	
	System.out.println(q.get_lb("x[1]")+" "+q.get_ub("x[2]"));

	boolean res=q.contract("sat");
	System.out.println("feasible? "+res);

	System.out.println(q.get_lb("x[1]")+" "+q.get_ub("x[2]"));

	q.set_domain("x[1]",-7,-7);
	q.set_domain("x[2]",-7,-7);

	res=q.contract("sat");
	System.out.println("feasible? "+res);


    }


}