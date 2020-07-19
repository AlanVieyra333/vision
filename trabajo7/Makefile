PROJ=clasificador.o
OBJS=lib/image_process.o lib/utils.o
CFLAGS=-Iinclude
OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs

build: $(PROJ) hu-moments.o

run: $(PROJ)
	./$(PROJ) images/circulo/imagen7.png

$(PROJ): main.cpp $(OBJS)
	g++ $(CFLAGS) -o $@ $< $(OBJS) $(OPENCV_LIBS)
	@echo $@ compilado correctamente.

hu-moments.o: hu-moments.cpp $(OBJS)
	g++ $(CFLAGS) -o $@ $< $(OBJS) $(OPENCV_LIBS)
	@echo $@ compilado correctamente.
	@echo "Use example: ./hu-moments.o images/circulo 2"

lib/image_process.o: lib/image_process.cpp include/image_process.hpp
	g++ $(CFLAGS) -o $@ $< -c

lib/utils.o: lib/utils.cpp include/utils.hpp
	g++ $(CFLAGS) -o $@ $< -c

clean:
	rm -rf $(PROJ) **/*.o *.png

.PHONY: build run clean