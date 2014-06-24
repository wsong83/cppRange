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
 * Helper functions for tests
 * 24/06/2014   Wei Song
 *
 *
 */

#ifndef CPP_RANGE_TEST_UTIL_H
#define CPP_RANGE_TEST_UTIL_H

#include <iostream>
#include <sstream>

// objects to string templates
template <typename T>
std::string toString (const T& obj) {
  std::ostringstream sos;
  sos << obj;
  return sos.str();
}

template <typename T>
std::string toString (const boost::tuple<T,T,T>& obj) {
  std::ostringstream sos;
  sos << boost::get<0>(obj) << ";" << boost::get<1>(obj) << ";" << boost::get<2>(obj);
  return sos.str();
}

inline bool test(const std::string& result, const std::string& answer, bool use_newline, std::ostream& os, const std::string& comment = "") {
  os << (use_newline ? "\t" : "  ") << result << comment << std::endl;

  if(result != answer) {
    os << "!!! Error, it should be " << answer << std::endl;
    return false;
  } else
    return true;  
}


#endif
