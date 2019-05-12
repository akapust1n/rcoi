#!/bin/bash
pkill -f bin/news
pkill -f bin/comments
pkill -f bin/users
pkill -f bin/tests

./bin/news     --docroot . --http-address 0.0.0.0 --http-port 8081   	> log-news.txt 2>&1 &
./bin/comments --docroot . --http-address 0.0.0.0 --http-port 8082 	> log-comments.txt 2>&1 &
./bin/users    --docroot . --http-address 0.0.0.0 --http-port 8083  	> log-users.txt 2>&1 &
sleep 1 #wait servers start

./bin/tests
