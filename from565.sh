#!/bin/bash
if [ "$#" == "0" ]; then
	ME=`basename $0`
    echo "$ME <prefix>"
	echo "  prefix : convert <prefix>.rle to <prefix>.png"
	echo ""
	echo "  use ARIESVE=1 to use ARIESVE format (brg instead of rgb)"
	echo "  use SIZE=<width>x<height> to specify image size (default 480x800)"
	echo ""
    exit 1
fi

ARIESVE_ARGS=""
if [ "$ARIESVE" = "1" ]; then
	ARIESVE_ARGS="-ariesve"
fi
SIZE_ARGS="-size 480x800"
if [[ -n "$SIZE" ]] ; then
	SIZE_ARGS="-size $SIZE"
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/565tools $ARIESVE_ARGS -rle2raw < $1.rle > $1.raw
convert -depth 8 $SIZE_ARGS rgb:$1.raw $1.png

