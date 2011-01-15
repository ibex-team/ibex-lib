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
 * without even the implied warranty of ME5RCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_SPACE_H
#define _IBEX_SPACE_H

#include <bitset>
#include "Interval.h"
#include "IntervalVector.h"
#include "IntervalMatrix.h"
#include "IbexIdTable.h"
#include "IbexException.h"
#include "IbexSharedObject.h"

namespace ibex {

class Env;
class Operator;

/*@ \file */

/** \enum EntityType */
typedef enum {IBEX_NONE=-1, IBEX_VAR=0, IBEX_UPR=1, IBEX_EPR=2, IBEX_SYB=3} EntityType;

class Space;

/** \ingroup maingroup
 *
 * \brief Entity
 *
 * An entity is a symbol x with an associated domain and type. The way x takes values within the domain 
 * can have different semantics, and this is what the type indicates. Then, x can either be a variable, 
 * a quantified parameter or a symbolic constant.
 *
 * IBEX_VAR can also designate functions arguments bound to domains that do
 * not correspond to actual symbols but subexpressions. For instance, if x->f(x) and y->g(y) are two functions, 
 * then, in f(g(y)), symbol x is bound to a domain g(y). It is just a part of an interval computation. 
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class Entity {
 public:
  /* [hid] Create a new unbound entity (with no type, etc.). */
  Entity() : key(-1), type(IBEX_NONE), domain(&IGNORE), deriv(&IGNORE) { }
  
  /** Return true if the symbol is used. */
  inline bool used() const { return key!=-1; }

  /** Environment number of the corresponding symbol. */
  int key;

  /** Type (e.g., IBEX_VAR). */
  EntityType type;

  /** Domain where the symbol takes values. */
  INTERVAL* domain;

  /** Pointer to the domain of a partial derivative w.r.t. this entity, used
   * in derivation procedures (see #ibex::Env). */
  mutable INTERVAL* deriv;

  /** Used when the domain (or the derivative) is of no interest. */
  static INTERVAL IGNORE; 

  friend class Space;

 private:
  /* [hid] Since #domain and #deriv are duplicated pointers, such an assignment operator is dangerous.
   * Only accessible by #ibex::SpaceFactory when resizing the ent_env2dom table.
   */
  Entity& operator=(const Entity& e) {
    key=e.key;
    type   =e.type;
    domain =e.domain;
    deriv  =e.deriv;
    return *this;
  }
};
 
class SpaceFactory;

