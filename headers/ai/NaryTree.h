#ifndef _NARY_TREE_H_
#define _NARY_TREE_H_

#include "ai/NaryTreeNode.h"

class NaryTree
{
    NaryTreeNode *root;
public:
    NaryTree();
    ~NaryTree();

    void clear();
    void destroy(NaryTreeNode *);

    NaryTreeNode *initNewTree(TurnDetail);

    NaryTreeNode *createNodeAsBrotherOf(NaryTreeNode *, TurnDetail);
};

#endif