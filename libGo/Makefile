# GO Framework's makefile
# September, 19th 2008

CC = gcc
CFLAGS = -Wall -O3
LIB_NAME = libgo.a
INCLUDES = .
LIBS = -lm 

OBJ_FILES = gotcp.o goudp.o gomd5.o gocrypto.o goutil.o \
	    golog.o gopacket.o gosort.o

INSTALL_DIR = /usr/bin

all : $(LIB_NAME)

install:
	@cp libgo.a /usr/lib/
	@mkdir -p /usr/include/libgo
	@cp go*.h /usr/include/libgo/
	@echo "Fichiers installes"
	
clean :
	@rm -rf $(LIB_NAME) *~ *.o
	@echo "Nettoyage effectué"

$(LIB_NAME) : $(OBJ_FILES)
	@rm -rf $(LIB_NAME) 
	@ar cq $(LIB_NAME) $(OBJ_FILES)
	@echo "Bibliotheque créée."
	@rm $(OBJ_FILES)
	@echo "Objets nettoyés."

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@
	@echo "Compilation de $<..."
