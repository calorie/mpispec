# define some aliases

CC=mpicc

INCLUDE=/usr/local/include/mpispec

CFLAGS=-g -I$(INCLUDE)

LIB=/usr/local/lib/libcspec.a

#SRC = fabs_desc.c main.c
OBJ = fabs_desc.o main.o
EXE = sample

all: $(EXE)

$(EXE): $(OBJ) $(INCLUDE)/*.h $(LIB)
	$(CC) -o $(EXE) $(OBJ) $(LIB)

clean:
	rm $(EXE) $(OBJ)
