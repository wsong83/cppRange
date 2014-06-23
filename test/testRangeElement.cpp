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
 * Test program for RangeElement
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

template <typename T>
inline std::string toString (const boost::tuple<T,T,T>& obj) {
  std::ostringstream sos;
  sos << boost::get<0>(obj) << ";" << boost::get<1>(obj) << ";" << boost::get<2>(obj);
  return sos.str();
}

bool test(const string& result, const string& answer, bool use_newline, std::ostream& os, const string& comment = "") {
  os << (use_newline ? "\t" : "  ") << result << comment << endl;

  if(result != answer) {
    os << "!!! Error, it should be " << answer << endl;
    return false;
  } else
    return true;  
}

int main() {

  unsigned int index = 0;

  /////////////////////////////////////////////////////////////
  cout << "===========================" << endl;
  cout << "Test RangeElement " << endl;
  cout << "===========================" << endl;
  cout << endl;

  /////////////////////////////////////////////////////////////
  cout << "Test range construction and printing ... " << endl;
  
  // valid
  cout << ++index << ". construct and print a valid integer range RA [12:-5] =>";
  RangeElement<int> RA(12, -5);
  if(!test(toString(RA), "[12:-5]", false, cout)) return 1;


  cout << ++index << ". construct and print a valid integer range RB [12:-3] =>";
  RangeElement<int> RB(12,-3);
  if(!test(toString(RB), "[12:-3]", false, cout)) return 1;


  cout << ++index << ". construct and print a valid integer range RC [1:1] =>";
  RangeElement<int> RC(1);
  if(!test(toString(RC), "[1]", false, cout)) return 1;

  // invalid ranges
  cout << ++index << ". construct and print a default (invalid) integer range RD [] =>";
  RangeElement<int> RD;
  if(!test(toString(RD), "[]", false, cout)) return 1;

  
  cout << ++index << ". construct and print an invalid integer range RE [-2:0] =>";
  RangeElement<int> RE(-2,0);
  if(!test(toString(RE), "[]", false, cout)) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test weak order... " << endl;

  // self compare
  cout << ++index << ". RA > RA ?" ;
  if(!test(toString(RA > RA), "0", false, cout)) return 1;

  cout << ++index << ". RA >= RA ?" ;
  if(!test(toString(RA >= RA), "1", false, cout)) return 1;
  
  cout << ++index << ". RA == RA ?" ;
  if(!test(toString(RA == RA), "1", false, cout)) return 1;
  
  cout << ++index << ". RA != RA ?" ;
  if(!test(toString(RA != RA), "0", false, cout)) return 1;
  
  cout << ++index << ". RA <= RA ?" ;
  if(!test(toString(RA <= RA), "1", false, cout)) return 1;
  
  cout << ++index << ". RA < RA ?" ;
  if(!test(toString(RA < RA), "0", false, cout)) return 1;
  
  // compare with smaller range
  cout << ++index << ". RA > RB ?" ;
  if(!test(toString(RA > RB), "1", false, cout)) return 1;

  cout << ++index << ". RA >= RB ?" ;
  if(!test(toString(RA >= RB), "1", false, cout)) return 1;
  
  cout << ++index << ". RA == RB ?" ;
  if(!test(toString(RA == RB), "0", false, cout)) return 1;
  
  cout << ++index << ". RA != RB ?" ;
  if(!test(toString(RA != RB), "1", false, cout)) return 1;
  
  cout << ++index << ". RA <= RB ?" ;
  if(!test(toString(RA <= RB), "0", false, cout)) return 1;
  
  cout << ++index << ". RA < RB ?" ;
  if(!test(toString(RA < RB), "0", false, cout)) return 1;

  // compare with larger range
  cout << ++index << ". RB > RA ?" ;
  if(!test(toString(RB > RA), "0", false, cout)) return 1;

  cout << ++index << ". RB >= RA ?" ;
  if(!test(toString(RB >= RA), "0", false, cout)) return 1;
  
  cout << ++index << ". RB == RA ?" ;
  if(!test(toString(RB == RA), "0", false, cout)) return 1;
  
  cout << ++index << ". RB != RA ?" ;
  if(!test(toString(RB != RA), "1", false, cout)) return 1;
  
  cout << ++index << ". RB <= RA ?" ;
  if(!test(toString(RB <= RA), "1", false, cout)) return 1;
  
  cout << ++index << ". RB < RA ?" ;
  if(!test(toString(RB < RA), "1", false, cout)) return 1;
  
  // compare with overlaped range
  cout << ++index << ". RA > [0:-10] ?" ;
  if(!test(toString(RA > RangeElement<int>(0,-10)), "1", false, cout)) return 1;

  cout << ++index << ". RA >= [0:-10] ?" ;
  if(!test(toString(RA >= RangeElement<int>(0,-10)), "1", false, cout)) return 1;
  
  cout << ++index << ". RA <= [0:-10] ?" ;
  if(!test(toString(RA <= RangeElement<int>(0,-10)), "0", false, cout)) return 1;
  
  cout << ++index << ". RA < [0:-10] ?" ;
  if(!test(toString(RA < RangeElement<int>(0,-10)), "0", false, cout)) return 1;

  // compare with disjunctive range
  cout << ++index << ". [5:3] > [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,3) > RangeElement<int>(0,-10)), "1", false, cout)) return 1;

  cout << ++index << ". [5:3] >= [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,3) >= RangeElement<int>(0,-10)), "1", false, cout)) return 1;
  
  cout << ++index << ". [5:3] <= [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,3) <= RangeElement<int>(0,-10)), "0", false, cout)) return 1;
  
  cout << ++index << ". [5:3] < [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,3) < RangeElement<int>(0,-10)), "0", false, cout)) return 1;


  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test seubset and superset ... " << endl;

  cout << ++index << ". RA is a subset of RA ?" ;
  if(!test(toString(RA.subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a proper subset of RA ?" ;
  if(!test(toString(RA.proper_subset(RA)), "0", false, cout)) return 1;

  cout << ++index << ". RA is a subset of RB ?" ;
  if(!test(toString(RA.subset(RB)), "0", false, cout)) return 1;
  
  cout << ++index << ". RB is a subset of RA ?" ;
  if(!test(toString(RB.subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RB is a proper subset of RA ?" ;
  if(!test(toString(RB.proper_subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a subset of [0:-10] ?" ;
  if(!test(toString(RA.subset(RangeElement<int>(0,-10))), "0", false, cout)) return 1;
 
  cout << ++index << ". [0:-10] is a subset of RA ?" ;
  if(!test(toString(RangeElement<int>(0,-10).subset(RA)), "0", false, cout)) return 1;

  cout << ++index << ". [] is a subset of RA ?" ;
  if(!test(toString(RangeElement<int>().subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper subset of RA ?" ;
  if(!test(toString(RangeElement<int>().proper_subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a subset of [] ?" ;
  if(!test(toString(RA.subset(RangeElement<int>())), "0", false, cout)) return 1;

  cout << ++index << ". [] is a subset of [] ?" ;
  if(!test(toString(RangeElement<int>().subset(RangeElement<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper subset of [] ?" ;
  if(!test(toString(RangeElement<int>().proper_subset(RangeElement<int>())), "0", false, cout)) return 1;

  cout << ++index << ". RA is a superset of RA ?" ;
  if(!test(toString(RA.superset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a proper superset of RA ?" ;
  if(!test(toString(RA.proper_superset(RA)), "0", false, cout)) return 1;

  cout << ++index << ". RA is a superset of RB ?" ;
  if(!test(toString(RA.superset(RB)), "1", false, cout)) return 1;
  
  cout << ++index << ". RA is a proper superset of RB ?" ;
  if(!test(toString(RA.proper_superset(RB)), "1", false, cout)) return 1;

  cout << ++index << ". RB is a superset of RA ?" ;
  if(!test(toString(RB.superset(RA)), "0", false, cout)) return 1;

  cout << ++index << ". RA is a superset of [0:-10] ?" ;
  if(!test(toString(RA.superset(RangeElement<int>(0,-10))), "0", false, cout)) return 1;
 
  cout << ++index << ". [0:-10] is a superset of RA ?" ;
  if(!test(toString(RangeElement<int>(0,-10).superset(RA)), "0", false, cout)) return 1;

  cout << ++index << ". [] is a superset of RA ?" ;
  if(!test(toString(RangeElement<int>().subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a superset of [] ?" ;
  if(!test(toString(RA.superset(RangeElement<int>())), "1", false, cout)) return 1;

  cout << ++index << ". RA is a proper superset of [] ?" ;
  if(!test(toString(RA.proper_superset(RangeElement<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a superset of [] ?" ;
  if(!test(toString(RangeElement<int>().superset(RangeElement<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper superset of [] ?" ;
  if(!test(toString(RangeElement<int>().proper_superset(RangeElement<int>())), "0", false, cout)) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test overlap... " << endl;

  cout << ++index << ". RA & RA =>" ;
  if(!test(toString(RA & RA), toString(RA), false, cout)) return 1;

  cout << ++index << ". RA & RB =>" ;
  if(!test(toString(RA & RB), toString(RB), false, cout)) return 1;

  cout << ++index << ". RB & RA =>" ;
  if(!test(toString(RB & RA), toString(RB), false, cout)) return 1;

  cout << ++index << ". RA & [0:-10] =>" ;
  if(!test(toString(RA & RangeElement<int>(0,-10)), "[0:-5]", false, cout)) return 1;

  cout << ++index << ". RA & [] =>" ;
  if(!test(toString(RA & RangeElement<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". [5:3] & [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,3) & RangeElement<int>(0,-10)), "[]", false, cout)) return 1;

  cout << ++index << ". [5:0] & [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,0) & RangeElement<int>(0,-10)), "[0]", false, cout)) return 1;  

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test combine... " << endl;

  cout << ++index << ". RA | RA =>" ;
  if(!test(toString(RA | RA), toString(RA), false, cout)) return 1;

  cout << ++index << ". RA | RB =>" ;
  if(!test(toString(RA | RB), toString(RA), false, cout)) return 1;

  cout << ++index << ". RB | RA =>" ;
  if(!test(toString(RB | RA), toString(RA), false, cout)) return 1;

  cout << ++index << ". RA | [0:-10] =>" ;
  if(!test(toString(RA | RangeElement<int>(0,-10)), "[12:-10]", false, cout)) return 1;

  cout << ++index << ". RA | [] =>" ;
  if(!test(toString(RA | RangeElement<int>()), toString(RA), false, cout)) return 1;

  cout << ++index << ". [5:3] | [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,3) | RangeElement<int>(0,-10)), "[]", false, cout,
           "\n  *Cannot be calculated using RangeElement, using RangeMap instead.")) return 1;

  cout << ++index << ". [5:1] | [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,0) | RangeElement<int>(0,-10)), "[5:-10]", false, cout)) return 1;  

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test hull... " << endl;

  cout << ++index << ". the minimal range contains RA and RA ?" ;
  if(!test(toString(RA.hull(RA)), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and RB ?" ;
  if(!test(toString(RA.hull(RB)), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains RB and RA ?" ;
  if(!test(toString(RB.hull(RA)), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and [0:-10] ?" ;
  if(!test(toString(RA.hull(RangeElement<int>(0,-10))), "[12:-10]", false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and [] ?" ;
  if(!test(toString(RA.hull(RangeElement<int>())), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains [5:3] and [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,3).hull(RangeElement<int>(0,-10))), "[5:-10]", false, cout)) return 1;

  cout << ++index << ". the minimal range contains [5:1] and [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,0).hull(RangeElement<int>(0,-10))), "[5:-10]", false, cout)) return 1;  

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test reduce... " << endl;

  cout << ++index << ". RA deducted by RA ?" ;
  if(!test(toString(RA.complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". RA deducted by RB ?" ;
  if(!test(toString(RA.complement(RB)), "[-4:-5]", false, cout)) return 1;

  cout << ++index << ". RB deducted by RA ?" ;
  if(!test(toString(RB.complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". RA deducted by [0:-10] ?" ;
  if(!test(toString(RA.complement(RangeElement<int>(0,-10))), "[12:1]", false, cout)) return 1;

  cout << ++index << ". RA deducted by [] ?" ;
  if(!test(toString(RA.complement(RangeElement<int>())), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] deducted by RA ?" ;
  if(!test(toString(RangeElement<int>().complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". [10:0] deducted by [3:2] ?" ;
  if(!test(toString(RangeElement<int>(10,0).complement(RangeElement<int>(3,2))), "[]", false, cout,
           "\n  *Cannot be calculated using RangeElement, using RangeMap instead.")) return 1;

  cout << ++index << ". [10:0] deducted by [0] ?" ;
  if(!test(toString(RangeElement<int>(10,0).complement(RangeElement<int>(0))), "[10:1]", false, cout)) return 1;  

  cout << ++index << ". [10:0] deducted by [10:8] ?" ;
  if(!test(toString(RangeElement<int>(10,0).complement(RangeElement<int>(10,8))), "[7:0]", false, cout)) return 1;  

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test divide... " << endl;

  cout << ++index << ". RA divided by RA ?" ;
  if(!test(toString(RA.divide(RA)), "[];[12:-5];[]", false, cout)) return 1;

  cout << ++index << ". RA divided by RB ?" ;
  if(!test(toString(RA.divide(RB)), "[];[12:-3];[-4:-5]", false, cout)) return 1;

  cout << ++index << ". RB divided by RA ?" ;
  if(!test(toString(RB.divide(RA)), "[];[12:-3];[-4:-5]", false, cout)) return 1;

  cout << ++index << ". RA divided by [0:-10] ?" ;
  if(!test(toString(RA.divide(RangeElement<int>(0,-10))), "[12:1];[0:-5];[-6:-10]", false, cout)) return 1;

  cout << ++index << ". RA divided by [] ?" ;
  if(!test(toString(RA.divide(RangeElement<int>())), "[];[12:-5];[]", false, cout)) return 1;

  cout << ++index << ". [] divided by RA ?" ;
  if(!test(toString(RangeElement<int>().divide(RA)), "[];[12:-5];[]", false, cout)) return 1;

  cout << ++index << ". [] divided by [] ?" ;
  if(!test(toString(RangeElement<int>().divide(RD)), "[];[];[]", false, cout)) return 1;

  cout << ++index << ". [10:0] divided by [3:2] ?" ;
  if(!test(toString(RangeElement<int>(10,0).divide(RangeElement<int>(3,2))), "[10:4];[3:2];[1:0]", false, cout)) return 1;

  cout << ++index << ". [3:2] divided by [10:0] ?" ;
  if(!test(toString(RangeElement<int>(3,2).divide(RangeElement<int>(10,0))), "[10:4];[3:2];[1:0]", false, cout)) return 1;

  cout << ++index << ". [10:3] divided by [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,3).divide(RangeElement<int>(2,0))), "[10:3];[];[2:0]", false, cout)) return 1;

  cout << ++index << ". [2:0] divided by [10:3] ?" ;
  if(!test(toString(RangeElement<int>(2,0).divide(RangeElement<int>(10,3))), "[10:3];[];[2:0]", false, cout)) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test other helper functions... " << endl;

  // adjacent
  cout << ++index << ". [10:2] is adjacent to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,2).connected(RangeElement<int>(2,0))), "1", false, cout)) return 1;

  cout << ++index << ". [10:3] is adjacent to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,3).connected(RangeElement<int>(2,0))), "1", false, cout)) return 1;

  cout << ++index << ". [10:4] is adjacent to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,4).connected(RangeElement<int>(2,0))), "0", false, cout)) return 1;

  cout << ++index << ". [2:0] is adjacent to [10:2] ?" ;
  if(!test(toString(RangeElement<int>(2,0).connected(RangeElement<int>(10,2))), "1", false, cout)) return 1;

  cout << ++index << ". [2:0] is adjacent to [10:3] ?" ;
  if(!test(toString(RangeElement<int>(2,0).connected(RangeElement<int>(10,3))), "1", false, cout)) return 1;

  cout << ++index << ". [2:0] is adjacent to [10:4] ?" ;
  if(!test(toString(RangeElement<int>(2,0).connected(RangeElement<int>(10,4))), "0", false, cout)) return 1;

  cout << ++index << ". [3:2] is adjacent to [10:0] ?" ;
  if(!test(toString(RangeElement<int>(3,2).connected(RangeElement<int>(10,0))), "1", false, cout)) return 1;

  cout << ++index << ". [10:0] is adjacent to [3:2] ?" ;
  if(!test(toString(RangeElement<int>(10,0).connected(RangeElement<int>(3,2))), "1", false, cout)) return 1;

  // dimension

  // bit size
  cout << ++index << ". the bit size of [10:0] is ?" ;
  if(!test(toString(RangeElement<int>(10,0).size()), "11", false, cout)) return 1;

  // first
  cout << ++index << ". the upper boundary of [10:0] is ?" ;
  if(!test(toString(RangeElement<int>(10,0).upper()), "10", false, cout)) return 1;

  // second
  cout << ++index << ". the lower boundary of [10:0] is ?" ;
  if(!test(toString(RangeElement<int>(10,0).lower()), "0", false, cout)) return 1;


  cout << "\nRangeElement<int> test successful!" << endl;
  cout << endl;
  
  return 0;
}
