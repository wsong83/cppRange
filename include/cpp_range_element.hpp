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
#include <boost/tuple/tuple.hpp>

#include "cpp_range_define.hpp"

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
    RangeElement(const T& rh, const T& rl, bool compress = true)
      : compressed(compress), invalid(false), r_pair(rh, rl) {}

    // type conversion
    RangeElement(const RangeElement& r)
      : compressed(r.compressed), invalid(r.invalid), r_pair(r.r_pair) {}

    //////////////////////////////////////////////
    // Helpers

    // set compressed
    virtual void set_compress(bool b) { compressed = b; }

    // size of bit
    virtual T size_bit() const {
      if(is_valid())
        return r_pair.first - r_pair.second + min_unit<T>();
      else
        return 0;
    } 

    // valid range expression
    virtual bool is_valid() const {
      return !invalid && !(r_pair.first < r_pair.second);
    }

    // check whether range r is enclosed in this range 
    virtual bool is_enclosed(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) return false;
      else
        return (
                !(r_pair.first < r.r_pair.first) && 
                !(r.r_pair.second < r_pair.second)
                );      
    }

    // check whether range r is equal with this range 
   virtual bool is_same(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) return false;
      else
        return (
                r_pair.first == r.r_pair.first &&
                r_pair.second == r.r_pair.second
                );
    }

    // check whether r is adjacent to this range
    virtual bool is_adjacent(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) return false;
      else
        return (!(r_pair.first + min_unit<T>() < r.r_pair.second) && !(r.r_pair.first + min_unit<T>() < r_pair.second));
    }
      
    // weak order
    virtual bool less(const RangeElement& r) const {
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
    virtual bool is_overlapped(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) 
        return false;
      else 
        return (!(r_pair.first < r.r_pair.second) && !(r.r_pair.first < r_pair.second));
    }

    // simple combine without check
    virtual RangeElement combine(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) 
        return RangeElement();
      else 
        return RangeElement(std::max(r_pair.first, r.r_pair.first),
                            std::min(r_pair.second, r.r_pair.second),
                            compressed);
    }

    // simple overlap without check
    virtual RangeElement overlap(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) 
        return RangeElement();
      else 
        return RangeElement(std::min(r_pair.first, r.r_pair.first),
                            std::max(r_pair.second, r.r_pair.second),
                            compressed);
    }
    
    // simple reduce without check
    virtual RangeElement reduce(const RangeElement& r) const {
      if(!is_valid() || !r.is_valid()) 
        return RangeElement();
      
      RangeElement rv(*this);
      if(!(r.r_pair.second > r_pair.first) && (r_pair.second < r.r_pair.second))
        rv.r_pair.first = r.r_pair.second - min_unit<T>();
      if(!(r.r_pair.first < r_pair.second) && (r_pair.first > r.r_pair.first))
        rv.r_pair.second = r.r_pair.first + min_unit<T>();
       return rv;
    }

    // normalize divide
    virtual boost::tuple<RangeElement, RangeElement, RangeElement> 
    divideBy(const RangeElement& r) const {
      boost::tuple<RangeElement, RangeElement, RangeElement> rv;
      if(!is_valid() || !r.is_valid()) 
        return rv;

      RangeElement RAnd = overlap(r);
      if(RAnd.is_valid()) {
        RangeElement ROr = combine(r);
        RangeElement RMinus = ROr - RAnd;
        boost::get<0>(rv) = RangeElement(ROr.first(), RMinus.second(), compressed);
        boost::get<1>(rv) = RAnd;
        boost::get<2>(rv) = RangeElement(RMinus.first(), ROr.second(), compressed);
      } else {
        if(less(r)) {
          boost::get<0>(rv) = RangeElement(r.first(), r.second(), compressed);
          boost::get<2>(rv) = *this;
        } else {
          boost::get<2>(rv) = RangeElement(r.first(), r.second(), compressed);
          boost::get<0>(rv) = *this;          
        }
      }
      return rv;
    }

    // stream out function
    virtual std::ostream& streamout(std::ostream& os) const{
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
  template <class T>
  bool operator> (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.is_enclosed(rhs) && !lhs.is_same(rhs);
  }

  // rhs range is enclosed in lhs
  template <class T>
  bool operator>= (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.is_enclosed(rhs);
  }

  // lhs range is enclosed in rhs (not equal)
  template <class T>
  bool operator< (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return rhs.is_enclosed(lhs) && !rhs.is_same(lhs);
  }

  // lhs range is enclosed in rhs
  template <class T>
  bool operator<= (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return rhs.is_enclosed(lhs);
  }
  
  // two ranges are equal
  template <class T>
  inline bool operator== (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return rhs.is_same(lhs);
  }

  // two ranges are not equal
  template <class T>
  inline bool operator!= (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return !rhs.is_same(lhs);
  }

  // return the overlapped range
  // function does not check the result's validation
  template <class T>  
  RangeElement<T> operator& (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.overlap(rhs);
  }

  // return the combined range
  // function does not check the result's validation
  template <class T>  
  RangeElement<T> operator| (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.combine(rhs);
  }

  // return the reduced range
  // function does not check the result's validation
  template <class T>  
  RangeElement<T> operator- (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.reduce(rhs);
  }

  // return the standard division (high, overlapped, low)
  template <class T>
  boost::tuple<RangeElement<T>, RangeElement<T>, RangeElement<T> > 
  operator^ (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.divideBy(rhs);
  }

  // standard out stream
  template<class T>
  std::ostream& operator<< (std::ostream& os, const RangeElement<T>& r) {
    return r.streamout(os);
  }

}


#endif
