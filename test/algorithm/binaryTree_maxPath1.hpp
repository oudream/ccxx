//求二叉树中节点的最大距离

#include "global.h"

using namespace std;

/*
http://www.cnblogs.com/miloyip/archive/2010/02/25/1673114.html
*/

struct NODE
{
    NODE *pLeft;
    NODE *pRight;
};

struct RESULT
{
    int nMaxDistance;
    int nMaxDepth;
};

RESULT GetMaximumDistance(NODE* root)
{
    if (!root)
    {
        RESULT empty = { 0, -1 };   // trick: nMaxDepth is -1 and then caller will plus 1 to balance it as zero.
        return empty;
    }

    RESULT lhs = GetMaximumDistance(root->pLeft);
    RESULT rhs = GetMaximumDistance(root->pRight);

    RESULT result;
    result.nMaxDepth = max(lhs.nMaxDepth + 1, rhs.nMaxDepth + 1);
    result.nMaxDistance = max(max(lhs.nMaxDistance, rhs.nMaxDistance), lhs.nMaxDepth + rhs.nMaxDepth + 2);
    return result;
}

void Link(NODE* nodes, int parent, int left, int right)
{
    if (left != -1)
        nodes[parent].pLeft = &nodes[left];

    if (right != -1)
        nodes[parent].pRight = &nodes[right];
}

void testBinaryTreeMaxPath11()
{
    // P. 241 Graph 3-12
    NODE test1[9] = { 0 };
    Link(test1, 0, 1, 2);
    Link(test1, 1, 3, 4);
    Link(test1, 2, 5, 6);
    Link(test1, 3, 7, -1);
    Link(test1, 5, -1, 8);
    cout << "test1: " << GetMaximumDistance(&test1[0]).nMaxDistance << endl;

    // P. 242 Graph 3-13 left
    NODE test2[4] = { 0 };
    Link(test2, 0, 1, 2);
    Link(test2, 1, 3, -1);
    cout << "test2: " << GetMaximumDistance(&test2[0]).nMaxDistance << endl;

    // P. 242 Graph 3-13 right
    NODE test3[9] = { 0 };
    Link(test3, 0, -1, 1);
    Link(test3, 1, 2, 3);
    Link(test3, 2, 4, -1);
    Link(test3, 3, 5, 6);
    Link(test3, 4, 7, -1);
    Link(test3, 5, -1, 8);
    cout << "test3: " << GetMaximumDistance(&test3[0]).nMaxDistance << endl;

    // P. 242 Graph 3-14
    // Same as Graph 3-2, not test

    // P. 243 Graph 3-15
    NODE test4[9] = { 0 };
    Link(test4, 0, 1, 2);
    Link(test4, 1, 3, 4);
    Link(test4, 3, 5, 6);
    Link(test4, 5, 7, -1);
    Link(test4, 6, -1, 8);
    cout << "test4: " << GetMaximumDistance(&test4[0]).nMaxDistance << endl;
}