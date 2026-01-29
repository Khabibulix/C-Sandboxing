#!/bin/bash

BIN=./parser.exe
DIR=cases
KEEPERS=keepers

for f in "$DIR"/*.bin; do
	$BIN "$f"
  	ret=$?

	case $ret in 
	  0|23|3|7)
	    rm -f "$f" 
	    ;;
	  *)
	    echo "KEEP $(basename "$f") -> $ret" 
	    mv "$f" "$KEEPERS/"
	    ;;
	esac
done
