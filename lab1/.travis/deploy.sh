#!/bin/bash
eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 travis # Allow read access to the private key
ssh-add travis # Add the private key to SSH

docker save akapust1n/lab1 > lab1.tar
scp lab1.tar root@kapust1n.ru:/home/apps/
# Skip this command if you don't need to execute any additional commands after deploying.
ssh -o "StrictHostKeyChecking no" apps@$IP  <<EOF
  pwd
  docker load < lab1.tar
  docker stop $(docker ps -q --filter ancestor=akapust1n/lab1 )  
  docker run -d -p 5000:8080 akapust1n/lab1 
EOF
