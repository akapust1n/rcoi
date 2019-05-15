#!/bin/bash
cd /rcoi/backend
pkill -f bin/gateway
./bin/gateway  --docroot . --http-address 0.0.0.0 --http-port 8080 	> log-gateway.txt 2>&1


