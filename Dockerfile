FROM ubuntu:18.04
RUN apt-get update
RUN apt-get update && apt-get install -y python3-pip git-core cmake
RUN pip3 install conan &&  conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
RUN conan install wt/4.0.3@bincrafters/stable -g cmake -s compiler.libcxx=libstdc++11 && find conanbuildinfo.cmake -type f -exec sed -i 's/wttest//g' {} \;

