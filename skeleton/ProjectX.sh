#!/bin/bash

function usage {
	echo
	echo "Forsaken help"
	echo
	echo "./forsaken <options> [-- [game options]]"
	echo
	echo "options: help, console / debug, noprompt"
	echo
	echo "console:	winedbg console"
	echo "debug:	debugging without console"
	echo "noprompt: run right away"
	echo
	echo "Put permanant options into, 'options' file"
	echo
	exit 1
}

basename=`basename $0`
dirname=`dirname $0`
# if relative path make absolute
if [ "." = "`echo $dirname | cut -b 1`" ]; then
	dirname=$PWD/$dirname
fi
cd $dirname

debug=false
prompt=true

[ -z "$WINE" ]		&& export WINE=wine
export OPTIONS="`cat Configs/linux.options 2> /dev/null` $OPTIONS"

while [ ! -z "$1" ]; do
	arg="$1"; shift
	case $arg in
	# do not prompt for acceptance
	"noprompt")
		prompt=false
	;;
	# help
	"help"|"-h"|"--help")
		usage
	;;
	# debug mode
	"debug"|"console") 
		debug=true
		# drop to winedbg console
		if [ "$arg" = "console" ]; then
			export WINE=winedbg
		fi
		# Tell forsaken to print debug messages
		export OPTIONS="$OPTIONS -debug"
		# http://wiki.winehq.org/DebugChannels
		#+seh,+relay,+tid,+ddraw,+dinput,+dsound
		if [ -z "$WINEDEBUG" ]; then
			# display the forsaken debug messages
			export WINEDEBUG="+debugstr"	
		fi
	;;
	# rest of options belong to forsaken
	"--")
		export OPTIONS="$OPTIONS $@"
		break 
	;;
	esac
done

if [ -z "$VERSION" ]; then
	export VERSION=`ls $dirname/*.exe | sort | tail -n 1`
else
	export VERSION=$VERSION
fi

while true; do

	echo
	if $debug && [ ! -z "$WINEDEBUG" ]; then
		echo WINEDEBUG:		$WINEDEBUG
	fi
	if [ ! -z "$WINEOPTIONS" ]; then
		echo WINEOPTIONS:	$WINEOPTIONS
	fi
	echo VERSION:		`basename $VERSION`
	if [ ! -z "$OPTIONS" ]; then
		echo OPTIONS:		$OPTIONS
	fi
	echo
	echo Edit Configs/linux.options to permanantly change options

	if ! $prompt; then
		break;
	fi
	
	echo
	echo "Make a selection"
	echo "S to start"
	echo "Q to quit"
	echo "V to select another another version."
	echo
	echo -n "> "
	read -n 1 char
	echo
	case "$char" in
	"q"|"Q")
		exit 0
	;;
	"v"|"V")
		x=1
		echo
		echo "Type the number and hit enter"
		echo
		for file in $dirname/*.exe; do
			echo "$x: `basename $file`"
			x=$(($x+1))
		done
		echo
		read v
		export VERSION=`ls $dirname/*.exe | head -n $v | tail -n 1`
	;;
	# enter key
	"s"|"S") 
		break
		prompt=false # break out of loop
	;;
	# unknown input
	*) 
		echo "Unknown option."
	;; 
	esac
done

echo "launching..."
$WINE $WINEOPTIONS $VERSION $OPTIONS

