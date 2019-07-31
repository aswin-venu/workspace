/* Compute sha256 hash of any given file
 * Auther : Aswin
 * Date   : 31-07-2019
 * This file is under GPL V1
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>

/* @function fsize
 * @desc Function to find the size of a given file
 * @param file_name : file name
 * @returns : File size in long int
 */
static long int fsize(char *file_name)
{
    FILE *pfile;
    long int size;

    pfile = fopen(file_name, "r");
    
    /* Seek till the end of the file */
    fseek(pfile, 0, SEEK_END);
    
    /* Return the size of the file */ 
    size =  ftell(pfile);
    fclose(pfile);
    
    return size;
}

/* @function : hash_SHA256
 * @desc  : This function computes the sha256 of a given content.
 * @param : content - pointer to a an array of unsigned characters
 * @param : length - length of the content array
 * @param : hash - pointer to an array where you want the hash to get stored
 */
static void hash_SHA256(unsigned char* content, long int length, unsigned char *hash)
{
    
    unsigned i;
    unsigned int l = gcry_md_get_algo_dlen(GCRY_MD_SHA256); /* get digest length (used later to print the result) */

    gcry_md_hd_t h;
    gcry_md_open(&h, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE); /* initialise the hash context */
    gcry_md_write(h, content, length); /* hash some text */
    gcry_md_final(h);
    hash = gcry_md_read(h, GCRY_MD_SHA256); /* get the result */

    for (i = 0; i < l; i++)
    {   
        printf("%02x", hash[i]); /* print the result */
    }   
    printf("\n");

}

int main(int argc, char **argv)
{
    FILE *filepointer;
    char *file_name;
    long int file_size;
    unsigned char *file_content;
    unsigned char *hash;
    
    if(argc <= 0){
        printf("Please provide the arguments");
        exit(0);
    }

    /* Get the file size */
    file_size = fsize(argv[1]);
    
    printf("Size of the file is = %d", file_size);
   
    filepointer = fopen(argv[1],"r");
    
    /* Check if the file pointer is empty or not */
    if(filepointer == NULL){
        printf("Failed to read the file %s!", "/home/dell/text.txt");
        exit(0);
    }
     
    /* File successfully opened */
    printf("File successfully opened!");
    
    /* Dynamically allocate the buffer for the file content */
    file_content = (unsigned char*)malloc(file_size);
    hash = (unsigned char*)malloc(32);
 
    /* Check memory got allocated or not */
    if(file_content == NULL || hash == NULL){
        printf("Dynamic memory allocation failed");
        exit(0);
    }
 
    fread(file_content, 1, file_size, filepointer);
    
    hash_SHA256(file_content, file_size, hash);
    
    /* Free up the memory */
    free(file_content);
    free(hash);
    /* Close the file */
    fclose(filepointer);
}
