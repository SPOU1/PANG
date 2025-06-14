 
OSX_LDFLAGS= -L/usr/local/lib    -lm   -framework GLUT -framework OpenGL -framework Cocoa 

LinuxLFLAGS= -lGL -lGLU -lglut -lm
Linux_LDFLAGS= -L/usr/lib/x86_64-linux-gnu  

#
# How to make .c into .o
#
.cpp.o: commonstuff.h
	g++   -c $< -w


osX_STL:	Shape.o  Ball.o Bullet.o mainPANG.o commonstuff.h Man.o ObjectsListSTL.o GLstuff.o
	g++ Shape.o GLstuff.o  Bullet.o Man.o Ball.o mainPANG.o ObjectsListSTL.o $(OSX_LDFLAGS)  -o PANG_STL  -Wno-deprecated-declarations


osX:	Shape.o  Ball.o Bullet.o mainPANG.o commonstuff.h Man.o ObjectsList.o GLstuff.o
	g++ Shape.o GLstuff.o Bullet.o Man.o Ball.o  mainPANG.o ObjectsList.o  $(OSX_LDFLAGS)  -o PANG  -Wno-deprecated-declarations


Linux:	Shape.o  Ball.o Bullet.o mainPANG.o commonstuff.h Man.o ObjectsList.o GLstuff.o Animal.o AudioPlayer.o Obstacle.o
	g++ Shape.o  Bullet.o Man.o GLstuff.o Ball.o mainPANG.o ObjectsList.o Animal.o AudioPlayer.o Obstacle.o -Wl,--copy-dt-needed-entries $(Linux_LDFLAGS)  $(LinuxLFLAGS)  -o PANG  -Wno-deprecated-declarations

clean:
	rm -f *~
	rm -f *.o
