#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)             //whole encoding part occurs
{
    if(open_files(encInfo) == e_failure)            //opening files
    {
	printf("File open failed\n");
	return e_failure;
    }
    printf("File opened sucessfully\n");
    if(check_capacity(encInfo) == e_failure)          //checking the capacity of the image
    {
	printf("File and Image size is not matched\n");
	return e_failure;
    }
    printf("Image size is enough to store secret text\n");
    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)==e_failure) //copying header file of the image
    {
	printf("Error : copy bmp header Failed!!\n");
	return e_failure;
    }
    printf("Copying .bmp file header success\n");
    if(encode_magic_string(MAGIC_STRING,encInfo) == e_failure)  //encoding magic string in stego image
    {
	printf("Encoding magic_string is failed\n");
	return e_failure;
    }

    printf("Encoding magic_string is success\n");

    if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)!=e_success)  //encoding secret file extention size in stego image
    {
	printf("Encoding secret file extn size is failed\n");
	return e_failure;
    }
    printf("Encoding magic_string is Successful\n");

    if(encode_secret_file_extn(encInfo ->extn_secret_file ,encInfo )==e_failure)    //encoding extention in stego image
    {
	printf("Encoding secret file extn is failed");
	return e_failure;
    }

	printf("Encoding secret file extn is successful\n");

    if(encode_secret_file_size(encInfo -> size_secret_file,encInfo) !=e_success)   //encoding secret file size in stego image
    {
	printf("Encoding secret code size is failed\n");
	return e_failure;
    }
           
	printf("Encoding secret code size is successful\n");
    if(encode_secret_file_data(encInfo)!= e_success)                              //encoding secret file data in stego image
    {
	printf("Encoding secret file data failed\n");
	return e_failure;
    }
	printf("Encoding secret file data Successful\n");
   if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)!=e_success) //encoding remaining data to stego image
    {
	printf("Copy remaining image data failed\n");
	return e_failure;
    }
	printf("Copy remaining image data successful\n");
   
    return e_success;
}




uint get_file_size(FILE*fptr)
{
  
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status check_capacity(EncodeInfo *encInfo)    //checks the capacity of image that can store the given data or not
{
    encInfo -> size_secret_file=get_file_size(encInfo->fptr_secret);

    if( get_image_size_for_bmp(encInfo->fptr_src_image) >= 54+(strlen(MAGIC_STRING)+4+strlen(encInfo->extn_secret_file)+4+encInfo -> size_secret_file)*8 )
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)  //copy the image header information as it is
{
    char buff[54];
    rewind(fptr_src_image);
    fread(buff,1,54,fptr_src_image);
    fwrite(buff,1,54,fptr_dest_image);
    ftell(fptr_dest_image);
    if(ftell(fptr_dest_image)==54)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


Status encode_magic_string(char *magic_string, EncodeInfo *encInfo) //encoding the magic string (encrpytion key)
{

    if(encode_data_to_image(magic_string,strlen(magic_string),encInfo -> fptr_src_image,encInfo -> fptr_stego_image)== e_failure)
    {
        return e_failure;
    }
        return e_success;

}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)  //encodes the data to image
{
    char buff[8];
    for(int i=0;i<size;i++)
    {
       fread(buff,1,8,fptr_src_image);
       encode_byte_to_lsb(data[i],buff);
       fwrite(buff,1,8,fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer) //encodes the data to the lsb of the image
{
    int fetch;
    for(int i=0;i<8;i++)
    {
	image_buffer[i]= image_buffer[i] & 0xFE;
	fetch= (data & (1<<i)) >>i;
	image_buffer[i]= image_buffer[i] | fetch;
    }
    return e_success;
}




Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) //checks the commanline arguments are valid or not
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
	encInfo -> src_image_fname=argv[2];
    }
    else
    {
	return e_failure;
    }
    if( argv[3]!=NULL)
    {
	encInfo -> secret_fname=argv[3];
	strcpy(encInfo -> extn_secret_file,strstr(argv[3],"."));
    }
    else
    {
	return e_failure;
    }
    if(argv[4] != NULL && strstr(argv[4],".bmp")==".bmp")
    {
	encInfo -> stego_image_fname=argv[4];
    }
    else
    {
	encInfo -> stego_image_fname="stego_img.bmp";
    }
    return e_success;
}

Status encode_secret_file_extn_size(int file_size, EncodeInfo *encInfo)  //encodes file extn size
{
    if(encode_secret_file_size(file_size,encInfo)== e_failure)
    {
        return e_failure;
    }
        return e_success;
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)   //encodes file extn
{
   if(encode_data_to_image(file_extn,strlen(file_extn),encInfo -> fptr_src_image,encInfo -> fptr_stego_image)==e_success)
   {
       return e_success;
   }
   else
   {
       return e_failure;
   }
}


Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)     //encodes the secret file size
{
    char buff[32];
    int i,fetch;
    fread(buff,1,32,encInfo -> fptr_src_image);
    for(i=0;i<32;i++)
    {
	buff[i]=buff[i] & 0XFE;
	fetch=(file_size & (1<<i))>>i;
	buff[i]= buff[i] | fetch;
    }
    fwrite(buff,1,32,encInfo -> fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)             //encodes the data which we want to encode to the image
{
    rewind(encInfo -> fptr_secret);
    int size=encInfo-> size_secret_file;
    char buff[size];
    fread(buff,1,size,encInfo -> fptr_secret);
    encode_data_to_image(buff, size, encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
    return e_success;

}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)  //copying the remaining data of the image
{
    char buff;
    while(fread(&buff,1,1,fptr_src)>0)
    {
	fwrite(&buff,1,1,fptr_dest);
    }
    return e_success;
}
