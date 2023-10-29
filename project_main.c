/*
Name: Dhatchaneswaran R
Date: 28/8/2023
Project : Stegonography
   */

#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc,char *argv[])                                     //getting command line arguments from user
{
    
    EncodeInfo encInfo;                                             //structure variable for encoding
    DecodeInfo decInfo;                                             //structure variable for decoding
    uint img_size;
    if(argc == 1)
    {
	printf("Invalid commandline arguments\n");               //error message for invalid command line arguments
    }

    if(check_operation_type(argv)== e_encode)                       //checking user input for encoding
    {
	encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
	if(read_and_validate_encode_args(argv,&encInfo)==e_success)  //reading command lines are valid for encoding
	{
	    if(do_encoding(&encInfo)==e_success)                    //calling do encoding
	    {
		printf("Encoding successful\n");
	    }
	    else
	    {
		printf("Encoding failure\n");
	    }
	}
   }
   else if(check_operation_type(argv)==e_decode)                   //checking user input for decoding
    {
	if(read_and_validate_decode_args(argv,&decInfo)==e_success)  //reading command lines are valid for decoding
	{
	    if(do_decoding(&decInfo,argv)==e_success)                   //calling do decoding
	    {
		printf("Decoding successful\n");
	    }
	    else
	    {
		printf("Decoding failure\n");
	    }
	}

    }
    else
    {
	printf("Invalid commandline arguments\n");               //error message for invalid command line arguments
    }
    
}


OperationType check_operation_type(char *argv[])               //defining function for check operation
{
    if(argv[1] != NULL)
    {
	if(strcmp(argv[1],"-e")==0)                            // -e means it returns e_encode
	{
	    return e_encode;
	}
	else if(strcmp(argv[1],"-d")==0)
	{
	    return e_decode;                                   //-d means it returns e_decode
	}
	else
	{
	    return e_unsupported;                             //invalid means e_unsupported
	}
    }
}
