FROM akapust1n/rcoi-base
RUN git clone https://github.com/akapust1n/rcoi
RUN cd rcoi  && cd lab2 && cp ../../conanbuildinfo.cmake /rcoi/lab2/conanbuildinfo.cmake  && cmake CMakeLists.txt  && make
RUN cd /rcoi/lab3/ && npm install && chmod +x /rcoi/lab2/run.sh
RUN pwd
CMD /rcoi/lab2/run.sh