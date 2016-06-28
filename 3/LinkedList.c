#include "LinkedList.h"

#pragma mark - Singly Linked List Structure

typedef struct _linked_list_node {
    void *pData;
    struct _linked_list_node *pNext;
} LinkedListNode;

typedef struct _linked_list {
    LinkedListNode *pHead;
    LinkedListNode *pTail;
    int itemSize;
    int length;
} LinkedList;

#pragma mark - Inner Function

static LinkedListNode *sllNodeAt(const LinkedList *pList, int index) {
    LinkedListNode *pNode = pList->pHead;
    for (int i = 0; i < index; i++) {
        pNode = pNode->pNext;
    }
    return pNode;
}

#pragma mark - Singly Linked List Make List

LinkedList *LinkedListInit(int itemSize) {
    if (itemSize <= 0) {
        return NULL;
    }

    LinkedList *pList = malloc(sizeof(LinkedList));
    if (!pList) {
        return NULL;
    }

    pList->pHead = NULL;
    pList->pTail = NULL;
    pList->itemSize = itemSize;
    pList->length = 0;

    return pList;
}

LinkedList *LinkedListSubList(const LinkedList *pList, int start, int length) {
    if (!pList) {
        return NULL;
    }

    if (start < 0 || length < 0 || start + length > pList->length) {
        return NULL;
    }

    LinkedList *pOut = LinkedListInit(pList->itemSize);
    if (!pOut) {
        return NULL;
    }

    LinkedListNode *pNode = sllNodeAt(pList, start);
    for (int i = 0; i < length; i++) {
        if (!LinkedListAppendItem(pOut, pNode->pData)) {
            LinkedListDestroy(pOut);
            return NULL;
        }
        pNode = pNode->pNext;
    }

    return pOut;
}

LinkedList *LinkedListCopy(const LinkedList *pList) {
    return LinkedListSubList(pList, 0, pList->length);
}

LinkedList *LinkedListConcat(const LinkedList *pListA, const LinkedList *pListB) {
    if (!pListA || !pListB) {
        return NULL;
    }

    LinkedList *pOut = LinkedListCopy(pListA);
    if (!pOut) {
        return NULL;
    }

    if (!LinkedListAppendLList(pOut, pListB)) {
        LinkedListDestroy(pOut);
        return NULL;
    }

    return pOut;
}

#pragma mark - Singly Linked List Get Properties

int LinkedListLength(const LinkedList *pList) {
    return pList ? pList->length : -1;
}

int LinkedListItemSize(const LinkedList *pList) {
    return pList ? pList->itemSize : -1;
}

#pragma mark - Singly Linked List Manipulate Whole List

void LinkedListDestroy(LinkedList *pList) {
    if (!pList) {
        return;
    }

    LinkedListClear(pList);
    free(pList);
}

void LinkedListClear(LinkedList *pList) {
    if (!pList) {
        return;
    }

    while (pList->pHead) {
        LinkedListDeleteItem(pList, 0);
    }
}

void LinkedListTraverse(LinkedList *pList, void (*pFunc)(void *)) {
    if (!pList || !pFunc) {
        return;
    }

    LinkedListNode *pNode = pList->pHead;
    while (pNode) {
        pFunc(pNode->pData);
        pNode = pNode->pNext;
    }
}

BOOL LinkedListSort(LinkedList *pList, int (*pCompareFunc)(const void *, const void *), BOOL ascend) {
    if (!pList || !pCompareFunc) {
        return FALSE;
    }

    if (pList->length < 2) {
        return TRUE;
    }

    // Bubble sort
    BOOL isInOrder = FALSE;
    /*LinkedListNode *pCurr = pList->pHead;
    while (pCurr->pNext && !isInOrder) {
        //LinkedListNode *pNode = pCurrHead;
        LinkedListNode *pNode = pList->pHead;
        isInOrder = TRUE;
        while (pNode->pNext) {
            if ((ascend && 0 < pCompareFunc(pNode->pData, pNode->pNext->pData)) ||
                (!ascend && 0 > pCompareFunc(pNode->pData, pNode->pNext->pData))) {
                void *pT = pNode->pData;
                pNode->pData = pNode->pNext->pData;
                pNode->pNext->pData = pT;
                isInOrder = FALSE;
            }
            pNode = pNode->pNext;
        }
        pCurr = pCurr->pNext;
    }*/
    for (int i = 0; i < pList->length - 1; i++) {
        LinkedListNode *pNode = pList->pHead;
        isInOrder = TRUE;
        for (int j = 0; j < pList->length - 1 - i; j++) {
            if ((ascend && 0 < pCompareFunc(pNode->pData, pNode->pNext->pData)) ||
                (!ascend && 0 > pCompareFunc(pNode->pData, pNode->pNext->pData))) {
                void *pT = pNode->pData;
                pNode->pData = pNode->pNext->pData;
                pNode->pNext->pData = pT;
                isInOrder = FALSE;
            }
            pNode = pNode->pNext;
        }
    }

    return TRUE;
}

