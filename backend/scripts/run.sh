#!/bin/bash
cd /rcoi/backend
pkill -f bin/gateway
pkill -f bin/news
pkill -f bin/comments
pkill -f bin/users
pkill -f bin/likehistory
./bin/gateway  --docroot . --http-address 0.0.0.0 --http-port 8080 	> log-gateway.txt 2>&1 &
./bin/news     --docroot . --http-address 0.0.0.0 --http-port 8081   	> log-news.txt 2>&1 &
./bin/comments --docroot . --http-address 0.0.0.0 --http-port 8082 	> log-comments.txt 2>&1 &
./bin/users    --docroot . --http-address 0.0.0.0 --http-port 8083  	> log-users.txt 2>&1 &
./bin/likehistory  --docroot . --http-address 0.0.0.0 --http-port 8084  > log-likehistory.txt 2>&1 &

echo "Services are started"
