
#include "types.h"   //cotains user defined types

typedef struct _DecodeInfo
{
      /* decoded file Info  */
    char *decode_fname;           // to store new file name
    FILE *fptr_decoded;          //to store new file address
    int secret_file_extn_size;   // to store secret file extention size
    char *secret_file_extn;      //to store secret file extention
    int secret_file_size;        //to store secret file size
    char *magic_data;            //to store magic string data

    /* stego image Info  */
    char *stego_image_fname;  //stego image name
    FILE *fptr_stego_image;   //stego image address

} DecodeInfo;




Status open_files_dec(DecodeInfo *decInfo);

/* raed and validate decode args from cmd lne arg  */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo,char *argv[]);

/*to decode Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* it is used perform decoding in every function */
Status decode_data_from_image(int size, FILE *fptr_stego_image,char *data);
/* Decode a byte from LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/*  Decode secret file extn size  */
Status decode_secret_file_extn_size(FILE *fptr_stego_image,DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(char *secret_file_extn, DecodeInfo *decInfo);

/* Decode secret file size*/
Status decode_secret_file_size(FILE *fptr_stego_image,DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);


