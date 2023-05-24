CXXFLAG = -Wall -Wextra -Werror -std=c++98
NAME = webserv
SRCS = srcs/main.cpp srcs/Post.cpp srcs/Get.cpp srcs/util.cpp srcs/htmlheader.cpp\
		class/My_socket.cpp class/Member.cpp class/MemberRepository.cpp
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