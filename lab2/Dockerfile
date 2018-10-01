FROM akapust1n/test
RUN git clone https://github.com/akapust1n/rcoi
RUN cp /conanbuildinfo.cmake /rcoi/lab2/conanbuildinfo.cmake
RUN git checkout develop && cd rcoi/lab1 && cmake CMakeLists.txt && make 


