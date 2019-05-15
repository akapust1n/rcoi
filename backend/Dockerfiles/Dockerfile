FROM akapust1n/rcoi-base
RUN git clone https://github.com/akapust1n/rcoi
RUN cd rcoi  && cd backend && cp ../../conanbuildinfo.cmake /rcoi/backend/conanbuildinfo.cmake  && cmake CMakeLists.txt -DDOCKERCOMPOSE=1 && make
