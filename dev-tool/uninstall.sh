#!/bin/bash

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

if [[ `whoami` != "root" ]]; then
	echo -e "\e[91mRoot permission needed!\e[39m"
	exit 1
	# todo: userspace
fi

p=`which quexpipe-plugin-build`
if [ $? -ne 0 ]; then
	echo -e "\e[93mThere is no quexpipe-plugin-build installed!\e[39m" >&2
else
	rm $p
	sudo rm -R `echo $p | sed -r "s=/bin/quexpipe-plugin-build=="`/include/quexpipe
	if [ $? -eq 0 ]; then
		printf "\e[36mSuccessfully uninstalled!\n\e[39m"
	else
		printf "\e[93mSomething went wrong. Check the files manually!\n\e[39m"
	fi
fi
