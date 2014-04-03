#include <iostream>
#include "ai/NaryTree.h"


NaryTree::NaryTree()
{
    root = NULL;
}

NaryTree::~NaryTree()
{
    destroy(root);
    root = NULL;
}

void NaryTree::destroy(NaryTreeNode *node)
{
    if(node == NULL)
        return;

    destroy(node->child);
    destroy(node->brother);

    delete node;
    node = NULL;
}

NaryTreeNode *NaryTree::initNewTree(TurnDetail details)
{
    destroy(root);
    root = createNodeAsBrotherOf(NULL, details);
    return root;
}

NaryTreeNode *NaryTree::createNodeAsBrotherOf(NaryTreeNode *brother, TurnDetail details)
{
    NaryTreeNode *tmp = new NaryTreeNode();
    tmp->child = tmp->brother = NULL;

    if(brother != NULL)
        brother->brother = tmp;

    tmp->details = details;
    return tmp;
}