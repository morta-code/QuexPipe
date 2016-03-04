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

####### Tools and options

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

deb:
	equivs-build -f debcontrol

build_to_bin: deb
	@$(COPY) *.deb ../bin/

clean:
	@$(DEL_FILE) quexpipe-dev-tool_*

install:
	@$(INSTALL_PROGRAM) bin/quexpipe-plugin-build /usr/bin
	@$(COPY_DIR) include /usr

uninstall: /usr/bin/quexpipe-plugin-build
	@$(DEL_FILE) /usr/bin/quexpipe-plugin-build
	@rm -f -R /usr/include/quexpipe
