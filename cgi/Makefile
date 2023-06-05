CXXFLAG = -Wall -Wextra -Werror -std=c++98
NAME = cgi.exe
SRCS = cgi_exe.cpp cgi_main.cpp Member.cpp MemberRepository.cpp util.cpp
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAG) -o $(NAME) $(OBJS)

.cpp.o :
	$(CXX) $(CXXFLAG) -c $< -o $@

clean :
	rm -rf */*.o *.o

fclean : clean
	rm -rf $(NAME)

re : fclean all