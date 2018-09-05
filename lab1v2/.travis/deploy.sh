eval "$(ssh-agent -s)" # Start ssh-agent cache
chmod 600 .travis/travis # Allow read access to the private key
ssh-add .travis/travis # Add the private key to SSH


# Skip this command if you don't need to execute any additional commands after deploying.
ssh -o "StrictHostKeyChecking no" apps@$IP -p $PORT <<EOF
  pwd
  cd $WDEPLOY_DIR
  git reset --hard origin/backend~1 
  git pull  origin backend
  cd backend
  rm *.o Makefile
  conan install .
  qmake
  make
  doxygen cf.conf
  pkill -f backend
  nohup ./backend > log.out 2> log.err < /dev/null &
EOF
