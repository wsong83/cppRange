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

    // set compressed
    void set_compress(bool b) { compressed = b; }

    // size of bit
    T size() const {
      return r_pair.first - r_pair.second + 1;
    } 

    // valid range expression
    bool is_valid() const {
      return !(r_pair.first < r_pair.second);
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

    // check whether r has shared range with this range
    template <class Y>
    bool is_overlapped(const RangeElement<Y>& r) const {
      return (!(r_pair.first < r.r_pair.second) && !(r.r_pair.first < r_pair.second));
    }

    // implicit convert from Range to bool
    bool operator bool() const {return is_valid();}

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

  // rhs range is enclosed in lhs
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

  // return the overlapped range
  // function does not check the result's validation
  template <class T, class Y>  
  Range operator& (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return rv<T>(std::min<T>(lhs.r_pair.first, rhs.r_pair.first), 
                 std::max<T>(lhs.r_pair.second, rhs.r_pair.second), 
                 lhs.compressed);
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

    // type convert from different ranges
    template <class Y>
    Range(const Range<Y>& r)
      : compressed(r.compressed) {
      r_array.resize(r.r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        r_array[i] = r.r_array[i];
    }

    //////////////////////////////////////////////
    // Helpers

    // set compressed
    void set_compress(bool b) { compressed = b; }

    // size of dimension
    unsigned int size_dimension() const {
      return r_array.size();
    }

    // size of bits
    T size_bit() const {
      T rv(1);
      for(unsigned int i=0; i<r_array.size(); i++)
        T = T * r_array[i].size();
      return rv;
    }

    // add a higher dimension
    // this method is very expernsive, do not use if possible
    template <class Y>
    void push_front(const RangeElement<Y>& r) {
      r_array.insert(0, r);
      r_array[0].set_compress(compressed);
    }

    // add a lower dimension
    template <class Y>
    void push_back(const RangeElement<Y>& r) {
      r_array.push_back(r);
      r_array[r_array.size()-1].set_compress(compressed);
    }

    // check whether the range expression is valid
    bool is_valid() const {
      for(unsigned int i=0; i<r_array.size(); i++)
        if(!r_array[i].is_valid()) return false;
      return true;
    }

    // check whether range r is enclosed in this range 
    template <class Y>
    bool is_enclosed(const Range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        if(!(r_array[i] >= r.r_array[i])) return false;
      return true;
    }

    // check whether r is equal with this range
    template <class Y>
    bool is_same(const Range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
       for(unsigned int i=0; i<r_array.size(); i++)
        if(r_array[i] != r.r_array[i]) return false;
      return true;
    }

    // overlap range r with this range
    template <class Y>
    Range overlap(const Range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
      Range rv(compressed);
      rv.r_array.reserve(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        rv.r_array[i] = r_array[i] & r.r_array[i];
      return rv;
    }


    // implicit convert from Range to bool
    bool operator bool() const {return !r_array.empty() && is_valid();} 

  private:
    bool compressed;                       // whether to compress single bit range when streamout
    std::vector<RangeElement<T> > r_array; // the range array
  };

  /////////////////////////////////////////////
  // overload operators

  // rhs range is enclosed in lhs (not equal)
  template <class T, class Y>
  bool operator> (const Range<T>& lhs, const Range<Y>& rhs) {
    return lhs.is_enclosed(rhs) && !lhs.is_same(rhs);
  }

  // rhs range is enclosed in lhs
  template <class T, class Y>
  bool operator>= (const Range<T>& lhs, const Range<Y>& rhs) {
    return lhs.is_enclosed(rhs);
  }

  // lhs range is enclosed in rhs (not equal)
  template <class T, class Y>
  bool operator< (const Range<T>& lhs, const Range<Y>& rhs) {
    return rhs.is_enclosed(lhs) && !rhs.is_same(lhs);
  }

  // lhs range is enclosed in rhs
  template <class T, class Y>
  inline bool operator<= (const Range<T>& lhs, const Range<Y>& rhs) {
    return rhs.is_enclosed(lhs);
  }
  
  // two ranges are equal
  template <class T, class Y>  
  inline bool operator== (const Range& lhs, const Range& rhs) {
    return rhs.is_same(lhs);
  }

  // return the overlapped range
  template <class T, class Y>  
  Range operator& (const Range<T>& lhs, const Range<Y>& rhs) {
    return lhs.overlap(rhs);
  }

  // return the combined range (in first), if not combination, return them in descending ordered
  std::pair<Range, Range> operator| (const Range& lhs, const Range& rhs);

  // return the standard division (high, overlapped, low)
  boost::tuple<Range, Range, Range> operator^ (const Range& lhs, const Range& rhs);

  // helper function to handle the range of signals
  std::pair<std::string, Range> divide_signal_name(const std::string&);

}

#endif
