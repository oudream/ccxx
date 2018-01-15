
#include "global.h"
using namespace std;


//首先我们定义的二元查找树节点的数据结构如下：
struct BSTreeNode
{
    int m_nValue; // value of node
    BSTreeNode *m_pLeft; // left child of node 38
    BSTreeNode *m_pRight; // right child of node
};

void helper(BSTreeNode *& head, BSTreeNode *& tail, BSTreeNode *root) {
    BSTreeNode *lt, *rh;
    if (root == NULL) {
        head = NULL, tail = NULL;
        return;
    }
    helper(head, lt, root->m_pLeft);
    helper(rh, tail, root->m_pRight);
    if (lt!=NULL) {
        lt->m_pRight = root;
        root->m_pLeft = lt;
    } else  {
        head = root;
    }
    if (rh!=NULL) {
        root->m_pRight=rh;
        rh->m_pLeft = root;
    } else {
        tail = root;
    }
}

/**
 * @param root The root node of the tree
 * @return The head node of the converted list.
 */
BSTreeNode * treeToLinkedList(BSTreeNode * root) {
    BSTreeNode * head, * tail;
    helper(head, tail, root);
    return head;
}

void testTree2Linkedlist11()
{
    BSTreeNode bsTreeNode[7];
    for (int i = 0; i < sizeof(bsTreeNode) / sizeof(bsTreeNode[0]); ++i)
    {
        bsTreeNode[i].m_nValue = 0;
        bsTreeNode[i].m_pLeft = 0;
        bsTreeNode[i].m_pRight = 0;
    }
    BSTreeNode * oBSTreeNode = bsTreeNode;
    bsTreeNode[0].m_nValue = 10;
    ++oBSTreeNode; bsTreeNode[0].m_pLeft = oBSTreeNode;
    ++oBSTreeNode; bsTreeNode[0].m_pRight = oBSTreeNode;
    bsTreeNode[1].m_nValue = 6;
    ++oBSTreeNode; bsTreeNode[1].m_pLeft = oBSTreeNode;
    ++oBSTreeNode; bsTreeNode[1].m_pRight = oBSTreeNode;
    bsTreeNode[2].m_nValue = 14;
    ++oBSTreeNode; bsTreeNode[2].m_pLeft = oBSTreeNode;
    ++oBSTreeNode; bsTreeNode[2].m_pRight = oBSTreeNode;
    bsTreeNode[3].m_nValue = 4;
    bsTreeNode[4].m_nValue = 8;
    bsTreeNode[5].m_nValue = 12;
    bsTreeNode[6].m_nValue = 16;

    BSTreeNode * r = treeToLinkedList(bsTreeNode);

    cout << r->m_nValue << endl;

}