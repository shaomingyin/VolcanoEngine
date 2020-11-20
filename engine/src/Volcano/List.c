/*
 *
 */
#include <Volcano/List.h>

int volcanoListLength(VolcanoList *list)
{
    int length = 0;
    VolcanoListNode *node;

    VOLCANO_ASSERT(list != NULL);

    VOLCANO_LIST_FOREACH(node, list) {
        length += 1;
    }

    return length;
}

bool volcanoListIsContains(VolcanoList *list, VolcanoListNode *node)
{
    VolcanoListNode *temp;

    VOLCANO_ASSERT(list != NULL);
    VOLCANO_ASSERT(node != NULL);

    VOLCANO_LIST_FOREACH(temp, list) {
        if (temp == node)
            return true;
    }

    return false;
}
