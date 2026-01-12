#include <gtest/gtest.h>
#include <print>

#include "scan.hpp"

// ТЕСТИРОВАНИЕ ОШИБОЧНЫХ СЛУЧАЕВ

TEST(ScanTest, MismatchedFormat) {
    auto result = stdx::scan<std::string>("number", "{}");
    ASSERT_FALSE(result);
    ASSERT_FALSE(result.error().get_message().empty());
}

TEST(ScanTest, InvalidIntFormat) {
    auto result = stdx::scan<int>("42", "{%s}");
    ASSERT_FALSE(result);
    ASSERT_TRUE(result.error().get_message().find("does not match") != std::string::npos);
}

TEST(ScanTest, MismatchedFormatSpecifiers) {
    auto result = stdx::scan<int, int>("42", "{%d}");
    ASSERT_FALSE(result);
    ASSERT_TRUE(result.error().get_message().find("Expected") != std::string::npos);
}

TEST(ScanTest, InvalidIntValue) {
    auto result = stdx::scan<int>("abc", "{%d}");
    ASSERT_FALSE(result);
    ASSERT_TRUE(result.error().get_message().find("Failed to parse") != std::string::npos);
}

TEST(ScanTest, MissingClosingBrace) {
    auto result = stdx::scan<int>("42", "{%d");
    ASSERT_FALSE(result);
}

TEST(ScanTest, InvalidFloatValue) {
    auto result = stdx::scan<float>("not_a_number", "{%f}");
    ASSERT_FALSE(result);
    ASSERT_TRUE(result.error().get_message().find("Failed to parse") != std::string::npos);
}

TEST(ScanTest, MissingRequiredValue) {
    auto result = stdx::scan<int, int>("42", "{%d} {%d}");
    ASSERT_FALSE(result);
}

// ТЕСТИРОВАНИЕ УСПЕШНЫХ СЛУЧАЕВ - ПРОСТЫЕ ТИПЫ

TEST(ScanTest, ParseSingleInt) {
    auto result = stdx::scan<int>("42", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 42);
}

TEST(ScanTest, ParseNegativeInt) {
    auto result = stdx::scan<int>("-123", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), -123);
}

TEST(ScanTest, ParseZeroInt) {
    auto result = stdx::scan<int>("0", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 0);
}