/** \ingroup maingroup
 *
 * \brief Space (Entity types & domains)
 *
 * A space contains the types and domains bound to some symbols of a given environment 
 * (e.g., the symbol "x" is a <b>variable</b> with
 * domain [0,1]). 
 *
 * A space usually contains information on which symbols are used by an operator (contractor,
 * bisector, etc.) and the types and domains bound to these symbols (e.g., the symbol "x" 
 * is a <b>variable</b> with domain [0,1]). Every contrator is working on a given space.
 * Because operators only handle "flat" data, all the symbols handled in this class are 
 * zero-dimensional, not vector nor matrix symbols.
 * In particular, a symbol argument of any of the above functions must be zero-dimensional.
 * If A is a matrix, A[1][2] is an example of valid zero-dimensional symbol. 
 * \warning If v is a 1-component vector, v is not considered as a zero-dimensional 
 * symbol (use v[1] instead).
 * This also holds for one-rowed or one-columned matrices. 
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
 class Space : public virtual SharedObject {
 public:
  /** Create an empty space.   
   * \warning This constructor should only be used by #ibex::SpaceFactory. */
  Space(const Env& env);  

  /** Duplicate this instance. 
   * All members are passed by copy, expect the environment (passed by reference). */
  Space(const Space& d);

  /** Assignment operator. All members are passed by copy (except the environment).
   * \pre Both spaces must be defined on the same environment. */ 
  Space& operator=(const Space& s); 

  /** Create a space with a space factory. */
  Space(const SpaceFactory& builder);

  /** Deletes this instance. */
  ~Space();

  /** The environment. */
  const Env& env;
  
  /**
   * The current box (domain) of variables and exist-params. 
   * 
   * This field can be directly read/written. <br>
   * \warning All Bias/Profil functions start indexes from 1. 
   * E.g., \a box(1) is the interval of the <b>first</b> variable.
   * \a box(0) returns an undefined result.
   */
  INTERVAL_VECTOR box;    

  /**
   * The box (domain) of univ-params
   * 
   * This field can be directly read/written. <br>
   * \warning All Bias/Profil functions start indexes from 1.  
   * E.g., \a upx(1) is the interval of the <b>first</b> univ-param.
   * \a upx(0) returns an undefined result.
   */
  INTERVAL_VECTOR upx;    

  /**
   * The box (domain) of exist-params. 
   * 
   * This field can be directly read/written. <br>
   * \warning All Bias/Profil functions start indexes from 1. 
   * E.g., \a epx(1) is the interval of the <b>first</b> exist-param.
   * \a epx(0) returns an undefined result.
   */
  INTERVAL_VECTOR epx;    

  /**
   * The box (domain) of symbolic constants. 
   * 
   * This field can be directly read/written. <br>
   * \warning All Bias/Profil functions start indexes from 1. 
   * E.g., \a sbx(1) is the interval of the <b>first</b> symbolic constant.
   * \a sbx(0) returns an undefined result.
   */
  INTERVAL_VECTOR sbx;    

  /** Add an entity in the space. 
   * This method is very slow: in O(n), where n is the number of keys
   * in the environment. It should only be called at construction.
   */
  void add_entity(int key, INTERVAL& x, EntityType type);

  /** Return the entity bound to a symbol.
   * \warning The entity domain is passed by reference. The method is
   * declared "const" just for convenience. */
  const Entity& entity(const char* symbol) const;

  /** Return the entity bound to a symbol.
   * \param key - the environment number of the symbol. */
  inline const Entity& entity(int key) const {
    if (!used(key)) {
      stringstream s;
      s << "Unbound symbol key (" << key << ")";
      throw NonRecoverableException(s.str());
    }
    else return ent_env2dom[key];
  }

  /** Return the domain bound to a symbol.
   * \param key - the environment number of the symbol. */
  inline INTERVAL& domain(int key) {
    return *entity(key).domain;
  }

  /** Return the domain bound to a symbol.
   * \param key - the environment number of the symbol. */
  inline const INTERVAL& domain(int key) const {
    return *entity(key).domain;
  }

  /** Return true if a symbol is bound to an entity inside this space. */
  inline bool used(int key) const {
    return key<size && ent_env2dom[key].used();
  }

  /** Return the number of variables in the space*/
  inline int nb_var() const { return nb_ent[IBEX_VAR]; }

  /** Return the number of exist-params in the space */
  inline int nb_epr() const { return nb_ent[IBEX_EPR]; }

  /** Return the number of univ-params in the space */
  inline int nb_upr() const { return nb_ent[IBEX_UPR]; }

  /** Return the number of symbolic constants in the space */
  inline int nb_syb() const { return nb_ent[IBEX_SYB]; }

  /** Return the number of entities of a given type in the space */
  inline int nb(EntityType type) const { return nb_ent[type]; }

  /** Return the name of variable \a var.
   *  \pre 0 <= \a var < #nb_var(). */
  const char* var_name(int var) const; 

  /** Return the name of the exist-param \a epr.
   *  \pre 0 <= \a epr < #nb_epr(). */
  const char* epr_name(int epr) const;

  /** Return the name of the univ-param \a upr. 
   *  \pre 0 <= \a upr < #nb_upr(). */
  const char* upr_name(int upr) const;

  /** Return the name of the symbolic constant \a syb. 

   *  \pre 0 <= \a syb < #nb_syb(). */
  const char* syb_name(int syb) const;

  /** Return the key (environment number) of an entity symbol.
   * \param type - the entity type.
   * \param num - the domain (component) number. */
  int key(EntityType type, int num) const {
    return ent_dom2env[type][num];
  }

  /** Return the component number of an entity
   * in the corresponding domain array .*/
  int component(int key) const {
    return ((hash_map<int,int>&) dom_num)[key];
  }

  /** Return a reference to the domain of a given type.
   * This reference is either #box, #upx, #epx or #sbx. */
  inline const INTERVAL_VECTOR& vec_domain(EntityType type) const {
    switch (type) {
    case IBEX_VAR : return box; break;
    case IBEX_UPR : return upx; break;
    case IBEX_EPR : return epx; break;
    default       : return sbx; break;
    }
  }


  /** Return a reference to the domain of a given type.
   * This reference is either #box, #upx, #epx or #sbx. */
  inline INTERVAL_VECTOR& vec_domain(EntityType type) {
    return (INTERVAL_VECTOR&) ((const Space&) *this).vec_domain(type);
  }

  /** Return a const reference to the domain of an entity.
   * This reference is either a component of #box, #upx, #epx or #sbx. 
   * \param type - the entity type.
   * \param num - the domain (component) number. */
  inline const INTERVAL& domain(EntityType type, int num) const {
    return vec_domain(type)(num+1);
  }

  /** Return a reference to the domain of an entity.
   * This reference is either a component of #box, #upx, #epx or #sbx. 
   * \param type - the entity type.
   * \param num - the domain (component) number. */
  inline INTERVAL& domain(EntityType type, int num) {
    return (INTERVAL&) ((const Space&) *(this)).domain(type,num);
  }

  /** Return (by reference) the domain of an entity bound to a symbol.
   * This reference is either a component of #box, #upx, #epx or #sbx. */
  inline INTERVAL& domain(const char* symbol) {
    return *entity(symbol).domain;
/*     const Entity& entity(symbol); */
/*     return domain(e.type,e.dom_num); */
  }

  /** Return the entity associated to a domain (component) number. 
   * \param type - the entity type.
   * \param dom_num - the domain (component) number. */
  inline const Entity& ent(EntityType type, int dom_num) const {
    return entity(key(type, dom_num));
  }


  /** Return the domain number of a variable.
   * \param symbol - the variable symbol. */
  int var(const char* symbol) const { 
    const Entity& e=entity(symbol);
    if (e.type!=IBEX_VAR) throw NonRecoverableException(string("Entity \"")+symbol+"\" exists but has not the required type (variable)");
    else return component(e.key);
  }

  /** Return the domain (component) number of an exist param.
   * \param symbol - the exist param symbol. */
  int epr(const char* symbol) const { 
    const Entity& e=entity(symbol);
    if (e.type!=IBEX_EPR) throw NonRecoverableException(string("Entity \"")+symbol+"\" exists but has not the required type (exist param)");
    else return component(e.key);
  }

  /** Return the domain (component) number of a univ param.
   * \param symbol - the univ param symbol. */
  int upr(const char* symbol) const { 
    const Entity& e=entity(symbol);
    if (e.type!=IBEX_UPR) throw NonRecoverableException(string("Entity \"")+symbol+"\" exists but has not the required type (symbolic constant)");
    else return component(e.key);
  }

  /** Return the domain (component) number of a symbolic constant.
   * \param symbol - the symbol. */
  int syb(const char* symbol) const { 
    const Entity& e=entity(symbol);
    if (e.type!=IBEX_SYB) throw NonRecoverableException(string("Entity \"")+symbol+"\" exists but has not the required type (univ param)");
    else return component(e.key);
  }

  /** Enables/disables output flags. This method allows to control which data is outputted when an instruction such
   * as <tt>cout << space</tt> is used. 
   *
   * \param flags - an array of 5 bits: (from most to least significant) <br>
   * <ul><li>if bit 3 is set, spaces of variables will be streamed out (default value is <b>set</b>).  
   *     <li>if bit 2 is set, spaces of univ-params will be streamed out (default value is <b>unset</b>).
   *     <li>if bit 1 is set, spaces of exist-params will be streamed out (default value is <b>set</b>).
   *     <li>if bit 0 is set, spaces of symbolic constants will be streamed out (default value is <b>unset</b>).
   * </ul>
   * \note Components of an interval vector are aligned by columns, displayed with
   *       the current digits precision along with their associated entity names. 
   *       There is a "hidden" 4th bit. If set, internal structures will be also displayed. */
  void set_output_flags(short flags) const {
    this->flags = bitset<5>(flags);
  }

  /** Synchronizes with another space \a space.
   * The complexity is linear in the number of entities in the <b>source</b> space (i.e., this object).
   *
   * \param space - the space to synchronize with.
   * \param flags - an array of 16 bits specifying which type of entity must be synchronized.
   * The following table shows all the combinaisons of entity types with their associated bit. For example,
   * if bit 10 is set, then any entity \a x that is a symbolic constant (SYB) in the current space and an 
   * exist-parameter (EPR) in \a space will be <i>synchronized</i>, i.e., the domain of \a x in \a space will overwrite 
   * the current domain of \a x. By default, all bits are set (65535).
   * \param logic_op - if true (=AND), an entity domain is simply set to the corresponding domain in \a space,
   * erasing any previous assignment. If false (=OR), the domain of an entity is set to the hull of its current interval 
   * value with the corresponding domain in \a space.
   *
   * <table border="1" align="center">
   * <tr><td colspan="5">Bits controling entities to be synchronized (1=least significant bit)</td></tr>
   * <tr><th>&nbsp;</th><th>VAR</th><th>EPR</th><th>SYB</th><th>UPR</th></tr>
   * <tr><th>VAR</th><td>1</td><td>2</td><td>3</td><td>4</td></tr>
   * <tr><th>EPR</th><td>5</td><td>6</td><td>7</td><td>8</td></tr>
   * <tr><th>SYB</th><td>9</td><td>10</td><td>11</td><td>12</td></tr>
   * <tr><th>UPR</th><td>13</td><td>14</td><td>15</td><td>16</td></tr>
   * </table>
   * \pre Both spaces must have been built 
   * from the same environment (which is automatic with transformations). E.g., this means that you should not 
   * call this method with spaces loaded from different files. */
  void sync(const Space& space, unsigned short flags=65535, bool logic_op=true);

  /** Streams out the space. */
  friend std::ostream& operator<<(std::ostream&, const Space& space);

  /** Default output flags value (12). */
  static int default_flags_value;

 private:
  friend class SpaceFactory;

  void copy_tables(const Space& s);

  mutable bitset<5> flags;  // flags for output

  /* ------------------------------------------*/
  /* conversion table: environment ==>> space */
  Entity *ent_env2dom;

  /* size of ent_env2dom */
  int size;                          

  int nb_ent[4];

  /* ------------------------------------------*/
  /* conversion tables: space ==>> environment */
  int *ent_dom2env[4];       

  /** Maps (the environment number of) and entity to its component number inside 
   * its domain vector. Example: if x is the first variable (in the space, not
   * the environment), its "domain number" will be 0 and the first component of #box 
   * will point to the domain of x. The following code will display three times
   * the same interval:
   * \code
   * void foo(const char* name) {
   *   Entity& entity = space.ent(name);
   *   if (entity.type==IBEX_VAR) {
   *     int i = space.var(name);
   *     cout << ent.domain() << endl;
   *     cout << space.box(i+1) << endl;
   *     cout << space.domain(IBEX_VAR,i) << endl;
   *   }
   * }
   * \endcode
  */
  hash_map<int,int> dom_num; // associate to an key the dom_num

};

