#!/bin/bash


BIN=./parser.exe
MUT=./mutate.exe
CORPUS=corpus
SEEDS=$CORPUS/seeds
KEEPERS=$CORPUS/keepers
CRASHES=$CORPUS/crashes
TMP=$CORPUS/tmp

while true; do
  for seed in $KEEPERS/*.bin $SEEDS/*.bin; do
	  [ -f "$seed" ] || continue

	  # mutate
	  rm -f $TMP/*
	  $MUT "$seed" "$TMP" 200

	  # testing mutations
	  for f in $TMP/*.bin; do
	    "$BIN" "$f"
	    ret=$?

	    if [ $ret -ge 128 ]; then
	      echo "[!!!] CRASH FOUND (ret=$ret)"
	      mv "$f" "$CRASHES/"
	      exit 0
	    fi

	    case $ret in
	      0|3|7|23)
	        rm -f "$f"
		;;
	      1)
		echo "[!!!] RET 1 FOUND"
		mv "$f" "$CRASHES/"
		exit 0
		;;
	      *)
                echo "[+] KEEP $ret"
		mv "$f" "$KEEPERS/"
		;;
	    esac
	done
  done
done
