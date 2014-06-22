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
 * Simple multi-dimensional Range
 * 20/05/2014   Wei Song
 *
 *
 */

#ifndef _CPP_RANGE_MULTI_H_
#define _CPP_RANGE_MULTI_H_

#include <list>
#include <vector>
#include <utility>
#include <ostream>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>

#include "cpp_range_element.hpp"

namespace CppRange {

  ////////////////////////////////////////////////// 
  // Range
  // 
  // multi-dimensional range expression
  //
  //////////////////////////////////////////////////

  template <class T>
  class Range {
  private:
    std::vector<RangeElement<T> > r_array;              // the range array

  public:
    //////////////////////////////////////////////
    // constructors
    
    Range() {}
    explicit Range(const std::list<RangeElement<T> >&); // construct from a list of 
                                                        // RangeElements
    explicit Range(const std::vector<RangeElement<T> >&);
                                                        // construct from a vector of 
                                                        // RangeElements
    explicit Range(const std::list<std::pair<T,T> >&);  // construct from a list of
                                                        // raw range pairs
    explicit Range(const std::vector<std::pair<T,T> >&);
                                                        // construct from a vector of 
                                                        // raw range pairs

    //////////////////////////////////////////////
    // helpers

    // data accesser
    RangeElement<T>& operator[] (unsigned int);         // access a certain dimension
                                                        // start from the highest (left)
    const RangeElement<T>& operator[] (unsigned int) const;

    unsigned int dimension() const;                     // the number of dimensions
    T size() const;                                     // the size of the range
    void add_upper(const RangeElement<T>&);             // add a higher dimension, expensive
                                                        // do not use whenever possible
    void add_lower(const RangeElement<T>&);             // add a lower dimension
    void add_dimension(const RangeElement<T>&, unsigned int pos);
                                                        // add a dimension at position 'pos'
    void remove_upper();                                // remove the highest dimension
    void remove_lower();                                // remove the lowest dimension
    void remove_dimension(unsigned int pos);            // remove a dimension at position 'pos'
    virtual bool empty() const;                         // ? this is an empty range 
    virtual bool subset(const Range&) const;            // ? this is a subset of r
    virtual bool proper_subset( const Range&) const;    // ? this is a proper subset of r
    virtual bool superset(const Range&) const;          // ? this is a superset of r
    virtual bool proper_superset( const Range&) const;  // ? this is a proper superset of r
    virtual bool singleton() const;                     // ? this is a singleton range 
    virtual bool equal(const Range& r) const;           // ? this == r 
    virtual bool connected(const Range& r) const;       // ? this and r are connected
    virtual bool less(const Range& r) const;            // weak order compare
    virtual bool overlap(const Range& r) const;         // this & r != []
    virtual bool disjoint(const Range& r) const;        // this & r == []
    virtual Range combine(const Range& r) const;        // get the union of this and r
    virtual Range hull(const Range& r) const;           // get the minimal superset of the union
                                                        // of this and r
    virtual Range intersection(const Range& r) const;   // get the intersection of this and r
    virtual Range complement(const Range& r) const;     // substract r from this range
    virtual boost::tuple<Range, Range, Range>
    divide(const Range& r) const;                       // standard divide/partition this and r
    
    virtual std::ostream& streamout(std::ostream& os) const;
                                                        // stream out the range

  protected:
    bool comparable(const Range& r) const;              // ? this and r can be compared 
    bool operable(const Range& r) const;                // ? this and r are operable
                                                        // only one dimension is not equal
  };

  /////////////////////////////////////////////
  // implementation of class methods

  // constructors

  // construct from a list of RangeElements
  templat<class T> inline
  explicit Range<T>::Range(const std::list<RangeElement<T> >& l)
    : r_array(std::vector<RangeElement<T> >(l.begin(), l.end())) {} 

  // construct from a vector of RangeElements
  templat<class T> inline
  explicit Range<T>::Range(const std::vector<RangeElement<T> >& l)
    : r_array(l) {}

  // construct from a list of raw range pairs
  templat<class T> inline
  explicit Range<T>::Range(const std::list<std::pair<T,T> >& l) {
    r_array.resize(l.size());
    unsigned int i = 0;
    typedef std::pair<T,T> local_range_pair;
    BOOST_FOREACH(const local_range_pair& r, l)
      r_array[i++]=RangeElement<T>(r.first, r.second);
  }

