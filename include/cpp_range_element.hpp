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
    bool invalid;               // set to true when constructed in default, but cleared whenever modified
    std::pair<T, T> r_pair;     // specific range expression
  public:
    //////////////////////////////////////////////
    // data accessers
    T& upper() { invalid = false; return r_pair.first;}
    const T& upper() const { return r_pair.first;}
    T& lower() { invalid = false; return r_pair.second;}
    const T& lower() const { return r_pair.second;}

    //////////////////////////////////////////////
    // constructors

    // default to construct an range with undefined value
    RangeElement()
      : invalid(true) {}

    // single bit range
    RangeElement(const T& r)
      : invalid(false), r_pair(r, r) {}

    // bit range
    RangeElement(const T& rh, const T& rl)
      : invalid(false), r_pair(rh, rl) {}

    // type conversion
    RangeElement(const RangeElement& r)
      : invalid(r.invalid), r_pair(r.r_pair) {}

    //////////////////////////////////////////////
    // Helpers

    virtual T size() const;                             // get the size of the range
    virtual bool empty() const;                         // ? this is empty
    virtual bool in(T num) const;                       // ? num belongs to this range
    virtual bool subset(const RangeElement& r) const;   // ? this is a subset of r
    virtual bool proper_subset(const RangeElement& r) const;
                                                        // ? this is a proper subset of r
    virtual bool superset(const RangeElement& r) const; // ? this is a superset of r
    virtual bool proper_superset(const RangeElement& r) const;
                                                        // ? this is a proper superset of r
    virtual bool singleton() const;                     // ? this is a singleton range 
    virtual bool equal(const RangeElement& r) const;    // ? this == r 
    virtual bool connected(const RangeElement& r) const;
                                                        // ? this and r are connected
    virtual bool less(const RangeElement& r) const;     // weak order compare
    virtual bool overlap(const RangeElement& r) const;  // this & r != []
    virtual bool disjoint(const RangeElement& r) const; // this & r == []
    virtual RangeElement combine(const RangeElement& r) const;  
                                                        // get the union of this and r
    virtual RangeElement hull(const RangeElement& r) const;
                                                        // get the minimal superset of the union
                                                        // of this and r
    virtual RangeElement intersection(const RangeElement& r) const;
                                                        // get the intersection of this and r
    virtual RangeElement complement(const RangeElement& r) const;
                                                        // substract r from this range
    virtual boost::tuple<RangeElement, RangeElement, RangeElement>
    divide(const RangeElement& r) const;                // standard divide/partition this and r
    
    
    // stream out function
    virtual std::ostream& streamout(std::ostream& os) const{
      os << "[";
      if(is_valid()) {
        if(r_pair.first == r_pair.second)
          os << r_pair.first;
        else 
          os << r_pair.first << ":" << r_pair.second;
      }
      os << "]";
      return os;
    }

  };

  /////////////////////////////////////////////
  // implementation of class methods

  // get the size of the range
  template<class T> inline
  T RangeElement<T>::size() const {
    T bsize(r_pair.first - r_pair.second + min_unit<T>());
    return !invalid && bsize > T(0) ? bsize : T(0);
  }
    
  // check whether the range is empty
    template<class T> inline
  bool RangeElement<T>::empty() const {
    return size_bit() == T(0);
  }

  // check whether a number is in this range
  template<class T> inline
  bool RangeElement<T>::in(T num) const {
    return !empty() && !(num > upper) && !(num < lower());
  }

  // check this is a subset of r 
  template<class T> inline
  bool RangeElement<T>::subset(const RangeElement<T>& r) const {
    if(empty())        return true;
    else if(r.empty()) return false;
    else               return !(r.upper() < upper()) && !(lower() < r.lower());      
  }

  // check this is a proper subset of r 
  template<class T> inline
  bool RangeElement<T>::proper_subset(const RangeElement<T>& r) const {
    if(empty())        return !r.empty();
    else if(r.empty()) return false;
    else               return (r.upper() > upper()) && (lower() > r.lower());      
  }

  // check this is a superset of r
  template<class T> inline
  bool RangeElement<T>::superset(const RangeElement<T>& r) const {
    if(r.empty())    return true;
    else if(empty()) return false;
    else             return !(upper() < r.upper()) && !(r.lower() < lower());      
  }

  // check this is a proper superset of r 
  template<class T> inline
  bool RangeElement<T>::proper_superset(const RangeElement<T>& r) const {
    if(r.empty())    return !empty();
    else if(empty()) return false;
    else             return (upper() > r.upper()) && (r.lower() > lower());      
  }

  // check whether this range is a singleton range
  template<class T> inline
  bool RangeElement<T>::singleton() const {
    return empty() || size() == min_unit<T>();
  }  

  // check whether this is equal with r
  template<class T> inline
  bool RangeElement<T>::equal(const RangeElement<T>& r) const {
    if(empty())        return r.empty();
    else if(r.empty()) return false;
    else               return (upper() == r.upper()) && (r.lower() == lower());
  }

  // check whether r is adjacent to this range
  template<class T> inline
  bool RangeElement<T>:: connected(const RangeElement<T>& r) const {
    if(empty() || r.empty()) return false;
    else return 
           (!(upper() + min_unit<T>() < r.lower()) 
            && !(r.upper() + min_unit<T>() < lower()));
  }

  // weak order
  template<class T> inline
  bool RangeElement<T>::less(const RangeElement<T>& r) const {
    if(empty() || r.empty())          return false;
    else if(upper() < r.upper())      return true;
    else if(upper() > r.upper())      return false;
    else if(lower() > r.lower())      return true;
    else if(lower() < r.lower())      return false;
    else                              return false;
  }
  
  // check whether r has a shared range with this range
  template<class T> inline
  bool RangeElement<T>::overlap(const RangeElement<T>& r) const {
    if(empty() || r.empty()) return false;
    else                     return !(upper() < r.lower()) && !(r.upper() < lower());
  }

  // check whether r does not have a shared range with this range
  template<class T> inline
  bool RangeElement<T>::disjoint(const RangeElement<T>& r) const {
    if(empty() || r.empty()) return true;
    else                     return (upper() < r.lower()) || (r.upper() < lower());
  }
  
  // union to ranges
  template<class T> inline
  RangeElement<T> RangeElement<T>::combine(const RangeElement<T>& r) const {
    if(empty())            return r;
    else if(r.empty())     return *this;
    else if(connected(r))  return hull(r); 
    else                   return RangeElement<T>(); // or throw an exception
  }
  
  // the minimal range to contain both this and r
  template<class T> inline
  RangeElement<T> RangeElement<T>::hull(const RangeElement<T>& r) const {
    if(empty())            return r;
    else if(r.empty())     return *this;
    else 
      return RangeElement<T>(std::max(upper(), r.upper()),
                             std::min(lower(), r.lower()));
  }
  
  // intersection
  template<class T> inline
  RangeElement<T> RangeElement<T>::intersection(const RangeElement<T>& r) const {
    if(empty() || r.empty()) return RangeElement<T>();
    else 
      return RangeElement<T>(std::min(upper(), r.upper()),
                             std::max(lower(), r.lower()));
  }

  // abstraction
  template<class T> inline
  RangeElement<T> RangeElement<T>::complement(const RangeElement<T>& r) const {
    if(empty())        return RangeElement<T>();
    else if(r.empty()) return *this;
    
    RangeElement<T> RAnd = intersection(r);
    if(RAnd.empty())     return *this;
    else if(equal(RAnd)) return RangeElement<T>();
    else {
      RangeElement<T> rv(*this);
      if(upper() != RAnd.upper())
        rv.lower() = RAnd.upper() + min_unit<T>();
      if(lower() != RAnd.lower())
        rv.upper() = RAnd.lower() - min_unit<T>();
      return rv;
    }
  }

  // normalize divide
  template<class T> inline
  boost::tuple<RangeElement<T>, RangeElement<T>, RangeElement<T> > 
  RangeElement<T>::divide(const RangeElement<T>& r) const {
    boost::tuple<RangeElement<T>, RangeElement<T>, RangeElement<T> > rv;
    if(empty() || r.empty()) {
      boost::get<1>(rv) = hull(r);
      return rv;
    }

    RangeElement<T> RAnd = intersection(r);
    if(RAnd.empty()) {
      if(less(r)) {
        boost::get<0>(rv) = r;
        boost::get<2>(rv) = *this;
      } else {
        boost::get<2>(rv) = r;
        boost::get<0>(rv) = *this;          
      }
    } else {
      RangeElement<T> ROr = hull(r);
      boost::get<0>(rv) = RangeElement<T>(ROr.upper(), RAnd.upper() +  min_unit<T>());
      boost::get<1>(rv) = RAnd;
      boost::get<2>(rv) = RangeElement<T>(RAnd.lower() -  min_unit<T>(), ROr.lower());
    }
    return rv;
  }



  /////////////////////////////////////////////
  // overload operators

  // rhs range is enclosed in lhs (not equal)
  template <class T>
  bool operator> (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return rhs.less(lhs);
  }

  // rhs range is enclosed in lhs
  template <class T>
  bool operator>= (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return rhs.less(lhs) || lhs.is_same(rhs);
  }

  // lhs range is enclosed in rhs (not equal)
  template <class T>
  bool operator< (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.less(rhs);
  }

  // lhs range is enclosed in rhs
  template <class T>
  bool operator<= (const RangeElement<T>& lhs, const RangeElement<T>& rhs) {
    return lhs.less(rhs) || lhs.is_same(rhs);
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
