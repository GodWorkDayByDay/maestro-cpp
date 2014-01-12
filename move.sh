#!/bin/bash

V="$1"

MIN=2049
MAX=10239

if [ "${V: -1}" == "%" ]
then
	PC="${V%?}"
	V=$(( (${V%?} * ($MAX - $MIN)) / 100 + $MIN ))
fi

if [ -z $V ]
then
	V=mid
fi

case "$V" in
	min) ;&
	-)
		V=$MIN;;
	max) ;&
	+)
		V=$MAX;;
	mid) ;&
	c)
		V=$(( ($MIN + $MAX) / 2 ));;
esac

(( $V < $MIN )) && V=$MIN
(( $V > $MAX )) && V=$MAX

let BIT7="(($V >> 7) & 127) | (($V & 127) << 8)"

BYTES=`echo "obase=16; $BIT7" | bc`

while (( ${#BYTES} < 4 ))
do
	BYTES=0$BYTES
done

echo "84:xx:$BYTES" 1>&2

for CHAN in `seq 0 5`
do
	echo -n "840$CHAN$BYTES"
done | xxd -r -p | sudo tee /dev/ttyACM0 > /dev/null
