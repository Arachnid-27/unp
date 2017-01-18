HDR = ../include
LIB = ../libunp.a

CC = gcc
CFLAGS = -D_REENTRANT -Wall -O2 -g -I $(HDR)
LDLIBS = $(LIB) -lpthread
