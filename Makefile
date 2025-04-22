NAME = ft_ls
OBJDIR = build
DEPDIR = dependancies
SRC = src/main.c src/usrInHandling.c src/file.c
SRCEXT = .c
OBJEXT = .o
OBJ = $(addprefix $(OBJDIR)/,$(SRC:$(SRCEXT)=$(OBJEXT)))
OBJOUT = $(addprefix $(OBJDIR)/,$(notdir $(OBJ)))
CC = cc
RM = rm -f
FLAGS = -Wall -Werror -Wextra -g
INCLUDES = -I./includes -I./$(DEPDIR)/libft
LIBS = -L./$(DEPDIR)/libft -lft -ltinfo

#=====================================
#===**DO NOT EDIT AFTER THIS LINE**===
#=====================================

all: directory clone  $(NAME)
clone:
	@if ! test -d $(DEPDIR)/libft; then\
		git clone git@github.com:HaSYxD/libft.git $(DEPDIR)/libft;\
	fi
directory:
	@if ! test -d $(DEPDIR); then\
		mkdir $(DEPDIR);\
	fi
	@if ! test -d $(OBJDIR); then\
		mkdir $(OBJDIR);\
	fi

$(NAME): $(OBJ)
	cd $(DEPDIR)/libft;make
	$(CC) $(OBJOUT) $(FLAGS) $(LIBS) -o $(NAME)

$(OBJDIR)/%$(OBJEXT): %$(SRCEXT)
	$(CC) $(INCLUDES) $(FLAGS) -c $< -o $(addprefix $(OBJDIR)/,$(notdir $@))

clean:
	$(RM) $(OBJOUT)

fclean: clean
	cd $(DEPDIR)/libft;make fclean
	$(RM) $(NAME)
	@if test -d $(OBJDIR); then\
		rm -r $(OBJDIR);\
	fi

wipe: fclean
	@if test -d $(DEPDIR)/libft; then\
		rm -rf $(DEPDIR)/libft;\
	fi
	$(RM) $(NAME)

re: fclean all
