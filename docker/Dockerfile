FROM dockcross/windows-shared-x86
RUN cd /usr/src/mxe && make json-c -j6
RUN cd /usr/src/mxe && make curl -j6
RUN cd /usr/src/mxe && make readline -j6
