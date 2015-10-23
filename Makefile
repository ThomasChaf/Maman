CC=		gcc

SRC_DIR=	sources
INC_DIR=	includes
LIB_DIR=	libraries

SRC=		$(SRC_DIR)/main.c \
		$(SRC_DIR)/maman.c \
		$(SRC_DIR)/analyser.c \
		$(SRC_DIR)/curl_session.c \
		$(SRC_DIR)/gumbo_session.c

LDFLAGS=	-L./$(LIB_DIR)/modulary -lmod -L/usr/local/lib -lgumbo -lcurl

CFLAGS=		-I./$(LIB_DIR)/modulary -I./$(INC_DIR) -I/usr/local/include

OBJ=		$(SRC:.c=.o)

NAME=		maman

all:		$(NAME)

$(NAME):	$(OBJ)
		gcc -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
