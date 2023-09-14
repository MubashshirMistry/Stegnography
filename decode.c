#include<stdio.h>
#include<string.h>
#include"common.h"
#include"types.h"
#include"decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strcmp(strchr(argv[2],'.'),".bmp") == 0)
    {
        decInfo -> stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3] == NULL)
    {
        decInfo -> decode_fname = "decode.txt";
    }
    else if(strcmp(strchr(argv[3],'.'),".txt") != 0)
    {
        printf("Pass argument 3 with .txt entension\n");
        return e_failure;
    }
    else
    {
        decInfo -> decode_fname = argv[3];
    }
    return e_success;

}

Status open_decode_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }

    // Secret file
    decInfo->decode_secret = fopen(decInfo->decode_fname, "w");
    // Do Error handling
    if (decInfo->decode_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_fname);

        return e_failure;
    }


    // No failure return e_success
    return e_success;
}


Status decode_magic_string(char *magic_string,char *magic,DecodeInfo *decInfo)
{
    char str[8];
    int res = 0;
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

    for(int i = 0; i < strlen(magic_string); i++)
    {
        fread(str,8,1,decInfo->fptr_stego_image);
        magic[i]=decode_byte_from_lsb(str);
    }

    magic[strlen(magic_string)] = '\0';

    if(strcmp(magic,magic_string) == 0)
    {
        return e_success;
    }
    return e_failure;
}
char decode_byte_from_lsb (char *image_buffer)
{
    char data =0;
    for(int i=0; i<8; i++)
    {
        image_buffer[i]&=1;
        data=(data<<1) | image_buffer[i];
    }
    return data;
}

Status decode_extn_size(DecodeInfo *decInfo, FILE *fptr_stego_image)
{
    char str[32];
    if(fread(str,32,1,decInfo->fptr_stego_image) == 0)
    {
        return e_failure;
    }
    decInfo -> file_extn_size = decode_size_to_lsb(str);

    return e_success;

}
uint decode_size_to_lsb(char *buffer)
{
    unsigned data =0;
    for(int i=0;i<32;i++)
    {
        buffer[i] &= 1;
        data = (data<<1) | buffer[i];
    }
    return data;

}
Status decode_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    char str[8];

    for(int i = 0; i < decInfo -> file_extn_size; i++)
    {
        if(fread(str, 8, 1,decInfo -> fptr_stego_image) ==0)
        {
            return e_failure;
        }

        decInfo -> extnfile[i] = decode_byte_from_lsb(str);
    }
    return e_success;

}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char str[32];
    if(fread(str,32,1,decInfo->fptr_stego_image) == 0)
    {
        return e_failure;
    }
    decInfo -> size_decode_file = decode_size_to_lsb(str);
    return e_success;

}
Status decode_data_to_image(int size, DecodeInfo *decInfo)
{
    char str[8];
    for(int i=0 ; i<decInfo->size_decode_file;i++)
    {
        if(fread(str, 8,1,decInfo -> fptr_stego_image) == 0)
        {
            return e_failure;
        }
        decInfo->decode_data[0] = decode_byte_from_lsb(str);

        fwrite(decInfo->decode_data,1,1,decInfo->decode_secret);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("Opening files is success\n");
        if(decode_magic_string(MAGIC_STRING,decInfo->magic,decInfo) == e_success)
        {
            printf("Magic string decoded successfully\n");
            if(decode_extn_size(decInfo,decInfo ->fptr_stego_image) == e_success)
            {
                printf("File extension size decoded successfully\n");
                if(decode_file_extn( decInfo -> extnfile, decInfo) == e_success)
                {
                    printf("Decoding file extension succesfull\n");
                    if(decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Secret file size decoded successfully\n");
                        if(decode_data_to_image(decInfo->size_decode_file,decInfo) == e_success)
                        {
                            printf("Secret file data decoded successfully\n");
                        }
                        else
                        {
                            printf("Decoding secret file data failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Secret file size decoding failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Decoding file extension failed\n");
                    return e_failure;
                }
            }
            else
            {
                printf("File extension size decoding failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("Magic String decoding failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("Open Files is a failure\n");
        return e_failure;
    }
    return e_success;
}
