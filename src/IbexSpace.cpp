/*---------------------------------------------------------------------------------
 * 
 * Space
 * -------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
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

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "IbexSpace.h"
#include "IbexEnv.h"

namespace ibex {

INTERVAL Entity::IGNORE(0,0);

int Space::default_flags_value = 12; // 1100

Space::Space(const Env& env) : env(env), box(0), upx(0), epx(0), sbx(0), 
			       flags(default_flags_value), ent_env2dom(new Entity[env.nb_keys()]), 
			       size(env.nb_keys()) {

  for (int type=0; type<4; type++) {
    nb_ent[type]=0;
    ent_dom2env[type] = NULL;
  } 
}

Space::Space(const SpaceFactory& builder) : env(builder.get_env()), box(0), upx(0), epx(0), sbx(0), 
					    flags(default_flags_value), ent_env2dom(new Entity[env.nb_keys()]), 
					    size(env.nb_keys()) {

  for (int type=0; type<4; type++) {
    nb_ent[type]=0;
    ent_dom2env[type] = NULL;
  } 
  builder.build_space(this);
}

Space::Space(const Space& s) : env(s.env), box(s.box), upx(s.upx), epx(s.epx), sbx(s.sbx), 
			       flags(s.flags), ent_env2dom(new Entity[s.env.nb_keys()]), 
			       size(s.env.nb_keys()), dom_num(s.dom_num) { 

  for (int t=0; t<4; t++) {
    ent_dom2env[t] = new int[s.nb((EntityType) t)];
  }
  copy_tables(s);
}

Space& Space::operator=(const Space& s) {

  check_context(*this,s);

  flags = s.flags;
  for (int t=0; t<4; t++) {
    EntityType type = (EntityType) t;
    if (nb(type)!=s.nb(type)) {
      delete[] ent_dom2env[type];
      ent_dom2env[type] = new int[s.nb(type)];
      Resize(vec_domain(type), s.nb(type));
    }
    vec_domain(type) = s.vec_domain(type);
  }
  
  dom_num = s.dom_num;
  copy_tables(s);
  return *this;
}

/* copy [but do not CREATE] tables (see constructors & assignment operator) */
void Space::copy_tables(const Space& s) {
  for (int type=0; type<4; type++) {
    nb_ent[type]=s.nb((EntityType) type);
    for (int i=0; i<s.nb((EntityType) type); i++) 
      ent_dom2env[type][i] = s.ent_dom2env[type][i];
  }

  for (int i=0; i<env.nb_keys(); i++) {
    Entity& e(s.ent_env2dom[i]);
    Entity& e2(ent_env2dom[i]);
    e2.key = e.key;
    e2.type    = e.type;
    e2.domain  = (INTERVAL*) (e.used()? &domain(e.type, s.component(e.key)) : &Entity::IGNORE);
  }
}

Space::~Space() {
  delete[] ent_env2dom;

  if (ent_dom2env[0]) {
    for (int type=0; type<4; type++) delete[] ent_dom2env[type];
  }
}

/*================================================================================*/


const Entity& Space::entity(const char* symbol) const {
  return entity(env.symbol_key(symbol));
}

void Space::add_entity(int key, INTERVAL& x, EntityType type) {
  int n=env.nb_keys();

  if (n>size) { 
    /*----------- environment has changed ==> rebuild table ----------------------*/
    Entity *tmp = new Entity[n];
    for (int i=0; i<size; i++) 
      tmp[i]=ent_env2dom[i]; // safe assignment (but pointers are copied)
    delete[] ent_env2dom;
    ent_env2dom = tmp;
    size=n;
    /*----------------------------------------------------------------------------*/
  }

  Entity& e(ent_env2dom[key]);
  e.key = key;
  e.domain = &x;
  e.type = type;
}

const char* Space::var_name(int var) const { 
  return env.symbol_name(ent_dom2env[IBEX_VAR][var]); 
}

const char* Space::epr_name(int epr) const { 
  return env.symbol_name(ent_dom2env[IBEX_EPR][epr]); 
}

const char* Space::upr_name(int upr) const { 
  return env.symbol_name(ent_dom2env[IBEX_UPR][upr]); 
}

const char* Space::syb_name(int syb) const { 
  return env.symbol_name(ent_dom2env[IBEX_SYB][syb]); 
}

void Space::sync(const Space& space, unsigned short flag, bool logic_op) {

  if (&space == this) return; 

  if (&env != &space.env) throw NonRecoverableException("Cannot synchronize spaces with different environments");  

  const Space *scanned; // the space we scan to detect the common entities
  const Space *other;   // the other space
  
  if (nb_ent[0]+nb_ent[1]+nb_ent[2]+nb_ent[3]<
      space.nb_ent[0]+space.nb_ent[1]+space.nb_ent[2]+space.nb_ent[3]) { 
    scanned=this; other=&space; 
  } else {  
    scanned=&space; other=this; 
  }
 
  for (int type=0; type<4; type++) {

    for (int i=0; i<scanned->nb_ent[type]; i++) {
      int key = scanned->ent_dom2env[type][i];
      if (other->used(key)) {
	const Entity& dest_ent(space.ent_env2dom[key]);
	const Entity& src_ent(ent_env2dom[key]);
	if (flag & (1 << (type*4+dest_ent.type))) {
	  if (logic_op) *src_ent.domain = *dest_ent.domain;
	  else *src_ent.domain |= *dest_ent.domain;
	}
      }
    }
  }
}


} // end namespace

