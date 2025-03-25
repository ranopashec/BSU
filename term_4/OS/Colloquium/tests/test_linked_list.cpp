#include <gtest/gtest.h>
#include "../include/linked_list.h"

TEST(ReverseLinkedListTest, NonEmptyList) {
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);

    ListNode* reversed = reverseLinkedList(head);

    EXPECT_EQ(reversed->value, 3);
    EXPECT_EQ(reversed->next->value, 2);
    EXPECT_EQ(reversed->next->next->value, 1);

    // Очистка памяти
    delete reversed->next->next;
    delete reversed->next;
    delete reversed;
}

TEST(ReverseLinkedListTest, EmptyList) {
    EXPECT_THROW(reverseLinkedList(nullptr), std::invalid_argument);
}
