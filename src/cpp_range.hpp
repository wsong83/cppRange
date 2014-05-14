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

    // weak order
    template <class Y>
    bool less(const RangeElement<Y>& r) const {
      if(r_pair.first < r.r_pair.first)
        return true;
      else if(r_pair.first > r.r_pair.first)
        return false;
      else if(r_pair.second > r.r_pair.second)
        return true;
      else if(r_pair.second < r.r_pair.second)
        return false;
      else
        return false;
    }

    // check whether r has shared range with this range
    template <class Y>
    bool is_overlapped(const RangeElement<Y>& r) const {
      return (!(r_pair.first < r.r_pair.second) && !(r.r_pair.first < r_pair.second));
    }

    // simple combine without check
    template <class Y>
    RangeElement combine(const RangeElement<Y>& r) const {
      return RangeElement(std::max(r_pair.first, r.r_pair.first),
                          std::min(r_pair.second, r.r_pair.second),
                          compressed);
    }

    // simple overlap without check
    template <class Y>
    RangeElement combine(const RangeElement<Y>& r) const {
      return RangeElement(std::min(r_pair.first, r.r_pair.first),
                          std::max(r_pair.second, r.r_pair.second),
                          compressed);
    }
    
    // simple reduce without check
    template <class Y>
    RangeElement reduce(const RangeElement<Y>& r) const {
      if(less(r))
        return RangeElement(std::min(r_pair.first, r.r_pair.second - 1),
                            r_pair.second,
                            compressed);
      else
        return RangeElement(r_pair.first,
                            std::max(r_pair.second, r.r_pair.first + 1),
                            compressed);
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

  // two ranges are not equal
  template <class T, class Y>
  inline bool operator!= (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return !rhs.is_same(lhs);
  }

  // return the overlapped range
  // function does not check the result's validation
  template <class T, class Y>  
  Range operator& (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.overlap(rhs);
  }

  // return the combined range
  // function does not check the result's validation
  template <class T, class Y>  
  Range operator| (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.combine(rhs);
  }

  // return the reduced range
  // function does not check the result's validation
  template <class T, class Y>  
  Range operator- (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.reduce(rhs);
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

    // check whether r is adjacent to this range
    template <class Y>
    bool is_adjacent(const Range<Y>& r) const {
      if(!is_operable(r)) return false;
      for(unsigned int i=0; i<r_array.size(); i++)
        if(r_array[i].first + 1 >= r.r_array[i].second ||
           r_r_array[i].first + 1 >= r_array[i].second)
          return true;
      return rv;
    }
      

    // simple combine without check
    template <class Y>
    Range combine(const Range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
      Range rv(compressed);
      rv.r_array.reserve(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        rv.r_array[i] = r_array[i] | r.r_array[i];
      return rv;
    }

    // simple overlap without check
    template <class Y>
    Range overlap(const Range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
      Range rv(compressed);
      rv.r_array.reserve(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        rv.r_array[i] = r_array[i] & r.r_array[i];
      return rv;
    }

    // simple reduce without check
    template <class Y>
    Range reduce(const Range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
      Range rv(compressed);
      rv.r_array.reserve(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++) {
        if(r_array[i] != r.r_array[i])
          rv.r_array[i] = r_array[i] - r.r_array[i];
        else
          rv.r_array[i] = r_array[i];
      }

      // if same, means this and r are equal, the reduced one is none
      if(rv == *this) 
        return Range();
      else 
        return rv;
    }

    // weak order
    template <class Y>
    bool less(const range<Y>& r) const {
      assert(r_array.size() == r.r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++) {
        if(r_array[i] != r.r_array[i])
          return r_array[i].less(r.r_array[i]);
      }
      return false;
    }

    // check whether the ranges can be compared
    // indicating they have the same number of dimensions
    template <class Y>
    bool is_comparable(const Range<Y>& r) const {
      return r_array.size() == r.r_array.size();
    }
    
    // check whether the range and this range satify the operable condition:
    // only one dimension is not equal
    template <class Y>
    bool is_operable(const Range<Y>& r) const {
      if(r_array.size() != r.r_array.size()) return false;
      unsigned int cnt = 0;     // counting the different dimensions
      for(unsigned int i=0; i<r_array.size(); i++)
        if(r_array[i] != r.r_array[i]) cnt++;
      if(cnt > 1) return false;
      else return true;
    }

    // check whether the range and this range are adjacent
    template <class Y>
    bool is_adjacent(const Range<Y>& r) const {
      if(!is_operable(r)) return false;
      for(unsigned int i=0; i<r_array.size(); i++) {
        if(r_array[i] != r.r_array[i]) {
          if(r_array[i].is_adjacent(r.r_array[i]))
            return true;
          else
            return false;
        }
      }
      
      return true;
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
    if(lhs.is_comparable(rhs))
      return lhs.is_enclosed(rhs) && !lhs.is_same(rhs);
    else 
      return false;
  }

  // rhs range is enclosed in lhs
  template <class T, class Y>
  bool operator>= (const Range<T>& lhs, const Range<Y>& rhs) {
    if(lhs.is_comparable(rhs))
      return lhs.is_enclosed(rhs);
    else 
      return false;
  }

  // lhs range is enclosed in rhs (not equal)
  template <class T, class Y>
  bool operator< (const Range<T>& lhs, const Range<Y>& rhs) {
    if(lhs.is_comparable(rhs)) 
      return rhs.is_enclosed(lhs) && !rhs.is_same(lhs);
    else 
      return false;
  }

  // lhs range is enclosed in rhs
  template <class T, class Y>
  inline bool operator<= (const Range<T>& lhs, const Range<Y>& rhs) {
    if(lhs.is_comparable(rhs)) 
      return rhs.is_enclosed(lhs);
    else 
      return false;
  }
  
  // two ranges are equal
  template <class T, class Y>  
  inline bool operator== (const Range& lhs, const Range& rhs) {
    if(lhs.is_comparable(rhs)) 
      return rhs.is_same(lhs);
    else 
      return false;
  }

  // two ranges are not equal
  template <class T, class Y>  
  inline bool operator!= (const Range& lhs, const Range& rhs) {
    return !(rhs == lhs);
  }

  // return the overlapped range
  template <class T, class Y>  
  Range<T> operator& (const Range<T>& lhs, const Range<Y>& rhs) {
    if(lhs.is_operable(rhs)) 
      return lhs.overlap(rhs);
    else 
      return Range<T>();
  }

  // return the combined range
  template <class T, class Y>
  Range<T> operator| (const Range<T>& lhs, const Range<Y>& rhs) {
    if(lhs.is_adjacent(rhs))
      return lhs.combine(rhs);
    else
      return Range<T>();
  }

  // return the reduced range
  template <class T, class Y>
  Range<T> operator- (const Range<T>& lhs, const Range<Y>& rhs) {
    if(lhs.is_comparable(rhs))
      return lhs.reduce(rhs);
    else
      return Range<T>();
  }

  // return the standard division (high, overlapped, low)
  template <class T, class Y>
  boost::tuple<Range<T>, Range<T>, Range<T> > 
  operator^ (const Range<T>& lhs, const Range<Y>& rhs) {
    boost::tuple<Range<T>, Range<T>, Range<T> > rv;
    if(lhs.is_operable(rhs)) {
      rv.get<1>() = lhs & rhs;
      if(lhs.less(rhs)) {
        rv.get<0>() = rhs - rv.get<1>();
        rv.get<2>() = lhs - rv.get<1>();
      } else {
        rv.get<0>() = lhs - rv.get<1>();
        rv.get<2>() = rhs - rv.get<1>();
      }
    } else if(lhs.is_comparable(rhs)) {
      if(lhs.less(rhs)) {
        rv.get<0>() = rhs;
        rv.get<2>() = lhs;
      } else {
        rv.get<0>() = lhs;
        rv.get<2>() = rhs;
      }
    }
    return rv;
  }

}

#endif
