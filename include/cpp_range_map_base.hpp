/*
 * Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk> 
 *    Advanced Processor Technologies Group, School of Computer Science
 *    University of Manchester, Manchester M13 9PL UK
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *    
 *        http://www.apache.org/licenses/LICENSE-2.0
 *    
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 *
 */

/* 
 * An C++ STL static range calculation library
 * 20/05/2014   Wei Song
 *
 * Note: this class is not suppose to be used by user directly
 *
 */

#ifndef _CPP_RANGE_MAP_BASE_H_
#define _CPP_RANGE_MAP_BASE_H_

#include <list>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

#include "cpp_range_multi.hpp"

namespace CppRange {

  //////////////////////////////////////////////////
  // RangeMapBase
  //
  // store the current range and its child ranges of RangeMap
  //
  //////////////////////////////////////////////////
  template <class T>
  class RangeMapBase : public RangeElement<T> {
  private:
    std::list<RangeMapBase> child;    // sub-dimensions
    unsigned int level;               // level of sub-ranges
  public:

    //////////////////////////////////////////////
    // constructors

    RangeMapBase();
    RangeMapBase(const T&);                             // a single bit range 
    RangeMapBase(const T&, const T&);                   // single level range
    explicit RangeMapBase(const RangeElement<T>&);      // range element to range map base
    explicit RangeMapBase(const Range<T>&);             // multidimensional range to range map base
    explicit RangeMapBase(const RangeElement<T>&, const std::list<RangeMapBase>&);
                                                        // multidimensional range map base
    template<class InputIterator>
    RangeMapBase(const RangeElement<T>&, InputIterator first, InputIterator last);
                                                        // build a multidimensional range map 
                                                        // using iterators
    RangeMapBase& operator= (const RangeElement<T>&);   // convert a RangeElement to a RangeMapBase


    //////////////////////////////////////////////
    // Helpers

    using RangeElement<T>::upper;
    using RangeElement<T>::lower;

    virtual unsigned int dimension() const;             // the number of dimensions
    virtual T size() const;                             // the size of the range
                                                        // ** check: validation beforehand
                                                        // ** the same for all followings
    virtual bool valid() const;                         // ? this is a valid range
    virtual bool empty() const;                         // ? this is an empty range 
    virtual bool equal(const Range& r) const;           // ? this == r 
    virtual bool overlap(const Range& r) const;         // this & r != []
    virtual bool disjoint(const Range& r) const;        // this & r == []
    virtual boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> 
    combine(const Range& r) const;                      // get the union of this and r
    virtual Range intersection(const Range& r) const;   // get the intersection of this and r
    virtual boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> 
    complement(const Range& r) const;                   // subtract r from this range
    
    virtual std::ostream& streamout(std::ostream& os) const;
                                                        // stream out the range
  protected:

    void set_child(const std::list<RangeMapBase>&);     // set a new child range list 
    bool add_child(const RangeMapBase&);                // insert a sub-range to the child list
    
    //////////////////////////////////
    // static helper functions

    static T size(const std::list<RangeMapBase>&);      // calculate the bit size of a range list
    static bool empty(const std::list<RangeMapBase>&);  // ? a range list is empty
    static bool valid(const std::list<RangeMapBase>&, unsigned int level);  
                                                        // ? a range list is valid
    static bool subset(const std::list<RangeMapBase>&, const std::list<RangeMapBase>&);
                                                        // subset relation of two range lists
    static bool equal(const std::list<RangeMapBase>&, const std::list<RangeMapBase>&);
                                                        // ? two range lists are equal
    static std::list<RangeMapBase> 
    combine(const std::list<RangeMapBase>&, const std::list<RangeMapBase>&);
                                                        // combine two range lists
    static std::list<RangeMapBase>
    intersection(const std::list<RangeMapBase>&, const std::list<RangeMapBase>&);
                                                        // get the intersection of two range lists
    static std::list<RangeMapBase>
    complement(const std::list<RangeMapBase>&, const std::list<RangeMapBase>&);
                                                        // get the result of list l - list r
    static void normalize(std::list<RangeMapBase>&);    // normalize a range list
    static void add_child(std::list<RangeMapBase>&, const RangeMapBase&);
                                                        // add a Range into a list of ranges
    static const std::list<RangeMapBase>& validate(const std::list<RangeMapBase>&);
                                                        // validate a range list

  private:
    // Disable some derived member functions

