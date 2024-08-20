NAME = ircserv
NAME_BONUS = ircserv_bonus

SRCS = main.cpp \
		srcs/message.cpp \
		cmd/channel.cpp \
		cmd/command.cpp \
		cmd/privmsg.cpp \

SRC_BONUS = bonus/main_bonus.cpp \
			bonus/bot.cpp

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 

OBJS = $(SRCS:.cpp=.o)
OBJ_DIR_BONUS = $(SRC_BONUS:.cpp=.o)

all: $(NAME)
bonus : $(NAME_BONUS) $(NAME)

%.o: %.cpp srcs/client.hpp srcs/server.hpp cmd/command.hpp
	$(CC) $(FLAGS)  -c $< -o $@

$(NAME): $(OBJS) 
	$(CC) $(FLAGS) $^ -o $@

$(NAME_BONUS) : $(OBJ_DIR_BONUS)
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) $(OBJ_DIR_BONUS)

fclean: clean
	rm -rf $(NAME) $(NAME_BONUS)

re: fclean all
	
.PHONY : all clean fclean re