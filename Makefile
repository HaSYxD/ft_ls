NAME = ft_ls
OBJDIR = build
DEPDIR = dependancies
SRC = src/main.c src/usrInHandling.c src/file.c src/display.c
SRCEXT = .c
OBJEXT = .o
OBJ = $(SRC:$(SRCEXT)=$(OBJEXT))
CC = cc
RM = rm -f
FLAGS = -Wall -Werror -Wextra -g
INCLUDES = -I./includes -I./$(DEPDIR)/libft
LIBS = -L./$(DEPDIR)/libft -lft -ltinfo

#=====================================
#===**DO NOT EDIT AFTER THIS LINE**===
#=====================================

all: clone  $(NAME)

clone:
	@if ! test -d $(DEPDIR)/libft; then\
		git clone -b \(Feature\)---A-complete-arena-allocator-aiming-to-replace-the-garbage-collector git@github.com:HaSYxD/libft.git $(DEPDIR)/libft;\
	fi

$(NAME): $(OBJ)
	cd $(DEPDIR)/libft;make
	$(CC) $(OBJ) $(FLAGS) $(LIBS) -o $(NAME)

%$(OBJEXT): %$(SRCEXT)
	$(CC) $(INCLUDES) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

wipe: fclean
	@if test -d $(DEPDIR)/libft; then\
		rm -rf $(DEPDIR)/libft;\
	fi
	$(RM) $(NAME)

re: fclean all
