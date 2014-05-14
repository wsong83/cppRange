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
 * Test program for the C++ STL static range calculation library
 * 14/05/2014   Wei Song
 *
 *
 */

#include "cpp_range.hpp"
#include <iostream>

using namespace CppRange;
using std::cout;
using std::endl;

int main() {
  cout << "print out range [7:-3] (should be [7:-3]): ";
  cout << RangeElement<int>(7,-3) << endl;
 
  cout << "calculate [7:0] > [3:1] (should be 1): ";
  cout << (RangeElement<int>(7,0) > RangeElement<int>(3, 1)) << std::endl;
  
  cout << "calculate [7:0] > [3:-5] (should be 0): ";
  cout << (RangeElement<int>(7,0) > RangeElement<int>(3, -5)) << std::endl;

  cout << "calculate [7:4] == [7:4] (should be 1): ";
  cout << (RangeElement<int>(7,4) == RangeElement<int>(7, 4)) << std::endl;

  cout << "calculate [7:4] | [3:-9] (should be [7:-9]): ";
  cout << (RangeElement<int>(7,4) | RangeElement<int>(3, -9)) << std::endl;

  cout << "calculate [7:4] - [6:-9] (should be [7]): ";
  cout << (RangeElement<int>(7,4) - RangeElement<int>(6, -9)) << std::endl;

  cout << "calculate [7:4] - [0:-9] (should be [7:4]): ";
  cout << (RangeElement<int>(7,4) - RangeElement<int>(0, -9)) << std::endl;

  cout << "calculate [7:4] & [0:-9] (should be []): ";
  cout << (RangeElement<int>(7,4) & RangeElement<int>(0, -9)) << std::endl;

  cout << "calculate [7:4] & [5] (should be [5]): ";
  cout << (RangeElement<int>(7,4) & RangeElement<int>(5)) << std::endl;

  return 0;
}
