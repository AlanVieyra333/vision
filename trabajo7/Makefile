PROJ=Trabajo7
OBJS=lib/image_process.o lib/utils.o
CFLAGS=-Iinclude
OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs

$(PROJ): main.cpp $(OBJS)
	g++ $(CFLAGS) -o $@ $< $(OBJS) $(OPENCV_LIBS)

lib/image_process.o: lib/image_process.cpp include/image_process.hpp
	g++ $(CFLAGS) -o $@ $< -c

lib/utils.o: lib/utils.cpp include/utils.hpp
	g++ $(CFLAGS) -o $@ $< -c

clean:
	rm -rf $(PROJ) **/*.o *.png

.PHONY: clean