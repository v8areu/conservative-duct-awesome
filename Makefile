#   
#	File name		: Makefile
#	Date			: 
#	Version			: 
#	Author			: 
#

DEST			= .

HDRS			= global.hpp				\
			  preprocess/initial_parameters.hpp	\
			  preprocess/initial_condition.hpp	\
			  mainprocess/main_initial.hpp		\
			  mainprocess/main_predictor.hpp	\
			  mainprocess/main_corrector.hpp	\
			  mainprocess/main_final.hpp		\
			  misc/post_output.hpp			\

LIBS			=	

INPS			=	

COMPILER		= g++ 

OPTFLAG			= -std=c++11 -O2

MAKEFILE		= Makefile


PROGRAM			= Conservative_Duct

SRCS			= main.cpp				\
			  preprocess/initial_parameters.cpp	\
			  preprocess/initial_condition.cpp	\
			  mainprocess/main_initial.cpp		\
			  mainprocess/main_predictor.cpp	\
			  mainprocess/main_corrector.cpp	\
			  mainprocess/main_final.cpp		\
			  misc/post_output.cpp			\

OBJS			= $(SRCS:.cpp=.o) 	

.cpp.o:
			$(COMPILER) $(OPTFLAG) -c $*.cpp -o $*.o 

all:			$(PROGRAM)

$(PROGRAM):		$(OBJS) $(LIBS)
			@echo -n "Loading Program $(PROGRAM) ... "
			@$(COMPILER) $(OPTFLAG) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
			@echo "done"

clean:;			@rm -f $(SRCS:.cpp=.o) $(SRCS:.cpp=.il) $(PROGRAM)


