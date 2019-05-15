#!/bin/bash
cd /rcoi/backend
pkill -f bin/comments
./bin/comments --docroot . --http-address 0.0.0.0 --http-port 8082 	> log-comments.txt 2>&1 



