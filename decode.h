#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint image_capacity;
    uint file_extn_size;
    char magic[2];
    char extnfile[MAX_FILE_SUFFIX];
    //char extnfile[MAX_FILE_SUFFIX];

    /* decode File Info */
    char *decode_fname;
    FILE *decode_secret;
    char extn_decode_file[MAX_FILE_SUFFIX];
    char decode_data[MAX_SECRET_BUF_SIZE];
    long size_decode_file;

    /* Stego Image Info 
    char *stego_image_fname;
    FILE *fptr_stego_image;*/

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);


/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* decode Magic String */
Status decode_magic_string(char *magic_string, char *magic,DecodeInfo *decInfo);

/* decode file entension size*/
Status decode_extn_size(DecodeInfo *decInfo, FILE *fptr_stego_image);

/* decode secret file extenstion */
Status decode_file_extn(char *file_extn, DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode function, which does the real decoding */
Status decode_data_to_image(int size, DecodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/* Encode size to LSB */
uint decode_size_to_lsb(char *buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif

