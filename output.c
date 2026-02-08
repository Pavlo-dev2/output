#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>

int retfdfb0();
//returns file deckriptor fb0;

int getinfos(int fbo);
//gets infos(finfo, vinfo);

long* retscreensize();
//returns screen size in pixels([x, y, fbolength]);

int* setsize(long hight, long wigth, char type);
//set screen size;
//type 0 - hight*wight;
//type 1 - all block have same hight and wigth, higth depandse on how much elements with this wigth will fit on the screen; 

long* retblockinfo();
//returns block info;
//[0] - x - number of blocks
//[1] - y - number of blocks
//[2] - block - hight
//[3] - block - wigth

//info
static struct fb_fix_screeninfo finfo;
static struct fb_var_screeninfo vinfo;
static long screen_hight;//screen hight in blocks
static long screen_wigth;//screen wigth in blocks
static long block_hight;
static long block_wigth;

int retfdfb0()
{
	int fb0 = open("/dev/fb0", O_RDWR);
}

int getinfos(int fb0)
{
	ioctl(fb0, FBIOGET_VSCREENINFO, &vinfo);
	ioctl(fb0, FBIOGET_FSCREENINFO, &finfo);
}

long *retscreensize()
{
	long *info = calloc(3, sizeof(long));
	
	info[0] = vinfo.xres;
	info[1] = vinfo.yres;
	info[2] = finfo.smem_len/(vinfo.bits_per_pixel/8);
	
	return info;
}

int* setsize(long hight, long wigth, char type)
{
	if (type == 0)
	{
		screen_hight = hight;
		screen_wigth = wigth;
		block_hight = vinfo.yres/screen_hight;
		block_wigth = vinfo.xres/screen_wigth;
	}
	else if (type == 1)
	{
		screen_wigth = screen_hight = hight;
		block_wigth = block_hight = vinfo.xres/screen_wigth;
		screen_hight = vinfo.yres/block_wigth;
	}
	else
	{
		return NULL;
	}
	return 0;
}

long* retblockinfo()
{
	long *info = calloc(4, sizeof(long));
	
	info[0] = screen_wigth;
	info[1] = screen_hight;
	info[2] = block_wigth;
	info[3] = block_hight;
	
	return info;
}


