CC=g++
FILE=myfrmd
CLASS=board
CFLAGS=-Wall -g -std=c++11
LDFLAGS=-lssl

all: $(FILE)

$(CLASS).o: $(CLASS).cpp
	$(CC) $(CFLAGS) -c $(CLASS).cpp -o $@

$(FILE).o: $(FILE).cpp
	$(CC) $(CFLAGS) -c $(FILE).cpp -o $@

$(FILE): $(FILE).o $(CLASS).o
	$(CC) $(FILE).o $(CLASS).o -o $@

clean:
	rm $(FILE) $(FILE).o