BOOL LinkedListReverse(LinkedList *pList) {
    if (!pList) {
        return FALSE;
    }

    for (int i = 0; i < pList->length / 2; i++) {
        if (!LinkedListSwapItems(pList, i, pList->length - 1 - i)) {
            // Perhaps never happen
            return FALSE;
        }
    }

    return TRUE;
}

// Return -1 if no such item, return -2 if parameters invalid
int LinkedListFind(const LinkedList *pList, const void *pVal, int (*pCompareFunc)(const void *, const void *)) {
    if (!pList || !pVal || !pCompareFunc) {
        return -2;
    }

    LinkedListNode *pNode = pList->pHead;
    for (int i = 0; pNode; i++) {
        if (0 == pCompareFunc(pNode->pData, pVal)) {
            return i;
        }
        pNode = pNode->pNext;
    }

    return -1;
}

#pragma mark - Singly Linked List Manipulate Single Item

BOOL LinkedListGetItem(const LinkedList *pList, int index, void *pOut) {
    if (!pList || !pOut) {
        return FALSE;
    }

    if (index < 0 || index >= pList->length) {
        return FALSE;
    }

    memcpy(pOut, sllNodeAt(pList, index)->pData, pList->itemSize);

    return TRUE;
}

BOOL LinkedListGetHeadItem(const LinkedList *pList, void *pOut) {
    if (!pList || !pOut) {
        return FALSE;
    }

    if (!pList->pHead) {
        return FALSE;
    }

    memcpy(pOut, pList->pHead->pData, pList->itemSize);

    return TRUE;
}

BOOL LinkedListGetTailItem(const LinkedList *pList, void *pOut) {
    if (!pList || !pOut) {
        return FALSE;
    }

    if (!pList->pTail) {
        return FALSE;
    }

    memcpy(pOut, pList->pTail->pData, pList->itemSize);

    return TRUE;
}

BOOL LinkedListSetItem(LinkedList *pList, int index, const void *pIn) {
    if (!pList || !pIn) {
        return FALSE;
    }

    if (index < 0 || index >= pList->length) {
        return FALSE;
    }

    memcpy(sllNodeAt(pList, index)->pData, pIn, pList->itemSize);

    return TRUE;
}

// Accept index range from 0 to pList->length
BOOL LinkedListInsertItem(LinkedList *pList, int index, const void *pIn) {
    if (!pList || !pIn) {
        return FALSE;
    }

    if (index < 0 || index > pList->length) {
        return FALSE;
    }

    LinkedListNode *pNode = malloc(sizeof(LinkedListNode));
    if (!pNode) {
        return FALSE;
    }

    pNode->pData = malloc((size_t) pList->itemSize);
    if (!pNode->pData) {
        free(pNode);
        return FALSE;
    }

    memcpy(pNode->pData, pIn, pList->itemSize);
    pNode->pNext = NULL;

    if (index == 0) {
        pNode->pNext = pList->pHead;
        pList->pHead = pNode;
        if (pList->length == 0) {
            pList->pTail = pNode;
        }
    } else {
        LinkedListNode *pPrev = sllNodeAt(pList, index - 1);
        pNode->pNext = pPrev->pNext;
        pPrev->pNext = pNode;
        if (index == pList->length) {
            pList->pTail = pNode;
        }
    }

    pList->length++;

    return TRUE;
}

// Accept index range from 0 to pList->length
BOOL LinkedListInsertLList(LinkedList *pList, int index, const LinkedList *pNewList) {
    if (!pList || !pNewList) {
        return FALSE;
    }

    if (index < 0 || index > pList->length) {
        return FALSE;
    }

    if (pList->itemSize != pNewList->itemSize) {
        return FALSE;
    }

    LinkedList *pTempList = LinkedListCopy(pNewList);
    if (!pTempList) {
        return FALSE;
    }

    if (pList->length == 0) {
        pList->pHead = pTempList->pHead;
        pList->pTail = pTempList->pTail;
    } else {
        if (index == 0) {
            pTempList->pTail->pNext = pList->pHead;
            pList->pHead = pTempList->pHead;
        } else if (index == pList->length) {
            pList->pTail->pNext = pTempList->pHead;
            pList->pTail = pTempList->pTail;
        } else {
            pTempList->pTail->pNext = sllNodeAt(pList, index);
            sllNodeAt(pList, index - 1)->pNext = pTempList->pHead;
        }
    }

    pList->length += pNewList->length;
    free(pTempList); // Not Destroy because should not free the nodes

    return TRUE;
}

BOOL LinkedListAppendItem(LinkedList *pList, const void *pIn) {
    return LinkedListInsertItem(pList, pList->length, pIn);
}

BOOL LinkedListAppendLList(LinkedList *pList, const LinkedList *pNewList) {
    return LinkedListInsertLList(pList, pList->length, pNewList);
}

