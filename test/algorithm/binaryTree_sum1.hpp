
#include "global.h"

using namespace std;

struct BinaryTreeNode
{
    int m_nValue;
    BinaryTreeNode * m_pLeft;
    BinaryTreeNode * m_pRight;
};

int count = 0;

void findPath(BinaryTreeNode *pTreeNode,
              int expectedSum,
              vector<int>& path,
              int & currentSum)
{
    if( !pTreeNode )
        return;
    currentSum += pTreeNode->m_nValue;
    path.push_back(pTreeNode->m_nValue);
    bool isLeaf = !(pTreeNode->m_pLeft) && !(pTreeNode->m_pRight);
    if(currentSum == expectedSum && isLeaf)
    {
        vector<int>::iterator iter;
        for(iter = path.begin(); iter != path.end(); iter++)
        {
            cout << *iter << "\t";
        }
        cout << endl;
    }
    if(pTreeNode->m_pLeft)
        findPath(pTreeNode->m_pLeft, expectedSum, path, currentSum);
    if(pTreeNode->m_pRight)
        findPath(pTreeNode->m_pRight, expectedSum, path, currentSum);

    currentSum -= pTreeNode->m_nValue;
    path.pop_back();
}

void addTree(BinaryTreeNode **T, int num)
{
    if(*T == NULL)
    {
        *T = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
        (*T)->m_nValue = num;
        (*T)->m_pLeft = NULL;
        (*T)->m_pRight = NULL;
    }
    else if((*T)->m_nValue > num)
        addTree(&((*T)->m_pLeft), num);
    else if((*T)->m_nValue < num)
        addTree(&((*T)->m_pRight), num);
    else
        cout << "重复加入同一结点" << endl;
}

int testBinaryTreeSum11()
{
    BinaryTreeNode * T = NULL;
    addTree(&T, 10);
    addTree(&T, 12);
    addTree(&T, 5);
    addTree(&T, 7);
    addTree(&T, 4);

    vector<int> path;
    int sum = 0;
    findPath(T, 22, path, sum);

    return 0;
}