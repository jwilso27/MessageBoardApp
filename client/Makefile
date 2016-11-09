CC=g++
FILE=myfrm
CFLAGS=-Wall -g -std=c++11
LDFLAGS=-lssl

all: $(FILE)

$(FILE).o: $(FILE).cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(FILE): $(FILE).o
	$(CC) $< -o $@

clean:
	rm $(FILE) $(FILE).o
