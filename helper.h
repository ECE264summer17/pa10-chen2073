#ifndef PA10_HELPER
#define PA10_HELPER

#include <stdio.h>

void huffmanHeadingBits(char *filename, TreeNode *huffman);

int writeBit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curbyte);

void char2bits(FILE * fptr, int ch, unsigned char * whichbit, unsigned char * curbyte);

void HeaderHelper(TreeNode * tn, FILE * fptr, unsigned char * whichbit, unsigned char * curbyte);

#endif
