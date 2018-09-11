#!/bin/bash
eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 travis # Allow read access to the private key
ssh-add travis # Add the private key to SSH

docker save akapust1n/lab1 > lab1.tar
scp lab1.tar root@kapust1n.ru:/home/apps/

ssh -o "StrictHostKeyChecking no" apps@$IP  <<EOF
  pwd
  docker ps
  docker ps -a -q --filter ancestor=akapust1n/lab1 | xargs -r docker rm -f
  docker rmi $(docker images --format '{{.Repository}}' | grep 'akapust1n/lab1') --force
  docker load < lab1.tar
  docker run -d -p 5000:8080 akapust1n/lab1 
EOF
