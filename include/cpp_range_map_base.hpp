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
 *
 */

#ifndef _CPP_RANGE_MAP_BASE_H_
#define _CPP_RANGE_MAP_BASE_H_

#include <list>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

#include "cpp_range_element.hpp"

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

    // default to construct an range with undefined value
    RangeMapBase()
      : RangeElement<T>(), level(0) {}

    // single bit range
    RangeMapBase(const T& r, bool compress = true)
      : RangeElement<T>(r, compress), level(1) {}

    // bit range
    RangeMapBase(const T& rh, const T& rl, bool compress = true)
      : RangeElement<T>(rh, rl, compress), level(1) {}

    // type conversion
    RangeMapBase(const RangeElement<T>& r)
      : RangeElement<T>(r), level(1) {}

    // combined build
    RangeMapBase(const RangeElement<T>& r, const std::list<RangeMapBase>& rlist)
      : RangeElement<T>(r), child(rlist) {
      if(rlist.empty())
        level = 1;
      else
        level = 1 + rlist.front().level;
    }

    // copy
    RangeMapBase(const RangeMapBase& r)
      : RangeElement<T>(r), child(r.child), level(r.level) {}

    // assign
    RangeMapBase& operator= (const RangeElement<T>& r) {
      *this = RangeMapBase(r);
    }

    //////////////////////////////////////////////
    // Helpers

    // RangeElement::first();
    // RangeElement::second();
    
    const std::list<RangeMapBase>& get_child() const {
      return child;
    }

    void set_child(const std::list<RangeMapBase>& c) {
      child = c;
      if(c.empty())
        level = 1;
      else
        level = 1 + c.front().level;      
    }

    unsigned int get_level() const {
      return level;
    }
    
    virtual void set_compress(bool compress) {
      RangeElement<T>::set_compress(compress);
      set_compress(child);
    }

    // size of bit
    virtual T size_bit() const {
      return size_bit(child) * RangeElement<T>::size_bit();
    }
    
    // valid range expression
    virtual bool is_valid() const {
      return RangeElement<T>::is_valid() && is_valid(child);
    }
    
    // check whether range r is enclosed in this range 
    virtual bool is_enclosed(const RangeMapBase& r) const {
      return 
        (level == r.level) 
        && RangeElement<T>::is_enclosed(r) 
        && is_enclosed(child, r.child);
    }
    

    // check whether range r is equal with this range
    virtual bool is_same(const RangeMapBase& r) const {
      return 
        (level == r.level)
        && RangeElement<T>::is_same(r)
        && (child.size() == r.child.size())
        && is_same(child, r.child());
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
        rv.set_child(overlap(child, r.child));

      return rv;
    }
    
    // reduce with another range
    virtual boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> 
    reduce(const RangeMapBase& r) const {
      if(level != r.level)
        return RangeMapBase();     

      RangeElement<T> RAnd = RangeElement<T>::overlap(r);
      boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> rv;
      boost::get<1>(rv) = *this;
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

  private:
    // Disable some derived member functions

    // is_adjacent() is too difficult and with no explicit usage in RangeMap
    void is_adjacent();
    
    // divideBy() is no longer useful because the combine() is now capable of handling
    // complex ranges
    void divideBy();

  public:
    //////////////////////////////////
    // static helper functions

    // set compress state for child list
    static void set_compress(std::list<RangeMapBase>& rlist, bool compress) {
      BOOST_FOREACH(RangeMapBase& b, rlist)
        b.set_compress(compress);      
    }

    // claculat the bit size
    static T size_bit(const std::list<RangeMapBase>& rlist) {
      T rv = 0;
      BOOST_FOREACH(const RangeMapBase& b, rlist)
        rv = rv + b.size_bit();
      return rv;
    }

    // valid range expression
    static bool is_valid(const std::list<RangeMapBase>& rlist) {
      BOOST_FOREACH(const RangeMapBase& b, rlist)
        if(!b.is_valid()) return false;
      return true;
    }

    // check whether a range list encloses another one
    static bool is_enclosed(const std::list<RangeMapBase>& lhs_arg, 
                     const std::list<RangeMapBase>& rhs_arg
                     ) {
      
      std::list<RangeMapBase> lhs = lhs_arg;
      std::list<RangeMapBase> rhs = rhs_arg;

      typename std::list<RangeMapBase<T> >::iterator lit, rit;
      for(lit = lhs.begin(), rit = rhs.begin();
          lit == lhs.end() || rit == rhs.end();
          ) {
        
        if(lit->RangeElement<T>::overlap(*rit).is_valid()) {
          
          // using the standard divide function
          RangeElement<T> rH, rM, rL;
          boost::tie(rH, rM, rL) = lit->RangeElement<T>::divideBy(*rit);
          
          // check the higher part
          if(rH.is_valid() && rit->RangeElement<T>::is_enclosed(rH)) // rit > rH
          return false;
        
          // the overlapped part
          if(!is_enclosed(lit->child, rit->child))
            return false;
          
          // check the lower part
          if(rL.is_valid()) {
            if(rit->RangeElement<T>::is_enclosed(rH)) {
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
          if(lit->RangeElement<T>::less(*rit))
            return false;
          else
            ++lit;
        }
      }

      if(rit != rhs.end())
        return false;
      else
        return true;
    }

    // check whether two range lists are equal
    static bool is_same(const std::list<RangeMapBase>& lhs_arg, 
                        const std::list<RangeMapBase>& rhs_arg
                        ) {
      typename std::list<RangeMapBase>::const_iterator it_loc, it_r;
      for(lit = lhs_arg.begin(), rit = rhs_arg.begin();
          lit != lhs_arg.end() && rit != rhs_arg.end();
          ++lit, ++rit) {
        if(!lit->is_same(*rit))
          return false;
      }
       
      if(lit != lhs_arg.end() || rit != rhs_arg.end())
        return false;
      else
        return true;
    }

    // weak order
    static bool less(const std::list<RangeMapBase>& lhs_arg, 
                     const std::list<RangeMapBase>& rhs_arg
                     ) {
      typename std::list<RangeMapBase<T> >::const_iterator lit, rit;
      for(lit = lhs_arg.begin(), rit = rhs_arg.begin();
          lit != lhs_arg.end() && rit != rhs_arg.end();
          ++lit, ++rit) {
        if(!lit->is_same(*rit)) // the first non-equal range
          return lit->less(*rit);
      }

      if(lit != lhs_arg.end()) // local hase larger range
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
        boost::tie(rH, rM, rL) = lit->RangeElement::divideBy(*rit);
       
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

      return rv;
    }

  };
    

}

#endif
