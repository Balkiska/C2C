CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = c2_server

# Directories for source files
SRCS = \
  main.c \
  Database/database.c \
  Network/listener.c \
  Server/server.c \
  interface/server_ui.c

.c -> .o
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	# On ajoute des include paths : -I. -I./Database -I./Network ...
	$(CC) $(CFLAGS) -I. -I./Database -I./Network -I./Server -I./interface -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
