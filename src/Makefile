#*******************************************************************************
# Copyright (c) 2016 MÓRÉH Tamás
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the GNU Lesser Public License v3
# which accompanies this distribution, and is available at
# http://www.gnu.org/licenses/
#
# This file is part of QuexPipe.
#
# QuexPipe is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# QuexPipe is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser Public License for more details.
#
# Contributors:
#     MÓRÉH Tamás - Software architecture design and implementation
#*****************************************************************************

####### Compiler, tools and options

CXX           = g++
DEFINES       = 
INCPATH       = 
LIBRARIES	  = -ldl
CXXFLAGS      = -pipe -O3 -std=c++0x -Wall -W -fPIC $(DEFINES) $(INCPATH) $(LIBRARIES)
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f

all: quexpipe

quexpipe: lexer_manager.cpp main.cpp
	@$(CXX) lexer_manager.cpp main.cpp -o quexpipe $(CXXFLAGS)

bin32: lexer_manager.cpp main.cpp
	@$(MKDIR) bin32
	@$(CXX) -m32 lexer_manager.cpp main.cpp -o bin32/quexpipe $(CXXFLAGS)

bin64: lexer_manager.cpp main.cpp
	@$(MKDIR) bin64
	@$(CXX) -m64 lexer_manager.cpp main.cpp -o bin64/quexpipe $(CXXFLAGS)

deball: deb64 deb32

deb32: bin32
	$(COPY) debcontrol bin32/debcontrol
	cd bin32; \
	sed -i "s=amd64=i386=" debcontrol; \
	sed -i "s=CHANGELOG=../CHANGELOG=" debcontrol; \
	sed -i "s=LICENSE=../../LICENSE=" debcontrol; \
	equivs-build -f -a i386 debcontrol

deb64: bin64
	$(COPY) debcontrol bin64/debcontrol
	cd bin64; \
	sed -i "s=CHANGELOG=../CHANGELOG=" debcontrol; \
	sed -i "s=LICENSE=../../LICENSE=" debcontrol; \
	equivs-build -f -a amd64 debcontrol

build_to_bin: deball
	$(COPY) bin32/*.deb ../bin/
	$(COPY) bin64/*.deb ../bin/

clean:
	@$(DEL_FILE) quexpipe
	@if [ -d bin32 ]; then $(DEL_FILE) bin32/*; $(DEL_DIR) bin32; fi
	@if [ -d bin64 ]; then $(DEL_FILE) bin64/*; $(DEL_DIR) bin64; fi

install: quexpipe
	@$(INSTALL_PROGRAM) quexpipe /usr/bin

uninstall: /usr/bin/quexpipe
	@$(DEL_FILE) /usr/bin/quexpipe
