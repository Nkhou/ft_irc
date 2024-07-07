NAME = ircserv

SRC = main.cpp \
	  srcs/message.cpp \

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address

OBJ_DIR = $(SRC:.cpp=.o)

all : $(NAME)

%.o: %.cpp srcs/server.hpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ_DIR)
	$(CC) $(FLAGS) $^ -o $@

clean :
	rm -f $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re