TEST(ScanTest, ParseMultipleInts) {
    auto result = stdx::scan<int, int>("10 20", "{%d} {%d}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 10);
    ASSERT_EQ(std::get<1>(values), 20);
}

TEST(ScanTest, ParseThreeInts) {
    auto result = stdx::scan<int, int, int>("1 2 3", "{%d} {%d} {%d}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 1);
    ASSERT_EQ(std::get<1>(values), 2);
    ASSERT_EQ(std::get<2>(values), 3);
}

TEST(ScanTest, ParseString) {
    auto result = stdx::scan<std::string>("hello", "{%s}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), "hello");
}

TEST(ScanTest, ParseFloat) {
    auto result = stdx::scan<float>("3.14", "{%f}");
    ASSERT_TRUE(result);
    ASSERT_FLOAT_EQ(std::get<0>(result->values()), 3.14f);
}

TEST(ScanTest, ParseNegativeFloat) {
    auto result = stdx::scan<float>("-2.5", "{%f}");
    ASSERT_TRUE(result);
    ASSERT_FLOAT_EQ(std::get<0>(result->values()), -2.5f);
}

TEST(ScanTest, ParseDouble) {
    auto result = stdx::scan<double>("2.718", "{%f}");
    ASSERT_TRUE(result);
    ASSERT_DOUBLE_EQ(std::get<0>(result->values()), 2.718);
}

TEST(ScanTest, ParseUnsignedInt) {
    auto result = stdx::scan<unsigned int>("100", "{%u}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 100u);
}

// ТЕСТИРОВАНИЕ УСПЕШНЫХ СЛУЧАЕВ - СМЕШАННЫЕ ТИПЫ

TEST(ScanTest, ParseIntAndString) {
    auto result = stdx::scan<int, std::string>("42 world", "{%d} {%s}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 42);
    ASSERT_EQ(std::get<1>(values), "world");
}

TEST(ScanTest, ParseStringAndInt) {
    auto result = stdx::scan<std::string, int>("hello 100", "{%s} {%d}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), "hello");
    ASSERT_EQ(std::get<1>(values), 100);
}

TEST(ScanTest, ParseIntStringChar) {
    auto result = stdx::scan<int, std::string>("99 test", "{%d} {%s}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 99);
    ASSERT_EQ(std::get<1>(values), "test");
}

TEST(ScanTest, ParseFloatAndDouble) {
    auto result = stdx::scan<float, double>("1.5 2.718", "{%f} {%f}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_FLOAT_EQ(std::get<0>(values), 1.5f);
    ASSERT_DOUBLE_EQ(std::get<1>(values), 2.718);
}

TEST(ScanTest, ParseComplexMixedTypes) {
    auto result = stdx::scan<int, std::string, float>("123 test 4.5", "{%d} {%s} {%f}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 123);
    ASSERT_EQ(std::get<1>(values), "test");
    ASSERT_FLOAT_EQ(std::get<2>(values), 4.5f);
}

// ТЕСТИРОВАНИЕ ПОГРАНИЧНЫХ СЛУЧАЕВ

TEST(ScanTest, ParseLargeInteger) {
    auto result = stdx::scan<int>("2147483647", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 2147483647);
}

TEST(ScanTest, ParseEmptyString) {
    auto result = stdx::scan<std::string>("", "{%s}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), "");
}

TEST(ScanTest, ParseNumberWithDashSeparator) {
    auto result = stdx::scan<int, int>("10-20", "{%d}-{%d}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 10);
    ASSERT_EQ(std::get<1>(values), 20);
}

TEST(ScanTest, ParseTimeFormat) {
    auto result = stdx::scan<int, int>("12:30", "{%d}:{%d}");
    ASSERT_TRUE(result);
    auto values = result->values();
    ASSERT_EQ(std::get<0>(values), 12);
    ASSERT_EQ(std::get<1>(values), 30);
}

TEST(ScanTest, ParseStringWithPrefix) {
    auto result = stdx::scan<std::string>("name=value", "name={%s}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), "value");
}

// ТЕСТИРОВАНИЕ МЕТОДА values()

TEST(ScanTest, AccessValuesConstRef) {
    auto result = stdx::scan<int, std::string>("42 test", "{%d} {%s}");
    ASSERT_TRUE(result);
    const auto &values = result->values();
    ASSERT_EQ(std::get<0>(values), 42);
    ASSERT_EQ(std::get<1>(values), "test");
}

TEST(ScanTest, AccessValuesMutableRef) {
    auto result = stdx::scan<int>("99", "{%d}");
    ASSERT_TRUE(result);
    auto &values = result->values();
    ASSERT_EQ(std::get<0>(values), 99);
}

// ТЕСТИРОВАНИЕ ПОЛУЧЕНИЯ ОШИБОК

TEST(ScanTest, ErrorMessageAccessible) {
    auto result = stdx::scan<int>("abc", "{%d}");
    ASSERT_FALSE(result);
    ASSERT_FALSE(result.error().get_message().empty());
    ASSERT_TRUE(result.error().what() != nullptr);
}

TEST(ScanTest, MultipleErrorChecks) {
    auto result = stdx::scan<int, int>("42", "{%d} {%d}");
    ASSERT_FALSE(result);
    const auto &error = result.error();
    ASSERT_FALSE(error.get_message().empty());
}

// ТЕСТИРОВАНИЕ ПОДДЕРЖКИ НОВЫХ ТИПОВ

TEST(ScanTest, ParseInt8) {
    auto result = stdx::scan<int8_t>("42", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 42);
}

TEST(ScanTest, ParseInt16) {
    auto result = stdx::scan<int16_t>("1234", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 1234);
}

TEST(ScanTest, ParseInt32) {
    auto result = stdx::scan<int32_t>("56789", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 56789);
}

TEST(ScanTest, ParseInt64) {
    auto result = stdx::scan<int64_t>("123456789", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 123456789LL);
}

TEST(ScanTest, ParseUint8) {
    auto result = stdx::scan<uint8_t>("255", "{%u}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 255);
}

TEST(ScanTest, ParseUint16) {
    auto result = stdx::scan<uint16_t>("65535", "{%u}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 65535);
}

TEST(ScanTest, ParseUint32) {
    auto result = stdx::scan<uint32_t>("4294967295", "{%u}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 4294967295U);
}

TEST(ScanTest, ParseUint64) {
    auto result = stdx::scan<uint64_t>("18446744073709551615", "{%u}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 18446744073709551615ULL);
}

TEST(ScanTest, ParseStringView) {
    auto result = stdx::scan<std::string_view>("hello", "{%s}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), "hello");
}

// ТЕСТИРОВАНИЕ CV-КАЛИФИЦИРОВАННЫХ ТИПОВ

TEST(ScanTest, ParseConstInt) {
    auto result = stdx::scan<const int>("42", "{%d}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), 42);
}

TEST(ScanTest, ParseConstString) {
    auto result = stdx::scan<const std::string>("world", "{%s}");
    ASSERT_TRUE(result);
    ASSERT_EQ(std::get<0>(result->values()), "world");
}
