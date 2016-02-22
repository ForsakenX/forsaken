#!/bin/bash -x
cd "$(dirname -- "$0")"
mkdir -p ../lib/pkgconfig 2>/dev/null
rm build.log
set -e # die if any of the following fail
{

	# these must generally be built in this order
	./zlib-1.2.5.build
	./libpng-1.4.4.build
	./lua-5.1.4.build
	./luasocket-2.0.2.build

	# these are independent
	./enet-1.2.5.build
	./SDL-1.2.14.build
	./openal-soft-1.14.build

} 2>&1 | tee build.log
if [[ ${PIPESTATUS[0]} != 0 ]]; then
	echo "Failed to build..."
	exit 1
fi
echo "Done!"
