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
    unsigned int level;                // level of sub-ranges    
  public:
    
    //////////////////////////////////////////////
    // constructors
    // default to construct an range with undefined value
    RangeMap()
      : level(0) {}

    // single bit range
    RangeMap(const T& r, bool compress = true) 
      : level(1) {
      child.push_back(RangeMapBase<T>(r, compress));
    }

    // bit range
    RangeMap(const T& rh, const T& rl, bool compress = true) 
      : level(1) {
      child.push_back(RangeMapBase<T>(rh, rl, compress));
    }

    // type conversion
    explicit RangeMap(const RangeMapBase<T>& r) 
      : level(r.get_level()) {
      child.push_back(r);
    }

    // type conversion
    explicit RangeMap(const RangeElement<T>& r) 
      : level(1) {
      child.push_back(RangeMapBase<T>(r));
    }

    // type conversion
    explicit RangeMap(const Range<T>& r)
      : level(r.size_dimension()) {
      add_child(RangeMapBase<T>(r));
    }

    // combined build
    explicit RangeMap(const std::list<RangeMapBase<T> >& rlist)
      : child(rlist) {
      if(rlist.empty())
        level = 0;
      else
        level = rlist.front().level;
    }

    // combined build
    explicit RangeMap(const std::list<RangeElement<T> >& rlist)  
      : level(1) {
      BOOST_FOREACH(const RangeElement<T>& r, rlist)
        child.push_back(RangeMapBase<T>(r));
    }

    // copy
    explicit RangeMap(const RangeMap& r)
      : child(r.child), level(r.level) {}
    
    // assign
    RangeMap& operator= (const RangeElement<T>& r) {
      this->push_back(RangeMapBase<T>(r));
      this->level = 1;
    }
   
    // assign
    RangeMap& operator= (const RangeMapBase<T>& r) {
      this->push_back(r);
      this->level = r.get_level();
    }

    //////////////////////////////////////////////
    // Helpers

  private:
    const std::list<RangeMapBase<T> >& get_child() const {
      return child;
    }

    void set_child(const std::list<RangeMapBase<T> >& c) {
      child = c;
      if(c.empty())
        level = 0;
      else
        level = c.front().get_level();      
    }

  public:

    bool add_child(const RangeMapBase<T>& r) {
      if(!r.is_valid()) return false;

      if(child.empty)
        level = r.level;
      else if(level != r.level)
        return false;

      RangeMapBase<T>::add_child(child, r);
      return true;
    }

    unsigned int get_level() const {
      return level;
    }

    virtual void set_compress(bool compress) {
      RangeMapBase<T>::set_compress(child);
    }
    
    // valid range expression
    virtual bool is_valid() const {
      return RangeMapBase<T>::is_valid(child);
    }

    // check whether range r is enclosed in this range
    virtual bool is_enclosed(const RangeMap& r) const {
      return 
        (level == r.level) 
        && RangeMapBase<T>::is_enclosed(child, r.child);
    }

    // check whether range r is equal with this range
    virtual bool is_same(const RangeMap& r) const {
      return 
        (level == r.level)
        && (child.size() == r.child.size())
        && RangeMapBase<T>::is_same(child, r.child());
    }

    // weak order
    virtual bool less(const RangeMap& r) const {
      return 
        (level == r.level)
        && RangeMapBase<T>::less(child, r.child);
    }

    // check whether r has shared range with this range
    virtual bool is_overlapped(const RangeMap& r) const {
      return overlap(r).is_valid(); // if A&B != []; then A and B are overlapped
    }

    // combine two ranges
    virtual RangeMap combine(const RangeMap& r) const {
      if(level != r.level)
        return RangeMap();
      else
        return RangeMap(RangeMapBase<T>::combine(child, r.child));
    }

    // overlap two ranges
    virtual RangeMap overlap(const RangeMap& r) const {
      if(level != r.level)
        return RangeMap();
      else
        return RangeMap(RangeMapBase<T>::overlap(child, r.child));
    }

    // reduce with another range
    virtual RangeMap reduce(const RangeMap& r) const {
      if(level != r.level)
        return RangeMap();
      else
        return RangeMap(RangeMapBase<T>::reduce(child, r.child));
    }



  };


}


#endif
