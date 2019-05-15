#!/bin/bash
eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 .travis/travis # Allow read access to the private key
ssh-add .travis/travis # Add the private key to SSH

ssh -o "StrictHostKeyChecking no" apps@$IP  <<EOF
  cd /home/apps/rcoi && git add . && git reset --hard HEAD~1 && git pull
  cd front
  npm install . && npm run build
  docker ps -a -q --filter ancestor=akapust1n/courcework | xargs -r docker rm -f
  docker rmi $(docker images |grep  'akapust1n/courcework') --force
  docker run -d -p 8080:8080 akapust1n/courcework
EOF
