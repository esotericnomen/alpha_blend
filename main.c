#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar_t;

void write_data(unsigned char *buffer, const char *file_name, const long size) {
    FILE *fp = fopen(file_name,"a");
    fwrite(buffer, 1, size, fp);
    fclose(fp);
    return;
}

int OverlayMousePointer(unsigned char *frame_buffer,
        unsigned char *mouse_buffer,
        unsigned int x_coordinate,
        unsigned int y_coordinate){

    // corner cases will be handled latter
    if( (x_coordinate + 32 > 639) ||
            (y_coordinate + 32 > 479) )
        return -1;

    if(!frame_buffer || !mouse_buffer)
        return -2;

    unsigned int index = (x_coordinate)* 2 * 640 + (y_coordinate) * 2;

    for(int i=0; i<32; i++) {
        unsigned int *tmp = (unsigned int*)&(frame_buffer[index-2]);
        printf("tmp : %p Pixel : [%3u][%3u] | %2u %2u %2u \n",tmp, (index/2)/640, (index/2)%640, *tmp >> 0xB & 0x1F, *tmp >> 0x5 & 0x3F, *tmp & 0x1F);

        unsigned char fb_R = *tmp >> 0xB & 0x1F;
        unsigned char fb_G =  *tmp >> 0x5 & 0x3F;
        unsigned char fb_B =  *tmp & 0x1F;

        unsigned char mouse_R = mouse_buffer[i*4];
        unsigned char mouse_G = mouse_buffer[(i*4)+1];
        unsigned char mouse_B = mouse_buffer[(i*4)+2];
        unsigned char mouse_alpha = mouse_buffer[(i*4)+3];
        unsigned char inv_alpha = 256 - mouse_alpha;

        /*outputRed = (foregroundRed * foregroundAlpha) + (backgroundRed * (1.0 - foregroundAlpha));*/

        unsigned char blend_R = (mouse_R * mouse_alpha) + (fb_R * inv_alpha);
        unsigned char blend_G = (mouse_G * mouse_alpha) + (fb_G * inv_alpha);
        unsigned char blend_B = (mouse_B * mouse_alpha) + (fb_B * inv_alpha);
        *tmp = (blend_R & 0x1F) << 0xB | (blend_G & 0x3F) << 0x5 | (blend_B & 0x1F);
        printf(" %2u %2u %2u\n",*tmp >> 0xB & 0x1F, *tmp >> 0x5 & 0x3F, *tmp & 0x1F);
        index+=2;
    }
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
    unsigned int x_coordinate = rand()%640;
    unsigned int y_coordinate = rand()%480;

    /*Use predetrmined coordinates for fast testing*/
    x_coordinate = 10;
    y_coordinate = 10;

    printf("x_coordinate : %d, y_coordinate: %d\n",x_coordinate, y_coordinate);

    write_data(frame_buffer,"./frame_buffer.dat",640*480*2);

    OverlayMousePointer(frame_buffer, mouse_buffer, x_coordinate, y_coordinate);

    write_data(frame_buffer,"./blend_frame_buffer.dat",640*480*2);
}