/*
int drawrect(unsigned char r, unsigned char g, unsigned char b, char x, char y, int un, int fb0);
int clearfeld(unsigned char r, unsigned char g, unsigned char b, int fb0);
int* info(int fb0);
*/
/*
//clear feld in the midle of the screan, size y_max*y_max
int clearfeld(unsigned char r, unsigned char g,  unsigned char b, int fb0)
{
	//info struckts
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	//get infos
	if (ioctl(fb0, FBIOGET_VSCREENINFO, &vinfo) == -1 || ioctl(fb0, FBIOGET_FSCREENINFO, &finfo) == -1)
	{
		perror("Error while opening fb0");
		close(fb0);
		return 1;
	}

	//make fb0 a pointer
	long screensize = finfo.smem_len;
	uint8_t *fbp = (uint8_t*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb0, 0);
	if (fbp == MAP_FAILED)
	{
		perror("Mmap failed");
		close(fb0);
		return 1;
	}

	//feld cords, xstart, xend, ystart, yend, in the midle of the screan;
	long feldcords[4] = {0};
	feldcords[0] = (vinfo.xres - (float)vinfo.yres)/2.0;
	feldcords[1] = feldcords[0] + vinfo.yres;
	feldcords[2] = (vinfo.yres - (float)vinfo.yres)/2.0;
	feldcords[3] = feldcords[2] + vinfo.yres;
	
	//printf("XS0: %lu\nXE1: %lu\n", feldcords[0], feldcords[1]);
	//printf("YS2: %lu\nYE3: %lu\n", feldcords[2], feldcords[3]);


	//feld size in pixels
	float feld_heigth, feld_wigth;
	feld_wigth = feldcords[1] - feldcords[0];//x length
	feld_heigth = feldcords[3] - feldcords[2];//y length

	//printf("feld_wigth: %f;\nfeld_heigth: %f;\n", feld_wigth, feld_heigth);
	
	//sleep(1);

	//draw feld
	for (long ay = feldcords[2]; ay < feldcords[3]; ay++)
	{
		for (long ax = feldcords[0]; ax < feldcords[1]; ax++)
		{
            		long location = (ax + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (ay + vinfo.yoffset) * finfo.line_length;
			
			if (vinfo.bits_per_pixel == 32)
			{
				fbp[location] = b;
				fbp[location+1] = g;
				fbp[location+2] = r;
				fbp[location+3] = 0;
			}
		
		}
	}

	//end
	munmap(fbp, screensize);
}

//print fb0 info
int* info(int fb0)
{
	//info struckts
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	//get infos
	if (ioctl(fb0, FBIOGET_VSCREENINFO, &vinfo) == -1 || ioctl(fb0, FBIOGET_FSCREENINFO, &finfo) == -1)
	{
		perror("Error while opening fb0");
		close(fb0);
		return NULL;
	}
	
	printf("vinfo.xres: %f\nvinfo.yres: %f\n", (float) vinfo.xres, (float) vinfo.yres);
	printf("finfo.smem_len: %f\n", (float) finfo.smem_len);
	printf("vinfo.xoffset: %f\nvinfo.yoffset: %f\n", (float) vinfo.xoffset, (float) vinfo.yoffset);
	printf("vinfo.xbits_per_pixel: %f\nvinfo.byts_per_pixel: %f\n", (float) vinfo.bits_per_pixel, (float) vinfo.bits_per_pixel/8);
	printf("finfo.line_length: %f\n\n", (float) finfo.line_length);
}

//funktion to draw a square
int drawrect(unsigned char r, unsigned char g, unsigned char b, char x, char y, int un, int fb0)
{
	//info struckts
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	//get infos
	if (ioctl(fb0, FBIOGET_VSCREENINFO, &vinfo) == -1 || ioctl(fb0, FBIOGET_FSCREENINFO, &finfo) == -1)
	{
		perror("Error while opening fb0");
		close(fb0);
		return 1;
	}

	//make fb0 a pointer
	long screensize = finfo.smem_len;
	uint8_t *fbp = (uint8_t*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb0, 0);
	if (fbp == MAP_FAILED)
	{
		perror("Mmap failed");
		close(fb0);
		return 1;
	}
	
	//feld cords, xstart, xend, ystart, yend, in the midle of the screan;
	long feldcords[4] = {0};
	feldcords[0] = (vinfo.xres - (float)vinfo.yres)/2.0;
	feldcords[1] = feldcords[0] + vinfo.yres;
	feldcords[2] = (vinfo.yres - (float)vinfo.yres)/2.0;
	feldcords[3] = feldcords[2] + vinfo.yres;
	
	//printf("XS0: %lu\nXE1: %lu\n", feldcords[0], feldcords[1]);
	//printf("YS2: %lu\nYE3: %lu\n", feldcords[2], feldcords[3]);


	//feld size in pixels
	float feld_heigth, feld_wigth;
	feld_wigth = feldcords[1] - feldcords[0];//x length
	feld_heigth = feldcords[3] - feldcords[2];//y length

	//printf("feld_wigth: %f;\nfeld_heigth: %f;\n", feld_wigth, feld_heigth);
	
	//block size in pixel
	long block_wigth, block_heigth;
	block_wigth = feld_wigth/un;
	block_heigth = feld_heigth/un;
	
	//printf("block_wigth: %lu;\nblock_heigth: %lu;\n", block_wigth, block_heigth);

	//sleep(1);

	long xst = feldcords[0] + block_wigth*x;//x start
	long xed = xst+block_wigth;//x end
	long yst = feldcords[2] + block_heigth*y;//y start
	long yed = yst+block_heigth;//y end
	
	//draw block
	for (long ay = yst; ay < yed; ay++)
	{
		for (long ax = xst; ax < xed; ax++)
		{
            		long location = (ax + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (ay + vinfo.yoffset) * finfo.line_length;
			
			if (vinfo.bits_per_pixel == 32)
			{
				fbp[location] = b;
				fbp[location+1] = g;
				fbp[location+2] = r;
				fbp[location+3] = 0;
			}
		
		}
	}

	//end
	munmap(fbp, screensize);
}*/
