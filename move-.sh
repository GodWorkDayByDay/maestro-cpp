#!/bin/bash
(
	echo -en "\x84\x00\x01\x00"
	echo -en "\x84\x01\x01\x00"
	echo -en "\x84\x02\x01\x00"
	echo -en "\x84\x03\x01\x00"
	echo -en "\x84\x04\x01\x00"
	echo -en "\x84\x05\x01\x00"
) | sudo tee /dev/ttyACM0 > /dev/null
