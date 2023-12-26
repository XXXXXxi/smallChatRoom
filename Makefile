# Compiler and flags  
CXX = g++  
CXXFLAGS = -c -Wall  
LDFLAGS =   
  
# Sources and objects  
CLIENT_SOURCES = Client/main.cpp Utils/Message.cpp Utils/MessageQueue.cpp  
CLIENT_OBJECTS = $(CLIENT_SOURCES:.cpp=.o)  
  
SERVER_SOURCES = Server/main.cpp Server/PthreadPool.cpp Server/TaskQueue.cpp Server/SocketEpoll.cpp Utils/Message.cpp Utils/MessageQueue.cpp  
SERVER_OBJECTS = $(SERVER_SOURCES:.cpp=.o)  
  
# Targets  
all: client_v server_v  
  
client_v: $(CLIENT_OBJECTS)  
	$(CXX) $(LDFLAGS) $^ -o $@ -ljsoncpp -lpthread
  
server_v: $(SERVER_OBJECTS)  
	$(CXX) $(LDFLAGS) $^ -o $@ -lpthread -ljsoncpp
  
# Cleanup  
clean:  
	rm -f */*.o
  
# Generic rule for compiling C++ files to object files  
%.o: %.cpp  
	$(CXX) $(CXXFLAGS) $< -o $@