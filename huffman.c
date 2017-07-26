#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

long treeNodeCount(TreeNode *node)
{
   if (node == NULL) {
      return 0;
   }
   return node->count;
}


TreeNode *buildTreeNode(int label, TreeNode *left, TreeNode *right)
{
   TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
   if (node == NULL) {
      return NULL;
   }
   node->label = label;
   node->count = treeNodeCount(left) + treeNodeCount(right);
   node->left = left;
   node->right = right;
   return node;
}

int treeNodeCompare(TreeNode *tp1, TreeNode *tp2)
{
   long diff = tp1->count - tp2->count;
   if (diff == 0) {
      return tp1->label - tp2->label;
   } else {
      return (diff < 0? -1: 1);
   }
}

/* build a huffman tree */

TreeNode *buildHuffmanTree(ListNode *list)
{
   int label = ASCII_SIZE;
   ListNode *firstNode = removeListNode(&list);
   while (firstNode != NULL) {
      ListNode *secondNode = removeListNode(&list);
      if (secondNode == NULL) {
         TreeNode *huffman = (TreeNode *)firstNode->ptr;
         free(firstNode);
         return huffman;
      }
      TreeNode *newTreeNode = buildTreeNode(label, (TreeNode *)firstNode->ptr, (TreeNode *)secondNode->ptr);
      free(firstNode);
      free(secondNode);
      if (newTreeNode == NULL) {
         fprintf(stderr, "in buildHuffmanTree. can't allocate memory for new TreeNode. Quit\n");
         freeList(list);
         return NULL;
      }
      ListNode *newListNode = addListNode(&list, newTreeNode, treeNodeCompare);
      if (newListNode == NULL) {
         fprintf(stderr, "in buildHuffmanTree. can't allocate memory for new ListNode. Quit\n");
         freeHuffmanTree(newTreeNode);
         freeList(list);
         return NULL;
      }
      firstNode = removeListNode(&list);
      label++;
   }
   return NULL;
}

/* destroy a tree, deallocating all memory associated with the tree      */

void freeHuffmanTree(TreeNode *ptr)
{
   if (ptr == NULL) {
      return;
   }
   freeHuffmanTree(ptr->left);
   freeHuffmanTree(ptr->right);
   free(ptr);
}

/* helper function for a recursive call to print the huffman code        */

void huffmanPrintHelper(const TreeNode *node, int index, char *huffman_code, FILE * fp)
{
   if (node == NULL) { 
      return;
   }
   if (isLeafNode(node) == 1) {
      fprintf(fp, "%c:", node->label);
      int i;
      for (i = 0; i < index; i++) {
         fprintf(fp, "%c", huffman_code[i]);
      }
      fprintf(fp, "\n");
      return;
   }
   huffman_code[index] = '0';
   huffmanPrintHelper(node->left, index+1, huffman_code, fp);
   huffman_code[index] = '1';
   huffmanPrintHelper(node->right, index+1, huffman_code, fp);
}

/* given a huffman coding tree, print the huffman code for each ASCII    */

void huffmanPrint(const TreeNode *ptr, FILE * fp) 
{
   if (ptr == NULL) {
      return;
   }
   char huffman_code[ASCII_SIZE];
   huffmanPrintHelper(ptr, 0, huffman_code, fp);
}


/* is a given TreeNode a leaf node                                          */

int isLeafNode(const TreeNode *node) {
   if (node == NULL) {
      return 0;
   }
   if ((node->left == NULL) && (node->right == NULL)) {
      return 1;
   } else {
      return 0;
   }
}


ListNode *addListNode(ListNode **list, TreeNode *new_object, 
                  int (*cmpFunction)(TreeNode *, TreeNode *))
{
   if (new_object == NULL) {
      return NULL;
   }
   ListNode *node = (ListNode *)malloc(sizeof(ListNode));
   if (node == NULL) {
      return NULL;
   }
   ListNode dummy;
   dummy.next = *list;
   ListNode *prev = &dummy;
   ListNode *curr = *list;
   while (curr != NULL) {
      if (cmpFunction(curr->ptr, new_object) >= 0) {
         // node should be between prev and curr
         break;
      } else {
         prev = curr;
         curr = curr->next;
      }
   }
   node->ptr = new_object;
   node->next = curr;
   prev->next = node;
   *list = dummy.next;
   return node;
}

                       
ListNode *removeListNode(ListNode **list)
{
   ListNode *node = *list;
   if (node != NULL) {
      *list = node->next;
      node->next = NULL; // set the returned ListNode to be a single-node linked list
   }
   return node;
}


/* destroy an entire linked list    */

void freeList(ListNode *list)
{
   while (list != NULL) {
      ListNode *tmp = list->next;
      // call the destroy_fn to free the memory associated with list->ptr
      freeHuffmanTree(list->ptr);
      // free the memory for list 
      free(list);
      list = tmp;
   } 
}

/* print an entire linked list                                           */
 
void printList(const ListNode *list, FILE * fp) 
{
   while (list != NULL) {
      fprintf(fp,"%c:",(list->ptr->label));
      fprintf(fp,"%ld\n",(list->ptr->count));
      list = list->next;
   } 
}
