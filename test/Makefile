#
# Copyright (c) 2014-2014 Wei Song <songw@cs.man.ac.uk> 
#    Advanced Processor Technologies Group, School of Computer Science
#    University of Manchester, Manchester M13 9PL UK
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#    
#        http://www.apache.org/licenses/LICENSE-2.0
#    
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
# Makefile for test the range library
# 18/06/2014   Wei Song
#
#

TARGETS = testRangeElement testRange testRangeMap
INCDIRS = -I../include
CXX = g++
CXXFLAGS = -Wall -Wextra -g

all: $(TARGETS) test

$(TARGETS): %:%.cpp $($wildcard ../include/*.hpp)
	$(CXX) $(INCDIRS) $(CXXFLAGS) $< -o $@

test:
	./testRangeElement
	./testRange
	./testRangeMap

.PHONY: clean

clean:
	-rm $(TARGETS)


