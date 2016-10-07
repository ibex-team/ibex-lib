#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Detects line in noisy pictures using q-intersection. 
 *
 * 
 * 
 */









/*===========================================*/



bool  max_dist (IntervalVector& v1, IntervalVector& v2, Vector& prec)
{double d;
  for (int i=0; i< v1.size(); i++)
    { if (v1[i].lb() > v2[i].ub()) 
	d= v1[i].lb() - v2[i].ub();
      else
	if (v2[i].lb() > v1[i].ub())
	  d= v2[i].lb() - v1[i].ub();
      if (d > 10* prec[i]) return false;
    }
  
  return true;
}
	 
int main(int argc, char** argv) {

	vector<double> *x = new vector<double>;
	vector<double> *y = new vector<double>;

	int NP = atoi(argv[1]);
	int Q = atoi(argv[2]);
        double epseq=atof(argv[3]);
	int flist = atoi(argv[4]);
	int fixpoint = atoi(argv[5]);
	int gaplimit= atoi(argv[6]);
	int nbrand= atoi(argv[7]);
	int sr= atoi(argv[8]);



	double cputime =0;
	double totaltime=0;
     
	srand(sr);
	for (int i=0;i<NP;i++)
	  {
	    x->push_back(((double)rand()/(double)RAND_MAX)*100);
	    y->push_back(((double)rand()/(double)RAND_MAX)*100);
	  }

        
	


	cout << " nb points " << x->size() << endl;

	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << endl;

	double time=0;	
	int n=2;
	
	Variable u(2);
	Variable w(2);
	
	int p = NP;

	int K=2;
	int np;

	int NT3=(p-1)/2;
	np=(p-1)*p/2;
	cout << np << "  " << Q << endl;

	/*
	Function ***m_fun;
        m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];
	*/

	double *** linfun;
        linfun = new double**[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double*[n];
	    for (int j=0; j<n; j++)
	      linfun[i][j]= new double[K];
	  }
	
	int ** index;
	index = new int*[np];
	for (int i=0; i<np; i++)
	  index[i]=new int [2];


	/*
	double *** linfun1;

	linfun1 = new double**[p];
	    for (int i=0; i<p; i++)
	      {  linfun1[i] = new double*[n];
		for (int j=0; j<n; j++)
		  linfun1[i][j]= new double[1];
	      }	   
	
	*/ 


	Function**  m_func= new Function* [p] ;	
	IntervalMatrix* boxes = new IntervalMatrix (np,n);	 
	Variable v(2);

	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0= clock();
	int Q0=Q;
	
	//	int* index2 = new int [np];
	//	int* index3= new int [np];
	
	


	for (int qua=0; qua <2; qua++){
	  Timer::start();
	  int dir= pow(-1,qua%2);

	
	    int pmax=0;
	    Array<Ctc> m_ctc(p);
	    //	    Array<Ctc> m_ctc1(np);
	    Array<Ctc> m_ctc1(1);


	    
	    
	    double _box[2][2];

	    _box[0][0]=0;
	    _box[0][1]=1;
	    _box[1][0]=-500;
	    _box[1][1]=500;
	    IntervalVector box(2,_box);
	    
	    //	    for (int i=0; i<p; i++) {
	    for (int i=0; i<1; i++) {

	      m_func[i] = new Function(v,(dir*x->at(i) + v[0]*(y->at(i)- dir*x->at(i))
					  -v[1]-Interval(-epseq,epseq)));
	     
	      m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));
	    }
	    
	    m_ctc1.set_ref(0,m_ctc[0]);
	    
	    for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
		//		m_ctc1.set_ref(i,m_ctc[i]);
		//		m_fun[0][i]= new Function(v,(dir*x->at(i) +v[0]*(y->at(i)-dir*x->at(i))-Interval(-epseq,epseq)));

		linfun[i][0][0]=dir*x->at(i);
		linfun[i][1][0]=y->at(i)- dir*x->at(i);
	
	    }
	    

	   

	    if(K==2)
	      {
		int pp=0;
		double a1,a2;
		Interval d1,d2,x1,z1;
		  for (int i=0; i<p; i++) {
                    a1=y->at(i)-dir*x->at(i);
                    d1=Interval(-epseq,epseq)+dir*x->at(i);
		    for (int j =i+1 ;j<p; j++){



                    a2=y->at(j)-dir*x->at(j);
                   

                    d2=Interval(-epseq,epseq)+dir*x->at(j);


		    x1= (d1-d2)/(a1-a2);

		    z1=d1 - a1*x1;
		    //	    z1&=d2 - a2*x1;
		    //		    z2=d2 - a2*x1;
		    //   cout << x1 << " " << z1 << " " << z2 << " " <<  endl;

                    
		    IntervalVector* box1 = new IntervalVector (box);
		    //		    cout << "box avant inter " << x1 << " " << z1 <<  endl;
		    
                    (*box1)[0]&=-x1;
		    (*box1)[1]&=z1;
		    //                    (*box1)[1]&=z2;



		    
		    if ((*box1)[0].is_empty()) box1->set_empty();
		    else if ((*box1)[1].is_empty()) box1->set_empty();

		    (*boxes)[pp]= *box1;
		    if (!(box1->is_empty())) pmax++;
		

		    //		    cout << "box apres inter " << pp << " " << *box1 << endl;
		   
		    //		    m_ctc1.set_ref(pp,*new CtcCompo (m_ctc[i],m_ctc[j]));
		    //		    m_ctc1.set_ref(pp,m_ctc[i]);

 
		    //		    m_fun[0][pp]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))-Interval(-epseq,epseq)));
		    //		    m_fun[1][pp]= new Function(v,(x->at(j) +v[0]*(y->at(j)-diry*x->at(j))-Interval(-epseq,epseq)));

	
		    index[pp][0]=i;
		    index[pp][1]=j;
	

		    //		    cout << " pp " << pp << " i " << i << " j " << j << endl;
		    pp++;
		    delete box1;
		    }

		  }
		  cout << " pp " << pp << " pmax " << pmax << endl;
	      }


	    vector<IntervalVector> resgroup;


	    Vector prec(2);

	   
	    // les precisions dans l'article soumis

	    /*

	    prec[0]=0.000007;
	    prec[1]=0.0007;
	    */

	    /*
	    prec[0]=0.000002;
	    prec[1]=0.0002;
	    */
	    
	    
	    prec[0]=1.e-8;
	    prec[1]=1.e-6;

	    

	    CellStack buff;

	    //	    RoundRobin bs (prec,0.5);
	    RoundRobinNvar bs (1,prec,0.5);
	    // RoundRobinQInter bs (2,prec,0.5);
	  
	    // LargestFirst bs (prec,0.5);


            CtcQInter* ctcq;	    
	    //	    CtcQInter ctcq(3,m_ctc1,Q);
	    //	    CtcQInterAff ctcq(3,m_ctc1,Q,m_fun,QINTERPROJ,K);
	    if (flist==1)
	      ctcq = new CtcQInterAffLine (n,np,m_ctc1,linfun,index,boxes,epseq,Q,QINTERPROJ,K);
	    else
	      ctcq= new  CtcQInterLine (n,np,m_ctc1,linfun,index,boxes,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERCORE,K);
	    //	    CtcQInterPlane  ctcq(n,m_ctc1,linfun,epseq,Q, QINTERCORE,K );
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERFULL, K);




	    /*
	   K=1;
	    
	    for (int i=0; i<p; i++)  {

	      //		m_ctc1.set_ref(i,m_ctc[i]);

	//		m_fun[0][i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		linfun1[i][0][0]=x->at(i);
		linfun1[i][1][0]=y->at(i)-diry*x->at(i);
		linfun1[i][2][0]=z->at(i)-dirz*x->at(i);
	      }
	    Q=40;

	    CtcQInterPlane ctcq(n,m_ctc,linfun1,boxes,epseq,Q,flist,QINTERPROJ,K);
	    cout << " apres contracteur " << endl;
	    for (int i=0 ; i<np; i++)
	      {int count =0;
		if (!(boxes[i]->is_empty()))
		  {//cout << i << " " << *(boxes[i])  << endl;
		    countc=ctcq.points_count(*(boxes[i]));
		    if (count >= Q) 
		      cout << i << " " << *(boxes[i])  << " " << count << endl;
		  }
	      }
	    cout << " apres comptage " << endl;
	    K=3;
	    */

	    CtcFixPoint ctcf(*ctcq,0.1);
	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=ctcq;
	    else ctcs=&ctcf;
	    		       
	    
	    SolverQInter s(*ctcs,bs,buff,*ctcq);

	    //Solver s(ctcqf0,bs,buff);
	    s.time_limit = 1000;
	    s.trace=0;
	    Timer::stop();
            
	    cout << " temps avant resolution " << Timer::VIRTUAL_TIMELAPSE() <<  endl;

	    vector<IntervalVector> res=s.solve(box);


	    cout << "Number of branches : " << s.nb_cells << endl;
	    cout << "Number of solutions : " << res.size() << endl;
	    nb_cells +=s.nb_cells;
	    cputime += s.time;


	// cout << " nb sols " << res.size() << endl;   
	//	  cout << " nb active sols " << s.maximal_sol.size() << endl;   
	    
	    
	    s.report_maximal_solutions(res);

	    vector <int> maxsolinliers;
	    s.keep_one_solution_pergroup(res, maxsolinliers);
	    

	    /*
	    set<int> trueinliers;
	    for (int i=0; i< maxsolinliers.size(); i++)
	      {  trueinliers.insert(i%p);
		trueinliers.insert(index2[i]);
		trueinliers.insert(index3[i]);
	      }

	    set<int>::iterator iter = trueinliers.begin();
	    cout << " nb inliers " << trueinliers.size() << endl;
	    
	    while( iter !=trueinliers.end()){
	      cout << *iter << " " ; 
	      iter++;
	    }
	    
	    cout << endl;
	    */


	  /*
	if (res.size() >1)
	  {	resgroup.push_back(res[0]);

	for (int i=0; i<res.size(); i++)
	  {if (max_dist (res[i],resgroup.back(),  prec))
	      resgroup[resgroup.size()-1]= res[i]|resgroup.back();
	    else
	      resgroup.push_back(res[i]);
	      }
	cout << "regroupement  solutions " << endl;
	for (int i=0; i<resgroup.size(); i++)
	  {	  cout << resgroup[i] << " "  << endl;
	    int k=0;
	    for (int j=0; j<p; j++)
	      { Interval evalctc= m_func[j]->eval(resgroup[i]);
		if (evalctc.contains(0)) {k++; 
		  //		  cout << j << (*x)[j] << " " << (*y)[j] <<  " " << (*z)[j] <<  " " <<
		  //		    m_func[j]->eval(resgroup[i]) << endl;
		}
	      }

	    cout << "nb points " << k <<endl;
	  }


	vector<IntervalVector> resgroup2;

	if (resgroup.size() >1)
	  {	cout << "regroupement  solutions 2" << endl;
	    resgroup2.push_back(resgroup[0]);
	    for (int i=0; i<resgroup.size(); i++)
	  {if (max_dist (resgroup[i],resgroup2.back(), prec))
	      resgroup2[resgroup2.size()-1]= resgroup[i]|resgroup2.back();
	    else
	      resgroup2.push_back(resgroup[i]);
	  }


	for (int i=0; i<resgroup2.size(); i++)
	  {	  cout << resgroup2[i] << " "  << endl;
	    int k=0;
	    for (int j=0; j<p; j++)
	      { Interval evalctc= m_func[j]->eval(resgroup2[i]);
		if (evalctc.contains(0)) {k++; 
		  //		  cout << j << (*x)[j] << " " << (*y)[j] <<  " " << (*z)[j] <<  " " <<
		  //		    m_func[j]->eval(resgroup[i]) << endl;
		}
	      }

	    cout << "nb points " << k <<endl;
	    //  Q0=k+1;
	  }
	  
	  }
	  }
	  */
	  

		
	
	    Timer::start();
	for (int i=0; i<p; i++)
	  { delete m_func[i];
	       delete &m_ctc[i];

	  }
	/*
	if (K==2)
	  for (int i=0; i<np; i++)
	       delete &m_ctc1[i];
	*/
	m_ctc.clear();
	m_ctc1.clear();

	delete ctcq;
	Timer::stop();
	cout << " temps fin quadrant " << Timer::VIRTUAL_TIMELAPSE() <<  endl;
	 

	end = clock();
	totaltime += ((double)(end)-(double)(start))/CLOCKS_PER_SEC;
	start= clock();

	}
	  				       
	end=clock();
	cout << "Shape extraction : OK. Time : " << ((double)(end)-(double)(start0))/CLOCKS_PER_SEC << " seconds" << endl;
	cout << " total time " << totaltime << endl;
	cout << " cpu time " << cputime << endl;
	cout <<" total branch number " << nb_cells << endl;

	for (int i=0; i<p; i++)
	  {for (int j=0; j<n; j++)
	      delete [] linfun[i][j];
	    
	    delete [] linfun[i];
	  }
	delete [] linfun;
	for (int i=0; i<np; i++)
	  {for (int j=0; j<2; j++)
	    delete [] index[j];
	  }
	delete [] index;


	
	delete [] m_func;
	
	//	delete[] index2;
	//	delete[] index3;

	
};






