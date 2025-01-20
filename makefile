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

# Source files for the Malware component
SRC_MALWARE = $(SRC_DIR_MALWARE)/connect.c \
              $(SRC_DIR_MALWARE)/hide.c \
              $(SRC_DIR_MALWARE)/main.c

# Default target: build both executables
all: $(C2_EXEC) $(MALWARE_EXEC)

# Compile the C2 component
$(C2_EXEC): $(OBJ_C2)
	$(CC) $(CFLAGS) -o $@ $^

# Compile the Malware component
$(MALWARE_EXEC): $(OBJ_MALWARE)
	$(CC) $(CFLAGS) -o $@ $^

# Generate object files from source files
OBJ_C2 = $(SRC_C2:.c=.o)
OBJ_MALWARE = $(SRC_MALWARE:.c=.o)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files and executables
clean:
	rm -f $(OBJ_C2) $(OBJ_MALWARE) $(C2_EXEC) $(MALWARE_EXEC)

# Clean everything including backup files
fclean: clean
	rm -f *~ *.bak

# Recompile everything from scratch
re: fclean all

# Prevents conflicts with files named as the targets
.PHONY: all clean fclean re
