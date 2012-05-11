#include "stdafx.h"
#include "bmp.h"


int bmpWrite(AUX_RGBImageRec* image, const char *file_name)
{
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	unsigned char buf[54];
	FILE *f;

	ih.biSize		   = DWORD(sizeof(BITMAPINFOHEADER));
	ih.biWidth		   = LONG(image->sizeX);
	ih.biHeight		   = LONG(image->sizeY);
	ih.biPlanes		   = WORD(1);
	ih.biBitCount	   = WORD(24);
	ih.biCompression   = DWORD(0);
	ih.biSizeImage     = DWORD(image->sizeX * image->sizeY * 3);
	ih.biXPelsPerMeter = LONG(0);
	ih.biYPelsPerMeter = LONG(0);
	ih.biClrUsed       = DWORD(0);
	ih.biClrImportant  = DWORD(0);
	
	fh.bfType		   = WORD('MB');
	fh.bfSize		   = DWORD(image->sizeX * image->sizeY * 3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	fh.bfReserved1	   = WORD(0);
	fh.bfReserved2	   = WORD(0);
	fh.bfOffBits	   = DWORD(sizeof(BITMAPFILEHEADER)+ih.biSize);

	buf[0] = (unsigned char)(LOBYTE(fh.bfType));
	buf[1] = (unsigned char)(HIBYTE(fh.bfType));
	buf[2] = (unsigned char)(LOBYTE(LOWORD(fh.bfSize)));
	buf[3] = (unsigned char)(HIBYTE(LOWORD(fh.bfSize)));
	buf[4] = (unsigned char)(LOBYTE(HIWORD(fh.bfSize)));
	buf[5] = (unsigned char)(HIBYTE(HIWORD(fh.bfSize)));
	buf[6] = (unsigned char)(LOBYTE(fh.bfReserved1));
	buf[7] = (unsigned char)(HIBYTE(fh.bfReserved1));
	buf[8] = (unsigned char)(LOBYTE(fh.bfReserved2));
	buf[9] = (unsigned char)(HIBYTE(fh.bfReserved2));
	buf[10] = (unsigned char)(LOBYTE(LOWORD(fh.bfOffBits)));
	buf[11] = (unsigned char)(HIBYTE(LOWORD(fh.bfOffBits)));
	buf[12] = (unsigned char)(LOBYTE(HIWORD(fh.bfOffBits)));
	buf[13] = (unsigned char)(HIBYTE(HIWORD(fh.bfOffBits)));
	buf[14] = (unsigned char)(LOBYTE(LOWORD(ih.biSize)));
	buf[15] = (unsigned char)(HIBYTE(LOWORD(ih.biSize)));
	buf[16] = (unsigned char)(LOBYTE(HIWORD(ih.biSize)));
	buf[17] = (unsigned char)(HIBYTE(HIWORD(ih.biSize)));
	buf[18] = (unsigned char)(LOBYTE(LOWORD(ih.biWidth)));
	buf[19] = (unsigned char)(HIBYTE(LOWORD(ih.biWidth)));
	buf[20] = (unsigned char)(LOBYTE(HIWORD(ih.biWidth)));
	buf[21] = (unsigned char)(HIBYTE(HIWORD(ih.biWidth)));
	buf[22] = (unsigned char)(LOBYTE(LOWORD(ih.biHeight)));
	buf[23] = (unsigned char)(HIBYTE(LOWORD(ih.biHeight)));
	buf[24] = (unsigned char)(LOBYTE(HIWORD(ih.biHeight)));
	buf[25] = (unsigned char)(HIBYTE(HIWORD(ih.biHeight)));
	buf[26] = (unsigned char)(LOBYTE(ih.biPlanes));
	buf[27] = (unsigned char)(HIBYTE(ih.biPlanes));
	buf[28] = (unsigned char)(LOBYTE(ih.biBitCount));
	buf[29] = (unsigned char)(HIBYTE(ih.biBitCount));
	buf[30] = (unsigned char)(LOBYTE(LOWORD(ih.biCompression)));
	buf[31] = (unsigned char)(HIBYTE(LOWORD(ih.biCompression)));
	buf[32] = (unsigned char)(LOBYTE(HIWORD(ih.biCompression)));
	buf[33] = (unsigned char)(HIBYTE(HIWORD(ih.biCompression)));
	buf[34] = (unsigned char)(LOBYTE(LOWORD(ih.biSizeImage)));
	buf[35] = (unsigned char)(HIBYTE(LOWORD(ih.biSizeImage)));
	buf[36] = (unsigned char)(LOBYTE(HIWORD(ih.biSizeImage)));
	buf[37] = (unsigned char)(HIBYTE(HIWORD(ih.biSizeImage)));
	buf[38] = (unsigned char)(LOBYTE(LOWORD(ih.biXPelsPerMeter)));
	buf[39] = (unsigned char)(HIBYTE(LOWORD(ih.biXPelsPerMeter)));
	buf[40] = (unsigned char)(LOBYTE(HIWORD(ih.biXPelsPerMeter)));
	buf[41] = (unsigned char)(HIBYTE(HIWORD(ih.biXPelsPerMeter)));
	buf[42] = (unsigned char)(LOBYTE(LOWORD(ih.biYPelsPerMeter)));
	buf[43] = (unsigned char)(HIBYTE(LOWORD(ih.biYPelsPerMeter)));
	buf[44] = (unsigned char)(LOBYTE(HIWORD(ih.biYPelsPerMeter)));
	buf[45] = (unsigned char)(HIBYTE(HIWORD(ih.biYPelsPerMeter)));
	buf[46] = (unsigned char)(LOBYTE(LOWORD(ih.biClrUsed)));
	buf[47] = (unsigned char)(HIBYTE(LOWORD(ih.biClrUsed)));
	buf[48] = (unsigned char)(LOBYTE(HIWORD(ih.biClrUsed)));
	buf[49] = (unsigned char)(HIBYTE(HIWORD(ih.biClrUsed)));
	buf[50] = (unsigned char)(LOBYTE(LOWORD(ih.biClrImportant)));
	buf[51] = (unsigned char)(HIBYTE(LOWORD(ih.biClrImportant)));
	buf[52] = (unsigned char)(LOBYTE(HIWORD(ih.biClrImportant)));
	buf[53] = (unsigned char)(HIBYTE(HIWORD(ih.biClrImportant)));
	// TODO:
	// handle errors
	f = fopen(file_name, "wb");
	fwrite(buf,1,54,f);
	fwrite(image->data,1,image->sizeX * image->sizeY * 3,f);
	fclose(f);
	return 0;
};


	