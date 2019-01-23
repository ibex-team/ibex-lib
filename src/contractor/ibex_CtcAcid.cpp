//============================================================================
//                                  I B E X                                   
// File        : Adaptive CID (ACID) contractor
// Author      : Bertrand Neveu , Gilles Trombettoni
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 20, 2018
//============================================================================

#include "ibex_CtcAcid.h"
#include <algorithm>

using namespace std;

namespace ibex {

double CtcAcid::nbvarstat = 0;

CtcAcid::CtcAcid(const System& sys, const BitSet& cid_vars, Ctc& ctc, bool optim, int s3b, int scid,
		double var_min_width, double ct_ratio): Ctc3BCid (cid_vars,ctc,s3b,scid,cid_vars.size(),var_min_width),
				system(sys), nbcalls(0), nbctvar(0), ctratio(ct_ratio),  nbcidvar(0), nbtuning(0), optim(optim)  {
	// [gch] BNE check the argument "cid_vars.nb_set()" given to _3BCID
}

CtcAcid::CtcAcid(const System& sys, Ctc& ctc, bool optim, int s3b, int scid,
		double var_min_width, double ct_ratio): Ctc3BCid (BitSet::all(sys.nb_var),ctc,s3b,scid,sys.nb_var,var_min_width),
				system(sys), nbcalls(0), nbctvar(0), ctratio(ct_ratio), nbcidvar(0) ,  nbtuning(0), optim(optim) {
}

void CtcAcid::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcAcid::contract(IntervalVector& box, ContractContext& context) {
	// the initial contraction allows to set, after, the impact
	// to only one variable (in the slicing process). However, it turns
	// to be more efficient in practice to skip this initial contraction.
	// This is probably a side effect of the strategy chosen in DefaultOptimizer
	// (the call to CtcAcid(HC4) is preceded by a call to HC4 with higher precision).

	// note: we can transmit here the input impact to the contractor
	// as it is strictly less contracting that ACID (TODO: check that)
	//ctc.contract(box, context);
	//if (box.is_empty()) return;

	// --------------------- context ------------------
	context.impact.clear(); // should we restore the initial impact on return?
	this->context = &context;
	// ------------------------------------------------

	int nb_CID_var=cid_vars.size();                    // [gch]
	int nbvarmax=5*nb_CID_var;                         //  au plus 5*nbvar
	double *ctstat = new double[nbvarmax];

	int nbinitcalls=50;                                // longueur du réglage
	int factor = 20;                                   // détermine la période entre les débuts de 2 régalages  successifs : factor*nbinitcalls
	int nbcall1= nbcalls% (factor*nbinitcalls);        // pour savoir si on est dans une phase de réglage (nbcall1 < nbinitcalls )

	IntervalVector initbox (box);

	if (nbcall1 < nbinitcalls) {                       // réglage

		if (nbcalls < nbinitcalls) vhandled =nb_CID_var;     // premier réglage 3BCID une fois sur toutes les variables
		else vhandled = 2* nbcidvar;                       //  réglages suivants : sur 2 fois le réglage précédent

		if (vhandled< 2) vhandled= 2;                  // réglage minimum  à 2

		for (int i =0; i< nbvarmax; i++)               // initialisation du tableau des gains:
			ctstat[i]=0;                               // ctstat[i] gain moyen dû à l'appel de varcid sur la variable i
	}
	else
		vhandled = nbcidvar;                           // en dehors du réglage , on prend nbcidvar   ( la valeur réglée)

	if (vhandled > nbvarmax) vhandled=nbvarmax;        // pour rester raisonnable et dans les limites du tableau ctstat

	if (vhandled > 0) compute_smearorder(box);         // l'ordre sur les variables est calculé avec la smearsumrel
	if (optim) putobjfirst();                         // pour l'optim (si optim mis à true dans le constructeur, la dernière variable (objectf) est mise en premier
	for (int v=0; v<vhandled; v++) {
		int v1=v%nb_CID_var;                               // [gch] how can v be < nb_var?? [bne]  vhandled can be between 0 and nbvarmax
		int v2=smearorder[v1];

		// [gch]: impact handling:

		var3BCID(box, v2);                             // appel 3BCID sur la variable v2

		if (box.is_empty()) {
			context.output_flags.add(FIXPOINT);
			this->context = NULL;
			delete[] ctstat;
			return;
		}

		if (nbcall1 < nbinitcalls) {                   // on fait des stats pour le réglage courant
			for (int i=0; i<initbox.size(); i++) {
				//cout << i << " initbox " << initbox[i].diam() << " box " << box[i].diam() << endl;
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

	delete [] ctstat;

	this->context = NULL;
	context.prop.update(BoxEvent(box,BoxEvent::CONTRACT));
}

// en optim, l'objectif est placé en 1er
void CtcAcid::putobjfirst() {
	vector <int>::iterator result = find(smearorder.begin(), smearorder.end(), nb_var-1);
	smearorder.erase(result);
	smearorder.insert(smearorder.begin(),nb_var-1);
}


void CtcAcid::compute_smearorder(IntervalVector& box) {

	/*
	 * [gch] General comment for BNE:
	 * In the computations, I've always skipped the variables
	 * that are not to be varCIDed. As an important difference
	 * smearorder does not necessarily contain all the
	 * numbers from 0 to n-1.
	 */

	int nb_ctr=system.f_ctrs.image_dim(); //system.ctrs.size();

	if (smearorder.size()!=0) smearorder.clear();

	std::vector<int> varorder2;

	for (int i=0;i<nb_var;i++)
		if (cid_vars[i]) varorder2.push_back(i);       // [gch] (the condition)

	IntervalMatrix J(nb_ctr, nb_var);

	system.f_ctrs.jacobian(box,J);


	double* sum_smear=new double[nb_var];
	double* ctrjsum= new double[nb_ctr];

	for (int i=0; i<nb_ctr; i++) {
		ctrjsum[i]=0;
		for (int j=0; j<nb_var ; j++) {
			// [bne]  in case of infinite derivatives , natural ordering
			if (J[i][j].mag()==POS_INFINITY || box[j].diam()==POS_INFINITY) {
				for (int i1=0;i1 < nb_var; i1++)
				smearorder.push_back(varorder2[i1]);
				delete [] sum_smear;
				delete [] ctrjsum;
				return;
			}
			if (cid_vars[j])                           // [gch] (only varCIDed variables considered?)
				ctrjsum[i]+= J[i][j].mag() * box[j].diam();
		}
	}

	// calcul du tableau sum_smear   (la valeur de smearsumrel par variable)
	for (int i=0; i<nb_var; i++) {
		if (!cid_vars[i]) continue;                    // [gch]
		sum_smear[i]=0;
		for (int j=0; j<nb_ctr; j++) {
			if (ctrjsum[j]>1.e-5) {
				sum_smear[i]+= J[j][i].mag() * box[i].diam()/ctrjsum[j] ; // formule smearsumrel

				// sum_smear[i]+= abs(J[j][i]) * box[i].diam();          // variante smearsum
			}
		}
	}
	// tri des variables selon sum_smear :  resultat dans tableau smearorder
	for ( int  i=0; i<nb_var; i++) {
		if (!cid_vars[i]) continue;                    // [gch]
		int k=0;
		//int k0=0;
		double sz=0;
		for (unsigned int k1=0; (k1<(varorder2.size())); k1++) {
			if (sum_smear[varorder2[k1]]>sz) {
				k=k1;
				//k0=1;
				sz=sum_smear[varorder2[k1]];
			}
		}
		smearorder.push_back(varorder2[k]);
		vector <int>::iterator result = find(varorder2.begin(), varorder2.end(), varorder2[k]);
		varorder2.erase(result);
	}
	varorder2.clear();

	delete [] sum_smear;
	delete [] ctrjsum;

	//	assert(smearorder.size()==vhandled);       // [gch]   [bne] assert suppression : vhandled can be > nb_variables
}


double CtcAcid::nbvar_stat() {
	return nbvarstat;
}

} // end namespace ibex
