#!/bin/bash

cat globalState | while read -n 1 i; do
echo $i
if (($i == 0))
  then
    ./aggregateTime.o
  fi
done

source report.sh
