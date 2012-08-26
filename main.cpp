#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "inverse.h"

int main(int argc, char const *argv[]) {
  
  if (argc != 3) {
    printf("Usage: program src.jpg dst.jpg\n");
    return 0;
  }

  const char *srcFilePath = argv[1];
  const char *dstFilePath = argv[2];

  RawImage *srcImg = createRawImage();
  RawImage *dstImg = createRawImage();
  if (readImage(srcFilePath, srcImg) == SUCCESS) {
    printf("Read Success\n");

    inverseImage(srcImg, dstImg);
    printf("done inversing\n");

    if (writeImage(dstFilePath, dstImg) == SUCCESS) {
      printf("Write Success\n");
    } 

  } else {
    printf("Read Fail\n");
  }

  // Clean up
  destroyRawImage(srcImg);
  destroyRawImage(dstImg);
  return 0;
}