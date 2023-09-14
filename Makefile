CXX=g++
CXXFLAGS=-O2 -flto=auto -I./include -std=c++20 -pthread
LDFLAGS=-lcrypto

BIN=birbd
PREFIX=/usr/local

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

all: birbd

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

install:
	cp ./$(BIN) $(DESTDIR)$(PREFIX)/bin/
	[ -f /etc/birbd.conf ] || cp ./birbd.conf $(DESTDIR)/etc/birbd.conf

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)

clean:
	rm -f *.o
	rm -f $(BIN)

.PHONY: all install uninstall clean
