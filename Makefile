src = server.cpp
hdr = server.hpp
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
name = server
obj = $(src:.cpp=.o)

all : $(name)
$(name) : 
	$(CXX) $(CXXFLAGS) -o $(name) $(src)
clean: 
	rm -f $(obj)

fclean: clean
	rm -f $(name)

re : fclean all

.PHONY: all re fclean clean

.SECONDARY: $(obj)