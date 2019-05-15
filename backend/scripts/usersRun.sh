#!/bin/bash
cd /rcoi/backend
pkill -f bin/users
./bin/users    --docroot . --http-address 0.0.0.0 --http-port 8083  	> log-users.txt 2>&1 
