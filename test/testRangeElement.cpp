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

#include <iostream>
#include "cpp_range.hpp"
#include "test_util.hpp"

using namespace CppRange;
using std::cout;
using std::endl;
using std::list;
using std::vector;
using std::pair;
using std::string;


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

  cout << ++index << ". is RA valid ?";
  if(!test(toString(RA.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RA empty ?";
  if(!test(toString(RA.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RA ?";
  if(!test(toString(RA.size()), "18", false, cout)) return 1;

  cout << ++index << ". construct and print a valid integer range RB [12:-3] =>";
  RangeElement<int> RB(12,-3);
  if(!test(toString(RB), "[12:-3]", false, cout)) return 1;

  cout << ++index << ". is RB valid ?";
  if(!test(toString(RB.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RB empty ?";
  if(!test(toString(RB.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RB ?";
  if(!test(toString(RB.size()), "16", false, cout)) return 1;

  cout << ++index << ". construct and print a valid integer range RC [1:1] =>";
  RangeElement<int> RC(1);
  if(!test(toString(RC), "[1]", false, cout)) return 1;

  cout << ++index << ". is RC valid ?";
  if(!test(toString(RC.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RC empty ?";
  if(!test(toString(RC.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RC ?";
  if(!test(toString(RC.size()), "1", false, cout)) return 1;

  // empty ranges
  cout << ++index << ". construct and print a default (empty) integer range RD [] =>";
  RangeElement<int> RD;
  if(!test(toString(RD), "[]", false, cout)) return 1;

  cout << ++index << ". is RD valid ?";
  if(!test(toString(RD.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RD empty ?";
  if(!test(toString(RD.empty()), "1", false, cout)) return 1;

  cout << ++index << ". the size of RC ?";
  if(!test(toString(RD.size()), "0", false, cout)) return 1;

  cout << ++index << ". construct and print an invalid integer range RE [-2:0] =>";
  RangeElement<int> RE(-2,0);
  if(!test(toString(RE), "[]", false, cout)) return 1;

  cout << ++index << ". is RE valid ?";
  if(!test(toString(RE.valid()), "0", false, cout)) return 1;
  
  cout << ++index << ". is RE empty ?";
  if(!test(toString(RE.empty()), "1", false, cout,
           "\n  *invalid range is always considered empty.")) return 1;

  cout << ++index << ". the size of RE ?";
  if(!test(toString(RE.size()), "0", false, cout,
           "\n  *invalid range is always 0.")) return 1;

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

  // compare with an invalid range
  cout << ++index << ". RB > RE ?" ;
  if(!test(toString(RB > RE), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RB >= RE ?" ;
  if(!test(toString(RB >= RE), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB == RE ?" ;
  if(!test(toString(RB == RE), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB != RE ?" ;
  if(!test(toString(RB != RE), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB <= RE ?" ;
  if(!test(toString(RB <= RE), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB < RE ?" ;
  if(!test(toString(RB < RE), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;


  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test seubset and superset ... " << endl;

  cout << ++index << ". does 0 belongs to RA ?" ;
  if(!test(toString(RA.in(0)), "1", false, cout)) return 1;

  cout << ++index << ". does 12 belongs to RA ?" ;
  if(!test(toString(RA.in(12)), "1", false, cout)) return 1;

  cout << ++index << ". does 13 belongs to RA ?" ;
  if(!test(toString(RA.in(13)), "0", false, cout)) return 1;

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

  cout << ++index << ". RA is a subset of RE ?" ;
  if(!test(toString(RA.subset(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is a proper subset of RE ?" ;
  if(!test(toString(RA.proper_subset(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is a superset of RE ?" ;
  if(!test(toString(RA.superset(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is a proper superset of RE ?" ;
  if(!test(toString(RA.proper_superset(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test intersection... " << endl;

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

  cout << ++index << ". [] & RA =>" ;
  if(!test(toString(RangeElement<int>() & RA), "[]", false, cout)) return 1;

  cout << ++index << ". [] & [] =>" ;
  if(!test(toString(RangeElement<int>() & RangeElement<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". [5:3] & [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,3) & RangeElement<int>(0,-10)), "[]", false, cout)) return 1;

  cout << ++index << ". [5:0] & [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,0) & RangeElement<int>(0,-10)), "[0]", false, cout)) return 1;  

  cout << ++index << ". RA & RE =>" ;
  if(!test(toString(RA & RE), "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test union... " << endl;

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

  cout << ++index << ". [] | RA =>" ;
  if(!test(toString(RangeElement<int>() | RA), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] | [] =>" ;
  if(!test(toString(RangeElement<int>() | RangeElement<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". [5:3] | [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,3) | RangeElement<int>(0,-10)), "[]", false, cout,
           "\n  *Cannot be calculated using RangeElement, using RangeMap instead.")) return 1;

  cout << ++index << ". [5:1] | [0:-10] =>" ;
  if(!test(toString(RangeElement<int>(5,0) | RangeElement<int>(0,-10)), "[5:-10]", false, cout)) return 1;  

  cout << ++index << ". RA | RE =>" ;
  if(!test(toString(RA | RE), "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

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

  cout << ++index << ". the minimal range contains [] and RA ?" ;
  if(!test(toString(RangeElement<int>().hull(RA)), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and [] ?" ;
  if(!test(toString(RangeElement<int>().hull(RangeElement<int>())), "[]", false, cout)) return 1;

  cout << ++index << ". the minimal range contains [5:3] and [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,3).hull(RangeElement<int>(0,-10))), "[5:-10]", false, cout)) return 1;

  cout << ++index << ". the minimal range contains [5:1] and [0:-10] ?" ;
  if(!test(toString(RangeElement<int>(5,0).hull(RangeElement<int>(0,-10))), "[5:-10]", false, cout)) return 1;  

  cout << ++index << ". the minimal range contains RA and RE ?" ;
  if(!test(toString(RA.hull(RE)), "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test deduction... " << endl;

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

  cout << ++index << ". [] deducted by [] ?" ;
  if(!test(toString(RangeElement<int>().complement(RangeElement<int>())), "[]", false, cout)) return 1;

  cout << ++index << ". [10:0] deducted by [3:2] ?" ;
  if(!test(toString(RangeElement<int>(10,0).complement(RangeElement<int>(3,2))), "[]", false, cout,
           "\n  *Cannot be calculated using RangeElement, using RangeMap instead.")) return 1;

  cout << ++index << ". [10:0] deducted by [0] ?" ;
  if(!test(toString(RangeElement<int>(10,0).complement(RangeElement<int>(0))), "[10:1]", false, cout)) return 1;  

  cout << ++index << ". [10:0] deducted by [10:8] ?" ;
  if(!test(toString(RangeElement<int>(10,0).complement(RangeElement<int>(10,8))), "[7:0]", false, cout)) return 1;  

  cout << ++index << ". RA deducted by RE ?" ;
  if(!test(toString(RA.complement(RE)), "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

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

  cout << ++index << ". RA divided by RE ?" ;
  if(!test(toString(RA.divide(RE)), "[];[];[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test other helper functions... " << endl;

  // overlap
  cout << ++index << ". [10:2] is overlap to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,2).overlap(RangeElement<int>(2,0))), "1", false, cout)) return 1;

  cout << ++index << ". [10:3] is overlap to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,3).overlap(RangeElement<int>(2,0))), "0", false, cout)) return 1;

  cout << ++index << ". [10:4] is overlap to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,4).overlap(RangeElement<int>(2,0))), "0", false, cout)) return 1;

  cout << ++index << ". [2:0] is overlap to [10:2] ?" ;
  if(!test(toString(RangeElement<int>(2,0).overlap(RangeElement<int>(10,2))), "1", false, cout)) return 1;

  cout << ++index << ". [2:0] is overlap to [10:3] ?" ;
  if(!test(toString(RangeElement<int>(2,0).overlap(RangeElement<int>(10,3))), "0", false, cout)) return 1;

  cout << ++index << ". [2:0] is overlap to [10:4] ?" ;
  if(!test(toString(RangeElement<int>(2,0).overlap(RangeElement<int>(10,4))), "0", false, cout)) return 1;

  cout << ++index << ". [3:2] is overlap to [10:0] ?" ;
  if(!test(toString(RangeElement<int>(3,2).overlap(RangeElement<int>(10,0))), "1", false, cout)) return 1;

  cout << ++index << ". [10:0] is overlap to [3:2] ?" ;
  if(!test(toString(RangeElement<int>(10,0).overlap(RangeElement<int>(3,2))), "1", false, cout)) return 1;

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

  // disjoint
  cout << ++index << ". [10:2] is disjoint to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,2).disjoint(RangeElement<int>(2,0))), "0", false, cout)) return 1;

  cout << ++index << ". [10:3] is disjoint to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,3).disjoint(RangeElement<int>(2,0))), "1", false, cout)) return 1;

  cout << ++index << ". [10:4] is disjoint to [2:0] ?" ;
  if(!test(toString(RangeElement<int>(10,4).disjoint(RangeElement<int>(2,0))), "1", false, cout)) return 1;

  cout << ++index << ". [2:0] is disjoint to [10:2] ?" ;
  if(!test(toString(RangeElement<int>(2,0).disjoint(RangeElement<int>(10,2))), "0", false, cout)) return 1;

  cout << ++index << ". [2:0] is disjoint to [10:3] ?" ;
  if(!test(toString(RangeElement<int>(2,0).disjoint(RangeElement<int>(10,3))), "1", false, cout)) return 1;

  cout << ++index << ". [2:0] is disjoint to [10:4] ?" ;
  if(!test(toString(RangeElement<int>(2,0).disjoint(RangeElement<int>(10,4))), "1", false, cout)) return 1;

  cout << ++index << ". [3:2] is disjoint to [10:0] ?" ;
  if(!test(toString(RangeElement<int>(3,2).disjoint(RangeElement<int>(10,0))), "0", false, cout)) return 1;

  cout << ++index << ". [10:0] is disjoint to [3:2] ?" ;
  if(!test(toString(RangeElement<int>(10,0).disjoint(RangeElement<int>(3,2))), "0", false, cout)) return 1;

  cout << ++index << ". RA overlaps with RE ?";
  if(!test(toString(RA.disjoint(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is adjacent with RE ?";
  if(!test(toString(RA.disjoint(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is disjoint with RE ?";
  if(!test(toString(RA.disjoint(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  // dimension

  // upper
  cout << ++index << ". the upper boundary of RA is ?" ;
  if(!test(toString(RA.upper()), "12", false, cout)) return 1;

  // lower
  cout << ++index << ". the lower boundary of RA is ?" ;
  if(!test(toString(RA.lower()), "-5", false, cout)) return 1;

  // upper
  cout << ++index << ". the upper boundary of RE is ?" ;
  if(!test(toString(RE.upper()), "-2", false, cout,
           "\n  *upper() is not guarded by valid() chack, so the return value is wrong when the range is invalid.")) return 1;

  // lower
  cout << ++index << ". the lower boundary of RE is ?" ;
  if(!test(toString(RE.lower()), "0", false, cout,
           "\n  *lower() is not guarded by valid() chack, so the return value is wrong when the range is invalid.")) return 1;

  cout << "\nRangeElement<int> test successful!" << endl;
  cout << endl;
  
  return 0;
}
