CXXFLAG = -Wall -Wextra -Werror -std=c++98
NAME = webserv
NAME2 = cgi.exe
SRCS = srcs/main.cpp srcs/Post.cpp srcs/Get.cpp srcs/util.cpp srcs/htmlheader.cpp\
		class/My_socket.cpp class/Member.cpp class/MemberRepository.cpp class/Serv.cpp
CGIS = cgi/cgi_main.cpp cgi/cgi_exe.cpp cgi/util.cpp cgi/Member.cpp cgi/MemberRepository.cpp
OBJS = $(SRCS:.cpp=.o)
OBCS = $(CGIS:.cpp=.o)

all : $(NAME) $(NAME2)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAG) -o $(NAME) $(OBJS)

$(NAME2) : $(OBCS)
	$(CXX) $(CXXFLAG) -o $(NAME2) $(OBCS)

.cpp.o :
	$(CXX) $(CXXFLAG) -c $< -o $@

clean :
	rm -rf */*.o *.o

fclean : clean
	rm -rf memberdb.txt
	rm -rf $(NAME)
	rm -rf $(NAME2)

re : fclean all