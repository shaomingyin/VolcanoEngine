/*
 *
 */
#ifndef VOLCANO_LIST_H
#define VOLCANO_LIST_H

#include <Volcano/Common.h>

VOLCANO_C_BEGIN

typedef struct VolcanoListNodeStruct {
	struct VolcanoListNodeStruct* prev;
	struct VolcanoListNodeStruct* next;
} VolcanoListNode;

inline void volcanoListNodeReset(VolcanoListNode* node) {
	VOLCANO_ASSERT(node != NULL);
	node->prev = node;
	node->next = node;
}

inline bool volcanoListNodeIsLinked(const VolcanoListNode* node) {
	VOLCANO_ASSERT(node != NULL);
	return (node->prev != node);
}

inline void volcanoListNodeLink(VolcanoListNode* node, VolcanoListNode* prev, VolcanoListNode* next) {
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(prev != NULL);
	VOLCANO_ASSERT(next != NULL);
	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

inline void volcanoListNodeUnlink(VolcanoListNode* node) {
	VOLCANO_ASSERT(node != NULL);
	node->prev->next = node->next;
	node->next->prev = node->prev;
	volcanoListNodeReset(node);
}

inline VolcanoListNode* volcanoListNodePrev(VolcanoListNode* node) {
	VOLCANO_ASSERT(node != NULL);
	return node->prev;
}

inline VolcanoListNode* volcanoListNodeNext(VolcanoListNode* node) {
	VOLCANO_ASSERT(node != NULL);
	return node->next;
}

typedef struct {
	VolcanoListNode knot;
} VolcanoList;

inline void volcanoListReset(VolcanoList* list) {
	VOLCANO_ASSERT(list != NULL);
	volcanoListNodeReset(&list->knot);
}

inline bool volcanoListIsEmpty(const VolcanoList* list) {
	VOLCANO_ASSERT(list != NULL);
	return !volcanoListNodeIsLinked(&list->knot);
}

inline size_t volcanoListLength(const VolcanoList* list) {
	size_t length = 0;
	VolcanoListNode* p;
	VOLCANO_ASSERT(list != NULL);
	for (p = list->knot.next; p != &list->knot; p = p->next) {
		length++;
	}
	return length;
}

inline VolcanoListNode* volcanoListKnot(VolcanoList* list) {
	VOLCANO_ASSERT(list != NULL);
	return &list->knot;
}

inline VolcanoListNode* volcanoListFirst(VolcanoList* list) {
	VOLCANO_ASSERT(list != NULL);
	return list->knot.next;
}

inline VolcanoListNode* volcanoListLast(VolcanoList* list) {
	VOLCANO_ASSERT(list != NULL);
	return list->knot.prev;
}

inline VolcanoListNode* volcanoListRemoveFirst(VolcanoList* list) {
	VolcanoListNode* p;
	VOLCANO_ASSERT(list != NULL);
	p = volcanoListFirst(list);
	volcanoListNodeUnlink(p);
	return p;
}

inline VolcanoListNode* volcanoListRemoveLast(VolcanoList* list) {
	VolcanoListNode* p;
	VOLCANO_ASSERT(list != NULL);
	p = volcanoListLast(list);
	volcanoListNodeUnlink(p);
	return p;
}

inline void volcanoListInsertBefore(VolcanoList* list, VolcanoListNode* node) {
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	volcanoListNodeLink(node, list->knot.prev, &list->knot);
}

inline void volcanoListInsertAfter(VolcanoList* list, VolcanoListNode* node) {
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	volcanoListNodeLink(node, &list->knot, list->knot.next);
}

inline void volcanoListPrepend(VolcanoList* list, VolcanoListNode* node) {
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	volcanoListNodeLink(node, &list->knot, list->knot.next);
}

inline void volcanoListAppend(VolcanoList* list, VolcanoListNode* node) {
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	volcanoListNodeLink(node, list->knot.prev, &list->knot);
}

inline bool volcanoListContains(const VolcanoList* list, const VolcanoListNode* node) {
	const VolcanoListNode* p;
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	for (p = list->knot.next; p != &list->knot; p = p->next) {
		if (p == node) {
			return true;
		}
	}
	return false;
}

inline void volcanoListRemove(VolcanoList* list, VolcanoListNode* node) {
	VOLCANO_ASSERT(list != NULL);
	VOLCANO_ASSERT(node != NULL);
	VOLCANO_ASSERT(volcanoListContains(list, node));
	volcanoListNodeUnlink(node);
}

inline void volcanoListClear(VolcanoList* list) {
	VOLCANO_ASSERT(list != NULL);
	while (!volcanoListIsEmpty(list)) {
		volcanoListRemoveFirst(list);
	}
}

VOLCANO_C_END

#define VOLCANO_LIST_FOREACH(pnode, plist) \
	for ((pnode)  = volcanoListFirst(plist); \
		 (pnode) != volcanoListKnot(plist); \
		 (pnode)  = volcanoListNodeNext(pnode))

#endif /* VOLCANO_LIST_H */
