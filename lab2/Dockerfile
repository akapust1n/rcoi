FROM akapust1n/test
RUN git clone https://github.com/akapust1n/rcoi
RUN cd rcoi && git checkout develop && cd lab2 && cp ../../conanbuildinfo.cmake /rcoi/lab2/conanbuildinfo.cmake  && cmake CMakeLists.txt -DIS_TEST_BUILD=1 -DDOCKER=1 && make



