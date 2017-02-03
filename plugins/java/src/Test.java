import org.junit.*;
import org.junit.Test;
import static org.junit.Assert.*;

class Test {
	
	public static String strDomain(double[] a) {
		String s="";
	 	for (int i=0; i<a.length/2; i++) {
	 		s += s + "["+a[2*i]+";"+a[2*i+1]+"]";
	 	}
		return s;
	}
	
	public void cmpDomains(double[] a1, double[] a2) {
		double DELTA=1e-10;
		for (int i=0; i<a1.length; i++)
			Assert.assertEquals(a1[i],a2[i],DELTA);
	}
	
	@Test
	public void test1() {
        Ibex ibex=new Ibex(new int[]{0,0,1}, 1e-2);
        ibex.add_ctr("{0}^2+{1}^2={2}^2");
        ibex.add_ctr("({0}-1)^2+{1}^2={2}^2");
        double domains[]={-3,3,-3,3,1,2};
        int result=ibex.start_solve(domains);
        Assert.assertEquals(result, DISCRETE_NOT_INSTANCIATED);
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("Test");
    	
		/*
		Ibex ibex=new Ibex(new int[]{0,0}, 1e-2);  // all reals
		
		ibex.add_ctr("{0}-{1}=0"); 
		
		double domains[]={1.5,10.5,5.5,12.0};
		System.out.println("Before contract:");
		System.out.println(strDomain(domains));
		
		int result=ibex.contract(0,domains);
		
		if (result==Ibex.FAIL) {
			System.out.println("Failed!");
		} else if (result==Ibex.CONTRACT) {
			System.out.println("After contract #0:");
			System.out.println(strDomain(domains));
		} else {
			System.out.println("Nothing.");	
		}
		ibex.release();
        */
        
        /* Test several instances */
   		/*
	    Ibex ibex0 =new Ibex(new int[]{0}, 1e-2);
        ibex0.add_ctr("{0}>=0");
        Ibex ibex1 =new Ibex(new int[]{0}, 1e-2);
        ibex1.add_ctr("{0}<=2");
        Ibex ibex2 =new Ibex(new int[]{0}, 1e-2);
        ibex2.add_ctr("{0}=1");
        double domains[]={-3,3};
        System.out.println("Before contract:");
		System.out.println(strDomain(domains));
		int result=ibex0.contract(0,domains);
		System.out.println("After contract #0:");
		System.out.println(strDomain(domains));
		result=ibex1.contract(0,domains);
		System.out.println("After contract #1:");
		System.out.println(strDomain(domains));
		result=ibex2.contract(0,domains);
		System.out.println("After contract #2:");
		System.out.println(strDomain(domains));
        ibex0.release();
        ibex1.release();
        ibex2.release();
        */
       
        
        
		/*
		Ibex ibex = new Ibex(new int[]{0,1,0,1}, 1e-2);
        
        ibex.add_ctr("{0}^2+{2}^2={1}^2");
        ibex.add_ctr("({0}-1)^2+{2}^2={3}^2");
		
		double domains[]={-10,10,1,1,-10,10,1,1};
		int result=ibex.contract(0,domains);
		if (result==Ibex.FAIL) {
			System.out.println("Failed!");
		} else if (result==Ibex.CONTRACT) {
			System.out.println("After contract #0:");
			System.out.println(strDomain(domains));
		} else {
			System.out.println("Nothing.");	
		}
		 
		 	
		result=ibex.contract(1,domains);
		
		if (result==Ibex.FAIL) {
			System.out.println("Failed!");
		} else if (result==Ibex.CONTRACT) {
			System.out.println("After contract #1:");
			System.out.println(strDomain(domains));
		} else {
			System.out.println("Nothing.");	
		}
		ibex.release();
		
		*/
		 
//		ibex.add_int_ctr(2);
//		ibex.contract(1,domains);
//		System.out.println("After contract #1:");
//		System.out.println("(["+domains[0]+","+domains[1]+"] ; ["+domains[2]+","+domains[3]+"])");		
		 
		
	}
}