    // connected() is too difficult and with no explicit usage in RangeMap
    void connected();
    
    // divide() and hull() are no longer useful because the combine() is now capable of handling
    // complex ranges
    void divide();
    void hull();

    // kind impossible to implement a strict less
    void less();

    // not needed in a not user friendly class
    void subset();
    void proper_subset();
    void superset();
    void proper_superset();
    void singleton();
  };

  /////////////////////////////////////////////
  // implementation of class methods

  // constructors
  // default to construct an range with undefined value
  template<class T> inline
  RangeMapBase<T>::RangeMapBase()
    : RangeElement<T>(), level(0) {}

  // single bit range
  template<class T> inline
  RangeMapBase<T>::RangeMapBase(const T& r)
    : RangeElement<T>(r), level(1) {}

  // bit range
  template<class T> inline
  RangeMapBase<T>::RangeMapBase(const T& rh, const T& rl)
    : RangeElement<T>(rh, rl), level(1) {}

  // type conversion
  template<class T> inline
  RangeMapBase<T>::RangeMapBase(const RangeElement<T>& r)
    : RangeElement<T>(r), level(1) {}

  // type conversion
  template<class T> inline
  RangeMapBase<T>::RangeMapBase(const Range<T>& r) 
    :level(0) {
    std::vector<RangeElement<T> >::const_iterator it = r.begin();
    if(it != r.end()) {
      RangeElement<T> base_range = *it++;
      *this = RangeMapBase(base_range, it, r.end());
    }
  }

  // combined build
  template<class T> inline
  RangeMapBase<T>::RangeMapBase(const RangeElement<T>& r, const std::list<RangeMapBase>& rlist)
    : RangeElement<T>(r), child(validate(rlist)) {
    if(child.empty()) level = 1;
    else              level = child.front().level + 1;
  }

  // build a multidimensional range map using iterators
  template<class T> template<class InputIterator> inline
  RangeMapBase(const RangeElement<T>& r, InputIterator first, InputIterator last)
    : RangeElement<T>(r) {
    if(first != last) {
      RangeElement<T> base_range = *first++;
      child.push_back(RangeMapBase(base_range, first, last));
      level = child.front().level + 1;
    } else {
      level = 1;
    }
  }

  // assign
  template<class T> inline
  RangeMapBase<T>& RangeMapBase<T>::operator= (const RangeElement<T>& r) {
    *this = RangeMapBase(r);
  }

  //////////////////////////////////////////////
  // Helpers

  // get the number of dimensions
  template<class T> inline
  unsigned int RangeMapBase<T>::dimension() const {
    return level;
  }
    
  // get the size of this range
  template<class T> inline
  T RangeMapBase<T>::size(bool check) const {
    return size(child) * RangeElement<T>::size();
  }
  
  // whether the range is valid
  template<class T> inline
  bool RangeMapBase<T>::valid() const {
    return RangeElement<T>::valid() && valid(child, level - 1);
  }
  
  // whether the range is empty
  template<class T> inline
  bool RangeMapBase<T>::empty(bool check) const {
    return RangeElement<T>::empty() || empty(child);
  }

  // check whether this is a subset of r
  template<class T> inline
  bool RangeMapBase<T>::subset(const RangeMapBase& r) const {
    if(empty()) return true;
    if(r.empty()) return false;
    if(!comparable(r)) return false; // or throw an exception
    return RangeElement<T>::subset(r) && subset(child, r);
  }

  // check whether range r is equal with this range
  template<class T> inline
  bool RangeMapBase<T>::equal(const RangeMapBase& r) const {
    if(empty()) return r.empty();
    if(!comparable(r)) return false; // or throw an exception
    return RangeElement<T>::equal(r) && equal(child, r);
  }
  
    
    // weak order
    virtual bool less(const RangeMapBase& r) const {
      return 
        (level == r.level)
        && (
            RangeElement<T>::less(r) 
            || (
                RangeElement<T>::is_same(r) 
                && less(child, r.child)
                )
            );
    }

    // check whether r has shared range with this range
    virtual bool is_overlapped(const RangeMapBase& r) const {
      return overlap(r).is_valid(); // if A&B != []; then A and B are overlapped
    }

