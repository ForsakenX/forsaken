while read -n 100 garbage < /dev/random; do
	echo "$garbage"
done | ./client "$@"
