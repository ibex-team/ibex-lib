#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Detects plan in noisy pictures using q-intersection. 
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
	vector<double> *z = new vector<double>;

	double cputime =0;
	double totaltime=0;
	int pb = atoi(argv[1]);
	int NP = atoi(argv[2]);
	int Q = atoi(argv[3]);
        double epseq= atof (argv[4]);
	int flist=atoi(argv[5]);
	int fixpoint = atoi(argv[6]);
	int gaplimit= atoi(argv[7]);
	int nbrand= atoi(argv[8]);
	string bisect = argv[9];
	int sr= atoi(argv[10]);
	srand(sr);
	int Qi;
	switch(pb)
	  {case 1 : {Qi=0.1001*NP;break;}
	  //	  {case 1 : {Qi=0.099*NP;break;}
	  case 2 : {Qi=0.0501*NP;break;}
	  case 3 : {Qi=0.0401*NP;break;}
	    //	  case 4 : {Qi=0.0201*NP;break;}
	  case 4 : {Qi=0.0401*NP;break;}
	  case 5 : {Qi=0.0151*NP;break;}
	  case 6 : {Qi=0.0101*NP;break;}
	  }
for (int i=0;i<NP;i++)
  z->push_back(rand()%256 + (double)rand()/(double)RAND_MAX);
        if (pb <=3)
	  { int kk;
	    if (pb==1) kk=(150 * NP)/1000;
	    else if (pb==2) kk=(50 * NP) /1000;
	    else if (pb==3) kk=(40 * NP)/ 1000;
	    cout << " kk " << kk << endl;
	    for (int i=0;i<kk;i++)
	      {	
		y->push_back(rand()%256 +  (double)rand()/(double)RAND_MAX );
		y->push_back(rand()%256+ (double)rand()/(double)RAND_MAX );
		y->push_back(rand()%256+ (double)rand()/(double)RAND_MAX);
		y->push_back(22 + (((double)rand()/(double)RAND_MAX)*0.001));
		y->push_back(12 + (((double)rand()/(double)RAND_MAX)*0.001));
	      }
	    if (pb==1) kk=(250 * NP)/1000;
	    else if (pb==2) kk=(750 * NP)/1000;
	    else if (pb==3) kk=(800 * NP)/1000;
	    cout << " kk " << kk << endl;
	    for (int i=0;i<kk;i++)
	      y->push_back(rand()%256+ (double)rand()/(double)RAND_MAX );
	
	        int kk1 = NP/10;
		//int kk1=(40 * NP)/ 1000;
	    for (int i=0;i<kk1;i++)
	      {x->push_back(10+ (((double)rand()/(double)RAND_MAX)*0.001));
		if (pb==1)
		  x->push_back(10 + (((double)rand()/(double)RAND_MAX)*0.001));
		else
		  x->push_back(rand()%256 + (double)rand()/(double)RAND_MAX);
		x->push_back(100 + (((double)rand()/(double)RAND_MAX)*0.001));
		x->push_back(rand()%256 +  (double)rand()/(double)RAND_MAX);
		x->push_back(rand()%256 + (double)rand()/(double)RAND_MAX);
		
		x->push_back(rand()%256 + (double)rand()/(double)RAND_MAX);
		x->push_back(rand()%256  + (double)rand()/(double)RAND_MAX);
		x->push_back(rand()%256 +  (double)rand()/(double)RAND_MAX);
		x->push_back(rand()%256+  (double)rand()/(double)RAND_MAX);
		x->push_back(rand()%256+ (double)rand()/(double)RAND_MAX );
		
	      }
	    /*
	    if (pb==1) kk=(250 * NP)/1000;
	    else if (pb==2) kk=(750 * NP)/1000;
	    else if (pb==3) kk=(800 * NP)/1000;
	    cout << " kk " << kk << endl;
	    for (int i=0;i<kk;i++)

	      x->push_back(rand()%256+ (double)rand()/(double)RAND_MAX );
	    */

	    for (int i=0;i<NP-10*kk1;i++)
	      x->push_back(rand()%256 + (double)rand()/(double)RAND_MAX );
	    
	  }
	
	 
	else{

	  for (int i=0;i<NP;i++)
	    y->push_back(rand()%256 +(double)rand()/(double)RAND_MAX);
          if (pb==5){

	    for (int i=0;i<Qi;i++)  {

	    x->push_back(300- y->at(65*i) - 0.8*z-> at(65*i)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(- y->at(65*i+1)  + 0.9*z-> at(65*i+1)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-200+ y->at(65*i+2)  + 0.7*z-> at(65*i+2)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(1+ y->at(65*i+3)  - z-> at(65*i+3)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    
	    x->push_back(320- y->at(65*i+4) - 0.8* z-> at(65*i+4)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(10 - y->at(65*i+5)  +0.85* z-> at(65*i+5)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(5- y->at(65*i+6)  + 0.75*z-> at(65*i+6)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(15- y->at(65*i+7)  +0.65* z-> at(65*i+7)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(330- y->at(65*i+8) - 0.55* z-> at(65*i+8)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(20- y->at(65*i+9)  + 0.45*z-> at(65*i+9)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-210+ y->at(65*i+10)  + 0.35* z-> at(65*i+10)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(25 -y->at(65*i+11)  - 0.88*z-> at(65*i+11)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(340- y->at(65*i+12) - 0.78*z-> at(65*i+12)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(30 - y->at(65*i+13)  + 0.68*z-> at(65*i+13)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(35- y->at(65*i+14)  + 0.58*z-> at(65*i+14)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(40- y->at(65*i+15)  + 0.48*z-> at(65*i+15)+ (((double)rand()/(double)RAND_MAX)*0.001));

	     x->push_back(350- y->at(65*i+16) - 0.38*z-> at(65*i+16)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(3- y->at(65*i+17)  + 0.28*z-> at(65*i+17)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-220+ y->at(65*i+18)  + 0.92*z-> at(65*i+18)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( 12 + y->at(65*i+19)  - 0.82*z-> at(65*i+19)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(360- y->at(65*i+20) - 0.72*z-> at(65*i+20)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(12 - y->at(65*i+21)  + 0.62*z-> at(65*i+21)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(7- y->at(65*i+22)  + 0.52*z-> at(65*i+22)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(17- y->at(65*i+23)  + 0.42*z-> at(65*i+23)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(370- y->at(65*i+24) - 0.32*z-> at(65*i+24)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    for (int j=0; j<40; j++)  x->push_back(rand()%256 +((double)rand()/(double)RAND_MAX));
	    }
	    int K0=NP-Qi*65; 
	    for (int i=0; i< K0;i++) x->push_back(rand()%256 + ((double)rand()/(double)RAND_MAX));
	  }
	  else

	for (int i=0;i< Qi;i++)
	  {
	    int nn= 25;
	    x->push_back(300- y->at(nn*i) - 0.9*z-> at(nn*i)+ (((double)rand()/(double)RAND_MAX)*0.0001));
	    x->push_back(- y->at(nn*i+1)  + 0.8*z-> at(nn*i+1)+ (((double)rand()/(double)RAND_MAX)*0.0001));	
	    x->push_back(-200+ y->at(nn*i+2)  + 0.7*z-> at(nn*i+2)+ (((double)rand()/(double)RAND_MAX)*0.0001));
	    x->push_back( y->at(nn*i+3)  - z-> at(nn*i+3)+ (((double)rand()/(double)RAND_MAX)*0.0001));	

	    x->push_back(320- y->at(nn*i+4) - 0.5*z-> at(nn*i+4)+ (((double)rand()/(double)RAND_MAX)*0.0001));
	    x->push_back(10 - y->at(nn*i+5)  + 0.6*z-> at(nn*i+5)+ (((double)rand()/(double)RAND_MAX)*0.0001));
	    x->push_back(5- y->at(nn*i+6)  + 0.4*z-> at(nn*i+6)+ (((double)rand()/(double)RAND_MAX)*0.0001));
	    x->push_back(15- y->at(nn*i+7)  + 0.3*z-> at(nn*i+7)+ (((double)rand()/(double)RAND_MAX)*0.0001));

	    x->push_back(330- y->at(nn*i+8) - 0.55* z-> at(nn*i+8)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(20- y->at(nn*i+9)  + 0.45*z-> at(nn*i+9)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-210+ y->at(nn*i+10)  + 0.35* z-> at(nn*i+10)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(25 -y->at(nn*i+11)  - 0.88*z-> at(nn*i+11)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(340- y->at(nn*i+12) - 0.78*z-> at(nn*i+12)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(30 - y->at(nn*i+13)  + 0.68*z-> at(nn*i+13)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(35- y->at(nn*i+14)  + 0.58*z-> at(nn*i+14)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(40- y->at(nn*i+15)  + 0.48*z-> at(nn*i+15)+ (((double)rand()/(double)RAND_MAX)*0.001));

	     x->push_back(350- y->at(nn*i+16) - 0.38*z-> at(nn*i+16)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(3- y->at(nn*i+17)  + 0.28*z-> at(nn*i+17)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(-220+ y->at(nn*i+18)  + 0.92*z-> at(nn*i+18)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back( 12 + y->at(nn*i+19)  - 0.82*z-> at(nn*i+19)+ (((double)rand()/(double)RAND_MAX)*0.001));	
	    x->push_back(360- y->at(nn*i+20) - 0.72*z-> at(nn*i+20)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(12 - y->at(nn*i+21)  + 0.62*z-> at(nn*i+21)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(7- y->at(nn*i+22)  + 0.52*z-> at(nn*i+22)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(17- y->at(nn*i+23)  + 0.42*z-> at(nn*i+23)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    x->push_back(370- y->at(nn*i+24) - 0.32*z-> at(nn*i+24)+ (((double)rand()/(double)RAND_MAX)*0.001));
	    
	    
	    int K1=0;

		    //int K1=17;
	    //	    K1=0;
	    for (int j=0; j< K1;j++) x->push_back(rand()%256);
	  }
	  cout << NP << "  " << Qi << "  " << 65*Qi << endl;
	  if (pb==4) for (int i=0; i< NP-Qi*65;i++) x->push_back(rand()%256 +((double)rand()/(double)RAND_MAX));
	  if (pb==6) for (int j=0; j< NP/2;j++) x->push_back(rand()%256 + ((double)rand()/(double)RAND_MAX));
	}

	


	cout << " nb points " << x->size() << endl;
	//	for (int i=0; i< x->size(); i++) cout << (*x)[i] <<  " " << (*y)[i] << " " << (*z)[i] <<endl;
	
	int n=3;

	int p = NP;

	int K=3;
	int np;


	np= (p*(p-1)*(p-2))/6;
	cout << np << "  " << Q << endl;
	/*
	
	Function ***m_fun;
        m_fun=new Function **[K];
	for (int i=0; i<K; i++)
	  m_fun[i]=new Function*[np];
	
	*/
	double ** linfun;
        linfun = new double*[p];
	for (int i=0; i<p; i++)
	  {  linfun[i] = new double[n];
	  }
	
	int ** index;
	index = new int*[np];
	for (int i=0; i<np; i++)
	  index[i]=new int [3];

	
	Function* m_f0;
	Ctc* ctc0;
	Function**  m_func= new Function* [p] ;	
	IntervalMatrix* boxes = new IntervalMatrix (np,n);	 
	Variable v(3);

	clock_t start, start0, end;
	int nb_cells=0;
	start = clock();
	start0= clock();
	int Q0=Q;
	

	for (int oct=0; oct <4; oct++)

	  { 
	    Timer::start();
	    int pmax=0;
	    Array<Ctc> m_ctc(p);
	    //	    Array<Ctc> m_ctc1(np);
	    Array<Ctc> m_ctc1(1);


	    
	    int diry= pow(-1,oct%2);
	    int dirz= pow(-1,oct/2);
	    
	    double _box[3][2];

	    if (diry==1) {_box[0][0] = 0;
	      _box[0][1] = 1;}
	    else {_box[0][0] = -1;_box[0][1] = 0;}
	    if (dirz==1)
	      {	    _box[1][0] = 0;	    _box[1][1] = 1;}
	    else
	      {	    _box[1][0] = -1;	    _box[1][1] = 0;}
	    _box[2][0]=-500;
	    _box[2][1]=500;
	    IntervalVector box(3,_box);
	    
	    m_f0= new Function(v, (diry*v[0]+dirz*v[1]-1));

            ctc0=new CtcFwdBwd(*m_f0,LEQ);
	    for (int i=0; i<1; i++) {
	      //	    for (int i=0; i<p; i++) {
	      //only useful to build the contractor ;  not used in solving	 
	      m_func[i] = new Function(v,(x->at(i) +v[0]*(y->at(i)- diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-v[2]-Interval(-epseq,epseq)));
	      m_ctc.set_ref(i,(*new CtcFwdBwd(*m_func[i])));

	    }

	    m_ctc1.set_ref(0,m_ctc[0]);
	    for (int i=0; i<p; i++)  {
	    /* We must be on the plane defined by v */
	
	  //		m_fun[0][i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		linfun[i][0]=x->at(i);
		linfun[i][1]=y->at(i)-diry*x->at(i);
		linfun[i][2]=z->at(i)-dirz*x->at(i);
	      }

	    double a1,a2,a3,b1,b2,b3,c1;
	    Interval d1,d2,d3;	  
	    Interval x1,y1,z1;

	int pp=0;
	Interval eps(-epseq,epseq);

	for (int i=0; i<p; i++) {
	  //	  cout << " x " << x->at(i) << " y " << y->at(i) << " z " << z->at(i) << endl;
	  a1=linfun[i][1];
	  b1=linfun[i][2];
	  //	  a1=y->at(i)-diry*x->at(i);
	  //	  b1=z->at(i)-dirz*x->at(i);
	  d1=eps+x->at(i);
	  for (int j=i+1; j<p; j++){
	    a2=linfun[j][1];
	    b2=linfun[j][2];
	    //	    a2=y->at(j)-diry*x->at(j);
	    //	    b2=z->at(j)-dirz*x->at(j);
	    d2=eps+x->at(j);
	    for (int k=j+1; k<p; k++){
	      a3=linfun[k][1];
	      b3=linfun[k][2];
	      //	      a3=y->at(k)-diry*x->at(k);
	      //	      b3=z->at(k)-dirz*x->at(k);
	      //	      cout << a1 << " " << a2 << " " << a3 << " " << b1 << " " << b2 << " " << b3 << endl;
	      d3=eps+x->at(k);
	     
	      c1= (a1-a2) *(b1-b3) - (a1-a3)* (b1-b2);
	      x1=- ((d1-d2)*(b1-b3) - (d1-d3)*(b1-b2))/c1;
	      y1=- ( (d1-d3)*(a1-a2) -(d1-d2)*(a1-a3))/ c1;
	      IntervalVector box1(box);	      

	      /*	      
	      c1 = (a2-a1) *(b2-b3) - (a2-a3)* (b2-b1);
	      
	      x1&= ((d2-d1)*(b2-b3) - (d2-d3)*(b2-b1) )/c1;
	      y1&= ( (d2-d3)*(a2-a1) -(d2-d1)*(a2-a3)  )/ c1;


	      c1 = (a3-a2)* (b3-b1) - (a3-a1) * (b3-b2);
	      x1&= ((d3-d2)*(b3-b1) - (d3-d1) * (b3-b2))/c1;
	      y1&= ((d3-d1) * (a3-a2) - (d3-d2) * (a3-a1))/c1;
	      */
	      z1=d1 + a1*x1 + b1*y1;
	      //	      z1&=d2 - a2*x1 - b2*y1;
	      //	      z1&=d3 - a3*x1 - b3*y1;


		    //	           cout << x1 << " " << y1 << " " << "" <<  z1 << endl;

                    
	      //	      IntervalVector* box1= new IntervalVector(box);
	      //	      cout << "box avant inter " << x1 << " " << y1 << " " << -z1 <<  endl;
	      //	      cout << box << endl;
	      /*
	      (*box1)[0]&=-x1;
	      (*box1)[1]&=-y1;
	      (*box1)[2]&=z1;
	      */

	      /*
	      box1[0]&=-x1;
	      box1[1]&=-y1;
	      box1[2]&=z1;
	      */
	      x1&= box[0];
	      y1&= box[1];
	      z1&= box[2];

	      /*
	       (*boxes)[pp][0]= x1;
	       (*boxes)[pp][1]= y1;
	       (*boxes)[pp][2]= z1;
	      */
	       /*
	      if (x1.is_empty()) (*boxes)[pp].set_empty();
	      else if (y1.is_empty()) (*boxes)[pp].set_empty();
	      else if (z1.is_empty()) (*boxes)[pp].set_empty();
	      else if ((x1*diry+y1*dirz).lb()>1)(*boxes)[pp].set_empty();
	      else pmax++;
	       */

              if (!(x1.is_empty() || y1.is_empty() || z1.is_empty() || (x1*diry+y1*dirz).lb()>1))
		{

	      (*boxes)[pp][0]= x1;
	      (*boxes)[pp][1]= y1;
	      (*boxes)[pp][2]= z1;

	      index[pp][0]=i;
	      index[pp][1]=j;
	      index[pp][2]=k;
	      pmax++;
		}
	      else
		(*boxes)[pp].set_empty();

	      //box1[2]&=d1 - a1*x1 - b1*y1;
       
	      /*
	      (*box1)[0]=-x1;
	      (*box1)[1]=-y1;
	      (*box1)[2]=z1;
	      */

	      //	      cout << "box apres inter " << x1 << " " << y1 << " " << -z1 << endl;
	      /*
	      if ((*box1)[0].is_empty()) box1->set_empty();
	      else if ((*box1)[1].is_empty()) box1->set_empty();
	      else if ((*box1)[2].is_empty()) box1->set_empty();
	      else if (((*box1)[0]*diry+(*box1)[1]*dirz).lb()>1) box1->set_empty();   
	      */
	      /*
	      if (box1[0].is_empty()) box1.set_empty();
	      else if (box1[1].is_empty()) box1.set_empty();
	      else if (box1[2].is_empty()) box1.set_empty();
	      else if ((box1[0]*diry+box1[1]*dirz).lb()>1) box1.set_empty();   
	      */

	      //	      (*boxes)[pp]= *box1;
	    //	      (*boxes)[pp]= box1;
	      //if (!(box1->is_empty())) pmax++;
	      //	      if (!(box1.is_empty())) pmax++;
	      /*
	      index[pp][0]=i;
	      index[pp][1]=j;
	      index[pp][2]=k;
	      pp++;
	      */
	      //	      delete box1;

		
	      // m_ctc1.set_ref(p*nt+i,*new CtcCompo (m_ctc[i],m_ctc[j],m_ctc[k]));


 
		    //		    m_fun[0][p*nt+i]= new Function(v,(x->at(i) +v[0]*(y->at(i)-diry*x->at(i))+v[1]*(z->at(i)-dirz*x->at(i))-Interval(-epseq,epseq)));
		    //		    m_fun[1][p*nt+i]= new Function(v,(x->at(j) +v[0]*(y->at(j)-diry*x->at(j))+v[1]*(z->at(j)-dirz*x->at(j))-Interval(-epseq,epseq)));
		    //		    m_fun[2][p*nt+i]= new Function(v,(x->at(k) +v[0]*(y->at(k)-diry*x->at(k))+v[1]*(z->at(k)-dirz*x->at(k))-Interval(-epseq,epseq)));
	      pp++;
		  
	    }

	  }

	}
	 
	cout << " pp " << pp << " pmax " << pmax << endl;
	    vector<IntervalVector> resgroup;


	    Vector prec(3);
	    
	    
	    prec[0]=0.00001;
	    prec[1]=0.00001;
	    prec[2]=0.001;
	    
	    

	    // les precisions dans l'article soumis

	    
	    /*
	    prec[0]=0.000007;
	    prec[1]=0.000007;
	    prec[2]=0.0007;
	    */
	    


	    /*
	    prec[0]=0.000001;
	    prec[1]=0.000001;
	    prec[2]=0.0001;
	    */

	    CellStack buff;

	    Bsc * bs;
	    if (bisect=="rr")
	      bs = new RoundRobin(prec, 0.5);
	    else if (bisect =="rr2")
	      bs= new RoundRobinNvar(2,prec,0.5);
	    
	    


            CtcQInter* ctcq;	    
	    //	    CtcQInter ctcq(3,m_ctc1,Q);
	    //	    CtcQInterAff ctcq(3,m_ctc1,Q,m_fun,QINTERPROJ,K);
	    if (flist==1)
	      ctcq = new CtcQInterAffPlane (n,np,m_ctc1,linfun,index,boxes,epseq,Q,QINTERPROJ,K);
	    else
	      ctcq= new  CtcQInterPlane (n,np,m_ctc1,linfun,index,boxes,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERPROJ,K);
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERCORE,K);
	    //	    CtcQInterPlane  ctcq(n,m_ctc1,linfun,epseq,Q, QINTERCORE,K );
	    //	    CtcQInterPlane ctcq(n,m_ctc1,linfun,epseq,Q,QINTERFULL, K);

	    CtcCompo ctcqf0 (*ctc0,*ctcq);
	    Ctc3BCid cid(*ctcq,10,1,3);
	    CtcCompo ctcid(*ctcq ,cid);

	    //	    CtcCompo ctcqf0(*ctc0,ctcid);


	    //	    CtcFixPoint ctcf(ctcqf0,0.1);
	    CtcFixPoint ctcf(ctcid,0.1);

	    // Ctc3BCid cid(ctcqf0,5,1,3); 	    

	    Ctc* ctcs;
	    if (fixpoint==0) ctcs=&ctcqf0;
	    else ctcs=&ctcf;
	    		       
	    
	    SolverQInter s(*ctcs,*bs,buff,*ctcq);

	    //Solver s(ctcqf0,*bs,buff);
	    s.time_limit = 1000;
	    s.trace=0;
	    s.nbr=nbrand;
	    s.gaplimit=gaplimit;
	    cout << " avant resolution " << endl;

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
	      {  trueinliers.insert(index[i][0])
		trueinliers.insert(index[i][1]);
		trueinliers.insert(index[i][2]);
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
	  

		
	
	
	for (int i=0; i<p; i++)
	  { delete m_func[i];
	       delete &m_ctc[i];
	  }

	m_ctc.clear();
	m_ctc1.clear();
	delete ctcq;
	Timer::stop();

	cout << " fin octant " <<  Timer::VIRTUAL_TIMELAPSE() << endl;
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
	      delete [] linfun[i];
	delete [] linfun;

	for (int i=0; i<np; i++)
	    delete [] index[i];
	delete [] index;

	
	delete [] m_func;
	
}	  	






