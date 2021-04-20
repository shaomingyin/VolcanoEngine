/*
 *
 */
#ifndef CX_LIST_H
#define CX_LIST_H

#include <cx/common.h>

CX_BEGIN_DECLS

/* list node */

typedef struct cx_list_node_struct {
	struct cx_list_node_struct *prev;
	struct cx_list_node_struct *next;
} cx_list_node_t;

CX_INLINE void cx_list_node_reset(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

CX_INLINE bool cx_list_node_is_linked(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);

	return (node->prev != node);
}

CX_INLINE void cx_list_node_link(cx_list_node_t *node, cx_list_node_t *prev, cx_list_node_t *next)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(prev != NULL);
	CX_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

CX_INLINE void cx_list_node_unlink(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node;
	node->next = node;
}

CX_INLINE cx_list_node_t *cx_list_node_prev(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);

	return node->prev;
}
CX_INLINE cx_list_node_t *cx_list_node_next(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);

	return node->next;
}

/* list */

typedef struct {
	cx_list_node_t knot;
} cx_list_t;

CX_INLINE void cx_list_reset(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	cx_list_node_reset(&list->knot);
}

CX_INLINE cx_list_node_t *cx_list_head(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return cx_list_node_next(&list->knot);
}

CX_INLINE cx_list_node_t *cx_list_tail(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return cx_list_node_prev(&list->knot);
}

CX_INLINE cx_list_node_t *cx_list_knot(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return &list->knot;
}

CX_INLINE bool cx_list_is_empty(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return !cx_list_node_is_linked(&list->knot);
}

CX_INLINE int cx_list_length(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	int n = 0;
	cx_list_node_t *p;

	for (p = cx_list_head(list); p != cx_list_knot(list); p = cx_list_node_next(p))
		n += 1;

	return n;
}

CX_INLINE bool cx_list_is_contains(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cx_list_node_t *p;

	for (p = cx_list_head(list); p != cx_list_knot(list); p = cx_list_node_next(p)) {
		if (p == node)
			return true;
	}

	return false;
}

CX_INLINE void cx_list_prepend(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cx_list_node_link(node, cx_list_knot(list), cx_list_head(list));
}

CX_INLINE void cx_list_insert(cx_list_t *list, cx_list_node_t *node, cx_list_node_t *from)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(from != NULL);
	CX_ASSERT(cx_list_is_contains(list, from));

	cx_list_node_link(node, cx_list_node_prev(from), from);
}

CX_INLINE void cx_list_append(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cx_list_node_link(node, cx_list_tail(list), cx_list_knot(list));
}

CX_INLINE cx_list_node_t *cx_list_remove_head(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	cx_list_node_t *p = cx_list_head(list);
	cx_list_node_unlink(p);

	return p;
}

CX_INLINE cx_list_node_t *cx_list_remove_tail(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	cx_list_node_t *p = cx_list_tail(list);
	cx_list_node_unlink(p);

	return p;
}

CX_INLINE void cx_list_remove(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(cx_list_is_contains(list, node));

	cx_list_node_unlink(node);
}

CX_INLINE void cx_list_clear(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	while (!cx_list_is_empty(list))
		cx_list_remove_head(list);
}

CX_END_DECLS

#define CX_LIST_FOREACH(pnode, plist) \
	for ((pnode) = cx_list_head(plist); (pnode) != cx_list_knot(plist); (pnode) = cx_list_node_next(pnode))

#define CX_LIST_FOREACH_REVERSE(pnode, plist) \
	for ((pnode) = cx_list_tail(plist); (pnode) != cx_list_knot(plist); (pnode) = cx_list_node_prev(pnode))

#endif /* CX_LIST_H */
