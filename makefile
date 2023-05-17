CXX=gcc
CXXFLAG=-Wall -Wcast-align

INCLUDE=
OBJDIR=./src/
OBJ=$(OBJDIR)*.c
EXECDIR=./lib/
EXECUTABLE=$(EXECDIR)termsnake

all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJ)
	mkdir -p $(EXECDIR)
	$(CXX) $(CXXFLAG) $(INCLUDE) $(OBJ) -o $@

clean:
	rm -rf $(EXECDIR)
