/*
 *
 */
#ifndef VOLCANO_LIST_H
#define VOLCANO_LIST_H

#include <Volcano/Common.h>

VOLCANO_C_BEGIN

/* List Node */

typedef struct VolcanoListNodeStruct {
	struct VolcanoListNodeStruct *prev;
	struct VolcanoListNodeStruct *next;
} VolcanoListNode;

VOLCANO_INLINE void volcanoListNodeReset(VolcanoListNode *node)
{
	VOLCANO_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

VOLCANO_INLINE bool volcanoListNodeIsLinked(VolcanoListNode *node)
{
	VOLCANO_ASSERT(node != NULL);

	return (node->prev != node);
}

VOLCANO_INLINE void volcanoListNodeLink(VolcanoListNode *node, VolcanoListNode *prev, VolcanoListNode *next)
{
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(prev != NULL);
	VOLCANO_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

VOLCANO_INLINE void volcanoListNodeUnlink(VolcanoListNode *node)
{
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(node->prev != NULL);
	VOLCANO_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node;
	node->next = node;
}

VOLCANO_INLINE VolcanoListNode *volcanoListNodePrev(VolcanoListNode *node)
{
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(node->prev != NULL);
	VOLCANO_ASSERT(node->next != NULL);

	return node->prev;
}
VOLCANO_INLINE VolcanoListNode *volcanoListNodeNext(VolcanoListNode *node)
{
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(node->prev != NULL);
	VOLCANO_ASSERT(node->next != NULL);

	return node->next;
}

/* List */

typedef struct {
	VolcanoListNode knot;
} VolcanoList;

VOLCANO_INLINE void volcanoListReset(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	volcanoListNodeReset(&list->knot);
}

VOLCANO_INLINE VolcanoListNode *volcanoListHead(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	return volcanoListNodeNext(&list->knot);
}

VOLCANO_INLINE VolcanoListNode *volcanoListTail(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	return volcanoListNodePrev(&list->knot);
}

VOLCANO_INLINE VolcanoListNode *volcanoListKnot(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	return &list->knot;
}

VOLCANO_INLINE bool volcanoListIsEmpty(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	return !volcanoListNodeIsLinked(&list->knot);
}

VOLCANO_INLINE int volcanoListLength(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	int n = 0;
	VolcanoListNode *p;

	for (p = volcanoListHead(list); p != volcanoListKnot(list); p = volcanoListNodeNext(p))
		n += 1;

	return n;
}

VOLCANO_INLINE bool volcanoListIsContains(VolcanoList *list, VolcanoListNode *node)
{
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);

	VolcanoListNode *p;

	for (p = volcanoListHead(list); p != volcanoListKnot(list); p = volcanoListNodeNext(p)) {
		if (p == node)
			return true;
	}

	return false;
}

VOLCANO_INLINE void volcanoListPrepend(VolcanoList *list, VolcanoListNode *node)
{
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);

	volcanoListNodeLink(node, volcanoListKnot(list), volcanoListHead(list));
}

VOLCANO_INLINE void volcanoListAppend(VolcanoList *list, VolcanoListNode *node)
{
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);

	volcanoListNodeLink(node, volcanoListTail(list), volcanoListKnot(list));
}

VOLCANO_INLINE VolcanoListNode *volcanoListRemoveHead(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	VolcanoListNode *p = volcanoListHead(list);
	volcanoListNodeUnlink(p);

	return p;
}

VOLCANO_INLINE VolcanoListNode *volcanoListRemoveTail(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	VolcanoListNode *p = volcanoListTail(list);
	volcanoListNodeUnlink(p);

	return p;
}

VOLCANO_INLINE void volcanoListRemoveNode(VolcanoList *list, VolcanoListNode *node)
{
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(volcanoListIsContains(list, node));

	volcanoListNodeUnlink(node);
}

VOLCANO_INLINE void volcanoListClear(VolcanoList *list)
{
	VOLCANO_ASSERT(list != NULL);

	while (!volcanoListIsEmpty(list))
		volcanoListRemoveHead(list);
}

VOLCANO_C_END

#define VOLCANO_LIST_FOREACH(pNode, pList) \
	for ((pNode) = volcanoListHead(pList); (pNode) != volcanoListKnot(pList); (pNode) = volcanoListNodeNext(pNode))

#define VOLCANO_LIST_FOREACH_REVERSE(pNode, pList) \
	for ((pNode) = volcanoListTail(pList); (pNode) != volcanoListKnot(pList); (pNode) = volcanoListNodePrev(pNode))

#endif /* VOLCANO_LIST_H */