    // combine two ranges
    virtual boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> 
    combine(const RangeMapBase& r) const {
      boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> rv;
      
      if(level != r.level)
        return rv;     

      RangeElement<T> RAnd = RangeElement<T>::overlap(r);
      if(RAnd.is_valid()) {
        // get the standard division
        RangeElement<T> rH, rM, rL;
        boost::tie(rH, rM, rL) = RangeElement<T>::divideBy(r);
        boost::get<0>(rv) = rH;
        boost::get<1>(rv) = rM;
        boost::get<2>(rv) = rL;
        
        // get the higher part
        if(rH.is_valid()) {
          if(RangeElement<T>::is_enclosed(rH)) // this > rH
            boost::get<0>(rv).set_child(child);
          else
            boost::get<0>(rv).set_child(r.child);
        }

        // the overlapped part
        boost::get<1>(rv).set_child(combine(child, r.child));

        // get the lower part
        if(rL.is_valid()) {
          if(RangeElement<T>::is_enclosed(rL)) // this > rL
            boost::get<2>(rv).set_child(child);
          else
            boost::get<2>(rv).set_child(r.child);
        }
      } else {                  // two inadjacent ranges
        if(less(r)) {
          boost::get<0>(rv) = r;
          boost::get<2>(rv) = *this;
        } else {
          boost::get<0>(rv) = *this;
          boost::get<2>(rv) = r;
        }
      }
      return rv;
    }

    // overlap two ranges
    virtual RangeMapBase overlap(const RangeMapBase& r) const {
      if(level != r.level)
        return RangeMapBase();     

      RangeMapBase rv(RangeElement<T>::combine(r));
      if(rv.is_valid())
        rv.child = overlap(child, r.child);

      return rv;
    }
    
    // reduce with another range
    virtual boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> 
    reduce(const RangeMapBase& r) const {
      if(level != r.level)
        return RangeMapBase();     

      RangeElement<T> RAnd = RangeElement<T>::overlap(r);
      boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> rv;
      boost::get<1>(rv) = RAnd;
      if(RAnd.is_valid()) {
        // higher part
        if(first() > RAnd.first()) {
          boost::get<0>(rv) = *this;
          boost::get<0>(rv).second() = RAnd.first() + min_unit<T>();
        }

        // lower part
        if(RAnd.second() > second()) {
          boost::get<2>(rv) = *this;
          boost::get<2>(rv).first() = RAnd.second() - min_unit<T>();          
        }

        // the middle part
        boost::get<1>(rv).set_child(reduce(child, r.child));
      }

      return rv;
    }

    // stream out function
    virtual std::ostream& streamout(std::ostream& os) const{
      RangeElement<T>::streamout(os);
      if(!child.empty()) {
        if(child.size() > 1) {  // more than one sub-ranges
          os << "{";
          for(typename std::list<RangeElement<T> >::const_iterator it = child.begin();
              it != child.end(); ) {
            os << *it;
            ++it;
            if(it != child.end()) os << ";";
          }
          os << "}";
        } else {                // only one sub-range
          os << child.front();
        }
      }
      return os;
    }

  //////////////////////////////////
  // protected helper functions

  //////////////////////////////////
  // static helper functions

  // claculat the bit size
  template<class T>
  T RangeMapBase<T>::size(const std::list<RangeMapBase>& rlist) {
    // will not check the validation, the non-static method should check it
    T rv(0);
    BOOST_FOREACH(const RangeMapBase& b, rlist)
      rv = rv + b.size();  // disable check
    return rv;
  }

  // valid range expression
  template<class T>
  bool RangeMapBase<T>::valid(const std::list<RangeMapBase>& rlist, unsigned int l) {
    BOOST_FOREACH(const RangeMapBase& b, rlist)
      if(!b.valid() || l != b.level) return false;
    return true;
  }

  // check whether the child list is empty
  template<class T>
  bool RangeMapBase<T>::empty(const std::list<RangeMapBase>& rlist) {
    // will not check the validation, the non-static method should check it
    if(rlist.empty()) return false; // empty child means it is a leaf range
                                    // the emptiness is then depends on the base range
    BOOST_FOREACH(const RangeMapBase& b, rlist)
      if(!b.empty()) return true; // any non-empty sub-range means non-empty

    return true;                // all sub-ranges are empty, should be normalized!
  }

