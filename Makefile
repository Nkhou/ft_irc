NAME = ircserv
NAME_BONUS = ircserv_bonus

SRC = main.cpp \
	  srcs/message.cpp \
	  cmd/channel.cpp \
		cmd/command.cpp \

	  #bonus/srcs/main_bonus.cpp \

SRC_BONUS = bonus/main_bonus.cpp \
			bonus/bot.cpp


CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address

OBJ_DIR = $(SRC:.cpp=.o)

OBJ_DIR_BONUS = $(SRC_BONUS:.cpp=.o)

all : $(NAME) #$(NAME_BONUS)
bonus : $(NAME_BONUS) $(NAME)


%.o: %.cpp srcs/*.hpp cmd/*.hpp bonus/srcs/server.hpp bonus/bot.hpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ_DIR) #$(OBJ_DIR_BONUS)
	$(CC) $(FLAGS) $^ -o $@

$(NAME_BONUS) : $(OBJ_DIR_BONUS)
	$(CC) $(FLAGS) $^ -o $@

clean :
	rm -f $(OBJ_DIR) $(OBJ_DIR_BONUS)

fclean : clean
	rm -f $(NAME) $(NAME_BONUS)

re : fclean all

.PHONY : all clean fclean re