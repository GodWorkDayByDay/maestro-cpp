#!/bin/bash

#	Syntax:
#		./move  { position | percentage}  [ channel(s) ]
#
#	e.g.
#		./move 50%
#		./move 3080 "2 4 5"


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

CHANS="$2"

if [ -z "$CHANS" ]
then
	CHANS=`seq 0 5`
fi

for CHAN in $CHANS
do
	echo "84:0$CHAN:$BYTES" 1>&2
	echo -n "840$CHAN$BYTES"
done | xxd -r -p | sudo tee /dev/ttyACM0 > /dev/null
