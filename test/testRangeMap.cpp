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
 * Test program for RangeMap
 * 18/06/2014   Wei Song
 *
 *
 */

#include "cpp_range.hpp"
#include "test_util.hpp"
#include <iostream>

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
  cout << "Test RangeMap " << endl;
  cout << "===========================" << endl;
  cout << endl;

  /////////////////////////////////////////////////////////////
  cout << "Test range construction and printing ... " << endl;
 
  // valid
  cout << ++index << ". construct and print a valid integer range RA [3:0][12:-5] =>";
  Range<int> RMap_builderA; 
  RMap_builderA.add_lower(RangeElement<int>(3, 0)); 
  RMap_builderA.add_lower(RangeElement<int>(12, -5));
  RangeMap<int> RA(RMap_builderA);
  if(!test(toString(RA), "[3:0][12:-5]", false, cout)) return 1;

  cout << ++index << ". is RA valid ?";
  if(!test(toString(RA.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RA empty ?";
  if(!test(toString(RA.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RA ?";
  if(!test(toString(RA.size()), "72", false, cout)) return 1;

  cout << ++index << ". construct and print a valid integer range RB [3:0][12:-3] =>";
  Range<int> RMap_builderB(RMap_builderA);
  RMap_builder[1] = RangeElement<int>(-4,-5); // [3:0][-4:-5];
  RangeMap<int> RB(RA);
  RB = RB.complement(RangeMap<int>(RMap_builderB)); // [3:0][12:-5] - [3:0][-4:-5]
  if(!test(toString(RB), "[3:0][12:-3]", false, cout)) return 1;

  cout << ++index << ". is RB valid ?";
  if(!test(toString(RB.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RB empty ?";
  if(!test(toString(RB.empty()), "0", false, cout)) return 1;
 
  cout << ++index << ". the size of RB ?";
  if(!test(toString(RB.size()), "64", false, cout)) return 1;

  cout << ++index << ". construct and print a valid integer range RC [2][-4:-4] =>";
  std::list<RangeElement<int> > RC_rlist;
  RC_rlist.push_back(RangeElement<int>(2));
  RC_rlist.push_back(RangeElement<int>(-4)); 
  Range<int> RMap_builderC(RC_rlist);
  RangeMap<int> RC(RMap_builderC);
  if(!test(toString(RC), "[2][-4]", false, cout)) return 1;

  cout << ++index << ". is RC valid ?";
  if(!test(toString(RC.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RC empty ?";
  if(!test(toString(RC.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RC ?";
  if(!test(toString(RC.size()), "1", false, cout)) return 1;

  // empty ranges
  cout << ++index << ". construct and print a default (empty) integer range RD [] =>";
  RangeMap<int> RD;
  if(!test(toString(RD), "[]", false, cout)) return 1;

  cout << ++index << ". is RD valid ?";
  if(!test(toString(RD.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RD empty ?";
  if(!test(toString(RD.empty()), "1", false, cout)) return 1;

  cout << ++index << ". the size of RC ?";
  if(!test(toString(RD.size()), "0", false, cout)) return 1;

  cout << ++index << ". construct and print an invalid integer range RE [3:0][-2:0] =>";
  RMap_builderB[1] = RangeElement<int>(-2,0);
  RangeMap<int> RE(RMap_builderB);
  if(!test(toString(RE), "[]", false, cout)) return 1;

  cout << ++index << ". is RE valid ?";
  if(!test(toString(RE.valid()), "0", false, cout)) return 1;
  
  cout << ++index << ". is RE empty ?";
  if(!test(toString(RE.empty()), "1", false, cout,
           "\n  *invalid range is always considered empty.")) return 1;

  cout << ++index << ". the size of RE ?";
  if(!test(toString(RE.size()), "0", false, cout,
           "\n  *invalid range is always 0.")) return 1;

  cout << ++index << ". construct and print a valid integer range RF [3:0][12:-5][0] =>";
  RMap_builderA.add_lower(RangeElement<int>(0));
  RangeMap<int> RF(RMap_builderA);
  if(!test(toString(RF), "[3:0][12:-5][0]", false, cout)) return 1;

  cout << ++index << ". is RF valid ?";
  if(!test(toString(RA.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RF empty ?";
  if(!test(toString(RA.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RF ?";
  if(!test(toString(RA.size()), "72", false, cout)) return 1;

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

  cout << ++index << ". [] is a subset of RA ?" ;
  if(!test(toString(RangeMap<int>().subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper subset of RA ?" ;
  if(!test(toString(RangeMap<int>().proper_subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a subset of [] ?" ;
  if(!test(toString(RA.subset(RangeMap<int>())), "0", false, cout)) return 1;

  cout << ++index << ". [] is a subset of [] ?" ;
  if(!test(toString(RangeMap<int>().subset(RangeMap<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper subset of [] ?" ;
  if(!test(toString(RangeMap<int>().proper_subset(RangeMap<int>())), "0", false, cout)) return 1;

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

  cout << ++index << ". [] is a superset of RA ?" ;
  if(!test(toString(RangeMap<int>().subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a superset of [] ?" ;
  if(!test(toString(RA.superset(RangeMap<int>())), "1", false, cout)) return 1;

  cout << ++index << ". RA is a proper superset of [] ?" ;
  if(!test(toString(RA.proper_superset(RangeMap<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a superset of [] ?" ;
  if(!test(toString(Range<int>().superset(RangeMap<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper superset of [] ?" ;
  if(!test(toString(RangeMap<int>().proper_superset(RangeMap<int>())), "0", false, cout)) return 1;

  cout << ++index << ". RA is a subset of RC ?" ;
  if(!test(toString(RA.subset(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RA is a proper subset of RC ?" ;
  if(!test(toString(RA.proper_subset(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RA is a superset of RC ?" ;
  if(!test(toString(RA.superset(RC)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a proper superset of RC ?" ;
  if(!test(toString(RA.proper_superset(RC)), "1", false, cout)) return 1;

  cout << ++index << ". RB is a subset of RC ?" ;
  if(!test(toString(RB.subset(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RB is a proper subset of RC ?" ;
  if(!test(toString(RB.proper_subset(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RB is a superset of RC ?" ;
  if(!test(toString(RB.superset(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RB is a proper superset of RC ?" ;
  if(!test(toString(RB.proper_superset(RC)), "0", false, cout)) return 1;
  
  cout << ++index << ". RA is a subset of RF ?" ;
  if(!test(toString(RA.subset(RF)), "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a proper subset of RF ?" ;
  if(!test(toString(RA.proper_subset(RF)), "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a superset of RF ?" ;
  if(!test(toString(RA.superset(RF)), "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a proper superset of RF ?" ;
  if(!test(toString(RA.proper_superset(RF)), "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

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

  cout << ++index << ". RA & [] =>" ;
  if(!test(toString(RA & RangeMap<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". [] & RA =>" ;
  if(!test(toString(RangeMap<int>() & RA), "[]", false, cout)) return 1;

  cout << ++index << ". [] & [] =>" ;
  if(!test(toString(RangeMap<int>() & RangeMap<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". RA & RC =>" ;
  if(!test(toString(RA & RC), "[2][-4]", false, cout)) return 1;

  cout << ++index << ". RA & RF =>" ;
  if(!test(toString(RA & RF), "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

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

  cout << ++index << ". RA | [] =>" ;
  if(!test(toString(RA | RangeMap<int>()), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] | RA =>" ;
  if(!test(toString(RangeMap<int>() | RA), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] | [] =>" ;
  if(!test(toString(RangeMap<int>() | RangeMap<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". RA | RC =>" ;
  if(!test(toString(RA | RC), toString(RA), false, cout)) return 1;

  cout << ++index << ". RA | RF =>" ;
  if(!test(toString(RA | RF), "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA | RE =>" ;
  if(!test(toString(RA | RE), "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test deduction... " << endl;

  cout << ++index << ". RA deducted by RA ?" ;
  if(!test(toString(RA.complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". RA deducted by RB ?" ;
  if(!test(toString(RA.complement(RB)), "[3:0][-4:-5]", false, cout)) return 1;

  cout << ++index << ". RB deducted by RA ?" ;
  if(!test(toString(RB.complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". RA deducted by [] ?" ;
  if(!test(toString(RA.complement(RangeMap<int>())), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] deducted by RA ?" ;
  if(!test(toString(RangeMap<int>().complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". [] deducted by [] ?" ;
  if(!test(toString(RangeMap<int>().complement(RangeMap<int>())), "[]", false, cout)) return 1;

  cout << ++index << ". RA deducted by RC ?" ;
  if(!test(toString(RA.complement(RC)), "{[3][12:-5];[2]{[12:-3];[-5]};[1:0][12:-5]}", false, cout)) return 1;

  cout << ++index << ". RA deducted by RF ?" ;
  if(!test(toString(RA.complement(RF)), "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA deducted by RE ?" ;
  if(!test(toString(RA.complement(RE)), "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test other helper functions... " << endl;

  // overlap
  cout << ++index << ". RA overlaps with [3:0][15:12] ?";
  RMap_builderA[1] = RangeElement<int>(15:12);
  if(!test(toString(RA.overlap(RangeMap<int>(RMap_builderA))), "1", false, cout)) return 1;
  
  cout << ++index << ". RA overlaps with [3:0][15:13] ?";
  RMap_builderA[1].lower(13);
  if(!test(toString(RA.overlap(RangeMap<int>(RMap_builderA))), "0", false, cout)) return 1;

  cout << ++index << ". RA overlaps with [3:0][15:14] ?";
  RMap_builderA[1].lower(14);
  if(!test(toString(RA.overlap(RangeMap<int>(RMap_builderA))), "0", false, cout)) return 1;

  // disjoint
  cout << ++index << ". RA is disjoint with [3:0][15:12] ?";
  RMap_builderA[1].lower(12);
  if(!test(toString(RA.disjoint(RangeMap<int>(RMap_builderA))), "0", false, cout)) return 1;

  cout << ++index << ". RA is disjoint with [3:0][15:13] ?";
  RMap_builderA[1].lower(13);
  if(!test(toString(RA.disjoint(RangeMap<int>(RMap_builderA))), "1", false, cout)) return 1;

  cout << ++index << ". RA is disjoint with [3:0][15:14] ?";
  RMap_builderA[1].lower(14);
  if(!test(toString(RA.disjoint(RangeMap<int>(RMap_builderA))), "1", false, cout)) return 1;

  // invalid
  cout << ++index << ". RA overlaps with RC ?";
  if(!test(toString(RA.overlap(RC)), "1", false, cout)) return 1;

  cout << ++index << ". RA is disjoint with RC ?";
  if(!test(toString(RA.disjoint(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RA overlaps with RF ?";
  if(!test(toString(RA.overlap(RF)), "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is disjoint with RF ?";
  if(!test(toString(RA.disjoint(RF)), "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA overlaps with RE ?";
  if(!test(toString(RA.overlap(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is disjoint with RE ?";
  if(!test(toString(RA.disjoint(RE)), "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  // dimension
  cout << ++index << ". the number of dimensions in RA ?";
  if(!test(toString(RA.dimension()), "2", false, cout)) return 1;

  cout << ++index << ". the number of dimensions in RF ?";
  if(!test(toString(RF.dimension()), "3", false, cout)) return 1;

  cout << ++index << ". the number of dimensions in RE ?";
  if(!test(toString(RE.dimension()), "2", false, cout,
           "\n  *dimension() is not guarded by valid(), so even an invalid range can have a non-zero diemsnion number.")) return 1;


  cout << "\nRangeMap<int> test successful!" << endl;
  cout << endl;
  
  return 0;
}

  
  return 0;
}
