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

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <list>
#include <string>

namespace CppRange {

  //////////////////////////////////////////////////
  // Utility functions
  //////////////////////////////////////////////////
  

  // parse a simple range
  template<class T>
  std::pair<T,T> range_element_paser(const std::string& str_arg) {
    std::string str(str_arg);
    std::list<std::pair<T,T> > rv(0,0);
    char token;
    std::string data;
    size_t index;
    if(str.length() < 2) return rv; // throw e

    // check upper bound type
    token = str[0];
    switch(token) {
    case '[':
    case '(':
    default:
      ;                         // currently doing nothing
    }
    str.erase(0, 1);

    // check the lower bound type
    token = str[str.length()-1];
    switch(token) {
    case '[':
    case '(':
    default:
      ;                         // currently doing nothing
    }
    str.erase(str.length()-1, 1);
    
    std::vector<string> fields;
    boost::split(fields, str, boost::is_any_of(":"), boost::token_compress_on);
    switch(fields.size()) {
    case 0: return rv;
    case 1: return 
    }
    
    
    // get the upper bound
    index = str.find(':');
    
    // throw e when index == npos
    data = str.sub

  }

  // parse a simple range vector
  template<class T>
  std::list<std::pair<T,T> > range_vector_paser(const std::string& str) {
    

  }


}

#endif
