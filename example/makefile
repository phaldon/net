CC=clang++
#CC=g++-10

TEST := \
.timer.o\
.prog.o

TEST2 := \
.test.o

NETPATH := ..
GRAPHVIZPATH := /usr/local/Cellar/graphviz/2.44.1
TATPATH := /Users/chaowang/Desktop/work/program/TAT
LINKOPT := -lblas -llapack -L $(GRAPHVIZPATH)/lib -lgvc -lcgraph 
MAKEOPT := -g -std=c++17  -O3 -I $(TATPATH)/include -DTAT_USE_VALID_DEFAULT_TENSOR -DTAT_USE_SIMPLE_NAME -DTAT_USE_SINGULAR_MATRIX -DTAT_USE_SIMPLE_NOSYMMETRY -DTAT_USE_COPY_WITHOUT_WARNING -I $(GRAPHVIZPATH)/include -I$(NETPATH)/include -DNET_GRAPH_VIZ -DNET_SHOW_FIG_USE_ITERM

all: run.out 

test.out:$(TEST2)
	$(CC) $(FFLAGS) -o $@ $^ $(LINKOPT) 
run.out:$(TEST)
	$(CC) $(FFLAGS) -o $@ $^ $(LINKOPT) 
.%.o :	%.cpp
	$(CC) $(FFLAGS) -c -o $@ -c $(<) $(MAKEOPT)
clean:
	rm .*.o *.mod *.out 
.PHONY:	all clean test.out
