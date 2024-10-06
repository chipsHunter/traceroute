//
// Created by hvorostok on 05.10.2024.
//

#include "../tracing.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;


// Тест для недостижимой цели (Destination Unreachable)
TEST(TracingTest, DestinationUnreachable) {
    // Моделируем ICMP-сообщение типа "Destination Unreachable"
    unsigned char buffer[sizeof(struct ip) + sizeof(struct icmp)] = {0};

    struct ip *ip_header = (struct ip *) buffer;
    struct icmp *icmp_header = (struct icmp *) (buffer + sizeof(struct ip));

    ip_header->ip_hl = 5;  // Длина заголовка IP в словах (4 байта каждое)

    icmp_header->icmp_type = ICMP_DEST_UNREACH;
    icmp_header->icmp_code = ICMP_PORT_UNREACH;

    // Проверяем, что функция вернет true для "Destination Unreachable"
    EXPECT_TRUE(is_destination_unreachable(buffer, sizeof(buffer)));
}

// Тест для достижимой цели (Not Destination Unreachable)
TEST(TracingTest, NotDestinationUnreachable) {
    // Моделируем ICMP-сообщение типа, отличного от "Destination Unreachable"
    unsigned char buffer[sizeof(struct ip) + sizeof(struct icmp)] = {0};

    struct ip *ip_header = (struct ip *) buffer;
    struct icmp *icmp_header = (struct icmp *) (buffer + sizeof(struct ip));

    ip_header->ip_hl = 5;  // Длина заголовка IP в словах (4 байта каждое)

    icmp_header->icmp_type = ICMP_ECHO;  // Тип ICMP-сообщения, отличный от "Destination Unreachable"
    icmp_header->icmp_code = 0;

    // Проверяем, что функция вернет false для обычного ICMP-сообщения
    EXPECT_FALSE(is_destination_unreachable(buffer, sizeof(buffer)));
}

// Запускаем все тесты
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


