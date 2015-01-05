#!/bin/sh


for a in "$@"; do
ARGS="$ARGS\"$a\" "
done

cd /sharedwine

if [ -e /usr/bin/gksudo ]; then
#  xhost SI:localuser:sharedwine
  exec gksudo -l -u sharedwine -u sharedwine -- sh -c "WINEPREFIX=/sharedwine/default wine $ARGS" 
else
  exec kdesudo -u sharedwine -u sharedwine sh -c "WINEPREFIX=/sharedwine/default wine $ARGS" 
fi

