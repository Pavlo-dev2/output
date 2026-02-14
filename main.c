#include <stdio.h>
#include "output.h"
#include <stdlib.h>

int main()
{
	int fb0 = retfdfb0();
	getinfos(fb0);
	
	long *screensize = retscreensize();
	printf("FB0: %d\nx: %ld\ny: %ld\nlen: %ld\n", fb0, screensize[0], screensize[1], screensize[2]);
	
	setsize(16, 16, 1, 0);
	long *blockinfo = retblockinfo();
	printf("Xnum: %ld\nYnum: %ld\nXwigth: %ld\nYhight: %ld\n", blockinfo[0], blockinfo[1], blockinfo[2], blockinfo[3]);

	definefeld(1, 1, 'l', 'd');
	long *feldinfo = retfeldinfo();
	printf("Feld_W: %ld\nFeld_H: %ld\nXS: %ld\nYS: %ld\n", feldinfo[0], feldinfo[1], feldinfo[2], feldinfo[3]);
	
	free(screensize);
	free(blockinfo);
	return 0;
}
