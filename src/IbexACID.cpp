/*---------------------------------------------------------------------------------
 * 3BCID contractor
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2007 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/


#include "Ibex3BCID.h"
#include "IbexACID.h"


namespace ibex {

 REAL  Adapt3BCID::nbvarstat=0;
  const REAL Adapt3BCID::default_ctratio=0.005;



  // cid adaptatif :  calcul du nombre de variables nbcidvar sur lesquelles on applique cid 
  // au début   pour initialiser on essaye pendant nbinitcalls    nbcidvar = nbvar 
  // cette 1re phase de réglage permet de calculer nbcidvar pour la phase suivante
  // ensuite, on refait un réglage qui recalcule le nb de variables tous les factor * nbinitcalls  
  //en faisant des stats pendant  nbinitcalls  nouuds.
  //  pendant cette periode de réglage, on essaye avec  nbcidvar =  min (max (2, 2 * nbcidvar)  , 5 * nbvar) 


  //  Pour les stats, on determine apres combien de variables, on gagne moins en moyenne (sur les dimensions)que ctratio par domaine .
  //  ce nb de variables moyen (sur les noeuds de la phase de réglage) deviendra alors nbcidvar


void Adapt3BCID::contract() {
     int nb_var=Dimension(cid_space.box);
     int nbvarmax=5*nb_var;  //  au plus 5*nbvar
     REAL ctstat[nbvarmax];

     int nbinitcalls=50;  // longueur du réglage
     int factor = 20;    // détermine la période entre les débuts de 2 régalages  successifs : factor*nbinitcalls
     int nbcall1= nbcalls% (factor*nbinitcalls); // pour savoir si on est dans une phase de réglage (nbcall1 < nbinitcalls )
     // 1er reglage sur nbvar (on divise par 2 pour réobtenir nbvar 2 lignes plus loin dans la 1re phase de réglage
     if (nbcalls==0) nbcidvar= (nb_var+1)/2; 
     
     // if (nbcalls==0) nbcidvar=nb_var;  //  variante 1er reglage sur 2*nbvar

     INTERVAL_VECTOR initbox (space.box);
     
     if (nbcall1 < nbinitcalls) // réglage
       { vhandled = 2* nbcidvar;
	 if (vhandled< 2) vhandled= 2;   // réglage minimum  à 2
	 // initialisation du tableau des gains : ctstat[i] gain moyen dû à l'appel de varcid sur la variable i
         for (int i =0; i< nbvarmax; i++) 
	   ctstat[i]=0;
       }
     else vhandled = nbcidvar;   // en dehors du réglage , on prend nbcidvar   ( la valeur réglée)

     if (vhandled > nbvarmax) vhandled=nbvarmax; // pour rester raisonnable et dans les limites du tableau ctstat


     if (vhandled > 0) compute_smearorder();   // l'ordre sur les variables est calculé avec la smearsumrel

     for (int v=0; v<vhandled; v++)
       {
	 int v1=v%nb_var;

	 int v2=smearorder[v1]; 

	 int var_env=cid_space.key(IBEX_VAR, v2);
	 int var_space=space.component(var_env);
	 var3BCID(var_space);  // appel 3BCID sur la variable v2
	 if(space.box.empty())
	   throw EmptyBoxException();
	 if (nbcall1 < nbinitcalls)    // on fait des stats pour le réglage courant
	   { 
             for (int i=0; i< Dimension(initbox); i++)
	       if  (Diam (initbox(i+1)) !=0)
		 // gain sur la ième dimension de la boîte courante après var3BCID sur la v-ième variable
		 ctstat[v] += 1  - Diam(space.box(i+1)) / Diam (initbox(i+1));  
	     ctstat[v]= ctstat[v]/ Dimension(initbox); // gain moyen 
	   }
	
	 initbox=space.box;	 
       }

     int nvar=0;

     nbcalls++; nbcall1++;
     // pendant la phase de réglage
     if (nbcall1 <= nbinitcalls) {  
       // : on part de la dernière variable.
       // on s'arrête dès qu'on a trouvé une variable ayant gagné plus que ctratio, ce qui nous donne le nbre nvar de variables itéressantes.
       for (int v=vhandled-1 ; v>=0; v--)
	 if (ctstat[v] > ctratio)
	   {nvar=v+1;
	     break;}
        // calcul incrémental de la moyenne du nbre de variables intéressantes
       nbctvar= (nbctvar * (nbcall1 - 1) + nvar ) / nbcall1;

     }
 
     if (nbcall1== nbinitcalls) // fin de la phase de réglage  - détermination de nbcidvar - calcul incrémental de la moyenne
       {nbcidvar= (int) (nbctvar + 0.5);
	 nbtuning++;
	 nbvarstat = (nbvarstat * (nbtuning-1) + nbcidvar) / nbtuning;
       }

}


  // calcul de l'ordre dans lequel les variables seront traitées.
  // variante smear d'Ignacio Araya   (cf smearsumrel dans IbexBisector.cpp)
  void Adapt3BCID::compute_smearorder()
  {
    if (smearorder.size()!=0) smearorder.clear();
    vector<int> varorder2; 
 
  for (int i=0;i<space.nb_var();i++)
   {  varorder2.push_back(i);}

  INTERVAL_MATRIX J(system.nb_ctr(), system.nb_var());
  try{
  system.jacobian(J);
  }


  catch(EvalException)
    {for (int i=0;i < space.nb_var(); i++) smearorder.push_back(varorder2[i]); return;}

  REAL sum_smear[space.nb_var()];


  REAL ctrjsum[system.nb_ctr()];
  for (int i=0; i<system.nb_ctr(); i++) 
    {ctrjsum[i]=0;
      for (int j=0; j<space.nb_var() ; j++)
	{ctrjsum[i]+= Abs(J(i+1,j+1)) * Diam(system.box(j+1));
	}
    }


  // calcul du tableau sum_smear   (la valeur de smearsumrel par variable)
  for ( int  i=0; i< space.nb_var() ; i++)
   { 

     sum_smear[i]=0;
     for (int j=0; j<system.nb_ctr(); j++) 
       { if 
	 (ctrjsum[j]> 1.e-5)
	   sum_smear[i]+= Abs(J(j+1,i+1)) * Diam(system.box(i+1))/ctrjsum[j] ; // formule smearsumrel 
	 //   sum_smear[i]+= Abs(J(j+1,i+1)) * Diam(system.box(i+1)) ;   // variante smearsum
       }
   }
  // tri des variables selon sum_smear :  resultat dans tableau smearorder
  for ( int  i=0; i< space.nb_var() ; i++)
    {
     int k=0;
     int k0=0;
     double sz=0;
     for (int k1=0;k1<varorder2.size();k1++)
       {
	 if ( sum_smear[varorder2[k1]] > sz)
	   {k=k1; k0=1; sz= sum_smear[varorder2[k1]];}
       }
     smearorder.push_back(varorder2[k]);
     vector <int>::iterator result = find(varorder2.begin(), varorder2.end(), varorder2[k]);      
     varorder2.erase(result);
    }
  varorder2.clear();
  }

  REAL Adapt3BCID::nbvar_stat()
  {return nbvarstat;}

}
