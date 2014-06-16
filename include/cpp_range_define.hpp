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
 * 16/06/2014   Wei Song
 *
 *
 */

#ifndef _CPP_RANGE_DEFINE_H_
#define _CPP_RANGE_DEFINE_H_

namespace CppRange {

  //////////////////////////////////////////////////
  // Special definition for different ranges
  //////////////////////////////////////////////////

  // for all classes with default conversion from int
  template<class T>
  T min_unit() {
    return 1;
  }

  // specializations
  inline template<>
  double min_unit<double>() {
    return 0.0;
  }

  inline template<>
  float min_unit<double>() {
    return 0.0;
  }

  

}

#endif
