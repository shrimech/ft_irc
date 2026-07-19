SRC = Server.cpp main.cpp ../client/authentification.cpp ../client/commands.cpp ../client/client.cpp
OBJ = $(SRC:.cpp=.o)
HDR = Server.hpp ../client/authentification.hpp ../client/commands.hpp ../client/client.hpp
Name = Server
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