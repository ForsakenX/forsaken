#!/bin/bash -x
cd "$(dirname -- "$0")"
DIR=${PWD/*\/} # path to this folder
LIB=$PWD/lib # path to lib folder
export PKG_CONFIG_PATH="$LIB/pkgconfig"
cd ..
make "$@" || exit 1

# on osx we need to remap the path to dynamic libs
uname -a | grep -q Darwin &&
dyldinfo -dylibs ./projectx |
grep dylib |
while read -r path; do
	file=${path/*\/}
	[[ -f "$LIB/$file" ]] || continue
	install_name_tool -change \
		"$path" \
		"@executable_path/$DIR/lib/$file" \
		./projectx ||
			exit 1
done
exit 0
