SRCS = $(shell find *.cpp)
OBJS = $(addsuffix .o, $(basename $(SRCS)))

CC = g++
CFLAGS = -Wall -O2 -std=c++11
LDFLAGS = -lwiringPi

ledmatrix5: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJS)
	rm -f ledmatrix5

.PHONY: ledmatrix5 clean 
