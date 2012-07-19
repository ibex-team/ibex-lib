
class Test {
	
	public static void main(String[] args) {
		
		Ibex ibex=new Ibex();
		
		ibex.add_ctr(2,"{0}-{1}=0");
		
		double domains[]={1.0,10.0,1.0,10.0};
		System.out.println("Before contract:");
		System.out.println("(["+domains[0]+","+domains[1]+"] ; ["+domains[2]+","+domains[3]+"])");
		
		int result=ibex.contract(0,domains);
		
		if (result==Ibex.FAIL) {
			System.out.println("Failed!");
		} else if (result==Ibex.CONTRACT) {
			System.out.println("After contract:");
			System.out.println("(["+domains[0]+","+domains[1]+"] ; ["+domains[2]+","+domains[3]+"])");
		} else {
			System.out.println("Nothing.");	
		}
		 
		ibex=new Ibex();
		
		ibex.release();
	}
}