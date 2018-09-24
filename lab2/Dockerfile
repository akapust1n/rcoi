FROM akapust1n/test
RUN git clone https://github.com/akapust1n/rcoi
RUN cp /conanbuildinfo.cmake /rcoi/lab1/conanbuildinfo.cmake
RUN cd rcoi/lab1 && cmake CMakeLists.txt && make 
CMD rcoi/lab1/bin/lab1 --docroot . --http-address 0.0.0.0 --http-port 8080

