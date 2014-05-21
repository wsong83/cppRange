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
  public:

    //////////////////////////////////////////////
    // constructors

    // default to construct an range with undefined value
    RangeMapBase()
      : RangeElement<T>() {}

    // single bit range
    RangeMapBase(const T& r, bool compress = true)
      : RangeElement<T>(r, compress) {}

    // bit range
    RangeMapBase(const T& rh, const T& rl, bool compress = true)
      : RangeElement<T>(rh, rl, compress) {}

    // type conversion
    RangeMapBase(const RangeElement<T>& r)
      : RangeElement<T>(r) {}

    // combined build
    RangeMapBase(const RangeElement<T>& r, const std::list<RangeMapBase>& rlist)
      : RangeElement<T>(r), child(rlist) {}

    // copy
    RangeMapBase(const RangeMapBase& r)
      : RangeElement<T>(r), child(r.child) {}

    // assign
    RangeMapBase& operator= (const RangeElement<T>& r) {
      *this = RangeMapBase(r);
    }

    //////////////////////////////////////////////
    // Helpers

    // RangeElement::first();
    // RangeElement::second();
    
    virtual void set_compress(bool compress) {
      RangeElement<T>::set_compress(compress);
      BOOST_FOREACH(RangeMapBase& b, child)
        b.set_compress(compress);
    }

    // size of bit
    virtual T size_bit() const {
      // if any of the sub-range is invalid, the result will be 0
      T rv(RangeElement<T>::size_bit());
      BOOST_FOREACH(const RangeMapBase& b, child)
        rv = rv * b.size_bit();
      return rv;
    }
    
    // valid range expression
    virtual bool is_valid() const {
      if(RangeElement<T>::is_valid()) {
        BOOST_FOREACH(const RangeMapBase& b, child)
          if(!b.is_valid()) return false;
        return true;
      } else
        return false;
    }
    
    // check whether range r is enclosed in this range 
    virtual bool is_enclosed(const RangeMapBase& r) const {
      if(RangeElement<T>::is_enclosed(r)) {
        return is_enclosed(child, r.child);
      } else
        return false;
    }
    

    // check whether range r is equal with this range
    virtual bool is_same(const RangeMapBase& r) const {
      if(!RangeElement<T>::is_same(r))
        return false;
      
      if(child.size() != r.child.size())
        return false;

      typename std::list<RangeMapBase<T> >::const_iterator it_loc, it_r;
      for(it_loc = child.begin(), it_r = r.child.begin();
          it_loc != child.end() && it_r != r.child.end();
          ++it_loc, ++it_r)
        if(it_loc->is_same(*it_r)) 
          return false;
      
      return true;
    }
  
    
    // weak order
    virtual bool less(const RangeMapBase& r) const {
      if(!RangeElement<T>::is_valid() || !r.RangeElement<T>::is_valid()) 
        return false;
      
      if(is_same(r)) return false;

      if(RangeElement<T>::is_same(r)) {
        typename std::list<RangeMapBase<T> >::const_iterator it_loc, it_r;
        for(it_loc = child.begin(), it_r = r.child.begin();
            it_loc != child.end() && it_r != r.child.end();
            ++it_loc, ++it_r) {
          if(!it_loc->is_same(*it_r)) // the first non-equal range
            return it_loc->less(*it_r);
        }
        if(it_loc != child.end()) // local hase larger range
          return false;
        else
          return true;
      } else 
        return RangeElement<T>::less(r); // even the current range is not equal
    }

    // check whether r has shared range with this range
    virtual bool is_overlapped(const RangeMapBase& r) const {
      return overlap(r).is_valid(); // if A&B != []; then A and B are overlapped
    }

    // combine two ranges
    virtual boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> 
    combine(const RangeMapBase& r) const {
      boost::tuple<RangeMapBase, RangeMapBase, RangeMapBase> rv;
      RangeElement<T> RAnd = RangeElement<T>::overlap(r);
      if(r.is_valid()) {
        // get the standard division
        RangeElement<T> rH, rM, rL;
        boost::tie(rH, rM, rL) = RangeElement<T>::divideBy(r);
        boost::get<0>(rv) = rH;
        boost::get<1>(rv) = rM;
        boost::get<2>(rv) = rL;
        
        // get the higher part
        if(rH.is_valid()) {
          if(RangeElement<T>::is_enclosed(rH)) // this > rH
            boost::get<0>(rv).child = child;
          else
            boost::get<0>(rv).child = r.child;
        }

        // the overlapped part
        boost::get<1>(rv).child = combine(child, r.child);

        // get the lower part
        if(rL.is_valid()) {
          if(RangeElement<T>::is_enclosed(rL)) // this > rL
            boost::get<2>(rv).child = child;
          else
            boost::get<2>(rv).child = r.child;
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
      RangeMapBase rv(RangeElement<T>::combine(r));
      if(rv.is_valid() && child.size()) {       
        // {A|B} & {C|D} == {A&C | A&D | B&C | B&D}
        // the resulted list is in order automatically
        BOOST_FOREACH(const RangeElement<T>& cl, child) {
          BOOST_FOREACH(const RangeElement<T>& cr, r.child) {
            RangeElement<T> result = cl & cr;
            if(result.is_valid())
              rv.child.push_back(result);
          }
        }
        if(rv.child.empty())
          return RangeMapBase(); // child ranges combined empty
      }
      return rv;
    }
    
  private:
    // Disable some derived member functions

    // is_adjacent() is too difficult and with no explicit usage in RangeMap
    bool is_adjacent() const;


  protected:
    //////////////////////////////////
    // protected helper functions

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
          rv.push_back(rH);

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

    void reduce(std::list<RangeMapBase>& rlist) {}

  };
    

}

#endif
