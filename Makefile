CMP = g++

all: server client

server: servermain.o server.o conclser.o
	$(CMP) -o bin/server servermain.o server.o conclser.o -std=c++11 -pthread
	rm -f server.o servermain.o

servermain.o: server/servermain.cpp server/server.hpp conclser/conclser.hpp
	$(CMP) -c server/servermain.cpp -o servermain.o

server.o: server/server.cpp server/server.hpp
	$(CMP) -c server/server.cpp -o server.o

conclser.o: conclser/conclser.cpp conclser/conclser.hpp
	$(CMP) -c conclser/conclser.cpp -o conclser.o

client: clientmain.o client.o conclser.o
	$(CMP) -o bin/client clientmain.o client.o conclser.o -std=c++11 -pthread
	rm -f client.o clientmain.o conclser.o

clientmain.o: client/clientmain.cpp client/client.hpp conclser/conclser.hpp
	$(CMP) -c client/clientmain.cpp -o clientmain.o

client.o: client/client.cpp client/client.hpp
	$(CMP) -c client/client.cpp -o client.o
