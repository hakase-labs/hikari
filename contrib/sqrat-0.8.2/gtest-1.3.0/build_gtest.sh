#!/bin/sh -ex
g++ -I. -I./include -c src/gtest-all.cc && \
ar -rv libgtest.a gtest-all.o