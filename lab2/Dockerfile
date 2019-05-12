FROM akapust1n/rcoi-base
RUN git clone https://github.com/akapust1n/rcoi
RUN cd rcoi  && cd lab2 && cp ../../conanbuildinfo.cmake /rcoi/lab2/conanbuildinfo.cmake  && cmake CMakeLists.txt  && make
RUN chmod +x /rcoi/lab2/prodRun.sh
CMD /rcoi/lab2/prodRun.sh