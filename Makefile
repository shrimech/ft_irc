SERV = ./Server/
CLIENT = ./client

SRC = $(SERV)Server.cpp main.cpp $(CLIENT)/authentification.cpp $(CLIENT)/commands.cpp $(CLIENT)/client.cpp
OBJ = $(SRC:.cpp=.o)
HDR = $(SERV)Server.hpp $(CLIENT)/authentification.hpp $(CLIENT)/commands.hpp $(CLIENT)/client.hpp
Name = irserve
CXX = c++
CXXFLAGS =   -Wall -Wextra -Werror -std=c++98 -g

all: $(Name)
$(Name): $(OBJ) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(Name) $(OBJ)
$(OBJ): $(HDR)
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(Name)
re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJ)