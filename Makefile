CXX=g++
CXXFLAGS=-O2 -flto=auto -I./include -std=c++20
LDFLAGS=-lcrypto

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

all: birbd

birbd: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ /usr/lib/birb/libbirb.a

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm -f *.o
	rm -f birbd

.PHONY: all clean
