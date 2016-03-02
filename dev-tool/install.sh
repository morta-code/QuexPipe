#!/bin/bash


if [[ `whoami` != "root" ]]; then
	echo -e "\e[91mRoot permission needed!\e[39m"
	exit 1
	# todo: userspace
fi

which quexpipe > /dev/null
if [ $? -ne 0 ]; then
	echo -e "\e[93mWARNING: There is no quexpipe installed!\e[39m" >&2
fi

TARGET="/usr"
printf "\e[36mGive the installation target (where are /bin and /include subfolders)\e[39m\n[$TARGET] "
read target
if [[ $target == "" ]]; then
	target=$TARGET
fi

install -o root -m 555 bin/quexpipe-plugin-build $target/bin/
cp -R include/ $target
printf "\e[36mSuccessfully installed!\n\e[39m"
