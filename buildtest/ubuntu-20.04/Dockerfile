FROM ubuntu:20.04

ARG user
ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN mkdir -p /usr/lib/debug/usr
RUN ln -s /usr/lib/debug/lib /usr/lib/debug/usr/lib
RUN apt-get update && apt-get install -y sudo
RUN adduser ${user:-user} && \
    echo "${user:-user} ALL=(root) NOPASSWD:ALL" > /etc/sudoers && \
    chmod 0440 /etc/sudoers
RUN mkdir -p /usr/local/src && chown root:user /usr/local/src && \
   chmod g+w /usr/local/src
USER ${user:-user}
RUN sudo apt-get install -y bison flex texinfo git build-essential bsdmainutils
RUN cd /usr/local/src && git clone https://github.com/stephenrkell/binutils-gdb.git
RUN cd /usr/local/src/binutils-gdb && \
   chmod +x configure && \
   CFLAGS="-fPIC -g -O2" ./configure --prefix=/usr/local \
     --enable-gold --enable-plugins --enable-install-libiberty && \
   make -j4 && sudo make install
RUN cd /usr/local/bin && sudo rm -f ld && sudo ln -s ld.bfd ld
RUN sudo mkdir -p /usr/lib/meta && sudo chown root:staff /usr/lib/meta && \
   sudo chmod g+w /usr/lib/meta
RUN sudo apt-get install -y libelf-dev libdw-dev binutils-dev \
       autoconf automake libtool pkg-config autoconf-archive \
       g++ ocaml ocamlbuild ocaml-findlib libnum-ocaml-dev \
       default-jre-headless default-jdk-headless python3 python \
       make git gawk gdb wget \
       libunwind-dev libc6-dev-i386 zlib1g-dev libc6-dbg \
       libboost-iostreams-dev libboost-regex-dev libboost-serialization-dev libboost-filesystem-dev
RUN sudo ln -s /usr/lib/debug/lib/x86_64-linux-gnu/libc-2.31.so /usr/lib/debug/usr/lib/x86_64-linux-gnu/libc-2.31.so
RUN cd /usr/local/src && git clone https://github.com/ChrisBaish/liballocs.git # Use my version as it has the patched CIL for 4.08. Put this back to Stephen's version once the CIL patch has been accepted upstream
RUN cd /usr/local/src/liballocs && \
   git submodule update --init --recursive && \
   make -C contrib -j4
RUN cd /usr/local/src/liballocs && \
   ./autogen.sh && \
   (. contrib/env.sh && ./configure --prefix=/usr/local) && \
   make -j4
RUN sudo mkdir -p /usr/lib/meta && sudo chown root:user /usr/lib/meta && \
   sudo chmod g+w /usr/lib/meta

RUN sudo mkdir -p /usr/lib/debug/.build-id/f3/
RUN sudo ln -s `find /usr/lib/debug/.build-id/ -name "*.debug" | head -n1` /usr/lib/debug/.build-id/f3/ff3fda80b817c464a56eed59ff09dc864eaeb0.debug # Symlink the debug file into the expected place
RUN cd /usr/local/src/liballocs && \
   make -f tools/Makefile.meta /usr/lib/meta/lib/x86_64-linux-gnu/libc-2.31.so-meta.so
