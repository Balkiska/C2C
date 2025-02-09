# Project name
PROJECT_NAME = C2C

# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

# Directories for source files
SRC_DIR_C2 = C2
SRC_DIR_MALWARE = Malware
OBJS = main.o listener.o server.o database.o
all: c2_program

# final executable, "c2_program", is created by linking all object files
c2_program: $(OBJS)
	$(CC) $(CFLAGS) -o c2_program $(OBJS)

# main.o depends on main.c, as well as headers from listener.h and server.h if they're used
main.o: main.c listener.h server.h
	$(CC) $(CFLAGS) -c main.c

# listener.o depends on listener.c and listener.h
listener.o: listener.c listener.h
	$(CC) $(CFLAGS) -c listener.c

# server.o depends on server.c and its headers, including database.h
server.o: server.c server.h database.h
	$(CC) $(CFLAGS) -c server.c

# database.o depends on database.c and database.h
database.o: database.c database.h
	$(CC) $(CFLAGS) -c database.c

# "clean" target: remove all temporary files and the final binary
clean:
	rm -f $(OBJS) c2_program credentials.bin temp.bin
