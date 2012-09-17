//============================================================================
//                                  I B E X                                   
// File        : ibex_ACID.cpp
// Author      : Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 1, 2012
//============================================================================

#include "ibex_CtcAcid.h"

using namespace std;

namespace ibex {

double  CtcAcid::nbvarstat=0;
const double CtcAcid::default_ctratio=0.005;

CtcAcid::CtcAcid(const System& sys, const BoolMask& cid_vars, Ctc& ctc, int s3b, int scid,
		double var_min_width, double ct_ratio): Ctc3BCid (cid_vars,ctc,s3b,scid,cid_vars.nb_set(),var_min_width),
		system(sys), nbcalls(0), nbctvar(0), nbcidvar(0) ,  nbtuning(0), ctratio(ct_ratio) {
// [gch] BNE check the argument "cid_vars.nb_set()" given to _3BCID
}

void CtcAcid::contract(IntervalVector& box) {

	int nb_CID_var=cid_vars.nb_set();                  // [gch]
	impact.unset_all();                                // [gch]
	int nbvarmax=5*nb_CID_var;                         //  au plus 5*nbvar
	double ctstat[nbvarmax];

	int nbinitcalls=50;                                // longueur du réglage
	int factor = 20;                                   // détermine la période entre les débuts de 2 régalages  successifs : factor*nbinitcalls
	int nbcall1= nbcalls% (factor*nbinitcalls);        // pour savoir si on est dans une phase de réglage (nbcall1 < nbinitcalls )
	// 1er reglage sur nbvar (on divise par 2 pour réobtenir nbvar 2 lignes plus loin dans la 1re phase de réglage
	if (nbcalls==0) nbcidvar= (nb_CID_var+1)/2;

	// if (nbcalls==0) nbcidvar=nb_CID_var;                //  variante 1er reglage sur 2*nbvar

	IntervalVector initbox (box);

	if (nbcall1 < nbinitcalls) {                       // réglage
		vhandled = 2* nbcidvar;

		if (vhandled< 2) vhandled= 2;                  // réglage minimum  à 2

		for (int i =0; i< nbvarmax; i++)               // initialisation du tableau des gains:
			ctstat[i]=0;                               // ctstat[i] gain moyen dû à l'appel de varcid sur la variable i
	}
	else
		vhandled = nbcidvar;                           // en dehors du réglage , on prend nbcidvar   ( la valeur réglée)

	if (vhandled > nbvarmax) vhandled=nbvarmax;        // pour rester raisonnable et dans les limites du tableau ctstat

	if (vhandled > 0) compute_smearorder(box);         // l'ordre sur les variables est calculé avec la smearsumrel

   for (int v=0; v<vhandled; v++) {
    	int v1=v%nb_CID_var;                               // [gch] how can v be < nb_var?? [bne]  vhandled can be between 0 and nbvarmax
    	int v2=smearorder[v1];
	impact.set(v2);
    	var3BCID(box, v2);                             // appel 3BCID sur la variable v2
	impact.unset(v2); 
		if(box.is_empty())
			throw EmptyBoxException();
		if (nbcall1 < nbinitcalls) {                   // on fait des stats pour le réglage courant
			for (int i=0; i<initbox.size(); i++)
			  {//cout << i << " initbox " << initbox[i].diam() << " box " << box[i].diam() << endl;
			  if  (initbox[i].diam() !=0 && box[i].diam()!= POS_INFINITY)
			    // gain sur la ième dimension de la boîte courante après var3BCID sur la v-ième variable
			    ctstat[v] += 1  - box[i].diam() / initbox[i].diam();}
			ctstat[v]=ctstat[v]/ initbox.size();   // gain moyen
		}

		initbox=box;
	}

	int nvar=0;

	nbcalls++;
	nbcall1++;
	// pendant la phase de réglage
	if (nbcall1 <= nbinitcalls) {
		// : on part de la dernière variable.
		// on s'arrête dès qu'on a trouvé une variable ayant gagné plus que ctratio, ce qui nous donne le nbre nvar de variables itéressantes.
		for (int v=vhandled-1 ; v>=0; v--)
			if (ctstat[v] > ctratio) {
				nvar=v+1;
				break;
			}
		// calcul incrémental de la moyenne du nbre de variables intéressantes
		nbctvar= (nbctvar * (nbcall1 - 1) + nvar ) / nbcall1;
	}

	if (nbcall1==nbinitcalls) {                        // fin de la phase de réglage  - détermination de nbcidvar
		nbcidvar= (int) (nbctvar + 0.5);               // - calcul incrémental de la moyenne
		nbtuning++;
		nbvarstat = (nbvarstat * (nbtuning-1) + nbcidvar) / nbtuning;
	}
}

void CtcAcid::compute_smearorder(IntervalVector& box) {

	/*
	 * [gch] General comment for BNE:
	 * In the computations, I've always skipped the variables
	 * that are not to be varCIDed. As an important difference
	 * smearorder does not necessarily contain all the
	 * numbers from 0 to n-1.
	 */

	int nb_ctr=system.ctrs.size();

	if (smearorder.size()!=0) smearorder.clear();

	std::vector<int> varorder2;

	for (int i=0;i<nb_var;i++)
		if (cid_vars[i]) varorder2.push_back(i);       // [gch] (the condition)

	IntervalMatrix J(nb_ctr, nb_var);

	system.f.jacobian(box,J);


	double sum_smear[nb_var];
	double ctrjsum[nb_ctr];

	for (int i=0; i<nb_ctr; i++) {
		ctrjsum[i]=0;
		for (int j=0; j<nb_var ; j++) {
		  // [bne]  in case of infinite derivatives , natural ordering
		  if (J[i][j].mag()==POS_INFINITY || box[j].diam()==POS_INFINITY)
		    {for (int i1=0;i1 < nb_var; i1++)
		      smearorder.push_back(varorder2[i1]);
		      return;}
		  if (cid_vars[j])                           // [gch] (only varCIDed variables considered?)
		      ctrjsum[i]+= J[i][j].mag() * box[j].diam();
		}
	}

	// calcul du tableau sum_smear   (la valeur de smearsumrel par variable)
	for (int i=0; i<nb_var; i++) {
		if (!cid_vars[i]) continue;                    // [gch]
		sum_smear[i]=0;
		for (int j=0; j<nb_ctr; j++) {
			if (ctrjsum[j]>1.e-5)
			  {
				sum_smear[i]+= J[j][i].mag() * box[i].diam()/ctrjsum[j] ; // formule smearsumrel
                           
			    // sum_smear[i]+= abs(J[j][i]) * box[i].diam();          // variante smearsum
			  }
		}
	}
	// tri des variables selon sum_smear :  resultat dans tableau smearorder
	for ( int  i=0; i<nb_var; i++) {
		if (!cid_vars[i]) continue;                    // [gch]
		int k=0;
		int k0=0;
		double sz=0;
		for (int k1=0; k1<varorder2.size(); k1++) {
			if (sum_smear[varorder2[k1]]>sz) {
				k=k1;
				k0=1;
				sz=sum_smear[varorder2[k1]];
			}
		}
		smearorder.push_back(varorder2[k]);
		vector <int>::iterator result = find(varorder2.begin(), varorder2.end(), varorder2[k]);
		varorder2.erase(result);
	}
	varorder2.clear();

	assert(smearorder.size()==vhandled);               // [gch]
}

double CtcAcid::nbvar_stat() {
	return nbvarstat;
}

} // end namespace ibex
