# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 17:41:32 by lscheupl          #+#    #+#              #
#    Updated: 2025/04/01 16:38:47 by lscheupl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc -Wall -Wextra -Werror -g3

SRC = main.c
SRC += routine_utils.c
SRC += routine_utils_2.c
SRC += utils.c
SRC += routine.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	
re: fclean all