  // check whether 'lhs' is a subset of 'rhs'
  template<class T>
  bool RangeMapBase<T>::subset(const std::list<RangeMapBase>& lhs_arg, 
                               const std::list<RangeMapBase>& rhs_arg
                               ) {
    
    std::list<RangeMapBase> lhs = lhs_arg;
    std::list<RangeMapBase> rhs = rhs_arg;

    typename std::list<RangeMapBase<T> >::iterator lit, rit;
    for(lit = lhs.begin(), rit = rhs.begin();
        lit == lhs.end() || rit == rhs.end();
        ) {
      
      if(!lit->RangeElement<T>::intersection(*rit).empty()) {
        
        // using the standard divide function
        RangeElement<T> rH, rM, rL;
        boost::tie(rH, rM, rL) = lit->RangeElement<T>::divide(*rit);
          
        // check the higher part
        if(!rH.empty() && rH.subset(*lit)) // rH belongs to lit
          return false;
        
        // the overlapped part
        if(!subset(lit->child, rit->child))
          return false;
        
        // check the lower part
        if(!rL.empty()) {
          if(rL.subset(*rit)) {
            ++lit;
            *rit = RangeMapBase(rL, rit->child);
          } else {
            ++lit;
            *lit = RangeMapBase(rL, lit->child);
          }
        } else {
          ++lit;
          ++rit;
        }
      } else {
        if(rit->RangeElement<T>::less(*lit)) // some part belongs to lit but not rit
          return false;
        else
          ++rit;
      }
    }

    if(lit != lhs.end())
      return false;
    else
      return true;
  }

  // check whether two range lists are equal
  template<T>
  bool RangeMapBase<T>::equal(const std::list<RangeMapBase>& lhs_arg, 
                            const std::list<RangeMapBase>& rhs_arg
                            ) {
    typename std::list<RangeMapBase>::const_iterator lit, rit;
    for(lit = lhs_arg.begin(), rit = rhs_arg.begin();
        lit != lhs_arg.end() && rit != rhs_arg.end();
        ++lit, ++rit) {
      if(!lit->equal(*rit))
        return false;
    }
       
    if(lit != lhs_arg.end() || rit != rhs_arg.end())
      return false;
    else
      return true;
  }

    // combine two child lists
    static std::list<RangeMapBase> 
    combine (const std::list<RangeMapBase>& lhs_arg, 
             const std::list<RangeMapBase>& rhs_arg
             ) {
      std::list<RangeMapBase> lhs = lhs_arg;
      std::list<RangeMapBase> rhs = rhs_arg;
      std::list<RangeMapBase> rv;
      
      typename std::list<RangeMapBase<T> >::iterator lit, rit;
      for(lit = lhs.begin(), rit = rhs.begin();
          lit == lhs.end() || rit == rhs.end();
          ) {
        // using the standard combine function
        RangeMapBase rH, rM, rL;
        boost::tie(rH, rM, rL) = lit->combine(*rit);
       
        // check result
        if(rH.is_valid()) rv.push_back(rH);
        if(rM.is_valid()) {
          // the two ranges are overlapped
          rv.push_back(rM);

          if(rL.is_valid()) {
            if(lit->RangeElement<T>::is_enclosed(rL)) {
              // the lower part belongs to lit, rit proceeds and lit recounts
              ++rit;
              *lit = rL;
            } else {
              // otherwise lit proceeds and rit recounts
             ++lit;
             *rit = rL;
           }
          } else {
            // both range list proceed
            ++rit;
            ++lit;
          }
        } else {
          // the two ranges are disjunctive
          if(lit->RangeElement<T>::is_enclosed(rL))
            ++rit;
          else
            ++lit;
        }
      }

      // push the rest
      if(lit != lhs.end())
        rv.splice(rv.end(), lhs, lit, lhs.end());
      if(rit != rhs.end())
        rv.splice(rv.end(), rhs, rit, rhs.end());

      normalize(rv);
      return rv;
    }


    // overlap two ranges
    static std::list<RangeMapBase>
    overlap(const std::list<RangeMapBase>& lhs_arg, 
           const std::list<RangeMapBase>& rhs_arg) {

      std::list<RangeMapBase> rv;

      BOOST_FOREACH(const RangeMapBase<T>& cl, lhs_arg) {
        BOOST_FOREACH(const RangeMapBase<T>& cr, rhs_arg) {
          RangeMapBase<T> result = cl & cr;
          if(result.is_valid())
            rv.push_back(result);
        }
      }
      normalize(rv);
      return rv;
    }