  // construct from a vector of raw range pairs
  templat<class T> inline
  explicit Range<T>::Range(const std::vector<std::pair<T,T> >& l) {
    r_array.resize(l.size());
    unsigned int i = 0;
    typedef std::pair<T,T> local_range_pair;
    BOOST_FOREACH(const local_range_pair& r, l)
      r_array[i++]=RangeElement<T>(r.first, r.second);   
  }

  //////////////////////////////////////////////
  // data accesser
  templat<class T> inline
  RangeElement<T>& Range<T>::operator[] (unsigned int index) {
    return r_array.at(index);
  }
  
  templat<class T> inline
  const RangeElement<T>& Range<T>::operator[] (unsigned int index) const {
    return r_array.at(index);
  }
  
  //////////////////////////////////////////////
  // Helpers
  
  // size of dimension
  templat<class T> inline
  unsigned int Range<T>::dimension() const {
    return r_array.size();
  }

  // size of bits
  templat<class T> inline
  T Range<T>::size() const {
    if(empty()) return T(0);
    
    T rv(1);
    for(unsigned int i=0; i<r_array.size(); i++)
      rv = rv * r_array[i].size();
    return rv;
  }
  
  // add a higher dimension
  // this method is very expernsive, do not use if possible
  templat<class T> inline
  void Range<T>::add_upper(const RangeElement<T>& r) {
    r_array.insert(r_array.begin(), r);
  }

  // add a lower dimension
  templat<class T> inline
  void Range<T>::add_lower(const RangeElement<T>& r) {
    r_array.push_back(r);
  }

  // add a dimension
  // this method is very expernsive, do not use if possible
  templat<class T> inline
  void Range<T>::add_dimension(const RangeElement<T>& r, unsigned int pos) {
    // ** need to check pos < size ?
    r_array.insert(r_array.begin() + pos, r);
  }
   
  // remove the highest dimension
  // this method is very expernsive, do not use if possible
  templat<class T> inline
  void Range<T>::remove_upper(const RangeElement<T>& r) {
    r_array.erase(r_array.begin(), r);
  }

  // remove the lowest dimension
  templat<class T> inline
  void Range<T>::remove_lower(const RangeElement<T>& r) {
    r_array.pop_back(r);
  }

  // remove a dimension
  // this method is very expernsive, do not use if possible
  templat<class T> inline
  void Range<T>::remove_dimension(const RangeElement<T>& r, unsigned int pos) {
    // ** need to check pos < size ?
    r_array.erase(r_array.begin() + pos, r);
  }

  // check whether the range expression is empty
  templat<class T> inline
  bool Range<T>::empty() const {
    if(r_array.empty()) return true;
    for(unsigned int i=0; i<r_array.size(); i++) 
      if(r_array[i].empty()) return true;
    return false;
  }

