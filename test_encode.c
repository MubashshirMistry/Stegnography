#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc, char **argv)
{
    if(check_operation_type(argv) == e_encode)
    {
        printf("Encoding Selected\n");

        EncodeInfo encInfo;

        if(read_and_validate_encode_args(argv,&encInfo) == e_success)
        {
            printf("Reading and Validatig arguments successful\n");
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Encoding Successful\n");
            }
            else
            {
                printf("Encoding Failed\n");
                return -1;

            }
        }
        else 
        {
           printf("Reading and validating arguments failed\n");
          return e_failure;
        } 


    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Decoding Selected\n");

        DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            printf("Reading and Validatig arguments successful\n");
            if(do_decoding(&decInfo) == e_success)
            {
                printf("Decoding Successful\n");
            }
            else
            {
                printf("Decoding Failed\n");
                return -1;

            }
        }
        else 
        {
           printf("Reading and validating arguments failed\n");
          return e_failure;
        }
    }
    else 
    {
        printf("Invalid Input\nUsage for Encoding\n./a.out -e beautiful.bmp secret.txt [stego.bmp]\nUsage for Decoding\n./a.out -d stego.bmp [decode.txt]\n");
    }
    return 0;

}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }

    return 0;

}

