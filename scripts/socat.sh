#!/bin/bash

INTERNAL_IP=$1

echo "sudo socat tcp-l:80,fork,reuseaddr tcp:${INTERNAL_IP}:8080"
sudo socat tcp-l:80,fork,reuseaddr tcp:${INTERNAL_IP}:8080

