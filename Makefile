CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g 

SRC = Authentication/Authentication.cpp Client.cpp main.cpp Server.cpp\
		Authentication/exec_cmds.cpp \
		Authentication/help_func.cpp \
		Channel.cpp\
		operators/privMsg.cpp\
		operators/Kick.cpp\
		operators/Mode.cpp\
		operators/Invite.cpp\
		operators/Topic.cpp\
		operators/Operators.cpp
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
