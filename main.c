#include <stdio.h>
#include "output.h"
#include <stdlib.h>
#include <stdint.h>

int main()
{
	int fb0 = retfdfb0();
	getinfos(fb0);
	
	long *screensize = retscreensize();
	printf("FB0: %d\nx: %ld\ny: %ld\nlen: %ld\n", fb0, screensize[0], screensize[1], screensize[2]);
	
	setsize(16, 16, 1, 0);
	long *blockinfo = retblockinfo();
	printf("Xnum: %ld\nYnum: %ld\nXwigth: %ld\nYhight: %ld\n", blockinfo[0], blockinfo[1], blockinfo[2], blockinfo[3]);

	definefeld(16, 10, 'm', 'm');
	long *feldinfo = retfeldinfo();
	printf("Feld_W: %ld\nFeld_H: %ld\nXS: %ld\nYS: %ld\nXE: %ld\nYE: %ld\n", feldinfo[0], feldinfo[1], feldinfo[2], feldinfo[3], feldinfo[4], feldinfo[5]);
	
	uint8_t *fbp = retfbp(fb0);
	printf("FBP: %p\n", fbp);

	drawblock(fbp, -1, 0, 0, 128, 0);
	//drawblock(fbp, 15, 0, 0, 128, 0);
	drawblock(fbp, 0, -1, 128, 0, 0);
	drawblock(fbp, 15, 9, 128, 0, 0);

	closefbp(fbp);

	free(screensize);
	free(blockinfo);
	free(feldinfo);
	return 0;
}
