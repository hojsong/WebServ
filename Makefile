NAME = webserv
NAME2 = cgi.exe
CXX = c++
FLAG = -Wall -Werror -Wextra -std=c++98
RM = rm -rf
SRCS = \
	./srcs/main.cpp \
	./srcs/ParseConfig.cpp \
	./srcs/ServerManage.cpp \
	./srcs/Server.cpp \
	./srcs/Location.cpp \
	./srcs/utilParsing.cpp \
	./srcs/Response.cpp \
	./srcs/Request.cpp \
	./srcs/Connection.cpp \
	./srcs/Member.cpp \
	./srcs/MemberRepository.cpp \
	./srcs/cgi_util.cpp \
	./srcs/post_true.cpp

CGIS = \
	cgi/cgi_main.cpp \
	cgi/cgi_exe.cpp \
	cgi/util.cpp \
	cgi/Member.cpp \
	cgi/MemberRepository.cpp \
	cgi/file_post.cpp \
	cgi/headeradd.cpp
	
INCS = \
	./incs/
CNCS = \
	./cgi/

OBJS = $(SRCS:.cpp=.o)
OBCS = $(CGIS:.cpp=.o)

all : $(NAME) $(NAME2)

%.o : %.cpp
	@$(CXX) $(FLAG) -I $(INCS) -c $< -o $@

$(NAME) : $(OBJS) $(INCS)
	@$(CXX) $(FLAG) -I $(INCS) -o $(NAME) $(OBJS)

$(NAME2) : $(OBCS) $(CNCS)
	@$(CXX) $(FLAG) -I $(CNCS) -o $(NAME2) $(OBCS)

file : 
	@mkdir html/file

clean :
	@$(RM) $(OBJS)
	@$(RM) $(OBCS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(NAME2)
	@rm -rf memberdb.txt
	@rm -rf html/file

re : fclean all file

.PHONY : all  clean fclean file re
