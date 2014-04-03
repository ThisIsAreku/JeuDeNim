#ifndef _NARY_TREE_NODE_H_
#define _NARY_TREE_NODE_H_

#include "ai/TurnDetail.h"

struct NaryTreeNode
{
    NaryTreeNode *child = NULL;
    NaryTreeNode *brother = NULL;

    TurnDetail details;
};

#endif