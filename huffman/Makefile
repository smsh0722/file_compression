# Makefile
TARGET=main.out
CXX=g++
CXXFLAGS=-Wall
OBJECTS=main.o encoder.o decoder.o utilities.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp encoder.h decoder.h utilities.h
	$(CXX) $(CXXFLAGS) -c main.cpp

encoder.o: encoder.cpp utilities.h
	$(CXX) $(CXXFLAGS) -c encoder.cpp
decoder.o: decoder.cpp utilities.h
	$(CXX) $(CXXFLAGS) -c decoder.cpp
utilities.o: utilities.cpp
	$(CXX) $(CXXFLAGS) -c utilities.cpp

clean:
	rm $(OBJECTS) $(TARGET)