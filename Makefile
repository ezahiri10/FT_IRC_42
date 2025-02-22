CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = Authentication.cpp Client.cpp main.cpp Server.cpp 
HEADER = Client.hpp Replies.hpp Server.hpp 

OBJ = $(SRC:.cpp=.o)

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
