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
 * Utility functions fot the C++ STL static range calculation library
 * 25/06/2014   Wei Song
 *
 *
 */

#ifndef _CPP_RANGE_UTIL_H_
#define _CPP_RANGE_UTIL_H_

// just in case this header is used separately
#include "cpp_range_util_def.hpp"

#ifndef _CPP_RANGE_ELEMENT_H_
#include "cpp_range_element.hpp"
#endif

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <list>
#include <string>

namespace CppRange {

  //////////////////////////////////////////////////
  // Utility functions
  //////////////////////////////////////////////////
  

  // parse a simple range
  template<class T>
  RangeElement<T> paser_range(const std::string& str_arg) {
    // parse something like "[3:0]", "[4]" or even "[]"

    std::string str(str_arg);
    if(str.length() < 2) return rv; // throw e

    // for future use, current assume both bound types are closed
    char upper_bound_type = str[0];
    char lower_bound_type = str[str.length()-1];

    // get the two range boundary values
    std::vector<string> fields;
    boost::split(fields, str, boost::is_any_of("[:]"), boost::token_compress_on);
    switch(fields.size()) {
    case 0: {
      return RangeElement<T>();
    }
    case 1: {
      return RangeElement<T>(boost::lexi_cast<T>(fields[0]));
    }
    case 2: {
      return RangeElement<T>(boost::lexi_cast<T>(fields[0]),
                             boost::lexi_cast<T>(fields[1])
                             );
    }
    default:                   // error, should throw e
      return RangeElement<T>();
    }
  }

  // parse a simple range vector
  template<class T>
  std::list<RangeElement<T> > paser_range_list(const std::string& str) {
    // parse something like [5:3][2:1]
    
    std::list<RangeElement<T> > rv;
    
    // pattern to find a range
    boost::xpressive::sregex token = ('[' | '(') >> [0-9.:]* >> (')' | ']'); 

    boost::xpressive::wsregex_iterator cur(str.begin(), str.end(), token);
    boost::xpressive::wsregex_iterator end;

    for(; cur != end; ++cur) {
      const boost::xpressive::smatch& what = *cur;
      rv.push_back(parse_range(what[0]));
    }
    
    return rv;
  }

}

#endif
