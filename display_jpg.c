#include "display_jpg.h"
#include "jpeglib.h"
#include "jerror.h"

/* map shared memmory to a opened file */
void *fd_mmap(int fd, unsigned int filesize)
{
    caddr_t fdmem;

    if ((fdmem = mmap(0, filesize, PROT_READ,
                    MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror(__func__);
        return (void *) (-1);
    }

    return fdmem;
}

int display_jpg(char * filename, unsigned int *lcd_ptr,unsigned int x_s, unsigned int y_s, unsigned int fb_width,unsigned int fb_height,unsigned int fb_depth)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    int fd;
    unsigned char *buffer;
    struct stat st;

    // int fbdev;
    // char *fb_device;
    unsigned char *fbmem = (unsigned char *)lcd_ptr;


    unsigned char *fdmem;
    unsigned int screensize;
   
    register unsigned int x;
    register unsigned int y;

    
     /* open input jpeg file */
    if ((fd = open(filename, O_RDONLY)) < 0) {
        perror("open");
        exit(-1);
    }

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(-1);
    }

    fdmem = fd_mmap(fd, st.st_size);
    
        /* init jpeg decompress object error handler */
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, fdmem, st.st_size);
    
    /* read jpeg header */
    jpeg_read_header(&cinfo, TRUE);

    /* decompress process */
    jpeg_start_decompress(&cinfo);
    if ((cinfo.output_width > fb_width) ||
            (cinfo.output_height > fb_height)) {
        printf("too large jpeg file, can't display\n");
            }
    buffer = (unsigned char *) malloc(cinfo.output_width *
            cinfo.output_components);

       y = 0;
    // printf("%d  %d\n",fb_height,fb_width);
    // printf("%d\n",cinfo.output_components);
    // printf("%d\n",cinfo.output_height);
    // printf("%d\n",cinfo.output_width);
    // puts(filename);
    //printf("%d\n",st.st_size);
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        /*if (fb_depth == 16) {
            unsigned short color;

            for (x = 0; x < cinfo.output_width; x++) {
                color =
                    RGB888toRGB565(buffer[x * 3],
                            buffer[x * 3 + 1], buffer[x * 3 + 2]);
                fb_pixel(fbmem, fb_width, fb_height, x, y, color);
            }
        } else if (fb_depth == 24) {
            memcpy((unsigned char *) fbmem + y * fb_width * 3,
                    buffer, cinfo.output_width * cinfo.output_components);
        } else if (fb_depth == 32) {
            // memcpy((unsigned char *) fbmem + y * fb_width * 4,
                    // buffer, cinfo.output_width * cinfo.output_components);
            for (x = 0; x < cinfo.output_width; x++) {
                * (fbmem + y * fb_width * 4 + x * 4)     = (unsigned char)       buffer[x * 3 + 2];
                * (fbmem + y * fb_width * 4 + x * 4 + 1) = (unsigned char)       buffer[x * 3 + 1];
                * (fbmem + y * fb_width * 4 + x * 4 + 2) = (unsigned char)       buffer[x * 3 + 0];
                * (fbmem + y * fb_width * 4 + x * 4 + 3) = (unsigned char) 0;
            }
        }*/
        
        for (x = 0+x_s; x < cinfo.output_width+x_s; x++) {
                * (fbmem + (y+y_s) * fb_width * 4 + x * 4)     = (unsigned char)       buffer[(x-x_s) * 3 + 2];
                * (fbmem + (y+y_s) * fb_width * 4 + x * 4 + 1) = (unsigned char)       buffer[(x-x_s) * 3 + 1];
                * (fbmem + (y+y_s) * fb_width * 4 + x * 4 + 2) = (unsigned char)       buffer[(x-x_s) * 3 + 0];
                * (fbmem + (y+y_s) * fb_width * 4 + x * 4 + 3) = (unsigned char) 0;
            }
        y++;    // next scanline
    }


    /* finish decompress, destroy decompress object */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    /* release memory buffer */
    free(buffer);

    munmap(fdmem, (size_t) st.st_size);
    close(fd);
}

