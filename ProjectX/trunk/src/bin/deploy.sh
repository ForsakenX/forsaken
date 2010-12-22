#!/bin/bash

##a
#
# Description: 
#   Helps to build and deploy all needed versions.
#
# Usage:
#   You can run the script without any arguments to do a real deploy.
#   Provide a name argument to the script to deploy a test version.
#
# Notes:
#   You will be stopped from deploying a production version if uncommitted changes exist.
#   Script will properly set the version.h revision variable for you.
#   You should also setup ssh keys because build() will run scp in the background.
#
# SSH Setup: 
#   You should configure ~/.ssh/config so that ssh/scp lines below will work
#   Example:
#     Host fly.thruhere.net
#     Port 2222
#     User xxxx
#
##

# get version
pxrv=`svn info | grep Revision | awk '{print $NF}'`
sed -ri "/PXRV/ s/[0-9]+/$pxrv/g" version.h
#pxrv=`grep PXRV version.h | head -n 1 | awk '{print $3}' | sed 's/"//g'`
pxv=`grep PXV version.h | head -n 1 | awk '{print $3}' | sed 's/"//g'`
pxmpv=`grep PXMPV version.h | head -n 1 | awk '{print $3}' | sed 's/"//g'`
version="$pxv.$pxmpv.$pxrv"
echo "version: $version"

function build
{
	name=$1; shift
	echo "building $name..."
	make -e clean &>/dev/null
	if ! make -e "$@" &>/dev/null; then
		echo "failed to build $type: make -e $@"
	else
		mv projectx $name
		scp $name fly.thruhere.net:/download/versions/${folder}/ &
	fi
}

# test deployment

if [[ $1 ]]; then
	echo "deploying a test version"
	ssh fly.thruhere.net '
		cd /download/versions;
		mkdir linux/test 2>/dev/null;
		mkdir windows/test 2>/dev/null
	'
	folder="linux/test";   build "projectx-$version-debug-$1" DEBUG=1 PROFILE=1
	folder="windows/test"; build "projectx-$version-$1.exe"   MINGW=1 CROSS=1 DEBUG=1 PROFILE=1
	wait
	echo "finished"
	make -e clean
	exit
fi

# production deployments

if [[ "`svn diff`" ]]; then
	echo "quiting because there is uncommitted changes"
	exit 1
fi

ssh fly.thruhere.net '
	cd /download/versions
	mv linux/projectx* linux/old
	mv windows/projectx* windows/old
'

folder=linux

build "projectx-$version-debug"             DEBUG=1 PROFILE=1
build "projectx-$version-debug-static"      DEBUG=1 PROFILE=1 STATIC=1

#build "projectx-$version-debug-gl3"         GL=3 DEBUG=1 PROFILE=1
#build "projectx-$version-debug-static-gl3"  GL=3 DEBUG=1 PROFILE=1 STATIC=1

folder=windows

build "projectx-$version.exe"  MINGW=1 CROSS=1 DEBUG=1 PROFILE=1

wait

ssh fly.thruhere.net 'cd /download; ./zip.sh'

make -e clean
echo "finished"