    // reduce the content of a range list (rhs) from another (lhs)
    static std::list<RangeMapBase>
    reduce(const std::list<RangeMapBase>& lhs_arg, 
           const std::list<RangeMapBase>& rhs_arg) {
      
      std::list<RangeMapBase> lhs = lhs_arg;
      std::list<RangeMapBase> rhs = rhs_arg;
      std::list<RangeMapBase> rv;
      
      typename std::list<RangeMapBase<T> >::iterator lit, rit;
      for(lit = lhs.begin(), rit = rhs.begin();
          lit == lhs.end() || rit == rhs.end();
          ) {
        // using the standard combine function
        RangeElement<T> rH, rM, rL;
        boost::tie(rH, rM, rL) = lit->RangeElement<T>::divideBy(*rit);
       
        // check result
        if(rH.is_valid() && lit->RangeElement<T>::is_enclosed(rH)) {
          RangeMapBase mH(rH, lit->child);          
          rv.push_back(mH);
        }

        if(rM.is_valid()) {
          // the two ranges are overlapped
          RangeMapBase mM(rM);
          mM.set_child(reduce(lit->child, rit->child));
          rv.push_back(mM);

          if(rL.is_valid()) {
            if(lit->RangeElement<T>::is_enclosed(rL)) {
              // the lower part belongs to lit, rit proceeds and lit recounts
              ++rit;
              *lit = RangeMapBase(rL, lit->child);
            } else {
              // otherwise lit proceeds and rit recounts
             ++lit;
             *rit = RangeMapBase(rL, rit->child);
           }
          } else {
            // both range list proceed
            ++rit;
            ++lit;
          }
        } else {
          // the two ranges are disjunctive
          if(lit->RangeElement<T>::is_enclosed(rL))
            ++rit;
          else
            ++lit;
        }
      }

      // push the rest
      if(lit != lhs.end())
        rv.splice(rv.end(), lhs, lit, lhs.end());

      normalize(rv);
      return rv;
    }

    static void normalize (std::list<RangeMapBase>& rlist) {
      // nothing to be normalized if there is less than one sub-range
      if(rlist.size() <= 1) return true; 

      // it is assumed that all sub-ranges are not overlapped and in weak order

      typename std::list<RangeMapBase>::iterator it, nt;
      nt = rlist.begin();
      it = nt++;

      while(nt != rlist.end()) {
        if(it->RangeElement<T>::is_adjacent(*nt) && is_same(it->child, nt->child)) {
          it->second() = nt->second();
          nt = rlist.erase(nt);
        } else {
          it = nt++;
        }
      }
    }

  };
    
  template<class T>
  void RangeMapBase<T>::add_child(std::list<RangeMapBase>& rlist, const RangeMapBase& r) {
      
      std::list<RangeMapBase> rv;
      RangeMapBase mr = r;

      typename std::list<RangeMapBase<T> >::iterator lit;
      for(lit = rlist.begin();
          lit == rlist.end();
          ) {
        // using the standard combine function
        RangeMapBase rH, rM, rL;
        boost::tie(rH, rM, rL) = lit->combine(mr);
       
        // check result
        if(rH.is_valid()) rlist.insert(lit, rH);
        if(rM.is_valid()) {
          // the two ranges are overlapped
          rlist.insert(lit, rH);

          if(rL.is_valid()) {
            if(lit->RangeElement<T>::is_enclosed(rL)) {
              // the lower part belongs to lit, insertion finished
              mr = RangeMapBase();
              *lit = rL;
              break;
            } else {
              // otherwise lit proceeds and mr recounts
              lit = rlist.erase(lit);
              mr = rL;
           }
          } else {
            // rL empty also means finished
            mr = RangeMapBase();
            lit = rlist.erase(lit);
            break;
          }
        } else {
          // the two ranges are disjunctive
          if(lit->RangeElement<T>::is_enclosed(rL)) {
            // mr already inserted
            mr = RangeMapBase();
            break;
          } else
            ++lit;
        }
      }

      // push the rest
      if(mr.is_valid())
        rlist.insert(lit, mr);

      normalize(rlist);
    }

  // standard out stream
  template<class T>
  std::ostream& operator<< (std::ostream& os, const RangeMapBase<T>& r) {
    return r.streamout(os);
  }

    void set_child(const std::list<RangeMapBase>& c) {
      child = c;
      if(c.empty())
        level = 1;
      else
        level = 1 + c.front().level;      
    }

    // insert a sub-range
    bool add_child(const RangeMapBase& r) {
      if(!r.is_valid()) return false;

      if(child.empty)
        level = r.level + 1;
      else if(level != r.level + 1)
        return false;

      add_child(child, r);
      return true;
    }

}

#endif
