//
// Created by Simon on 25/03/2025.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
struct ListNode {
    int value;
    ListNode* next;
    ListNode(int val);
};

ListNode* reverseLinkedList(ListNode* head);
#endif //LINKED_LIST_H
