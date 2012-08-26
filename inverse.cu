#include <stdio.h>
#include <cuda_runtime.h>
#include "utils.h"

__global__ void inverseKernel(const unsigned char *src, unsigned char *dst, size_t width, size_t height, size_t numberOfComponents) {
  int col = blockDim.x * blockIdx.x + threadIdx.x;
  int row = blockDim.y * blockIdx.y + threadIdx.y;

  int index = width * numberOfComponents * row + col;

  // int index = blockDim.x * blockIdx.x + threadIdx.x;

  if (index < width * height * numberOfComponents) {
    dst[index] = 255 - src[index];
    // __syncthreads();
  }

}

extern "C" void inverseImage(const RawImage *src, RawImage *dst) {
  if (src == NULL || src->pixels == NULL || dst == NULL ) {
    return;
  }

  size_t size = src->width * src->height * src->numberOfComponents;

  dst->width = src->width;
  dst->height = src->height;
  dst->numberOfComponents = src->numberOfComponents;
  free(dst->pixels);
  dst->pixels = (unsigned char *)malloc(size);

  unsigned char *srcPixel = src->pixels;
  unsigned char *dstPixel = dst->pixels;

  unsigned char *deviceSrcPixel;
  unsigned char *deviceDstPixel;
  
  cudaMalloc((void**)&deviceSrcPixel, size);
  cudaMalloc((void**)&deviceDstPixel, size);

  cudaMemcpy(deviceSrcPixel, srcPixel, size, cudaMemcpyHostToDevice);

  // int threadsPerBlock = 256;
  // int blocksPerGrid = (size) / threadsPerBlock;
  dim3 dimBlock(16, 16);
  dim3 dimGrid((src->width * src->numberOfComponents + dimBlock.x - 1)  / dimBlock.x, (src->height + dimBlock.y - 1) / dimBlock.y);
  inverseKernel<<<dimGrid, dimBlock>>>(deviceSrcPixel, deviceDstPixel, src->width, src->height, src->numberOfComponents);

  cudaMemcpy(dstPixel, deviceDstPixel, size, cudaMemcpyDeviceToHost);

  cudaFree(deviceSrcPixel);
  cudaFree(deviceDstPixel);

}



