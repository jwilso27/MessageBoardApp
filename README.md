# MessageBoardApp
## CSE 30264

Jenna Wilson (jwilso27)
&
Madelyn Nelson (mnelso12)

### Files:
- program4.h: file containing all included libraries and functions used
- client subdirectory: includes myfrm.cpp and Makefile
- server subdirectory: includes myfrmd.cpp, Makefile, board.cpp and board.h for
  the board class, and one .board file pertaining to each existing message board 
  files (requires access to /afs/nd.edu)

### Usage:

Compile server and client
```
> cd client
> make
> cd ../server
> make
```

Run server
```
> myfrmd [port number] [admin password]
Username: [username]
Create Password: [user password]
```

Run client
```
> myfrm [hostname] [port number]
```

Enter commands from client

- CRT: create board
```
What operation would you like to execute?
> CRT
What is the name of the board that would you like to create?
> myBoard
Board successfully created
```

- LIS: list boards
```
What operation would you like to execute?
> LIS

myBoard

```

- MSG: leave message
```
What operation would you like to execute?
> MSG
What board would you like to leave a message on?
> myBoard
What message would you like to leave?
> Go Cubs
Message was successfully added to board

```

- DLT: delete message
```
What operation would you like to execute?
> DLT
What board would you like to delete a message from?
> myBoard
What message would you like to delete?
(Please enter the index number of the message)
> 0 
Message was successfully deleted from board

```

- RDB: read board
```
What operation would you like to execute?
> RDB
What board would you like to read?
> myBoard

mnelso12
[0] Go Cubs - mnelso12
[1] Go Irish - mnelso12

```
- EDT: edit message
```
What operation would you like to execute?
> EDT
What board would you like to edit a message from?
> myBoard
What message would you like to edit?
(Please enter the index number of the message)
> 0
What is the new message would you like to leave?
> World Series Champs
Message was successfully edited

```

- APN: append file
```
What operation would you like to execute?
> APN
What board would you like to append an attachment to?
> myBoard
What file would you like to attach?
> Makefile
File was successfully appended

```

- DWN: download file
```
What operation would you like to execute?
> DWN
What board would you like to download an attachment from?
> myBoard
What file would you like to download?
> Makefile
File downloaded successfully

```

- DST: destroy board
```
What operation would you like to execute?
> DST
What board would you like to destroy?
> myBoard
Board was successfully destroyed
```

- XIT: exit
```
The session has been closed
```
The server remains running

- SHT: shutdown server
```
What operation would you like to execute?
> SHT
Admin Password: [admin password]
The session has been closed
```
The server stops running, as well as the client
