FROM ubuntu:20.04

WORKDIR /src

COPY brick_game/tetris/* /src/brick_game/tetris/
COPY gui/cli/* /src/gui/cli/
COPY tests/* /src/tests/
COPY tetris.c /src/
COPY tetris.h /src/
COPY Makefile /src/

RUN apt-get update && \
    apt-get install -y apt-transport-https ca-certificates && \
    DEBIAN_FRONTEND=noninteractive apt-get install \
    -y valgrind build-essential make check \
    libncurses5-dev libsqlite3-dev nano && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*


# docker build -t my-ubuntu-valgrind .    
# docker run --rm -it my-ubuntu-valgrind bash