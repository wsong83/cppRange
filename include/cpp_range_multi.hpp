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
    bool compressed;                       // whether to compress single bit range when streamout
    std::vector<RangeElement<T> > r_array; // the range array

  public:
    //////////////////////////////////////////////
    // constructors
    
    // default constructor
    Range() : compressed(true) {}

    // Range with configuration
    Range(bool compress) : compressed(compress) {}

    // type convert from different ranges
    Range(const Range& r)
      : compressed(r.compressed) {
      r_array.resize(r.r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        r_array[i] = r.r_array[i];
    }

    // construct from a list of RangeElements
    Range(const std::list<RangeElement<T> >& l, bool compress = true) 
      : compressed(compress) {
      r_array.resize(l.size());
      unsigned int i = 0;
      BOOST_FOREACH(const RangeElement<T>& r, l)
        r_array[i++]=r;
    }
    // construct from a list of RangeElements
    Range(const std::vector<RangeElement<T> >& l, bool compress = true)
      : compressed(compress), r_array(l) {}

    // construct from a list of value pairs
    Range(const std::list<std::pair<T,T> >& l, bool compress = true) 
      : compressed(compress) {
      r_array.resize(l.size());
      unsigned int i = 0;
      typedef std::pair<T,T> local_range_pair;
      BOOST_FOREACH(const local_range_pair& r, l)
        r_array[i++]=RangeElement<T>(r.first, r.second, compress);
    }

    // construct from a list of RangeElements
    Range(const std::vector<std::pair<T,T> >& l, bool compress = true)
      : compressed(compress) {
      r_array.resize(l.size());
      unsigned int i = 0;
      typedef std::pair<T,T> local_range_pair;
      BOOST_FOREACH(const local_range_pair& r, l)
        r_array[i++]=RangeElement<T>(r.first, r.second, compress);   
    }

    //////////////////////////////////////////////
    // accesser
    RangeElement<T>& operator[] (unsigned int index) {
      return r_array.at(index);
    }

    const RangeElement<T>& operator[] (unsigned int index) const {
      return r_array.at(index);
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
      if(!is_valid()) return T(0);

      T rv(1);
      for(unsigned int i=0; i<r_array.size(); i++)
        rv = rv * r_array[i].size();
      return rv;
    }

    // add a higher dimension
    // this method is very expernsive, do not use if possible
    void push_front(const RangeElement<T>& r) {
      r_array.insert(0, r);
      r_array[0].set_compress(compressed);
    }

    // add a lower dimension
    void push_back(const RangeElement<T>& r) {
      r_array.push_back(r);
      r_array[r_array.size()-1].set_compress(compressed);
    }

    void pop_back() {
      r_array.pop_back();
    }

    // check whether the range expression is valid
    bool is_valid() const {
      if(r_array.empty()) return false;
      for(unsigned int i=0; i<r_array.size(); i++) 
        if(!r_array[i].is_valid()) return false;
      return true;
    }

    // check whether range r is enclosed in this range 
    bool is_enclosed(const Range& r) const {
      if(!is_valid() || !r.is_valid())
        return false;
      if(r_array.size() != r.r_array.size()) 
        return false;
      for(unsigned int i=0; i<r_array.size(); i++)
        if(!(r_array[i] >= r.r_array[i])) return false;
      return true;
    }

    // check whether r is equal with this range
    bool is_same(const Range& r) const {
      if(!is_valid() || !r.is_valid())
        return false;
      if(r_array.size() != r.r_array.size()) 
        return false;
      for(unsigned int i=0; i<r_array.size(); i++)
        if(r_array[i] != r.r_array[i]) return false;
      return true;
    }

    // simple combine without check
    Range combine(const Range& r) const {
      Range rv(compressed);
      if(!is_valid() || !r.is_valid())
        return rv;
      if(r_array.size() != r.r_array.size())
        return rv;
      rv.r_array.resize(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++)
        rv.r_array[i] = r_array[i] | r.r_array[i];
      return rv;
    }

    // simple overlap without check
    Range overlap(const Range& r) const {
      Range rv(compressed);
      if(!is_valid() || !r.is_valid())
        return rv;
      if(r_array.size() != r.r_array.size())
        return rv;
      rv.r_array.resize(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++) 
        rv.r_array[i] = r_array[i] & r.r_array[i];
      return rv;
    }

    // simple reduce without check
    Range reduce(const Range& r) const {
      Range rv(compressed);
      if(!is_valid() || !r.is_valid())
        return rv;
      if(r_array.size() != r.r_array.size())
        return rv;
      rv.r_array.resize(r_array.size());
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

    // normalize divide
    boost::tuple<Range, Range, Range> 
    divideBy(const Range& r) const {
      if(!is_valid() || !r.is_valid())
        return boost::tuple<Range, Range, Range>();

      Range rH(compressed), rM(compressed), rL(compressed);
      rH.r_array.resize(r_array.size());
      rM.r_array.resize(r_array.size());
      rL.r_array.resize(r_array.size());
      for(unsigned int i=0; i<r_array.size(); i++) {
        if(r_array[i] != r.r_array[i])
          boost::tie(rH.r_array[i], rM.r_array[i], rL.r_array[i]) = 
            r_array[i].divideBy(r.r_array[i]);
        else {
          rH.r_array[i] = r_array[i];
          rM.r_array[i] = r_array[i];
          rL.r_array[i] = r_array[i];
        }
      }
      return boost::tuple<Range, Range, Range>(rH, rM, rL);
    }

    // weak order
    bool less(const Range& r) const {
      if(!is_valid() || !r.is_valid())
        return false;
      if(r_array.size() != r.r_array.size())
        return false;
      for(unsigned int i=0; i<r_array.size(); i++) {
        if(r_array[i] != r.r_array[i])
          return r_array[i].less(r.r_array[i]);
      }
      return false;
    }

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

    // check whether the range and this range are adjacent
    bool is_adjacent(const Range& r) const {
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
