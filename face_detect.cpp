#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "facedetectcnn.h"
#define DETECT_BUFFER_SIZE 0x20000

int main(int argc, char *argv[]) {
    int w = atoi(argv[1]);
    int h = atoi(argv[2]);

    unsigned char *detect_buffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
    unsigned char img[w*h*3];

    while (1) {
        int r = read(0, img, w*h*3);
	if (r == 0)
	   break;
	unsigned char *ptr = img;
	for (int y = 0; y < h; y++) {
           for (int x = 0; x < w; x++, ptr += 3) {
	      unsigned char tmp = ptr[2];
	      ptr[2] = ptr[0];
	      ptr[0] = tmp;
	   }
	}
	int *detect = facedetect_cnn(detect_buffer, img, w, h, w*3);
	fprintf(stdout, "%d\n", (detect ? *detect : 0));
	if (!detect)
	   continue;
	for(int i = 0; i < *detect; i++) {
	    short * p = ((short*)(detect+1))+142*i;
	    int confidence = p[0];
	    int x = p[1];
	    int y = p[2];
	    int w = p[3];
	    int h = p[4];
	    fprintf(stdout, "{\"confidence\":%d,\"match\":[%d,%d,%d,%d],\"marks\":[[%d,%d],[%d,%d],[%d,%d],[%d,%d],[%d,%d]]}\n", 
	    	confidence, x, y, w, h, 
	    	p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13],p[14]);

	}
        fflush(stdout);
    }
    return 0;
}

