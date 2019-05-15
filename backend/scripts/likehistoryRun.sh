#!/bin/bash
cd /rcoi/backend
pkill -f bin/likehistory
./bin/likehistory  --docroot . --http-address 0.0.0.0 --http-port 8084  > log-likehistory.txt 2>&1
