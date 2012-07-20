public class Ibex {

	/* Possible contraction strategies. */
	public static final int COMPO      = 0;
	public static final int HC4        = 1;
	public static final int HC4_NEWTON = 2;
	
	/* Constants for the status of a contraction. */
	public static final int FAIL     = 0;
	public static final int ENTAILED = 1;
	public static final int CONTRACT = 2;
	public static final int NOTHING  = 3;
	
	/* Constants for describing a boolean domain (by an integer). */
	public static final int FALSE         = 0;
	public static final int TRUE          = 1;
	public static final int FALSE_OR_TRUE = 2;
	
    static {
    	System.loadLibrary("ibex");
    }

    /**
     * Create a new IBEX constraint with a default contractor.
     * 
     * The default contractor is COMPO.
     * 
     * Example: add_ctr(2,{0}={1}) will add the constraint x=y.
     * 
     * @param nb_var - Number of variables.
     * @param syntax - The constraint
     */
    public native void add_ctr(int nb_var, String syntax);
    
    /**
     * Same as add_ctr except that a specific contractor is used.
     * 
     * @param nb_var - Number of variables
     * @param syntax - The constraint
     * @param option - A value between COMPO, HC4 or HC4_NEWTON.
     */
    public native void add_ctr(int nb_var, String syntax, int option);
    
    /**
     * Call the contractor associated to a constraint.
     * 
     * @param i      - Number of the constraint (in the order of creation)
     * @param bounds - The bounds of domains under the following form:
     *                 (x1-,x1+,x2-,x2+,...,xn-,xn+), where xi- (resp. xi+) is the
     *                 lower (resp. upper) bound of the domain of the ith variable.
     * @param reif   - Domain of the reification with the following accepted values:
     *                 FALSE, TRUE, FALSE_OR_TRUE.
     *              
     * @return The status of contraction or fail/entailment test:
     * 
     *    FAIL     - No tuple satisfy the constraint. This value can only be returned
     *               if reif=1 or reif=2. Bounds of are not impacted.
     *        
     *    ENTAILED - All the tuples satisfy the constraint. This value can only be 
     *               returned if reif=0 or reif=2. Bounds of are not impacted.
     *        
     *    CONTRACT - At least one bound has been reduced by more than 0.1%. This 
     *               value can only be returned if reif=0 or reif=1. If reif=1 (resp. 0),  
     *               all the removed part is inconsistent (resp. consistent).
     *        
     *    NOTHING  - No bound has been reduced and nothing could be proven.
     */
    public native int contract(int i, double bounds[], int reif);    
 
    
    /**
     * Same as contract(int, double bounds[], int reif) with reif=1.
     */
    public native int contract(int i, double bounds[]);   
    

    /**
     * Free IBEX structures from memory
     */
    public native void release();
};