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
 * RangeElement
 * 20/05/2014   Wei Song
 *
 *
 */

#ifndef _CPP_RANGE_ELEMENT_H_
#define _CPP_RANGE_ELEMENT_H_

#include <utility>
#include <ostream>
#include <algorithm>

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
  private:
    bool compressed;            // whether to compress single bit range when streamout
    bool invalid;               // set to true when constructed in default, but cleared whenever modified
    std::pair<T, T> r_pair;     // specific range expression
  public:
    //////////////////////////////////////////////
    // data accessers
    T& first() { invalid = false; return r_pair.first;}
    const T& first() const { return r_pair.first;}
    T& second() { invalid = false; return r_pair.second;}
    const T& second() const { return r_pair.second;}

    //////////////////////////////////////////////
    // constructors

    // default to construct an range with undefined value
    RangeElement()
      : compressed(false), invalid(true) {}

    // single bit range
    RangeElement(const T& r, bool compress = true)
      : compressed(compress), invalid(false), r_pair(r, r) {}

    // bit range
    template<class Y>
    RangeElement(const T& rh, const Y& rl, bool compress = true)
      : compressed(compress), invalid(false), r_pair(rh, rl) {}

    // type conversion
    template <class Y>
    RangeElement(const RangeElement<Y>& r)
      : compressed(r.compressed), invalid(r.invalid), r_pair(r.r_pair) {}

    //////////////////////////////////////////////
    // Helpers

    // set compressed
    void set_compress(bool b) { compressed = b; }

    // size of bit
    T size() const {
      if(is_valid())
        return r_pair.first - r_pair.second + 1;
      else
        return 0;
    } 

    // valid range expression
    bool is_valid() const {
      return !invalid && !(r_pair.first < r_pair.second);
    }

    // check whether range r is enclosed in this range 
    template <class Y>
    bool is_enclosed(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) return false;
      else
        return (
                !(r_pair.first < r.r_pair.first) && 
                !(r.r_pair.second < r_pair.second)
                );      
    }

    // check whether range r is equal with this range
    template <class Y>
    bool is_same(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) return false;
      else
        return (
                r_pair.first == r.r_pair.first &&
                r_pair.second == r.r_pair.second
                );
    }

    // check whether r is adjacent to this range
    template <class Y>
    bool is_adjacent(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) return false;
      else
        return (!(r_pair.first + 1 < r.r_pair.second) && !(r.r_pair.first + 1 < r_pair.second));
    }
      
    // weak order
    template <class Y>
    bool less(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) 
        return false;
      else if(r_pair.first < r.r_pair.first)
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
      if(!is_valid() || !r.is_valid()) 
        return false;
      else 
        return (!(r_pair.first < r.r_pair.second) && !(r.r_pair.first < r_pair.second));
    }

    // simple combine without check
    template <class Y>
    RangeElement combine(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) 
        return RangeElement();
      else 
        return RangeElement(std::max(r_pair.first, r.r_pair.first),
                            std::min(r_pair.second, r.r_pair.second),
                            compressed);
    }

    // simple overlap without check
    template <class Y>
    RangeElement overlap(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) 
        return RangeElement();
      else 
        return RangeElement(std::min(r_pair.first, r.r_pair.first),
                            std::max(r_pair.second, r.r_pair.second),
                            compressed);
    }
    
    // simple reduce without check
    template <class Y>
    RangeElement reduce(const RangeElement<Y>& r) const {
      if(!is_valid() || !r.is_valid()) 
        return RangeElement();
      
      RangeElement rv(*this);
      if(!(r.r_pair.second > r_pair.first) && (r_pair.second < r.r_pair.second))
        rv.r_pair.first = r.r_pair.second - 1;
      if(!(r.r_pair.first < r_pair.second) && (r_pair.first > r.r_pair.first))
        rv.r_pair.second = r.r_pair.first + 1;
       return rv;
    }

    // normalize divide
    template <class Y>
    boost::tuple<RangeElement, RangeElement, RangeElement> 
    divideBy(const RangeElement<Y>& r) const {
      boost::tuple<RangeElement, RangeElement, RangeElement> rv;
      if(!is_valid() || !r.is_valid()) 
        return rv;

      RangeElement RAnd = overlap(r);
      RangeElement ROr = combine(r);
      RangeElement RMinus = ROr - RAnd;
      boost::get<0>(rv) = RangeElement(ROr.first(), RMinus.second(), compressed);
      boost::get<1>(rv) = RAnd;
      boost::get<2>(rv) = RangeElement(RMinus.first(), ROr.second(), compressed);
      return rv;
    }

    // stream out function
    std::ostream& streamout(std::ostream& os) const{
      os << "[";
      if(is_valid()) {
        if(compressed && r_pair.first == r_pair.second)
          os << r_pair.first;
        else 
          os << r_pair.first << ":" << r_pair.second;
      }
      os << "]";
      return os;
    }

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
  RangeElement<T> operator& (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.overlap(rhs);
  }

  // return the combined range
  // function does not check the result's validation
  template <class T, class Y>  
  RangeElement<T> operator| (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.combine(rhs);
  }

  // return the reduced range
  // function does not check the result's validation
  template <class T, class Y>  
  RangeElement<T> operator- (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.reduce(rhs);
  }

  // return the standard division (high, overlapped, low)
  template <class T, class Y>
  boost::tuple<RangeElement<T>, RangeElement<T>, RangeElement<T> > 
  operator^ (const RangeElement<T>& lhs, const RangeElement<Y>& rhs) {
    return lhs.divideBy(rhs);
  }

  // standard out stream
  template<class T>
  std::ostream& operator<< (std::ostream& os, const RangeElement<T>& r) {
    return r.streamout(os);
  }

}


#endif
