PROJ=Trabajo7
OBJS=lib/utils.o
CFLAGS=-Iinclude
LIBS=-lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs

$(PROJ): main.cpp $(OBJS)
	g++ $(CFLAGS) -o $@ $< $(OBJS) $(LIBS)

lib/utils.o: lib/utils.cpp include/utils.hpp
	g++ $(CFLAGS) -o $@ $< -c

clean:
	rm -rf $(PROJ) **/*.o

.PHONY: clean