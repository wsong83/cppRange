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
 * Test program for Range
 * 18/06/2014   Wei Song
 *
 *
 */

#include "cpp_range.hpp"
#include <iostream>
#include <sstream>

using namespace CppRange;
using std::cout;
using std::endl;
using std::list;
using std::vector;
using std::pair;
using std::string;

// objects to string templates
template <typename T>
inline std::string toString (const T& obj) {
  std::ostringstream sos;
  sos << obj;
  return sos.str();
}

int main() {

  /////////////////////////////////////////////////////////////
  cout << "===========================" << endl;
  cout << "Test Range " << endl;
  cout << "===========================" << endl;
  cout << endl;

  /////////////////////////////////////////////////////////////
  cout << "Test range construction and printing ... " << endl;
  
  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test weak order... " << endl;
  
  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test overlap ... " << endl;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test combine... " << endl;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test reduce... " << endl;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test divide... " << endl;
  
  return 0;
}
