CC=gcc
FILE=myfrmd
CFLAGS=-Wall -g
LDFLAGS=-lssl

all: $(FILE)

$(FILE).o: $(FILE).c
	$(CC) $(CFLAGS) -c $< -o $@

$(FILE): $(FILE).o
	$(CC) $(LDFLAGS) $< -o $@

clean:
	rm $(FILE) $(FILE).o
