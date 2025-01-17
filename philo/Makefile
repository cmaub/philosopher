# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmaubert <cmaubert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/07 12:07:39 by cmaubert          #+#    #+#              #
#    Updated: 2025/01/17 10:45:26 by cmaubert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

# Compiler
# CC		= clang

CC		= cc
# CFLAGS	= -Wall -Wextra -Werror
CFLAGS	= -Wall -Wextra -Werror -pthread -g -fsanitize=thread


# Sources
SRC_PATH	=	src/
SRC_FILES	= 00_main.c \
				01_parsing.c \
				02_init.c \
				03_dinner.c \
				04_actions.c \
				05_monitor.c \
				handle_values_mutex.c \
				create_threads_mutex.c \
				print.c \
				utils.c \
				synchro.c \
				time.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC_FILES))

# Header
INC			=	-I ./incl
#HEADER	=	incl/philo.h

# Objects
OBJ_PATH	= obj/
OBJ_FILES	= $(SRC_FILES:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ_FILES))

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	echo "Removing .o object files..."
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean
