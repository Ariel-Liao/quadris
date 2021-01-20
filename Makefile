CXX = g++ -std=c++17
CXXFLAGS = -Wall -g -MMD
GTKFLAGS = `pkg-config gtkmm-3.0 --cflags --libs`
SOURCES = $(wildcard *.cc)
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC=quadris

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(GTKFLAGS)
%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(GTKFLAGS)
-include ${DEPENDS}
.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)
