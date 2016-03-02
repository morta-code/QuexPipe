#!/bin/bash

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
