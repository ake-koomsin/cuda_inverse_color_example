typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int numberOfComponents;
  unsigned char *pixels;
} RawImage;

typedef int RESULT;

#define SUCCESS 0
#define FAIL -1

extern "C" RESULT readImage(const char *file, RawImage *rawImage);
extern "C" RESULT writeImage(const char *file, RawImage *rawImage);
extern "C" RawImage *createRawImage();
extern "C" void destroyRawImage(RawImage *rawImage);