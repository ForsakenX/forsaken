#!/bin/bash
trap 'killall -9 client; rm log.*' SIGINT
./client server &> log.server &
for ((x=1; x<40; x++)); do
	port="2`printf "%-.4d" $x`"
	player="player${port}"
	./client $player $port localhost 2300 &> log.$port &
done
./client player0 20000 localhost 2300 | tee log.20000
wait
