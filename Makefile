CXX=g++

CUDA_INSTALL_PATH=/usr/local/cuda
DYLD_LIBRARY_PATH=$(CUDA_INSTALL_PATH)/lib
CFLAGS= -I. -I$(CUDA_INSTALL_PATH)/include -m32
LDFLAGS= -L$(CUDA_INSTALL_PATH)/lib -lcudart -ljpeg -m32

all:
	$(CXX) $(CFLAGS) -c main.cpp -o obj/main.o
	$(CXX) $(CFLAGS) -c utils.c -o obj/utils.o
	nvcc $(CUDAFLAGS) -c inverse.cu -o obj/inverse.o
	$(CXX) $(LDFLAGS) obj/inverse.o obj/utils.o obj/main.o -o bin/inverse

clean:
	rm -f obj/*.o bin/inverse