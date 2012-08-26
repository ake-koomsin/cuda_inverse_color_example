#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "utils.h"

RESULT readImage(const char *file, RawImage *rawImage) {

  FILE *f = fopen(file, "rb");

  if (!f) {
    printf("Unable to open the file: %s\n", file);
    fclose(f);
    return FAIL;
  }

  // these are standard libjpeg structures for writing(decompression)
  struct jpeg_decompress_struct jpegObj;
  struct jpeg_error_mgr errorHandler;

  // 1. Set up jpeg compression object
  jpegObj.err = jpeg_std_error(&errorHandler); // set up error handler
  jpeg_create_decompress(&jpegObj); // init jpeg decompression object

  // 2. Read from stdio
  jpeg_stdio_src(&jpegObj, f);

  // 3. Read header
  jpeg_read_header(&jpegObj, TRUE);

  // 4. Start decompression
  jpeg_start_decompress(&jpegObj);

  // 5. Prepare for reading raw image
  size_t rawImageSize = jpegObj.output_width * jpegObj.output_height * jpegObj.num_components;
  rawImage->pixels = (unsigned char *)malloc(rawImageSize);
  rawImage->width = jpegObj.output_width;
  rawImage->height = jpegObj.output_height;
  rawImage->numberOfComponents = jpegObj.num_components;

  JSAMPROW row_buffer[1];
  size_t rowSize = jpegObj.output_width * jpegObj.num_components;
  row_buffer[0] = (unsigned char *)malloc(rowSize);

  // 6. Scan one row at a time
  unsigned int location = 0;

  while(jpegObj.output_scanline < jpegObj.image_height) {
    jpeg_read_scanlines(&jpegObj, row_buffer, 1);
    int i;
    for(i = 0; i < jpegObj.image_width * jpegObj.num_components; i++) {
      rawImage->pixels[location++] = row_buffer[0][i];
    }
  }

  // 7. clean up
  jpeg_finish_decompress(&jpegObj);
  jpeg_destroy_decompress(&jpegObj);
  free(row_buffer[0]);
  fclose(f);

  return SUCCESS;
}

RESULT writeImage(const char *file, RawImage *rawImage) {
  FILE *f = fopen(file, "wb");

  if (!f) {
    printf("Unable to write the file\n");
    fclose(f);
    return FAIL;
  }

  // these are standard libjpeg structures for writing(compression)
  struct jpeg_compress_struct jpegObj;
  struct jpeg_error_mgr errorHandler;

  // 1. Set up jpeg compression object
  jpegObj.err = jpeg_std_error(&errorHandler); // set up error handler
  jpeg_create_compress(&jpegObj); // init jpeg decompression object

  // 2. Write with stdio to the file
  jpeg_stdio_dest(&jpegObj, f);

  // 3. Set up parameter
  jpegObj.image_width = rawImage->width;  
  jpegObj.image_height = rawImage->height;
  jpegObj.input_components = rawImage->numberOfComponents;
  jpegObj.in_color_space = JCS_RGB; // JCS_GRAYSCALE for gray scale image
  jpeg_set_defaults(&jpegObj); // set default compression parameters

  // 4. Do the compression
  jpeg_start_compress(&jpegObj, TRUE);
  
  // 5. Write one row at a time
  JSAMPROW row_buffer[1];
  while( jpegObj.next_scanline < jpegObj.image_height ) {
    unsigned int nextRow = jpegObj.next_scanline * jpegObj.image_width *  jpegObj.input_components;
    row_buffer[0] = &(rawImage->pixels[nextRow]);
    jpeg_write_scanlines(&jpegObj, row_buffer, 1);
  }

  // 6. Clean up
  jpeg_finish_compress(&jpegObj);
  jpeg_destroy_compress(&jpegObj);
  fclose(f);

  return SUCCESS;
}

RawImage *createRawImage() {
  return (RawImage *)malloc(sizeof(RawImage));
}

void destroyRawImage(RawImage *rawImage) {
  free(rawImage->pixels);
  free(rawImage);
}