# Makefile

CC = gcc
CFLAGS = -Wall -Wextra

# Lista de fișiere sursă și obiect
SRC_FILES = imap.c smtp.c mail.c libStruct.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Numele executabilului
EXECUTABLE = mail_executable

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c libStruct.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)
