CFLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address
SRCS =	main.cpp\
		Client.cpp\
		Server.cpp\
		Channel.cpp\
		operators/Kick.cpp\
		operators/Mode.cpp\
		operators/Invite.cpp\
		operators/Topic.cpp\
		operators/utils.cpp

NAME = ircserv

OBJECTS = $(SRCS:.cpp=.o)

RM = rm -f

all: $(NAME)

%.o: %.cpp  Server.hpp Client.hpp Channel.hpp
	c++ $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJECTS)
	c++ $(CFLAGS) -o $(NAME) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re: fclean all