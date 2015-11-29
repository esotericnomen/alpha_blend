alpha:
#	gcc main.c --std=c99 -o alpha -DENABLE_TEST
	gcc main.c --std=c99 -o alpha
clean:
	rm alpha blend_frame_buffer.dat frame_buffer.dat
