import ibex.Ibex;
import org.junit.*;
import org.junit.Test;
import static org.junit.Assert.*;

import java.util.*;

public class IbexTest {
	
	public String strDomain(double[] a) {
		return "["+a[0]+";"+a[1]+"] ["+a[2]+";"+a[3]+"]";
	}

	public void cmpDomains(double[] a1, double[] a2) {
		double DELTA=1e-10;
		for (int i=0; i<a1.length; i++)
			Assert.assertEquals(a1[i],a2[i],DELTA);
	}

	public IbexTest() { }
	
	/*@Test
	public void test1() {

        Ibex ibex = new Ibex();

        ibex.add_ctr(2, "{0}+{1}=3");

        double domains[] = {1.0, 10.0, 1.0, 10.0};
        System.out.println("Before contract:");
        System.out.println("([" + domains[0] + "," + domains[1] + "] ; [" + domains[2] + "," + domains[3] + "])");

        int result = ibex.contract(0, domains);

        if (result == Ibex.FAIL) {
            System.out.println("Failed!");
        } else if (result == Ibex.CONTRACT) {
            System.out.println("After contract:");
            System.out.println("([" + domains[0] + "," + domains[1] + "] ; [" + domains[2] + "," + domains[3] + "])");
        } else {
            System.out.println("Nothing.");
        }
        ibex.release();
    }*/
	
	
	@Test
    public void test2() {
        Ibex ibex = new Ibex();
        ibex.add_ctr(2, "{0}^2+{1}^2<=1");

        double[] domains;
        double vv = Math.sqrt(2.) / 2.;
        	
        // CASE 1: the boolean is set to TRUE
        System.out.println("Case 1");
        Assert.assertEquals(ibex.contract(0, new double[]{2., 3., 2., 3.}, Ibex.TRUE), Ibex.FAIL);
        Assert.assertEquals(ibex.contract(0, new double[]{-.5, .5, -.5, .5}, Ibex.TRUE), Ibex.ENTAILED);
        domains = new double[]{-2., 1., -2., 1.};
        Assert.assertEquals(ibex.contract(0, domains, Ibex.TRUE), Ibex.CONTRACT);
        cmpDomains(domains, new double[]{-1., 1., -1., 1.});
        Assert.assertEquals(ibex.contract(0, domains, Ibex.TRUE), Ibex.NOTHING);


        // CASE 2: the boolean is set to FALSE
        System.out.println("Case 2");
        Assert.assertEquals(ibex.contract(0, new double[]{2., 3., 2., 3.}, Ibex.FALSE), Ibex.FAIL);
        Assert.assertEquals(ibex.contract(0, new double[]{-.5, .5, -.5, .5}, Ibex.FALSE), Ibex.ENTAILED);
        Assert.assertEquals(ibex.contract(0, new double[]{-2., 1., -2., -1.}, Ibex.FALSE), Ibex.NOTHING);
        domains = new double[]{0., 2., -vv, vv};
        Assert.assertEquals(ibex.contract(0, domains, Ibex.FALSE), Ibex.CONTRACT);
        cmpDomains(domains, new double[]{vv, 2., -vv, vv});

        // CASE 3: the boolean is set to UNKNOWN
        System.out.println("Case 3");
        Assert.assertEquals(ibex.contract(0, new double[]{2., 3., 2., 3.}, Ibex.FALSE_OR_TRUE), Ibex.FAIL);
        Assert.assertEquals(ibex.contract(0, new double[]{-.5, .5, -.5, .5}, Ibex.FALSE_OR_TRUE), Ibex.ENTAILED);
        Assert.assertEquals(ibex.contract(0, new double[]{-2., 1., -2., -1.}, Ibex.FALSE_OR_TRUE), Ibex.NOTHING);
        domains = new double[]{0., 2., -vv, vv};
        Assert.assertEquals(ibex.contract(0, domains, Ibex.FALSE_OR_TRUE), Ibex.NOTHING);
        cmpDomains(domains, new double[]{0., 2., -vv, vv});

        ibex.release();
    }
	
	@Test
    public void test3() {
        Ibex ibex = new Ibex();
        ibex.add_ctr(2, "{0}^2+{1}^2<=1");

        double[] domains;

        domains = new double[]{0., 1., 0., 1.};
        
       Assert.assertEquals(ibex.inflate(0, new double[]{0., 0.}, domains, true), Ibex.INFLATE);
        Assert.assertEquals(ibex.inflate(0, new double[]{0., 0.}, domains, true), Ibex.FULL_INFLATE);
        domains = new double[]{1., 2., 1., 2.};
        Assert.assertEquals(ibex.inflate(0, new double[]{1., 1.}, domains, true), Ibex.BAD_POINT);
        domains = new double[]{0., 1., -1., 0.};
        Assert.assertEquals(ibex.inflate(0, new double[]{1., 0.}, domains, true), Ibex.NOT_SIGNIFICANT);

        domains = new double[]{-1., 0., -1., 0.};
        Assert.assertEquals(ibex.inflate(0, new double[]{-1., -1.}, domains, false), Ibex.INFLATE);
        Assert.assertEquals(ibex.inflate(0, new double[]{-1., -1.}, domains, false), Ibex.FULL_INFLATE);
        domains = new double[]{0., .5, 0., .5};
        Assert.assertEquals(ibex.inflate(0, new double[]{0., 0.}, domains, false), Ibex.BAD_POINT);
        domains = new double[]{0., 1.01, -1., 0.};
        Assert.assertEquals(ibex.inflate(0, new double[]{1.01, 0.}, domains, false), Ibex.NOT_SIGNIFICANT);

        ibex.release();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("Test");
      }
}