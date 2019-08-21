#include "display_jpeg.h"
#include "painter.h"

//#include "lcd.h"


extern JSAMPLE * image_buffer;	/* Points to large array of R,G,B-order data */
extern int image_height;	/* Number of rows in image */
extern int image_width;

extern u_int32_t width, height;

struct my_error_mgr 
{
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);

	longjmp(myerr->setjmp_buffer, 1);
}

/*display_jpeg:显示jpeg图片
 *filename：图片的路径名
 *lcd_ptr：指向LCD映射首地址
 *x_s，y_s：左上角起始点
 */
int display_jpeg (char * filename, unsigned int *lcd_ptr,unsigned int x_s, unsigned int y_s)
{

	struct my_error_mgr jerr;
	struct jpeg_decompress_struct cinfo;
	int row_stride;		/* physical row width in output buffer */
	FILE * infile;		/* source file */

	if((infile = fopen(filename, "rb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", filename);
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
  
	if(setjmp(jerr.setjmp_buffer)) 
	{    
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return -1;
	}

	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, infile);
	
	jpeg_read_header(&cinfo, TRUE);
    
	jpeg_start_decompress(&cinfo);
  
	row_stride = cinfo.output_width * cinfo.output_components;

	char *buffer;  
	buffer = (char*)(*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	unsigned int x, y;
	unsigned int color;
	char * ptr_save = buffer; 

	y = y_s;
	while(cinfo.output_scanline < cinfo.output_height && y < cinfo.output_height) 
	{    
		buffer = ptr_save;
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1);
		
		for(x=x_s; x< x_s+cinfo.output_width && x<width; x++)
		{
			color = buffer[2] | buffer[1]<<8 | buffer[0]<<16;
			//lcd_draw_point(x, y, color, lcd_ptr);
			paintPoint(x,y,color);
			buffer += 3;
		}

		y++;
	}
	/*
	//显示图片路径
	wchar_t *w_pathname = NULL;
	if( mchars_to_wchars(filename, &w_pathname) == 0)
	{
		//显示图片路径
		//Lcd_Show_FreeType(w_pathname,32,0x00,x_s+10,y_s+50, lcd_ptr);
		free(w_pathname);
	}

	*/
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return 0;
}
