NAME = webserv
CXX = c++
FLAG = -Wall -Werror -Wextra -std=c++98 #-g3 -fsanitize=undefined
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
	./srcs/Connection.cpp

INCS = \
	./incs/

OBJS = $(SRCS:.cpp=.o)

%.o : %.cpp
	$(CXX) $(FLAG) -I $(INCS) -c $< -o $@

$(NAME) : $(OBJS) $(INCS)
	$(CXX) $(FLAG) -I $(INCS) -o $(NAME) $(OBJS)

all : $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all  clean fclean re
