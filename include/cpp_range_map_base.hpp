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

namespace CppRange {

  //////////////////////////////////////////////////
  // RangeMapChild
  //
  // store the child ranges of RangeMap
  //
  //////////////////////////////////////////////////
  template <class T>
  class RangeMapBase : public RangeElement {
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
    RangeMapBase(const RangeElement& r)
      : RangeElement<T>(r) {}

    // copy
    RangeMapBase(const RangeMapBase& r)
      : RangeElement<T>(r), child(r.child) {}

    //////////////////////////////////////////////
    // Helpers

    // RangeElement::first();
    // RangeElement::second();
    // RangeElement::set_compress();

    // size of bit
    virtual T size_bit() const {
      // if any of the sub-range is invalid, the result will be 0
      T rv(RangeElement<T>::size_bit());
      BOOST_FOREACH(const RangeMapBase b, child)
        rv = rv * b.size_bit();
      return rv;
    }
    
    // valid range expression
    virtual bool is_valid() const {
      if(RangeElement<T>::is_valid()) {
        BOOST_FOREACH(const RangeMapBase b, child)
          if(!b.is_valid()) return false;
        return true;
      } else
        return false;
    }
    
    // check whether range r is enclosed in this range 
    virtual bool is_enclosed(const RangeMapBase& r) const {
      return is_same(combine(r)); // if A|B == A, then A >= B
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
    virtual std::list<RangeMapBase> combine(const RangeMapBase& r) const {
      std::list<RangeMapBase> rv;
      RangeElement<T> RAnd = RangeElement<T>::overlap(r);
      if(r.is_valid()) {
        // get the standard division
        RangeElement<T> rH, rM, rL;
        boost::tie(rH, rM, rL) = RangeElement<T>::divideBy(r);
        RangeMapBase rmH(rH), rmM(rM), rmL(rL);

        // get the higher part
        if(rH.is_valid()) {
          if(RangeElement<T>::is_enclosed(rH)) // this > rH
            rmH.child = child;
          else
            rmH.child = r.child;
          rv.push_back(rmH);
        }

        // the overlapped part
        rmM.child = combine(child, r.child);
        rv.push_back(rmM);

        // get the lower part
        if(rL.is_valid()) {
          if(RangeElement<T>::is_enclosed(rL)) // this > rL
            rmL.child = child;
          else
            rmL.child = r.child;
          rv.push_back(rmL);
        }
      } else {                  // two inadjacent ranges
        rv.push_back(*this);
        if(less(r))
          rv.push_front(r);
        else
          rv.push_back(r);
      }
      return rv;
    }

    // simple overlap without check
    virtual RangeElement overlap(const RangeMapBase& r) const {
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
    virtual bool is_adjacent() const;


    //////////////////////////////////
    // private helper functions

    // combine two child lists
    std::list<RangeMapBase> 
    combine (const std::list<RangeMapBase>& lhs_arg, 
             const std::list<RangeMapBase>& rhs_arg
             ) {
      lhs = lhs_arg;
      rhs = rhs_arg;
      std::list<RangeMapBase> rv;
      
      typename std::list<RangeMapBase<T> >::const_iterator lit, rit;
      for(lit = lhs.begin(), rit = rhs.begin();
          lit == lhs.end() || rit = rhs.end();
          ) {
        RangeElement<T> RAnd = lit->RangeElement<T>::overlap(*rit);
        if(RAnd.is_valid()) {
          // using the standard combine function
          rv.splice(rv.end(), lit->combine(*rit));
          
          // check result
          if(lit->is_enclosed(rv.back()) && !rit->is_enclosed(rv.back())) {
            // the lower range belongs to lit
            ++rit;
            *lit = rv.back();
            rv.pop_back();
          } else if(rit->is_enclosed(rv.back()) && !lit->is_enclosed(rv.back())) {
            // the lower range belongs to rit
            ++lit;
            *rit = rv.back();
            rv.pop_back();
          } else {
            // the lower range belongs to both ranges
            ++rit;
            ++lit;
          }

        } else {
          if(lit->less(*rit))
            rv.push_back(*rit++);
          else
            rv.push_back(*lit++);
        }
      }
      // push the rest
      if(lit != lhs.end())
        rv.splice(rv.end(), lhs, lit, lhs.end());
      if(rit != rhs.end())
        rv.splice(rv.end(), rhs, rit, rhs.end());

      return rv;
    }
    


  };
    

}

#endif
