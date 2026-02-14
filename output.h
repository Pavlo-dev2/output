#ifndef OUTPUT_H
#define OUTPUT_H

int retfdfb0();
//returns file deckriptor fb0;

int getinfos(int fbo);
//gets infos(finfo, vinfo);

long* retscreensize();
//returns screen size in pixels([x, y, fbolength]);

int* setsize(long hight, long wigth, char type, char f_type);
//set screen size;
//type 0 - hight*wight;
//type 1 - all block have same hight and wigth, higth depandse on how much elements with this wigth will fit on the screen; 

long* retblockinfo();
//returns block info;
//[0] - x - number of blocks
//[1] - y - number of blocks
//[2] - block - hight
//[3] - block - wigth

int definefeld(long wigth, long hight, char x_pos, char y_pos);
//creates feld;
//height - number of blocks;
//wigth - number of blocks;
//x_pos - l/m/r(left, midle, right);
//y_pos - u/m/d(up, midle, down);

long* retfeldinfo();
//returns feld info;
//[0] - x - feld wigth in blocks;
//[1] - y - feld hight in  blocks;
//[2] - feld x start posision in pixel;
//[3] - feld y start posision in pyxel;

/*
int retfdfb0();
int drawrect(unsigned char r, unsigned char g, unsigned char b, char x, char y, int un, int fb0);
int clearfeld(unsigned char r, unsigned char g, unsigned char b, int fb0);
int* info(int fb0);
*/

#endif
