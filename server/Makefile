CC=g++
FILE=myfrmd
CLASS=board
CFLAGS=-Wall -g -std=c++11
LDFLAGS=-lssl

all: $(FILE)

$(CLASS).o: $(CLASS).cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(FILE).o: $(FILE).cpp ../program4.h
	$(CC) $(CFLAGS) -c $< -o $@

$(FILE): $(FILE).o $(CLASS).o
	$(CC) $^ -o $@

clean:
	rm $(FILE) $(FILE).o
