FROM ubuntu:18.04
RUN apt-get update
RUN apt-get update && apt-get install -y python3-pip git-core cmake
RUN pip3 install conan &&  conan remote add akap "https://api.bintray.com/conan/akapust1n/wt"
RUN conan install wt/4.0.4.1@akapust1n/stable &&  conan install fmt/5.2.0@bincrafters/stable && conan install

