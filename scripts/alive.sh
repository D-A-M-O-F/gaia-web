#!/bin/bash

RESULT=`ps -efa | grep -v grep | grep gaia-web -c`

if [ $RESULT -eq 1 ]
then
  exit 0
else
  exit 1
fi
