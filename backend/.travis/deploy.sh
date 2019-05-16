#!/bin/bash
eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 .travis/travis # Allow read access to the private key
ssh-add .travis/travis # Add the private key to SSH

ssh -o "StrictHostKeyChecking no" apps@$IP  <<EOF
  cd /home/apps/rcoi && git add . && git reset --hard HEAD~1 && git pull
  cd front
  npm install . && npm run build
  cd ../Dockerfiles && docker stack deploy --compose-file=docker-compose.yaml courcework-swarm
EOF
