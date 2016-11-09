CC=g++
FILE=myfrmd
CLASS=board
CFLAGS=-Wall -g
LDFLAGS=-lssl

all: $(FILE)

$(CLASS).o: $(CLASS).cpp $(CLASS).h
	$(CC) $(CFLAGS) -c $(CLASS).cpp -o $@

$(FILE).o: $(FILE).cpp $(CLASS).h
	$(CC) $(CFLAGS) -c $(FILE).cpp -o $@

$(FILE): $(FILE).o $(CLASS).o
	$(CC) $(LDFLAGS) $< -o $@

clean:
	rm $(FILE) $(FILE).o
