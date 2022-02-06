# Makefile
TARGET=main.out
CXX=g++
CXXFLAGS=-Wall
OBJECTS=main.o encoder.o decoder.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp encoder.h decoder.h
	$(CXX) $(CXXFLAGS) -c main.cpp

encoder.o: encoder.cpp
	$(CXX) $(CXXFLAGS) -c encoder.cpp
decoder.o: decoder.cpp
	$(CXX) $(CXXFLAGS) -c decoder.cpp

clean:
	rm $(OBJECTS) $(TARGET)