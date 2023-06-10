CXXFLAG = -Wall -Wextra -Werror -std=c++98
NAME = irc
SRCS = srcs/main.cpp class/Serv.cpp class/Member.cpp 
OBJS = $(SRCS:.cpp=.o)

all : $(NAME) $(NAME2)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAG) -o $(NAME) $(OBJS)

.cpp.o :
	$(CXX) $(CXXFLAG) -c $< -o $@

clean :
	rm -rf */*.o *.o

fclean : clean
	rm -rf $(NAME)

re : fclean all