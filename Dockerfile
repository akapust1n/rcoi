FROM ubuntu:18.04
RUN apt-get update
RUN apt-get update && apt-get install -y python3-pip git-core cmake libmysqlclient20 libmysqlclient-dev 
RUN pip3 install conan &&  conan remote add akap "https://api.bintray.com/conan/akapust1n/wt" && conan remote add bincrafters  "https://api.bintray.com/conan/bincrafters/public-conan"
RUN echo "[requires]" >conanfile.txt  && echo "wt/4.0.4.1@akapust1n/stable">>conanfile.txt && echo "fmt/5.2.0@bincrafters/stable" >> conanfile.txt && echo "jsonformoderncpp/3.2.0@vthiery/stable" >> conanfile.txt  && echo "gtest/1.8.1@bincrafters/stable" >> conanfile.txt && echo "boost_property_tree/1.66.0@bincrafters/stable" >> conanfile.txt && echo "[generators]">>conanfile.txt && echo "cmake" >>conanfile.txt 
RUN conan install . -s compiler.libcxx=libstdc++11 --build wt
RUN apt-get update && apt-get -y install curl gnupg && curl -sL https://deb.nodesource.com/setup_8.x  | bash - && apt-get -y install nodejs