/** \ingroup maingroup
 *
 * \brief Space factory.
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class SpaceFactory {
 public:
  /** Create a space factory */
  SpaceFactory(const Env& env);

  /** Builds the space. */
  Space* build_space(Space* s=NULL) const;

  /** Return a reference to the environment. */
  const Env& get_env() const { return env; }

  /** Delete this instance. */
  ~SpaceFactory();

  /** Set the type and domain of an entity from a symbol.
   *
   * \param id_key - the symbol number in the environment.
   * \param type - the type that must be given to the entity in the space (IBEX_VAR, IBEX_UPR, IBEX_EPR or IBEX_SYB)
   * \param domain - the domain that must be given to the entity in the space. */
  void set_entity(int id_key, EntityType type, const INTERVAL& domain);

  /** Set the type of an entity from a symbol.
   *
   * \param id_key - the symbol number in the environment.
   * \param type - the type that must be given to the entity in the space (IBEX_VAR, IBEX_UPR, IBEX_EPR or IBEX_SYB) */
  void set_entity(int id_key, EntityType type) { 
    set_entity(id_key, type, INTERVAL(0,0)); 
  }

  /** Set the type of an entity from a symbol.
   * This function can be used to set the type of all the components of an array simultaneously.
   * For instance, if x is a 2D array, the following code
   * \code
   * env.set_entity("x",IBEX_VAR);
   * \endcode
   * will set every symbol x[i][j] to a variable in the new space.
   *
   * \param name - the symbol corresponding to the entity (must be declared in the environment).
   * \param type - the type that must be given to the entity in the space (IBEX_VAR, IBEX_UPR, IBEX_EPR or IBEX_SYB) */
  void set_entity(const char* name, EntityType type);

  /** Set the type and domain of an entity from an symbol.
   * This function can be used to set the type of all the components of an array simultaneously (see above).
   *
   * \param name - the symbol corresponding to the entity (must be declared in the environment).
   * \param type - the type that must be given to the entity in the space (IBEX_VAR, IBEX_UPR, IBEX_EPR or IBEX_SYB)
   * \param domain - the domain that must be given to the entity in the space. */
  void set_entity(const char* name, EntityType type, const INTERVAL& domain);

  /** Return true if the symbol with number \a id_key is already set. */
  bool is_set(int id_key) const { 
    return tmp_space.used(id_key); 
  }

  /** Environment. */
  const Env& env;

 protected:

  mutable Space tmp_space;   // a temporary copy of the space under construction.
  mutable Space* new_space;  // the new built space (the value of this pointer is NULL until build is done).

  /* Better to avoid (such constructor would be really confusing, e.g., should it build a factory that builds 
     the same new space or a copy ?). See comments for the copy constructor of ContractorFactory. */
  SpaceFactory(const SpaceFactory& f); 
};


/* class SpaceCopy : public SpaceFactory { */
/** Create a new space copy factory */
/*   SpaceCopy(const Space& space); */
/* }; */

/** Checks if two spaces are connected to the same environment.
 * If they are not, an #NotSameEnvException exception is thrown.
 * Otherwise, nothing happens. */
inline void check_context(const Space& space1, const Space& space2) {
  if (&space1.env != &space2.env) throw NonRecoverableException("Spaces with different environments");
}

} // end namespace

#endif
