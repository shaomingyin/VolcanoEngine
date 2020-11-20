/*
 *
 */
#ifndef VOLCANO_LIST_H
#define VOLCANO_LIST_H

#include <Volcano/Common.h>

VOLCANO_BEGIN_C_DECLS

typedef struct VolcanoListNodeTag {
    struct VolcanoListNodeTag *prev;
    struct VolcanoListNodeTag *next;
} VolcanoListNode;

VOLCANO_INLINE static void volcanoListNodeReset(VolcanoListNode *node)
{
    VOLCANO_ASSERT(node != NULL);

    node->prev = node;
    node->next = node;
}

VOLCANO_INLINE static void volcanoListNodeLink(
    VolcanoListNode *node, VolcanoListNode *prev, VolcanoListNode *next)
{
    VOLCANO_ASSERT(node != NULL);
    VOLCANO_ASSERT(prev != NULL);
    VOLCANO_ASSERT(next != NULL);

    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

VOLCANO_INLINE static void volcanoListNodeUnlink(VolcanoListNode *node)
{
    VOLCANO_ASSERT(node != NULL);
    VOLCANO_ASSERT(node->prev != NULL);
    VOLCANO_ASSERT(node->next != NULL);

    node->prev->next = node->next;
    node->next->prev = node->prev;

    volcanoListNodeReset(node);
}

VOLCANO_INLINE static bool volcanoListNodeIsLinked(const VolcanoListNode *node)
{
    VOLCANO_ASSERT(node != NULL);

    return (node != node->next);
}

VOLCANO_INLINE static VolcanoListNode *volcanoListNodePrev(VolcanoListNode *node)
{
    VOLCANO_ASSERT(node != NULL);
    VOLCANO_ASSERT(node->prev != NULL);

    return node->prev;
}

VOLCANO_INLINE static VolcanoListNode *volcanoListNodeNext(VolcanoListNode *node)
{
    VOLCANO_ASSERT(node != NULL);
    VOLCANO_ASSERT(node->next != NULL);

    return node->next;
}

typedef struct {
    VolcanoListNode knot;
} VolcanoList;

int volcanoListLength(VolcanoList *list);
bool volcanoListIsContains(VolcanoList *list, VolcanoListNode *node);

VOLCANO_INLINE static void volcanoListReset(VolcanoList *list)
{
    VOLCANO_ASSERT(list != NULL);

    volcanoListNodeReset(&list->knot);
}

VOLCANO_INLINE static bool volcanoListIsEmpty(const VolcanoList *list)
{
    VOLCANO_ASSERT(list != NULL);

    return !volcanoListNodeIsLinked(&list->knot);
}

VOLCANO_INLINE static VolcanoListNode *volcanoListKnot(VolcanoList *list)
{
    VOLCANO_ASSERT(list != NULL);

    return &list->knot;
}

VOLCANO_INLINE static VolcanoListNode *volcanoListHead(VolcanoList *list)
{
    VOLCANO_ASSERT(list != NULL);

    return volcanoListNodeNext(&list->knot);
}

VOLCANO_INLINE static VolcanoListNode *volcanoListTail(VolcanoList *list)
{
    VOLCANO_ASSERT(list != NULL);

    return volcanoListNodePrev(&list->knot);
}

VOLCANO_INLINE static VolcanoListNode *volcanoListRemoveHead(VolcanoList *list)
{
    VolcanoListNode *node;

    VOLCANO_ASSERT(list != NULL);

    node = volcanoListHead(list);
    volcanoListNodeUnlink(node);

    return node;
}

VOLCANO_INLINE static VolcanoListNode *volcanoListRemoveTail(VolcanoList *list)
{
    VolcanoListNode *node;

    VOLCANO_ASSERT(list != NULL);

    node = volcanoListTail(list);
    volcanoListNodeUnlink(node);

    return node;
}

VOLCANO_INLINE static void volcanoListRemove(VolcanoList *list, VolcanoListNode *node)
{
    VOLCANO_ASSERT(list != NULL);
    VOLCANO_ASSERT(node != NULL);
    VOLCANO_ASSERT(volcanoListIsContains(list, node));

    volcanoListNodeUnlink(node);
}

VOLCANO_INLINE static void volcanoListRemoveAll(VolcanoList *list)
{
    VOLCANO_ASSERT(list != NULL);

    while (!volcanoListIsEmpty(list))
        volcanoListRemoveHead(list);
}

VOLCANO_INLINE static void volcanoListPrepand(VolcanoList *list, VolcanoListNode *node)
{
    VOLCANO_ASSERT(list != NULL);
    VOLCANO_ASSERT(node != NULL);

    volcanoListNodeUnlink(node);
    volcanoListNodeLink(node, volcanoListKnot(list), volcanoListHead(list));
}

VOLCANO_INLINE static void volcanoListAppend(VolcanoList *list, VolcanoListNode *node)
{
    VOLCANO_ASSERT(list != NULL);
    VOLCANO_ASSERT(node != NULL);

    volcanoListNodeUnlink(node);
    volcanoListNodeLink(node, volcanoListTail(list), volcanoListKnot(list));
}

VOLCANO_INLINE static void volcanoListMove(VolcanoList *to, VolcanoList *from)
{
    VOLCANO_ASSERT(to != NULL);
    VOLCANO_ASSERT(from != NULL);

    volcanoListHead(from)->prev = volcanoListTail(to);
    volcanoListTail(from)->next = volcanoListKnot(to);

    volcanoListTail(to)->next = volcanoListHead(from);
    volcanoListKnot(to)->prev = volcanoListTail(from);

    volcanoListReset(from);
}

VOLCANO_END_C_DECLS

#define VOLCANO_LIST_FOREACH(pnode, plist) \
    for ((pnode)  = volcanoListHead(plist); \
         (pnode) != volcanoListKnot(plist); \
         (pnode)  = volcanoListNodeNext(pnode))

#define VOLCANO_LIST_FOREACH_SAFE(pnode, pnext, plist) \
    for ((pnode)  = volcanoListHead(plist), (pnext) = volcanoListNodeNext(pnode); \
         (pnode) != volcanoListKnot(plist); \
         (pnode)  = (pnext))

#endif /* VOLCANO_LIST_H */
