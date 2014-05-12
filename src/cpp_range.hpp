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
 * 09/05/2014   Wei Song
 *
 *
 */

#ifndef _CPP_RANGE_H_
#define _CPP_RANGE_H_

#include <list>
#include <vector>
#include <utility>
#include <string>
#include <alogorithm>
#include <boost/tuple/tuple.hpp>

namespace CppRange {

  ////////////////////////////////////////////////// 
  // RangeElement
  // 
  // a single dimension range expression
  // the basic element of a multi-dimensional range
  //
  //////////////////////////////////////////////////

  template <class T>
  class RangeElement {
  public:
    //////////////////////////////////////////////
    // constructors

    // single bit range
    RangeElement(const T& r, bool compress = true)
      : compressed(compress), r_pair(T) {}

    // bit range
    RangeElement(const T& rh, const T& rl, bool compress = true)
      : compressed(compress), r_pair(std::max(rh, rl), std::min(rh, rl)) {}

    // type conversion
    template <class Y>
    RangeElement(const RangeElement<Y>& r)
      : compressed(r.compressed), r_pair(r.r_pair) {}

    //////////////////////////////////////////////
    // Helpers

    // size of bit
    T size() const {
      return r_pair.first - r_pair.second + 1;
    } 

    // check whether range r is enclosed in this range 
    template <calss Y>
    bool is_enclosed(const RangeElement<Y>& r) const {
        return (
                !(r_pair.first < r.r_pair.first) && 
                !(r.r_pair.second < r_pair.second)
                );      
    }

    // check whether range r is equal with this range
    template <calss Y>
    bool is_same(const RangeElement<Y>& r) const {
      return (
              r_pair.first == r.r_pair.first &&
              r_pair.second == r.r_pair.second
              );
    }

  private:
    bool compressed;            // whether to compress single bit range when streamout
    std::pair<T, T> r_pair;     // specific range expression
  };

  /////////////////////////////////////////////
  // overload operators

  // rhs range is enclosed in lhs (not equal)
  template <class T, class Y>
  bool operator> (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.is_enclosed(rhs) && !lhs.is_same(rhs);
  }

  // rhs range is enclosed 
  template <class T, class Y>
  bool operator>= (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.is_enclosed(rhs);
  }

  // lhs range is enclosed in rhs (not equal)
  template <class T, class Y>
  bool operator< (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return rhs.is_enclosed(lhs) && !rhs.is_same(lhs);
  }

  // lhs range is enclosed in rhs
  template <class T, class Y>
  bool operator<= (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return rhs.is_enclosed(lhs);
  }
  
  // two ranges are equal
  template <class T, class Y>
  inline bool operator== (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return rhs.is_same(lhs);
  }


  // ----------------------------------------------------------------- //


  ////////////////////////////////////////////////// 
  // Range
  // 
  // multi-dimensional range expression
  //
  //////////////////////////////////////////////////

  template <class T>
  class Range {
  public:
    //////////////////////////////////////////////
    // constructors
    
    // default constructor
    Range() : compressed(true) {}

    // Range with configuration
    Range(bool compress) : compressed(compress) {}

    //////////////////////////////////////////////
    // Helpers

    // size of dimension
    unsigned long size_dimension() const {
      return r_array.size();
    }

    // size of bits
    T size_bit() const {
      T rv(1);
      BOOST_FOREACH(const RangeElement& r, r_array) {
        rv = rv * r.size();
      }
      return rv;
    }

    // add a higher dimension
    template <class Y>
    void push_front(const RangeElement<Y>& r) {
      r_array.push_front(r);
    }

    // add a lower dimension
    template <class Y>
    void push_back(const RangeElement<Y>& r) {
      r_array.push_back(r);
    }

    // check whether range r is enclosed in this range 
    template <class Y>
    bool is_enclosed(const Range<Y>& r) const {
      assert(size_dimension() == r.size_dimension());
      list<dfgRangeElement<T> >::const_iterator lit = r_array.begin();
      list<dfgRangeElement<Y> >::const_iterator rit = r.r_array.begin();
      while(lit != r_array.end()) {
        if(!(*lit >= *rit))
          return false;
      }
      return true;
    }

    // check whether r is equal with this range
    template <class Y>
    bool is_same(const Range<Y>& r) const {
      assert(size_dimension() == r.size_dimension());
      list<dfgRangeElement<T> >::const_iterator lit = r_array.begin();
      list<dfgRangeElement<Y> >::const_iterator rit = r.r_array.begin();
      while(lit != r_array.end()) {
        if(!(*lit == *rit))
          return false;
      }
      return false;
    }
    bool operator bool() const {return !r_array.empty();} 

  private:
    bool compressed;                       // whether to compress single bit range when streamout
    std::vector<RangeElement<T> > r_array; // the range array
  };

  inline bool operator> (const Range& lhs, const Range& rhs) {
    return lhs.is_enclosed(rhs) && !lhs.is_same(rhs);
  }

  inline bool operator>= (const Range& lhs, const Range& rhs) {
    return lhs.is_enclosed(rhs);
  }

  inline bool operator< (const Range& lhs, const Range& rhs) {
    return rhs.is_enclosed(lhs) && !rhs.is_same(lhs);
  }

  inline bool operator<= (const Range& lhs, const Range& rhs) {
    return rhs.is_enclosed(lhs);
  }
  
  inline bool operator== (const Range& lhs, const Range& rhs) {
    return rhs.is_same(lhs);
  }

  // return the overlapped range
  Range operator& (const Range& lhs, const Range& rhs);

  // return the combined range (in first), if not combination, return them in descending ordered
  std::pair<Range, Range> operator| (const Range& lhs, const Range& rhs);

  // return the standard division (high, overlapped, low)
  boost::tuple<Range, Range, Range> operator^ (const Range& lhs, const Range& rhs);

  // helper function to handle the range of signals
  std::pair<std::string, Range> divide_signal_name(const std::string&);

}

#endif
