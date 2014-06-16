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
 * Complex multi-dimensional Range
 * 20/05/2014   Wei Song
 *
 *
 */

#ifndef _CPP_RANGE_MAP_H_
#define _CPP_RANGE_MAP_H_

#include <list>
#include <boost/foreach.hpp>

#include "cpp_range_map_base.hpp"

namespace CppRange {

  //////////////////////////////////////////////////
  // RangeMap
  //
  // A multi-dimensional range support multi-dimensional calculations
  //
  //////////////////////////////////////////////////
  template <class T>
  class RangeMap {
  private:
    std::list<RangeMapBase<T> > child; // sub-dimensions

  public:
    
    //////////////////////////////////////////////
    // constructors
    // default to construct an range with undefined value
    RangeMap() {}

    // single bit range
    RangeMap(const T& r, bool compress = true) {
      child.push_back(RangeMapBase<T>(r, compress));
    }

    // bit range
    RangeMap(const T& rh, const T& rl, bool compress = true) {
      child.push_back(RangeMapBase<T>(rh, rl, compress));
    }

    // type conversion
    RangeMap(const RangeMapBase<T>& r) {
      child.push_back(r);
    }

    // type conversion
    RangeMap(const RangeElement<T>& r) {
      child.push_back(RangeMapBase<T>(r));
    }

    // combined build
    RangeMap(const std::list<RangeMapBase<T> >& rlist)
      : child(rlist) {}

    // combined build
    RangeMap(const std::list<RangeElement<T> >& rlist) {
      BOOST_FOREACH(const RangeElement<T>& r, rlist)
        child.push_back(RangeMapBase<T>(r));
    }

    // copy
    RangeMap(const RangeMap& r)
      : child(r.child) {}
    
    // assign
    RangeMap& operator= (const RangeElement<T>& r) {
      this->push_back(RangeMapBase<T>(r));
    }
   
    // assign
    RangeMap& operator= (const RangeMapBase<T>& r) {
      this->push_back(r);
    }

    //////////////////////////////////////////////
    // Helpers

    virtual void set_compress(bool compress) {
      BOOST_FOREACH(RangeMapBase<T>& b, child)
        b.set_compress(compress);
    }
    
    // valid range expression
    virtual bool is_valid() const {
      BOOST_FOREACH(const RangeMapBase<T>& b, child)
        if(!b.is_valid()) return false;
      return true;
    }

    // check whether range r is enclosed in this range
    virtual bool is_enclosed(const RangeMap& r) const {
      return RangeMapBase<T>::is_enclosed(child, r.child);
    }

    // check whether range r is equal with this range
    virtual bool is_same(const RangeMap& r) const {
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
    virtual bool less(const RangeMap& r) const {
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
    }

    // check whether r has shared range with this range
    virtual bool is_overlapped(const RangeMap& r) const {
      return overlap(r).is_valid(); // if A&B != []; then A and B are overlapped
    }

    // combine two ranges
    virtual RangeMap combine(const RangeMap& r) const {
      return RangeMap(RangeMapBase<T>::combine(child, r.child));
    }

    // overlap two ranges
    virtual RangeMap overlap(const RangeMap& r) const {
      RangeMap rv;
      // {A|B} & {C|D} == {A&C | A&D | B&C | B&D}
      // the resulted list is in order automatically
      BOOST_FOREACH(const RangeMapBase<T>& cl, child) {
        BOOST_FOREACH(const RangeMapBase<T>& cr, r.child) {
          RangeMapBase<T> result = cl & cr;
          if(result.is_valid())
            rv.child.push_back(result);
        }
      }
      return rv;
    }

    // reduce with another range
    virtual RangeMapBase reduce(const RangeMap& r) const {
      return RangeMap(RangeMapBase<T>::reduce(child, r.child));
    }



  };


}


#endif