  // check whether this is a subset of r
  templat<class T> inline
  bool Range<T>::subset(const Range<T>& r) const {
    if(empty()) return true;
    if(r.empty()) return false;
    if(!comparable()) return false; // or throw an exception

    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].subset(r[i])) 
        return false;
    }
    return true;
  }

  // check whether this is a proper subset of r
  templat<class T> inline
  bool Range<T>::proper_subset(const Range<T>& r) const {
    if(empty()) return !r.empty();
    if(r.empty()) return false;
    if(!comparable(r)) return false; // or throw an exception
    
    bool proper = false;
    for(unsigned int i=0; i<r_array.size(); i++) {
      if(r_array[i].same(r[i])) 
        continue;
      else if(r_array[i].proper_subset(r[i]))
        proper = true;
      else
        return false;
    }
    return proper;
  }

  // check whether this is a superset of r
  templat<class T> inline
  bool Range<T>::superset(const Range<T>& r) const {
    if(r.empty()) return true;
    if(empty()) return false;
    if(!comparable(r)) return false; // or throw an exception

    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].superset(r[i])) 
        return false;
    }
    return true;
  }

  // check whether this is a proper superset of r
  templat<class T> inline
  bool Range<T>::proper_superset(const Range<T>& r) const {
    if(r.empty()) return !empty();
    if(empty()) return false;
    if(!comparable(r)) return false; // or throw an exception

    bool proper = false;
    for(unsigned int i=0; i<r_array.size(); i++) {
      if(r_array[i].same(r[i])) 
        continue;
      else if(r_array[i].proper_superset(r[i]))
        proper = true;
      else
        return false;
    }
    return proper;
  }

  // check whether this range is a singleton range
  template<class T> inline
  bool Range<T>::singleton() const {
    return empty() || size() == min_unit<T>();
  }  
  
  // check whether r is equal with this range
  template<class T> inline
  bool Range<T>::equal(const Range<T>& r) const {
    if(empty()) return r.empty();
    if(!comparable(r)) return false; // or throw an exception

    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].same(r.r_array[i]))
        return false;
    }
    return true;
  }

  // check whether the range and this range are adjacent
  template<class T> inline
  bool Range<T>::connected(const Range<T>& r) const {
    if(empty() || r.empty()) return false;
    if(comparable(r)) return false;
    
    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].same(r.r_array[i])) {
        if(!r_array[i].connected(r.r_array[i]))
          return false;
      }
    }
    return true;
  }    

  // weak order
  template<class T> inline
  bool Range<T>::less(const Range& r) const {
    if(empty()) return !r.empty();
    if(r.empty()) return false;
    if(!comparable(r)) return false; // or throw an exception
    
    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].same(r.r_array[i]))
        return r_array[i].less(r.r_array[i]);
    }
    return false;               // the two ranges are equal
  }

  // whether this and r has non-empty intersection
  template<class T> inline
  bool Range<T>::overlap(const Range<T>& r) const {
    if(empty() || r.empty())  return false;
    if(!comparable()) return false; // or throw an exception

    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].overlap(r.r_array[i]))
        return false;
    }
    return true;
  }

  // whether this and r have no shared range
  template<class T> inline
  bool Range<T>::disjoint(const Range<T>& r) const {
    if(empty() || r.empty())  return true;
    if(!comparable()) return false; // or throw an exception

    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].overlap(r.r_array[i]))
        return true;
    }
    return false;
  }

  // combine two ranges
  template<class T> inline
  Range<T> Range<T>::combine(const Range<T>& r) const {
    if(empty()) return r;
    if(r.empty()) return *this;
    if(!comparable(r)) return Range<T>(); // or throw an exception

    Range<T> rv;
    rv.r_array.resize(r_array.size());
    for(unsigned int i=0; i<r_array.size(); i++)
      rv.r_array[i] = r_array[i].combine(r.r_array[i]);
    return rv;
  }

  // get the minimal range contain the two ranges
  template<class T> inline
  Range<T> Range<T>::hull(const Range<T>& r) const {
    if(empty()) return r;
    if(r.empty()) return *this;
    if(!comparable(r)) return Range<T>(); // or throw an exception

    Range<T> rv;
    rv.r_array.resize(r_array.size());
    for(unsigned int i=0; i<r_array.size(); i++)
      rv.r_array[i] = r_array[i].hull(r.r_array[i]);
    return rv;
  }

  // get the shared range
  template<class T> inline
  Range<T> Range<T>::intersection(const Range<T>& r) const {
    if(empty() || r.empty()) return Range<T>();
    if(!comparable(r)) return Range<T>(); // or throw an exception

    Range<T> rv;
    rv.r_array.resize(r_array.size());
    for(unsigned int i=0; i<r_array.size(); i++)
      rv.r_array[i] = r_array[i].intersection(r.r_array[i]);
    return rv;
  }

  //substraction
  template<class T> inline
  Range<T> Range<T>::complement(const Range<T>& r) const {
    if(empty()) return Range<T>();
    if(r.empty()) return *this;
    if(!operable(r)) return Range<T>(); // or throw an exception

    Range<T> rv(r);
    rv.r_array.resize(r_array.size());
    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].equal(r.r_array[i])) {
        rv.r_array[i] = r_array[i].complement(r.r_array[i]);
        return rv;
      }
    }
    
    return Range<T>();          // the two ranges are equal
  }

  //substraction
  template<class T> inline
  boost::tuple<Range<T>, Range<T>, Range<T> >
  Range<T>::divide(const Range<T>& r) const {
    boost::tuple<Range<T>, Range<T>, Range<T> > rv;
    if(empty() || r.empty()) {
      boost::get<1>(rv) = hull(r);
      return rv;
    }
    
    if(!operable(r)) return rv; // or throw an exception

    boost::get<0>(rv) = r;
    boost::get<1>(rv) = r;
    boost::get<2>(rv) = r;
    
    for(unsigned int i=0; i<r_array.size(); i++) {
      if(!r_array[i].equal(r.r_array[i])) {
        RangeElement<T> rH, rM, rL;
        boost::tie(rH, rM, rL) = r_array[i].divide(r.r_array[i]);
        boost::get<0>(rv).r_array[i] = rH;
        if(rM.empty())
          boost::get<1>(rv) = Range<T>();
        else
          boost::get<1>(rv) = rM;
        boost::get<2>(rv).r_array[i] = rL;
       return rv;
      }
    }

    // the two ranges are equal
    boost::get<0>(rv) = Range<T>();
    boost::get<1>(rv) = r;
    boost::get<2>(rv) = Range<T>();    
  }

  ////////////////////////


    // check whether the ranges can be compared
    // indicating they have the same number of dimensions
    bool is_comparable(const Range& r) const {
      if(!is_valid() || !r.is_valid())
        return false;
      else 
        return r_array.size() == r.r_array.size();
    }
    
    // check whether the range and this range satify the operable condition:
    // only one dimension is not equal
    bool is_operable(const Range& r) const {
      if(!is_valid() || !r.is_valid())
        return false;
      
      if(r_array.size() != r.r_array.size()) return false;
      unsigned int cnt = 0;     // counting the different dimensions
      for(unsigned int i=0; i<r_array.size(); i++)
        if(r_array[i] != r.r_array[i]) cnt++;
      if(cnt > 1) return false;
      else return true;
    }

    // stream out function
    std::ostream& streamout(std::ostream& os) const{
      if(is_valid()) {
        for(unsigned int i=0; i<r_array.size(); i++)
          os << r_array[i];
      } else {
        os << "[]";
      }
      return os;
    }

  };

  /////////////////////////////////////////////
  // overload operators

  // rhs range is enclosed in lhs (not equal)
  template <class T>
  bool operator> (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_comparable(rhs))
      return lhs.is_enclosed(rhs) && !lhs.is_same(rhs);
    else 
      return false;
  }

  // rhs range is enclosed in lhs
  template <class T>
  bool operator>= (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_comparable(rhs))
      return lhs.is_enclosed(rhs);
    else 
      return false;
  }

  // lhs range is enclosed in rhs (not equal)
  template <class T>
  bool operator< (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_comparable(rhs)) 
      return rhs.is_enclosed(lhs) && !rhs.is_same(lhs);
    else 
      return false;
  }

  // lhs range is enclosed in rhs
  template <class T>
  inline bool operator<= (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_comparable(rhs)) 
      return rhs.is_enclosed(lhs);
    else 
      return false;
  }
  
  // two ranges are equal
  template <class T>  
  bool operator== (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_comparable(rhs)) 
      return rhs.is_same(lhs);
    else 
      return false;
  }

  // two ranges are not equal
  template <class T>  
  bool operator!= (const Range<T>& lhs, const Range<T>& rhs) {
    return !(rhs == lhs);
  }

  // return the overlapped range
  template <class T>  
  Range<T> operator& (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_comparable(rhs)) 
      return lhs.overlap(rhs);
    else 
      return Range<T>();
  }

  // return the combined range
  template <class T>
  Range<T> operator| (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_adjacent(rhs))
      return lhs.combine(rhs);
    else
      return Range<T>();
  }

  // return the reduced range
  template <class T>
  Range<T> operator- (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_operable(rhs))
      return lhs.reduce(rhs);
    else
      return Range<T>();
  }

  // return the standard division (high, overlapped, low)
  template <class T>
  boost::tuple<Range<T>, Range<T>, Range<T> > 
  operator^ (const Range<T>& lhs, const Range<T>& rhs) {
    if(lhs.is_adjacent(rhs))
      return lhs.divideBy(rhs);
    else
      return boost::tuple<Range<T>, Range<T>, Range<T> >();
  }

  // standard out stream
  template<class T>
  std::ostream& operator<< (std::ostream& os, const Range<T>& r) {
    return r.streamout(os);
  }

}

#endif
