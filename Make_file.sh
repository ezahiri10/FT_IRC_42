#!/bin/bash

SRC_FILES=$(find . -type f -name "*.cpp" | sed 's|^\./||' | tr '\n' ' ')
HEADER_FILES=$(find . -type f -name "*.hpp" | sed 's|^\./||' | tr '\n' ' ')

cat << EOF > Makefile
CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ${SRC_FILES}
HEADER = ${HEADER_FILES}

OBJ = \$(SRC:.cpp=.o)

NAME = a.out

all: \$(NAME)

\$(NAME): \$(OBJ)
	\$(CC) \$(CFLAGS) \$(OBJ) -o \$(NAME)

# Rule to compile .cpp files in subdirectories
%.o: %.cpp \$(HEADER)
	\$(CC) \$(CFLAGS) -c \$< -o \$@

clean:
	rm -f \$(OBJ)

fclean: clean
	rm -f \$(NAME)

re: fclean all
EOF
