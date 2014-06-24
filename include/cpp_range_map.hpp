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

    RangeMap();
    RangeMap(const T&);                                 // single bit range 
    RangeMap(const T&, const T&);                       // one dimensional range
    explicit RangeMap(const RangeElement<T>&);          // convert a RangeElement 
    explicit RangeMap(const Range<T>&);                 // convert a multidimensional range
    explicit RangeMap(const std::list<RangeElement<T> >&);
                                                        // build from a list of 
                                                        // RangeElement
    explicit RangeMap(const std::list<Range<T> >&);
                                                        // build from a list of 
                                                        // multidimensional range
  private:
    explicit RangeMap(const std::list<RangeMapBase<T> >&);
                                                        // build from a list of 
                                                        // RangeMapBase objects
                                                        // internal use only

  public:
    //////////////////////////////////////////////
    // Helpers

    virtual unsigned int dimension() const;             // the number of dimensions
    virtual T size() const;                             // the size of the range
    virtual bool valid() const;                         // ? this is a valid range
    virtual bool empty() const;                         // ? this is an empty range 
    virtual bool subset(const RangeMap&) const;         // ? this is a subset of r
    virtual bool proper_subset(const RangeMap&) const;  // ? this is a proper subset of r
    virtual bool superset(const RangeMap&) const;       // ? this is a superset of r
    virtual bool proper_superset( const RangeMap&) const;  
                                                        // ? this is a proper superset of r
    virtual bool singleton() const;                     // ? this is a singleton range 
    virtual bool equal(const RangeMap& r) const;        // ? this == r 
    virtual bool overlap(const RangeMap& r) const;      // this & r != []
    virtual bool disjoint(const RangeMap& r) const;     // this & r == []
    virtual RangeMap combine(const RangeMap& r) const;  // get the union of this and r
    virtual RangeMap intersection(const RangeMap& r) const;
                                                        // get the intersection of this and r
    virtual RangeMap complement(const RangeMap& r) const;
                                                        // subtract r from this range
    
    virtual std::ostream& streamout(std::ostream& os) const;
                                                        // stream out the range

  protected:
    virtual bool comparable(const RangeMap& r) const;      // ? this and r can be compared 

  };

  /////////////////////////////////////////////
  // implementation of class methods

  // constructors
  
  // default to construct an range with undefined value
  template<class T> inline
  RangeMap<T>::RangeMap() : level(0) {}

  // single bit range
  template<class T> inline
  RangeMap<T>::RangeMap(const T& r) 
    : level(1) {
    child.push_back(RangeMapBase<T>(r));
  }

  // bit range
  template<class T> inline
  RangeMap<T>::RangeMap(const T& rh, const T& rl) 
    : level(1) {
    child.push_back(RangeMapBase<T>(rh, rl));
  }

  // type conversion
  template<class T> inline
  RangeMap<T>::RangeMap(const RangeElement<T>& r) 
    : level(1) {
    child.push_back(RangeMapBase<T>(r));
  }

  // type conversion
  template<class T> inline
  RangeMap<T>::RangeMap(const Range<T>& r)
    : level(r.dimension()) {
    child.push_back(RangeMapBase<T>(r));
  }
  
  // combined build
  template<class T> inline
  RangeMap<T>::RangeMap(const std::list<RangeElement<T> >& rlist)  
    : level(0) {
    BOOST_FOREACH(const RangeElement<T>& r, rlist)
      RangeMapBase<T>::add_child(RangeMapBase<T>(r));
    if(!child.empty()) level = child.front().dimension();   
  }

  // combined build
  template<class T> inline
  RangeMap<T>::RangeMap(const std::list<Range<T> >& rlist)  
    : level(0) {
    BOOST_FOREACH(const Range<T>& r, rlist)
      RangeMapBase<T>::add_child(child, RangeMapBase<T>(r));
    if(!child.empty()) level = child.front().dimension();
  }

  // internal use, combined build
  // no check at all
  template<class T> inline
  RangeMap<T>::RangeMap(const std::list<RangeMapBase<T> >& rlist)  
    : level(0), child(rlist) {
    if(!child.empty()) level = child.front().dimension();
  }
 


  //////////////////////////////////////////////
  // Helpers
  
  // get the dimension of this range
  template<class T> inline
  unsigned int RangeMap<T>::dimension() const {
    return level;
  }

  // get the bit size of this range
  template<class T> inline
  T RangeMap<T>::size() const {
    if(!valid()) return T(0);  
    return RangeMapBase<T>::size(child);
  }
  
  // valid range expression
  template<class T> inline
  bool RangeMap<T>::valid() const {
    return RangeMapBase<T>::valid(child);
  }

  // check whether the range is empty
  template<class T> inline
  bool RangeMap<T>::empty() const {
    if(!valid()) return true;
    return RangeMapBase<T>::empty(child);
  }
  
  // check this is a subset of r
  template<class T> inline
  bool RangeMap<T>::subset(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(empty()) return true;
    if(r.empty()) return false;
    if(!comparable()) return false; // should throw an exception
    return RangeMapBase<T>::subset(child, r.child);
  }

  // check this is a proper subset of r
  template<class T> inline
  bool RangeMap<T>::proper_subset(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(empty()) return !r.empty();
    if(r.empty()) return false;
    if(!comparable()) return false; // should throw an exception
    return RangeMapBase<T>::subset(child, r.child) && !equal(r);
  }

  // check this is a superset of r
  template<class T> inline
  bool RangeMap<T>::superset(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(r.empty()) return true;
    if(empty()) return false;
    if(!comparable()) return false; // should throw an exception
    return RangeMapBase<T>::subset(r.child, child);
  }

  // check this is a proper superset of r
  template<class T> inline
  bool RangeMap<T>::proper_superset(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(r.empty()) return !empty();
    if(empty()) return false;
    if(!comparable()) return false; // should throw an exception
    return RangeMapBase<T>::subset(r.child, child) && !equal(r);
  }

  // check this is a singleton
  template<class T> inline
  bool RangeMap<T>::singleton() const {
    if(!valid()) return false;
    return empty() || size() == min_unit<T>();
  } 

  // check whether range r is equal with this range
  template<class T> inline
  bool RangeMap<T>::equal(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(empty()) return r.empty();
    if(!comparable()) return false; // should throw an exception
    return RangeMapBase<T>::equal(child, r.child);
  }

  // check whether r has shared range with this range
  template<class T> inline
  bool RangeMap<T>::overlap(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(empty() || r.empty()) return false;
    if(!comparable()) return false; // should throw an exception
    return !intersection(r).empty(); // if A&B != []; then A and B are overlapped
  }

  // check whether r is disjoint with this range
  template<class T> inline
  bool RangeMap<T>::disjoint(const RangeMap& r) const {
    if(!valid() || !r.valid()) return false;
    if(empty() || r.empty()) return true;
    if(!comparable()) return false; // should throw an exception
    return intersection(r).empty(); // if A&B == []; then A and B are disjoint
  }

  // combine two ranges
  template<class T> inline
  RangeMap<T> RangeMap<T>::combine(const RangeMap& r) const {
    if(!valid() || !r.valid()) return RangeMap();
    if(empty()) return r;
    if(r.empty()) return *this;
    if(!comparable()) return RangeMap(); // should throw an exception
    return RangeMap(RangeMapBase<T>::combine(child, r.child));
  }

  // get the shared range of two ranges
  template<class T> inline
  RangeMap<T> RangeMap<T>::intersection(const RangeMap& r) const {
    if(!valid() || !r.valid()) return RangeMap();
    if(empty()) return r;
    if(r.empty()) return *this;
    if(!comparable()) return RangeMap(); // should throw an exception
    return RangeMap(RangeMapBase<T>::intersection(child, r.child));
  }

  // this deducted by r
  template<class T> inline
  RangeMap<T> RangeMap<T>::complement(const RangeMap& r) const {
    if(!valid() || !r.valid()) return RangeMap();
    if(empty()) return RangeMap();
    if(r.empty()) return *this;
    if(!comparable()) return RangeMap(); // should throw an exception
    return RangeMap(RangeMapBase<T>::complement(child, r.child));
  }

  // stream out function
  template<class T> inline
  std::ostream& RangeMap<T>::streamout(std::ostream& os) const{
    if(!child.empty()) {
      if(child.size() > 1) {  // more than one sub-ranges
        for(typename std::list<RangeElement<T> >::const_iterator it = child.begin();
            it != child.end(); ) {
          os << *it;
          ++it;
          if(it != child.end()) os << ";";
        }
      } else {                // only one sub-range
        os << child.front();
      }
    } else                    // empty
      os << "[]";
    return os;
  }

  /////////////////////////////////////////////
  // protected help functions
  template<class T> inline
  bool RangeMap<T>::comparable(const RangeMap& r) const {
    // assume both ranges are valid
    return level == r.level;
  }


  /////////////////////////////////////////////
  // overload operators

  // rhs range is less than lhs
  template <class T>
  bool operator> (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return rhs.less(lhs);
  }

  // rhs range is less than or equal to lhs
  template <class T>
  bool operator>= (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return rhs.less(lhs) || lhs.equal(rhs);
  }

  // lhs range is larger than rhs
  template <class T>
  bool operator< (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return lhs.less(rhs);
  }

  // lhs range is larger than or equal to rhs
  template <class T>
  bool operator<= (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return lhs.less(rhs) || lhs.equal(rhs);
  }
  
  // two ranges are equal
  template <class T>
  inline bool operator== (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return rhs.equal(lhs);
  }

  // two ranges are not equal
  template <class T>
  inline bool operator!= (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return !rhs.equal(lhs);
  }

  // return the overlapped range
  // function does not check the result's validation
  template <class T>  
  RangeMap<T> operator& (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return lhs.intersection(rhs);
  }

  // return the combined range
  // function does not check the result's validation
  template <class T>  
  RangeMap<T> operator| (const RangeMap<T>& lhs, const RangeMap<T>& rhs) {
    if(!lhs.comparable(rhs)) return false; // or throw an exception
    return lhs.combine(rhs);
  }
  
  // standard out stream
  template<class T>
  std::ostream& operator<< (std::ostream& os, const RangeMap<T>& r) {
    return r.streamout(os);
  }

}


#endif
