#!/bin/bash
eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 .travis/travis # Allow read access to the private key
ssh-add .travis/travis # Add the private key to SSH

ssh -o "StrictHostKeyChecking no" apps@$IP  <<EOF
  docker pull akapust1n/courcework:lastest
  docker ps -a -q --filter ancestor=akapust1n/courcework | xargs -r docker rm -f
  docker rmi $(docker images --format '{{.Repository}}' | grep 'akapust1n/courcework') --force
  docker run -d -p 5001:3000 akapust1n/courcework
EOF
