# Makefile for OpenCV Stuffs

CC = g++
CFLAGS = -Wall -Werror

# NB: These libs and paths can be found by running
# pkg-config --libs opencv
# and
# pkg-config opencv
# on your *nix-ey platform of choice

LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui
HDRS =	
INCLUDES =

OUTPUTNAME = houghtest

# Build everything - nothing fancy here
all: $(OUTPUTNAME)

houghtest: houghtest.o toolbarconfig.o
	$(CC) $(LIBS) -o houghtest houghtest.o toolbarconfig.o

houghtest.o: houghtest.cpp
	$(CC) $(CFLAGS) -c -o houghtest.o houghtest.cpp

toolbarconfig.o: toolbarconfig.cpp
	$(CC) $(CFLAGS) -c -o toolbarconfig.o toolbarconfig.cpp

no-o:
	rm *.o

clean:
	rm *.o $(OUTPUTNAME)

