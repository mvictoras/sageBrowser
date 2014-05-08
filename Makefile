###################################################################
# Makefile for sageBrowser
###################################################################

#g++ -o webflow *.cc -lawesomium-1-7 -lglut -lGL -lSDL -ludev -lGLU -lsail  -o webflow

SDIR=../..
QUANTA_CFLAGS=-I${SDIR}/QUANTA/include
QUANTA_LDFLAGS=-L${SDIR}/lib -lquanta
COMPILER=g++

COMPILER_FLAGS=$(SAGE_CFLAGS) -O3 -I${SAGE_DIRECTORY}/include $(QUANTA_CFLAGS) $(GLEW_CFLAGS) $(GLSL_YUV_DEFINE) $(PORTAUDIO_CFLAGS) $(GLUT_CFLAGS)


LIBS=-lpthread ${SUN_LIBS} -lawesomium-1-7 -lm -ldl $(GLUT_LDFLAGS) -lGL -lGLU $(QUANTA_LDFLAGS) $(GLEW_LIB) -L${SAGE_DIRECTORY}/lib -lsail

LIBS+=$(PAUDIO_LIB)

default: sageBrowser

install: default
	cp sageBrowser ../../bin

sageBrowser: sageBrowser.o
	$(COMPILER) $(SAGE_LDFLAGS) -o sageBrowser sageBrowser.o $(LIBS)

sageBrowser.o:       sageBrowser.cpp
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDE_DIR) -c sageBrowser.cpp

clean:
	\rm -f sageBrowser *~ *.o

