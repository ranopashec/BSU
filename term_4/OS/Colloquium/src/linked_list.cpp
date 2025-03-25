#include "../include/linked_list.h"
#include <stdexcept>

ListNode::ListNode(int val) : value(val), next(nullptr) {}

ListNode* reverseLinkedList(ListNode* head) {
    if (!head) {
        throw std::invalid_argument("Список не может быть пустым");
    }

    ListNode* prev = nullptr;
    ListNode* current = head;
    ListNode* next = nullptr;

    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    return prev;
}
