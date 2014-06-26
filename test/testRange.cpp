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
  std::string result;

  /////////////////////////////////////////////////////////////
  cout << "===========================" << endl;
  cout << "Test Range " << endl;
  cout << "===========================" << endl;
  cout << endl;

  /////////////////////////////////////////////////////////////
  cout << "Test range construction and printing ... " << endl;
  
  // valid
  cout << ++index << ". construct and print a valid integer range RA [3:0][12:-5] =>";
  Range<int> RA; RA.add_lower(RangeElement<int>(3, 0)); RA.add_lower(RangeElement<int>(12, -5));
  if(!test(toString(RA), "[3:0][12:-5]", false, cout)) return 1;

  cout << ++index << ". is RA valid ?";
  if(!test(toString(RA.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RA empty ?";
  if(!test(toString(RA.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RA ?";
  if(!test(toString(RA.size()), "72", false, cout)) return 1;

  cout << ++index << ". construct and print a valid integer range RB [3:0][12:-3] =>";
  Range<int> RB; RB.add_upper(RangeElement<int>(12, -3)); RB.add_upper(RangeElement<int>(3, 0));
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
  Range<int> RC(RC_rlist);
  if(!test(toString(RC), "[2][-4]", false, cout)) return 1;

  cout << ++index << ". is RC valid ?";
  if(!test(toString(RC.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RC empty ?";
  if(!test(toString(RC.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RC ?";
  if(!test(toString(RC.size()), "1", false, cout)) return 1;

  // empty ranges
  cout << ++index << ". construct and print a default (empty) integer range RD [] =>";
  Range<int> RD;
  if(!test(toString(RD), "[]", false, cout)) return 1;

  cout << ++index << ". is RD valid ?";
  if(!test(toString(RD.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RD empty ?";
  if(!test(toString(RD.empty()), "1", false, cout)) return 1;

  cout << ++index << ". the size of RC ?";
  if(!test(toString(RD.size()), "0", false, cout)) return 1;

  cout << ++index << ". construct and print an invalid integer range RE [3:0][-2:0] =>";
  std::vector<std::pair<int,int> > RE_rvect;
  RE_rvect.push_back(std::pair<int,int>(3,0));
  RE_rvect.push_back(std::pair<int,int>(-2,0)); 
  Range<int> RE(RE_rvect);
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
  Range<int> RF("[3:0][12:-5][0]");
  if(!test(toString(RF), "[3:0][12:-5][0]", false, cout)) return 1;

  cout << ++index << ". is RF valid ?";
  if(!test(toString(RA.valid()), "1", false, cout)) return 1;
  
  cout << ++index << ". is RF empty ?";
  if(!test(toString(RA.empty()), "0", false, cout)) return 1;

  cout << ++index << ". the size of RF ?";
  if(!test(toString(RA.size()), "72", false, cout)) return 1;

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
  
  // compare with a range not operable but comparable
  cout << ++index << ". RB > RC ?" ;
  if(!test(toString(RB > RC), "1", false, cout)) return 1;

  cout << ++index << ". RB >= RC ?" ;
  if(!test(toString(RB >= RC), "1", false, cout)) return 1;
  
  cout << ++index << ". RB == RC ?" ;
  if(!test(toString(RB == RC), "0", false, cout)) return 1;
  
  cout << ++index << ". RB != RC ?" ;
  if(!test(toString(RB != RC), "1", false, cout)) return 1;
  
  cout << ++index << ". RB <= RC ?" ;
  if(!test(toString(RB <= RC), "0", false, cout)) return 1;
  
  cout << ++index << ". RB < RC ?" ;
  if(!test(toString(RB < RC), "0", false, cout)) return 1;

  // compare with a range with different number of dimensions
  cout << ++index << ". RB > RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB > RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB > RF);
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RB >= RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB >= RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB >= RF);
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;
  
  cout << ++index << ". RB == RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB == RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB == RF);
#endif
  if(!test(result, "0", false, cout)) return 1;
  
  cout << ++index << ". RB != RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB != RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "1";
  }
#else
  result = toString(RB != RF);
#endif
  if(!test(result, "1", false, cout)) return 1;
  
  cout << ++index << ". RB <= RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB <= RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB <= RF);
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;
  
  cout << ++index << ". RB < RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB < RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB < RF);
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;  

  // compare with an invalid range
  cout << ++index << ". RB > RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB > RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB > RE);
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RB >= RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB >= RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB >= RE);
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB == RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB == RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB == RE);
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB != RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB != RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB != RE);
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB <= RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB <= RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB <= RE);
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;
  
  cout << ++index << ". RB < RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RB < RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RB < RE);
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

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
  if(!test(toString(Range<int>().subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper subset of RA ?" ;
  if(!test(toString(Range<int>().proper_subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a subset of [] ?" ;
  if(!test(toString(RA.subset(Range<int>())), "0", false, cout)) return 1;

  cout << ++index << ". [] is a subset of [] ?" ;
  if(!test(toString(Range<int>().subset(Range<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper subset of [] ?" ;
  if(!test(toString(Range<int>().proper_subset(Range<int>())), "0", false, cout)) return 1;

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
  if(!test(toString(Range<int>().subset(RA)), "1", false, cout)) return 1;

  cout << ++index << ". RA is a superset of [] ?" ;
  if(!test(toString(RA.superset(Range<int>())), "1", false, cout)) return 1;

  cout << ++index << ". RA is a proper superset of [] ?" ;
  if(!test(toString(RA.proper_superset(Range<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a superset of [] ?" ;
  if(!test(toString(Range<int>().superset(Range<int>())), "1", false, cout)) return 1;

  cout << ++index << ". [] is a proper superset of [] ?" ;
  if(!test(toString(Range<int>().proper_superset(Range<int>())), "0", false, cout)) return 1;

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
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.subset(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.subset(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a proper subset of RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.proper_subset(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.proper_subset(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a superset of RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.superset(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.superset(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a proper superset of RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.proper_superset(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.proper_superset(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is a subset of RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.subset(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.subset(RE));
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is a proper subset of RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.proper_subset(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.proper_subset(RE));
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is a superset of RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.superset(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.superset(RE));
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is a proper superset of RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.proper_superset(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.proper_superset(RE));
#endif
  if(!test(result, "0", false, cout,
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
  if(!test(toString(RA & Range<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". [] & RA =>" ;
  if(!test(toString(Range<int>() & RA), "[]", false, cout)) return 1;

  cout << ++index << ". [] & [] =>" ;
  if(!test(toString(Range<int>() & Range<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". RA & RC =>" ;
  if(!test(toString(RA & RC), "[2][-4]", false, cout)) return 1;

  cout << ++index << ". RA & RF =>" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA & RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA & RF);
#endif
  if(!test(result, "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA & RE =>" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA & RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA & RE);
#endif
  if(!test(result, "[]", false, cout,
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
  if(!test(toString(RA | Range<int>()), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] | RA =>" ;
  if(!test(toString(Range<int>() | RA), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] | [] =>" ;
  if(!test(toString(Range<int>() | Range<int>()), "[]", false, cout)) return 1;

  cout << ++index << ". RA | RC =>" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA | RC);
  } catch ( RangeException_NonOperable e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA | RC);
#endif
  if(!test(result, "[]", false, cout,
           "\n  *not operable as more than one dimensions are different. Use RangeMap instead.")) return 1;

  cout << ++index << ". RA | RF =>" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA | RF);
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA | RF);
#endif
  if(!test(result, "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA | RE =>" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA | RE);
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA | RE);
#endif
  if(!test(result, "[]", false, cout,
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

  cout << ++index << ". the minimal range contains RA and [] ?" ;
  if(!test(toString(RA.hull(Range<int>())), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains [] and RA ?" ;
  if(!test(toString(Range<int>().hull(RA)), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and [] ?" ;
  if(!test(toString(Range<int>().hull(Range<int>())), "[]", false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and RC ?" ;
  if(!test(toString(RA.hull(RC)), toString(RA), false, cout)) return 1;

  cout << ++index << ". the minimal range contains RA and RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.hull(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA.hull(RF));
#endif
  if(!test(result, "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". the minimal range contains RA and RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.hull(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA.hull(RE));
#endif
  if(!test(result, "[]", false, cout,
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
  if(!test(toString(RA.complement(Range<int>())), toString(RA), false, cout)) return 1;

  cout << ++index << ". [] deducted by RA ?" ;
  if(!test(toString(Range<int>().complement(RA)), "[]", false, cout)) return 1;

  cout << ++index << ". [] deducted by [] ?" ;
  if(!test(toString(Range<int>().complement(Range<int>())), "[]", false, cout)) return 1;

  cout << ++index << ". RA deducted by RC ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.complement(RC));
  } catch ( RangeException_NonOperable e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA.complement(RC));
#endif
  if(!test(result, "[]", false, cout,
                    "\n  *not operable as more than one dimensions are different. Use RangeMap instead.")) return 1;

  cout << ++index << ". RA deducted by RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.complement(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA.complement(RF));
#endif
  if(!test(result, "[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA deducted by RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.complement(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "[]";
  }
#else
  result = toString(RA.complement(RE));
#endif
  if(!test(result, "[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test divide... " << endl;

  cout << ++index << ". RA divided by RA ?" ;
  if(!test(toString(RA.divide(RA)), "[];[3:0][12:-5];[]", false, cout)) return 1;

  cout << ++index << ". RA divided by RB ?" ;
  if(!test(toString(RA.divide(RB)), "[];[3:0][12:-3];[3:0][-4:-5]", false, cout)) return 1;

  cout << ++index << ". RB divided by RA ?" ;
  if(!test(toString(RB.divide(RA)), "[];[3:0][12:-3];[3:0][-4:-5]", false, cout)) return 1;

  cout << ++index << ". RA divided by [] ?" ;
  if(!test(toString(RA.divide(Range<int>())), "[];[3:0][12:-5];[]", false, cout)) return 1;

  cout << ++index << ". [] divided by RA ?" ;
  if(!test(toString(Range<int>().divide(RA)), "[];[3:0][12:-5];[]", false, cout)) return 1;

  cout << ++index << ". [] divided by [] ?" ;
  if(!test(toString(Range<int>().divide(RD)), "[];[];[]", false, cout)) return 1;

  cout << ++index << ". RA divided by RC ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.divide(RC));
  } catch ( RangeException_NonOperable e) {
    cout << "\n" << e.what() << endl;
    result = "[];[];[]";
  }
#else
  result = toString(RA.divide(RC));
#endif
  if(!test(result, "[];[];[]", false, cout,
           "\n  *not operable as more than one dimensions are different. Use RangeMap instead.")) return 1;

  cout << ++index << ". RA divided by RF ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.divide(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "[];[];[]";
  }
#else
  result = toString(RA.divide(RF));
#endif
  if(!test(result, "[];[];[]", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA divided by RE ?" ;
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.divide(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "[];[];[]";
  }
#else
  result = toString(RA.divide(RE));
#endif
  if(!test(result, "[];[];[]", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  /////////////////////////////////////////////////////////////
  cout << endl;
  cout << "Test other helper functions... " << endl;

  // overlap
  cout << ++index << ". RA overlaps with [3:0][15:12] ?";
  Range<int> m(RA); 
  std::vector<RangeElement<int> >::iterator it = m.begin();
  it++;
  *it = RangeElement<int>(15,12);
  if(!test(toString(RA.overlap(m)), "1", false, cout)) return 1;
  
  cout << ++index << ". RA overlaps with [3:0][15:13] ?";
  it->lower(13);
  if(!test(toString(RA.overlap(m)), "0", false, cout)) return 1;

  cout << ++index << ". RA overlaps with [3:0][15:14] ?";
  it->lower(14);
  if(!test(toString(RA.overlap(m)), "0", false, cout)) return 1;

  // adjacent
  cout << ++index << ". RA is adjacent with [3:0][15:12] ?";
  it->lower(12);
  if(!test(toString(RA.connected(m)), "1", false, cout)) return 1;

  cout << ++index << ". RA is adjacent with [3:0][15:13] ?";
  it->lower(13);
  if(!test(toString(RA.connected(m)), "1", false, cout)) return 1;

  cout << ++index << ". RA is adjacent with [3:0][15:14] ?";
  it->lower(14);
  if(!test(toString(RA.connected(m)), "0", false, cout)) return 1;

  // disjoint
  cout << ++index << ". RA is disjoint with [3:0][15:12] ?";
  m[1].lower(12);
  if(!test(toString(RA.disjoint(m)), "0", false, cout)) return 1;

  cout << ++index << ". RA is disjoint with [3:0][15:13] ?";
  m[1].lower(13);
  if(!test(toString(RA.disjoint(m)), "1", false, cout)) return 1;

  cout << ++index << ". RA is disjoint with [3:0][15:14] ?";
  m[1].lower(14);
  if(!test(toString(RA.disjoint(m)), "1", false, cout)) return 1;

  // invalid
  cout << ++index << ". RA overlaps with RC ?";
  if(!test(toString(RA.overlap(RC)), "1", false, cout)) return 1;

  cout << ++index << ". RA is adjacent with RC ?";
  if(!test(toString(RA.connected(RC)), "1", false, cout)) return 1;

  cout << ++index << ". RA is disjoint with RC ?";
  if(!test(toString(RA.disjoint(RC)), "0", false, cout)) return 1;

  cout << ++index << ". RA overlaps with RF ?";
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.overlap(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.overlap(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is adjacent with RF ?";
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.connected(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.connected(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA is disjoint with RF ?";
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.disjoint(RF));
  } catch ( RangeException_NonComparable e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.disjoint(RF));
#endif
  if(!test(result, "0", false, cout,
           "\n  *not comparable due to different numbers of dimensions.")) return 1;

  cout << ++index << ". RA overlaps with RE ?";
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.overlap(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.overlap(RE));
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is adjacent with RE ?";
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.connected(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.connected(RE));
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  cout << ++index << ". RA is disjoint with RE ?";
#ifndef CPP_RANGE_NO_EXCEPTION
  try {
    result = toString(RA.disjoint(RE));
  } catch ( RangeException_InvalidRange e) {
    cout << "\n" << e.what() << endl;
    result = "0";
  }
#else
  result = toString(RA.disjoint(RE));
#endif
  if(!test(result, "0", false, cout,
           "\n  *invalid operation due to RE is invalid.")) return 1;

  // dimension
  cout << ++index << ". the number of dimensions in RA ?";
  if(!test(toString(RA.dimension()), "2", false, cout)) return 1;

  cout << ++index << ". the number of dimensions in RF ?";
  if(!test(toString(RF.dimension()), "3", false, cout)) return 1;

  cout << ++index << ". the number of dimensions in RE ?";
  if(!test(toString(RE.dimension()), "2", false, cout,
           "\n  *dimension() is not guarded by valid(), so even an invalid range can have a non-zero diemsnion number.")) return 1;


  cout << "\nRange<int> test successful!" << endl;
  cout << endl;
  
  return 0;
}