BOOL LinkedListPrependItem(LinkedList *pList, const void *pIn) {
    return LinkedListInsertItem(pList, 0, pIn);
}

BOOL LinkedListPrependLList(LinkedList *pList, const LinkedList *pNewList) {
    return LinkedListInsertLList(pList, 0, pNewList);
}

BOOL LinkedListMoveItem(LinkedList *pList, int oldIndex, int newIndex) {
    if (!pList) {
        return FALSE;
    }

    if (oldIndex < 0 || oldIndex >= pList->length || newIndex < 0 || newIndex >= pList->length) {
        return FALSE;
    }

    if (oldIndex == newIndex) {
        return TRUE;
    }

    if (oldIndex == 0) {
        LinkedListNode *pThis = pList->pHead;
        LinkedListNode *pTemp = sllNodeAt(pList, newIndex);
        pList->pHead = pThis->pNext;
        pThis->pNext = pTemp->pNext;
        pTemp->pNext = pThis;
        if (newIndex == pList->length - 1) {
            pList->pTail = pThis;
        }
    } else if (oldIndex == pList->length - 1) {
        LinkedListNode *pThis = pList->pTail;
        pList->pTail = sllNodeAt(pList, oldIndex - 1);
        pList->pTail->pNext = NULL;

        if (newIndex == 0) {
            pThis->pNext = pList->pHead;
            pList->pHead = pThis;
        } else {
            LinkedListNode *pTempPrev = sllNodeAt(pList, newIndex - 1);
            LinkedListNode *pTemp = pTempPrev->pNext;
            pTempPrev->pNext = pThis;
            pThis->pNext = pTemp;
        }
    } else {
        LinkedListNode *pThisPrev = sllNodeAt(pList, oldIndex - 1);
        LinkedListNode *pThis = pThisPrev->pNext;
        LinkedListNode *pTemp = sllNodeAt(pList, newIndex);
        pThisPrev->pNext = pThis->pNext;

        if (newIndex == 0) {
            pThis->pNext = pList->pHead;
            pList->pHead = pThis;
        } else {
            if (oldIndex < newIndex) {
                pThis->pNext = pTemp->pNext;
                pTemp->pNext = pThis;
                if (newIndex == pList->length - 1) {
                    pList->pTail = pThis;
                }
            } else {
                sllNodeAt(pList, newIndex - 1)->pNext = pThis;
                pThis->pNext = pTemp;
            }
        }
    }

    return TRUE;
}

BOOL LinkedListSwapItems(LinkedList *pList, int aIndex, int bIndex) {
    if (!pList) {
        return FALSE;
    }

    if (aIndex < 0 || aIndex >= pList->length || bIndex < 0 || bIndex >= pList->length) {
        return FALSE;
    }

    if (aIndex == bIndex) {
        return TRUE;
    }

    LinkedListNode *pNodeA = sllNodeAt(pList, aIndex);
    LinkedListNode *pNodeB = sllNodeAt(pList, bIndex);
    void *pDataTemp = pNodeA->pData;
    pNodeA->pData = pNodeB->pData;
    pNodeB->pData = pDataTemp;

    return TRUE;
}

BOOL LinkedListReplaceItemAWithB(LinkedList *pList, int aIndex, int bIndex) {
    if (!pList) {
        return FALSE;
    }

    if (aIndex < 0 || aIndex >= pList->length || bIndex < 0 || bIndex >= pList->length) {
        return FALSE;
    }

    if (aIndex == bIndex) {
        return TRUE;
    }

    void *pData = sllNodeAt(pList, bIndex)->pData;
    LinkedListSetItem(pList, aIndex, pData);

    return TRUE;
}

BOOL LinkedListDeleteItem(LinkedList *pList, int index) {
    if (!pList) {
        return FALSE;
    }

    if (index < 0 || index >= pList->length) {
        return FALSE;
    }

    if (index > 0) {
        LinkedListNode *pPrev = sllNodeAt(pList, index - 1);
        LinkedListNode *pThis = pPrev->pNext;
        pPrev->pNext = pThis->pNext;
        free(pThis->pData);
        free(pThis);
        if (index == pList->length - 1) {
            pPrev->pNext = NULL;
            pList->pTail = pPrev;
        }
    } else {
        LinkedListNode *pThis = pList->pHead;
        pList->pHead = pThis->pNext;
        free(pThis->pData);
        free(pThis);
        if (pList->length == 1) {
            pList->pHead = NULL;
            pList->pTail = NULL;
        }
    }
    pList->length--;

    return TRUE;
}

BOOL LinkedListDeleteHeadItem(LinkedList *pList) {
    return LinkedListDeleteItem(pList, 0);
}

BOOL LinkedListDeleteTailItem(LinkedList *pList) {
    return LinkedListDeleteItem(pList, pList->length - 1);
}
