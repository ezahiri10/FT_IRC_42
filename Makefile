CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address #-g 

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

BOTCPP = BOTDIR/Bot.cpp BOTDIR/Player.cpp BOTDIR/main.cpp

BOTHPP = BOTDIR/Bot.hpp BOTDIR/Player.hpp

HEADER = Client.hpp Replies.hpp Server.hpp 

OBJ = $(SRC:.cpp=.o)

NAME = ircserv

BOT = bot

all: $(BOT) $(NAME)

BOTDIR/%.o: BOTDIR/%.cpp $(BOTHPP)
	$(CC) $(CFLAGS) -c $< -o $@

$(BOT): $(BOTCPP)
	$(CC) $(CFLAGS) $(BOTCPP) -o $(BOT)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BOT)

fclean: clean
	rm -f $(NAME) $(BOT)

re: fclean all