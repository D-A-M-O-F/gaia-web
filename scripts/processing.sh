#!/bin/bash

SESSION_ID=$1
GAIA_BIN_PATH="./"
DEV_PATH="/dev/shm/"

if [ -z "${SESSION_ID}" ]
then
  echo "./processing.sh <SESSION_ID> <GAIA_BIN_PATH> <DEV_PATH>"
  exit 1
fi

if [ ! -z "${2}" ]
then
  GAIA_BIN_PATH=$2
fi

if [ ! -z "${3}" ]
then
  DEV_PATH=$3
fi

ROOT_PATH="${DEV_PATH}/${SESSION_ID}"

OUTPUT_ZIP=output.zip

CONSOLE_LOG=${ROOT_PATH}/console.log
exec > ${CONSOLE_LOG} 2>&1

cd "${ROOT_PATH}/gtfs_in"
unzip -o input.zip

cd "${ROOT_PATH}"
echo ${GAIA_BIN_PATH}/gaia -c ./rcfg/gaia.cfg
${GAIA_BIN_PATH}/gaia -c ./rcfg/gaia.cfg

retVal=$?
echo "[${GAIA_BIN_PATH}/gaia -c ./rcfg/gaia.cfg] return value [${retVal}]"

cd "${ROOT_PATH}"

if [ -f "${OUTPUT_ZIP}" ]
then
  echo "rm ${OUTPUT_ZIP}"
  rm ${OUTPUT_ZIP}
fi

if [ $retVal -eq 0 ]
then
  echo "zip ${OUTPUT_ZIP} gtfs_out/* log/*"
  zip ${OUTPUT_ZIP} gtfs_out/* log/*
else
  echo "zip ${OUTPUT_ZIP} console.log"
  zip ${OUTPUT_ZIP} console.log
fi


echo `pwd`
