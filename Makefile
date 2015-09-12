CC=			gcc

SRC_DIR=	sources
INC_DIR=	includes
LIB_DIR=	libraries

SRC=		$(SRC_DIR)/main.c \
			$(SRC_DIR)/curl_session.c \
			$(SRC_DIR)/gumbo_session.c

LDFLAGS=	-L./$(LIB_DIR)/modulary -lmod

CFLAGS=		-I./$(LIB_DIR)/modulary -I./$(INC_DIR)

OBJ=		$(SRC:.c=.o)

NAME=		maman

all:		$(NAME)

$(NAME):	$(OBJ)
			gcc `pkg-config --cflags --libs gumbo` `curl-config --libs` -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
