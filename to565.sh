#!/bin/bash
if [ "$#" == "0" ]; then
	ME=`basename $0`
    echo "$ME <prefix>"
	echo "  prefix : convert <prefix>.png to <prefix>.rle"
	echo ""
	echo "  use ARIESVE=1 to use ARIESVE format (brg instead of rgb)"
	echo ""
    exit 1
fi

ARIESVE_ARGS=""
if [ "$ARIESVE" = "1" ]; then
	ARIESVE_ARGS="-ariesve"
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
convert -depth 8 $1.png rgb:$1.raw
$DIR/565tools $ARIESVE_ARGS -raw2rle -rle < $1.raw > $1.rle


