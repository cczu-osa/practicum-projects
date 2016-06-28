#ifndef __LinkedList__
#define __LinkedList__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _BOOL {
    FALSE = 0, TRUE
} BOOL;

#pragma mark - Type Definition

typedef struct _linked_list LinkedList;

#pragma mark - Singly Linked List Make List

LinkedList *LinkedListInit(int itemSize);

LinkedList *LinkedListSubList(const LinkedList *pList, int start, int length);

LinkedList *LinkedListCopy(const LinkedList *pList);

LinkedList *LinkedListConcat(const LinkedList *pListA, const LinkedList *pListB);

#pragma mark - Singly Linked List Get Properties

int LinkedListLength(const LinkedList *pList);

int LinkedListItemSize(const LinkedList *pList);

#pragma mark - Singly Linked List Manipulate Whole List

void LinkedListDestroy(LinkedList *pList);

void LinkedListClear(LinkedList *pList);

void LinkedListTraverse(LinkedList *pList, void (*pFunc)(void *));

BOOL LinkedListSort(LinkedList *pList, int (*pCompareFunc)(const void *, const void *), BOOL ascend);

BOOL LinkedListReverse(LinkedList *pList);

// Return -1 if no such item, return -2 if parameters invalid
int LinkedListFind(const LinkedList *pList, const void *pVal, int (*pCompareFunc)(const void *, const void *));

#pragma mark - Singly Linked List Manipulate Single Item

BOOL LinkedListGetItem(const LinkedList *pList, int index, void *pOut);

BOOL LinkedListGetHeadItem(const LinkedList *pList, void *pOut);

BOOL LinkedListGetTailItem(const LinkedList *pList, void *pOut);

BOOL LinkedListSetItem(LinkedList *pList, int index, const void *pIn);

// Accept index range from 0 to pList->length
BOOL LinkedListInsertItem(LinkedList *pList, int index, const void *pIn);

// Accept index range from 0 to pList->length
BOOL LinkedListInsertLList(LinkedList *pList, int index, const LinkedList *pNewList);

BOOL LinkedListAppendItem(LinkedList *pList, const void *pIn);

BOOL LinkedListAppendLList(LinkedList *pList, const LinkedList *pNewList);

BOOL LinkedListPrependItem(LinkedList *pList, const void *pIn);

BOOL LinkedListPrependLList(LinkedList *pList, const LinkedList *pNewList);

BOOL LinkedListMoveItem(LinkedList *pList, int oldIndex, int newIndex);

BOOL LinkedListSwapItems(LinkedList *pList, int aIndex, int bIndex);

BOOL LinkedListReplaceItemAWithB(LinkedList *pList, int aIndex, int bIndex);

BOOL LinkedListDeleteItem(LinkedList *pList, int index);

BOOL LinkedListDeleteHeadItem(LinkedList *pList);

BOOL LinkedListDeleteTailItem(LinkedList *pList);

#endif //__LinkedList__
