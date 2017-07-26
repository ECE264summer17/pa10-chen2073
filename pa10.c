#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

void writeSortedAsciiCount(char *filename, ListNode *list)
{
   FILE *fp = fopen(filename, "w");

   if (fp == NULL) {
      fprintf(stderr, "can't open the output file\n.");
   }
   printList(list, fp);
}
  

/* take an array of ascii counts, enqueue into a list */

ListNode *enqueAsciiCount(long *asciiCount) 
{
   ListNode *list = NULL;
   int i;
   for (i = 0; i < ASCII_SIZE; i++) {
      if (asciiCount[i] > 0) {
         TreeNode *newTreeNode = buildTreeNode(i, NULL, NULL);
         if (newTreeNode == NULL) {
            fprintf(stderr, "in enqueAsciiCount.  cannot allocate memory to create new TreeNode.  Quit.\n");
            freeList(list);
            return NULL;
         }
         newTreeNode->count = asciiCount[i];
         ListNode *newListNode = addListNode(&list, newTreeNode, treeNodeCompare);
         if (newListNode == NULL) {
            fprintf(stderr, "in enqueAsciiCount.  cannot allocate memory to create new ListNode.  Quit.\n");
            freeHuffmanTree(newTreeNode);
            freeList(list);
            return NULL;
         }
      }
   }
   return list;
}

/* count the occurrences in a file */

long *countLetters(FILE *fp)
{
   long *asciiCount = (long *)malloc(sizeof(long)*ASCII_SIZE);
   if (asciiCount == NULL) {
      return NULL;
   }
   int ch;
   for (ch = 0; ch < ASCII_SIZE; ch++) {
      asciiCount[ch] = 0;
   }
   fseek(fp, 0, SEEK_SET);
   while ((ch = fgetc(fp)) != EOF) {
      asciiCount[ch] += 1;
   }
   return asciiCount;
}

/* print the huffman code */

void printHuffmanTree(char *filename, TreeNode *huffman)
{
   FILE *fp = fopen(filename, "w");

   if (fp == NULL) {
      fprintf(stderr, "can't open the output file\n");
   }
   huffmanPrint(huffman,fp);
}


void huffmanHeadingBits(char *filename, TreeNode *huffman)
{

	// Write your code here








}

int main(int argc, char **argv)
{
   if (argc != 5) {
    printf("Not enough arguments");
    return EXIT_FAILURE;
   }
   FILE * inFile = fopen(argv[1], "r");
   if (inFile == NULL) {
      fprintf(stderr, "can't open the input file.  Quit.\n");
      return EXIT_FAILURE;
   }

   /* read and count the occurrences of characters */
   long *asciiCount = countLetters(inFile);
   fclose(inFile);

   if (asciiCount == NULL) {
      fprintf(stderr, "cannot allocate memory to count the characters in input file.  Quit.\n");
      return EXIT_FAILURE;
   }

   /* enqueue the non-zero characters into list */
   ListNode *list = enqueAsciiCount(asciiCount);
   free(asciiCount);
  
   if (list == NULL) {
      fprintf(stderr, "cannot allocate memory to store the ascii in priority queue.  Quit.\n");
      return EXIT_FAILURE;
   }

   writeSortedAsciiCount(argv[2], list);

   /* build the huffman tree */
   TreeNode *huffman = buildHuffmanTree(list);
   if (huffman == NULL) {
      fprintf(stderr, "cannot allocate memory to store a huffman coding tree.  Quit.\n");
      return EXIT_FAILURE;
   }

   /* print the huffman code */
   printHuffmanTree(argv[3], huffman);
   
   
   huffmanHeadingBits(argv[4], huffman);
   
   
   freeHuffmanTree(huffman);

   return EXIT_SUCCESS;
}
