# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmaubert <maubert.cassandre@gmail.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/07 12:07:39 by cmaubert          #+#    #+#              #
#    Updated: 2025/01/09 11:32:13 by cmaubert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

# Compiler
CC		= clang
CFLAGS	= -Wall -Wextra -Werror -pthread -g -fsanitize=thread
# CC		= cc
# CFLAGS	= -Wall -Wextra -Werror -pthread -g


# Sources
SRC_PATH	=	src/
SRC_FILES	= main.c \
				utils.c \
				parsing.c \
				init.c \
				dinner.c \
				handle_values_mutex.c \
				threads_mutex.c \
				write.c \
				monitor.c
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
