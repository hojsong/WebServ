NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=address
SUBCXXFLAG = -std=c++98
RM = rm
RMFLAGS = -f
SRCS = src/main.cpp 			\
		src/ConfigParse.cpp		\
		src/Location.cpp		\
		src/Server.cpp			\
		src/ServerManage.cpp	\
		src/Request.cpp			\
		src/GetMethod.cpp		\
		src/PostMethod.cpp
OBJS = $(SRCS:.cpp=.o)
INCLUDES = ./header

all : $(NAME)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(SUBCXXFLAG) -c -o $@ $^ -I $(INCLUDES)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(SUBCXXFLAG) $^ -o $@

clean :
	$(RM) $(RMFLAGS) $(OBJS)

fclean : clean
	$(RM) $(RMFLAGS) $(NAME)

re : fclean all

.PHONY : all clean fclean re