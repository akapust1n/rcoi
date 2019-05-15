#!/bin/bash
cd /rcoi/backend
pkill -f bin/news
./bin/news     --docroot . --http-address 0.0.0.0 --http-port 8081   	> log-news.txt 2>&1 


