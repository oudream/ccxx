
#include "global.h"
using namespace std;

struct BSTreeNode
{
    int m_nValue; // value of node
    BSTreeNode *m_pLeft; // left child of node
    BSTreeNode *m_pRight; // right child of node
};

typedef BSTreeNode DoubleList;
DoubleList * pHead;
DoubleList * pListIndex;

void convertToDoubleList(BSTreeNode * pCurrent);
// 创建二元查找树
void addBSTreeNode(BSTreeNode * & pCurrent, int value)
{
    if (NULL == pCurrent)
    {
        BSTreeNode * pBSTree = new BSTreeNode();
        pBSTree->m_pLeft = NULL;
        pBSTree->m_pRight = NULL;
        pBSTree->m_nValue = value;
        pCurrent = pBSTree;
    }
    else
    {
        if ((pCurrent->m_nValue) > value)
        {
            addBSTreeNode(pCurrent->m_pLeft, value);
        }
        else if ((pCurrent->m_nValue) < value)
        {
            addBSTreeNode(pCurrent->m_pRight, value);
        }
        else
        {
            cout<<"不能重复加入相同节点"<<endl;
        }
    }
}

// 遍历二元查找树  中序
void ergodicBSTree(BSTreeNode * pCurrent)
{
    if (NULL == pCurrent)
    {
        return;
    }
    if (NULL != pCurrent->m_pLeft)
    {
        ergodicBSTree(pCurrent->m_pLeft);
    }
    // 节点接到链表尾部
    convertToDoubleList(pCurrent);
    // 右子树为空
    if (NULL != pCurrent->m_pRight)
    {
        ergodicBSTree(pCurrent->m_pRight);
    }
}

// 二叉树转换成list
void  convertToDoubleList(BSTreeNode * pCurrent)
{
    pCurrent->m_pLeft = pListIndex;//left相当于前驱指针,right相当于后继指针
    if (NULL != pListIndex)
    {
        pListIndex->m_pRight = pCurrent;
    }
    else
    {
        pHead = pCurrent;//头结点即为中序遍历的第一个结点，本例中即为4结点
    }
    pListIndex = pCurrent;
    cout<<pCurrent->m_nValue<<endl;
}

int testTree2Linkedlist21()
{
    BSTreeNode * pRoot = NULL;
    pListIndex = NULL;//用于建立链表
    pHead = NULL;//链表头结点
    addBSTreeNode(pRoot, 10);
    addBSTreeNode(pRoot, 4);
    addBSTreeNode(pRoot, 6);
    addBSTreeNode(pRoot, 8);
    addBSTreeNode(pRoot, 12);
    addBSTreeNode(pRoot, 14);
    addBSTreeNode(pRoot, 15);
    addBSTreeNode(pRoot, 16);
    ergodicBSTree(pRoot);

    cout << pRoot->m_nValue << endl;

    return 0;
}