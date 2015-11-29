#include <stdio.h>

typedef unsigned char uchar_t;

void write_data(unsigned char *buffer, const char *file_name, const long size) {
    FILE *fp = fopen(file_name,"a");
    fwrite(buffer, 1, size, fp);
    fclose(fp);
    return;
}

int main() {
    unsigned char frame_buffer[640*480*2];
    unsigned char mouse_buffer[32*32*4];
    unsigned int i = 0;

    /* 
       Form the frame buffer and mouse buffer data for testing
    */
    /*
       Frame buffer structure and components representation

       |     frame_buffer[0]   |     frame_buffer[1]   |
       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
       |       R      |         G       |      B       |
       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    */
    for(i = 0; i < 640 * 480 * 2;) {
        unsigned int fb_rgb = rand() % 0xFFFF;
        frame_buffer[i++] = (unsigned char) (fb_rgb >> 8) & 0xFF;
        frame_buffer[i++] = (unsigned char) (fb_rgb & 0xFF);
        unsigned int *tmp = (unsigned int*)&(frame_buffer[i-2]);
        printf("tmp : %p Pixel : [%3u][%3u] | %2u %2u %2u\n",tmp, (i/2)/640, (i/2)%640, *tmp >> 0xB & 0x1F, *tmp >> 0x5 & 0x3F, *tmp & 0x1F);
    }

    /*
       Mouse buffer structure and components representation

       |        mouse[0]       |      mouse[1]         |      mouse[2]         |       mouse[3]        |
       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
       |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
       |           R           |          G            |         B             |        Alpha          |
       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
     */
    for(i = 0; i < 32 * 32*4;) {
        mouse_buffer[i++] = rand()%255;        // R
        mouse_buffer[i++] = rand()%255;        // G
        mouse_buffer[i++] = rand()%255;        // B
        mouse_buffer[i++] = rand()%255;        // Alpha
        printf("mPixel : [%3u][%3u] | %2u %2u %2u %2u\n", (i/4)/32, (i/4)%32, mouse_buffer[i-4], mouse_buffer[i-3],mouse_buffer[i-2],mouse_buffer[i-1]);
    }

    write_data(frame_buffer,"./frame_buffer.dat",640*480*2);
}
