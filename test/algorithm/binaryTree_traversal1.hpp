
#include "global.h"
using namespace std;

struct TreeTraversal
{
    int value; // value of node
    TreeTraversal *lchild; // left child of node
    TreeTraversal *rchild; // right child of node
};

void pre_order_traversal(TreeTraversal* root){
    //Do Something with root
    if (! root) return;
    cout << root->value << endl;

    if(root->lchild!=NULL)
        pre_order_traversal(root->lchild);
    if(root->rchild!=NULL)
        pre_order_traversal(root->rchild);
}

void in_order_traversal(TreeTraversal* root){
    if (! root) return;

    if(root->lchild!=NULL)
        in_order_traversal(root->lchild);

    //Do Something with root
    cout << root->value << endl;

    if(root->rchild!=NULL)
        in_order_traversal(root->rchild);
}

void post_order_traversal(TreeTraversal* root){
    if (! root) return;

    if(root->lchild!=NULL)
        post_order_traversal(root->lchild);
    if(root->rchild!=NULL)
        post_order_traversal(root->rchild);

    //Do Something with root
    cout << root->value << endl;
}

void Layer_Traver(TreeTraversal* root)
{
    int head = 0,tail = 0;
    TreeTraversal* p[7] = {NULL};
    TreeTraversal* tmp;
    if(root != NULL)
    {
        p[head] = root;
        tail++;
        //Do Something with p[head]
        cout << p[head]->value << endl;
    }
    else
    {
        return;
    }
    while(head < tail)
    {
        tmp = p[head];
        //Do Something with p[head]
        cout << p[head]->value << endl;
        if(tmp->lchild != NULL)//lchild
        {
            p[tail] = tmp->lchild;
            tail++;
        }
        if(tmp->rchild != NULL)//rchild
        {
            p[tail] = tmp->rchild;
            tail++;
        }
        head++;
    }
    return;
}

void testTreeTraversal11()
{
    TreeTraversal mTreeNode[7];
    for (int i = 0; i < sizeof(mTreeNode) / sizeof(mTreeNode[0]); ++i)
    {
        mTreeNode[i].value = 0;
        mTreeNode[i].lchild = 0;
        mTreeNode[i].rchild = 0;
    }
    TreeTraversal * oTreeNode = mTreeNode;
    mTreeNode[0].value = 10;
    ++oTreeNode; mTreeNode[0].lchild = oTreeNode;
    ++oTreeNode; mTreeNode[0].rchild = oTreeNode;
    mTreeNode[1].value = 6;
    ++oTreeNode; mTreeNode[1].lchild = oTreeNode;
    ++oTreeNode; mTreeNode[1].rchild = oTreeNode;
    mTreeNode[2].value = 14;
    ++oTreeNode; mTreeNode[2].lchild = oTreeNode;
    ++oTreeNode; mTreeNode[2].rchild = oTreeNode;
    mTreeNode[3].value = 4;
    mTreeNode[4].value = 8;
    mTreeNode[5].value = 12;
    mTreeNode[6].value = 16;

    pre_order_traversal(mTreeNode);

    in_order_traversal(mTreeNode);

    post_order_traversal(mTreeNode);

    Layer_Traver(mTreeNode);

    cout << ginput() << endl;
}