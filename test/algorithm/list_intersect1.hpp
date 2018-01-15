
#include "global.h"

using namespace std;

struct slist
{
    int m_nValue;
    slist * next;
};

bool IsExitsLoop(slist *head)
{
    slist *slow = head, *fast = head;

    while ( fast && fast->next )
    {
        slow = slow->next;
        fast = fast->next->next;
        if ( slow == fast ) break;
    }

    return !(fast == NULL || fast->next == NULL);
}

slist* FindLoopPort(slist *head)
{
    slist *slow = head, *fast = head;

    while ( fast && fast->next )
    {
        slow = slow->next;
        fast = fast->next->next;
        if ( slow == fast ) break;
    }

    if (fast == NULL || fast->next == NULL)
        return NULL;

    slow = head;
    while (slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

//http://blog.csdn.net/jiqiren007/article/details/6572685
//3、第三种思路是比较奇特的，在编程之美上看到的。先遍历第一个链表到他的尾部，然后将尾部的next指针
// 指向第二个链表(尾部指针的next本来指向的是null)。这样两个链表就合成了一个链表，判断原来的两个
// 链表是否相交也就转变成了判断新的链表是否有环的问题了：即判断单链表是否有环？