static void output_numb(std::ostream& os, const REAL x, streamsize prec) {

  bool scientif=false;

  if (fabs(x)!=0 && fabs(x)<1) {
    REAL px=floor(log(fabs(x))/log(10.));
    if (-px>prec+3) scientif = true;
    else {
      REAL xshift=fabs(x)*pow(10.,-px);
      REAL diff=xshift-floor(xshift);     
       if (diff>0 && -floor(log(diff)/log(10.))<prec) scientif=true;
    }
  } else {
    scientif = (fabs(x) > pow(10.,prec+5));
  }

  if (scientif) {
    os.setf(ios::showpoint);
    os.setf(ios::internal);
    os.setf(ios::scientific);
  } else {
    os.precision(prec);
  }
  os.width(prec+6);
  os << x;
  if (scientif) {
    os.unsetf(ios::scientific);
    os.unsetf(ios::showpoint);
    os.unsetf(ios::internal);
  } else {
    os.precision(prec);
  }
}

static void output_intv(std::ostream& os, const char* name, const INTERVAL x, streamsize prec) {
  os.setf(ios::right);
  os.width(7);
  os << name;
  os.setf(ios::left);
  os << " [";
  output_numb(os, Inf(x), prec);
  os << ",";
  output_numb(os, Sup(x), prec);
  os << "] ";
  os.unsetf(ios::left);
}

namespace ibex {

static const char* keyword(EntityType type) {
  switch (type) {
  case IBEX_VAR : return "var"; 
  case IBEX_UPR : return "upr";
  case IBEX_EPR : return "epr"; 
  default : return "syb"; 	
 }
}

std::ostream& operator<<(std::ostream& os, const Space& space) {
  for (int t=0; t<4; t++) {
    if (space.flags.test(3-t)) {
      EntityType type = (EntityType) t;
      for (int i=0; i<space.nb(type); i++) {	
	output_intv(os, space.env.symbol_name(space.key(type,i)), space.domain(type,i), os.precision());
	if (i%4==3 || i==space.nb(type)-1) os << endl;
      }
    }
  }
  
  if (!space.flags.test(4)) return os;

  os << "[ENV->DOM ENT] \n";  
  for (int i=0; i<space.env.nb_keys(); i++) {
    if (space.used(i)) {
      const Entity& e(space.entity(i));
      os << "\t" << e.key << "  --> " << space.component(i) << " (dom)   ";
      os << keyword(e.type) << "   dom=" << *e.domain << "   deriv=";
      if (e.deriv!=&Entity::IGNORE) os << *e.deriv;
      else os << " (ignore) ";
      os << endl;
    }
  }
    
  os << "[DOM->ENV ENT] \n";  
  for (int t=0; t<4; t++) {
    EntityType type = (EntityType) t;
    for (int i=0; i<space.nb(type); i++) 
      os << "\t" << keyword(type) << " " << i << " \"" << space.env.symbol_name(space.key(type,i)) << "\" --> " << space.key(type,i) << " (env)" << endl;
  }
  os << endl;
  
  return os;
}

/*--------------------------------------------------------------------------------*/
/* ----------------                SPACE FACTORY                        ---------*/

SpaceFactory::SpaceFactory(const Env& env) : env(env), tmp_space(env), new_space(NULL) { }

SpaceFactory::~SpaceFactory() { }

void SpaceFactory::set_entity(const char* symbol, EntityType type) {
  set_entity(symbol, type, INTERVAL(0,0));
}

void SpaceFactory::set_entity(const char* symbol, EntityType type, const INTERVAL& domain) {
  pair <int,Dim> info = env.symbol_info(symbol);
  int num = info.first;
  for (int i=0; i<info.second.size(); i++) 
    set_entity(num++,type,domain);
}

void SpaceFactory::set_entity(int key, EntityType type, const INTERVAL& domain) {

  if (new_space) throw NonRecoverableException("Cannot modify a space factory after instanciation.");

  if (tmp_space.used(key)) {                // should not happen often: an entity is modified 
    const Entity& e(tmp_space.entity(key)); // during space construction (see however MultiSystem constructor)
    tmp_space.nb_ent[e.type]--;          
    delete e.domain;                    // delete temp assignment
  }

  // temp assignment --> see also build_space(Space*)
  tmp_space.add_entity(key, *new INTERVAL(domain), type); 
  tmp_space.nb_ent[type]++;
}

Space* SpaceFactory::build_space(Space* s) const {

  if (new_space) {
    // illegal to use this factory twice as Space constructor parameter.
    if (s) throw NonRecoverableException("Cannot build two different instances with the same space factory.");
    else return new_space;
  }

  Resize(tmp_space.box, tmp_space.nb_var());
  Resize(tmp_space.upx, tmp_space.nb_upr());
  Resize(tmp_space.epx, tmp_space.nb_epr());
  Resize(tmp_space.sbx, tmp_space.nb_syb());

  int enc[4];          // entity counters

  for (int type=0; type<4; type++) {
    tmp_space.ent_dom2env[type] = new int[tmp_space.nb_ent[type]];
    enc[type]=0;
  }

  for (int key=0; key<env.nb_keys(); key++) {
 
    if (tmp_space.used(key)) {                                        
      Entity& e= (Entity&) tmp_space.entity(key);

      INTERVAL& dom=tmp_space.domain(e.type, enc[e.type]); 
      dom = *e.domain;
      delete e.domain;                                   // delete temp assignment
      tmp_space.dom_num[key]=enc[e.type];
      e.domain = &dom;                 
      tmp_space.ent_dom2env[e.type][enc[e.type]] = e.key;
      enc[e.type]++;
    }
  }

  if (s==NULL) s = new Space(tmp_space); // copy constructor
  else *s = tmp_space; // assignment
  return new_space = s;
}

} // end namespace
