all: client server
client:
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -g client.c -o client.exec -lzmq -lpthread
server:
	g++ -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -g server.c -o server.exec -lzmq -lpthread
