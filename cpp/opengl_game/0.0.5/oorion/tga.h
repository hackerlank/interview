/**
 *  Valid pixel format.
 */
typedef enum {
  TGA_PIXFMT_GRAY,    //!< Grayscale pixel format.
  TGA_PIXFMT_RGB,     //!< RGB pixel format.
  TGA_PIXFMT_RGBA     //!< RGB-Alpha pixel format.
} EPixFormat;

/**
 *  TGA image file information.
 */
typedef struct {
  int idlen;                //!< 1 byte
  int cmaptype;             //!< Specifies, if there is a colormap. (1 byte)
  int imagetype;            //!< Specifies type of the image. (1 byte)
  int cmapfirstidx;         //!< 2 bytes
  int cmaplen;              //!< 2 bytes
  int cmapentrysize;        //!< 1 byte
  int xorigin;              //!< 2 bytes
  int yorigin;              //!< 2 bytes
  int width;                //!< 2 bytes
  int height;               //!< 2 bytes
  int bitsperpixel;         //!< 1 byte
  int imageinfo;            //!< 1 byte

  int _alphabits;           //!< (derived from imageinfo)
  int _origin;              //!< (derived from imageinfo)
} TGA_HEADER;

typedef struct {
  int width;              //!< Width of the image.
  int height;             //!< Height of the image.
  int original_width;     //!< Original width of the image. Different from #width when #TGA_RESCALE flag is set.
  int original_height;    //!< Original height of the image. Different from #height when #TGA_RESCALE flag is set.
  EPixFormat pixformat;   //!< Pixels' format.
  int bytesperpixel;      //!< Bytes per one pixel.
  unsigned char *data;    //!< Pointer to image data.
} TGA_INFO;


//========================================================================
// Flags for TGA_Read
//========================================================================

/**
 *  Forces tgaRead() to rescale images to closest larger @c 2^Nx2^M resolution.
 */
#define TGA_RESCALE   0x00000001

/**
 *  Forces the first pixel of the image to be the upper left corner of the
 *  image (default is the lower left corner of the image).
 */
#define TGA_ORIGIN_UL 0x00000004


//========================================================================
// Prototypes
//========================================================================

int tgaRead(FILE *f, TGA_INFO *t, int flags);

int tgaWrite(const char *file_name, TGA_INFO *tga, int flags);

