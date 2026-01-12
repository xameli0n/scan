#pragma once

#include <charconv>
#include <cstdint>
#include <expected>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "types.hpp"

namespace stdx::details {

// Функция для парсинга значения с учетом спецификатора формата
template <typename T>
std::expected<T, scan_error> parse_value_with_format(std::string_view input, std::string_view fmt) {
    // Проверка соответствия типа и спецификатора формата
    if constexpr (std::is_same_v<std::remove_cv_t<T>, int>) {
        if (fmt != "%d") {
            return std::unexpected(scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'int'"});
        }
        int value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse int from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, unsigned int>) {
        if (fmt != "%u") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'unsigned int'"});
        }
        unsigned int value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse unsigned int from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, float>) {
        if (fmt != "%f") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'float'"});
        }
        float value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse float from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, double>) {
        if (fmt != "%f") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'double'"});
        }
        double value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse double from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, std::string>) {
        if (fmt != "%s") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'std::string'"});
        }
        return std::string(input);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, std::string_view>) {
        if (fmt != "%s") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'std::string_view'"});
        }
        return std::string_view(input);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, int8_t>) {
        if (fmt != "%d") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'int8_t'"});
        }
        int8_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse int8_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, int16_t>) {
        if (fmt != "%d") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'int16_t'"});
        }
        int16_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse int16_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, int32_t>) {
        if (fmt != "%d") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'int32_t'"});
        }
        int32_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse int32_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, int64_t>) {
        if (fmt != "%d") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'int64_t'"});
        }
        int64_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse int64_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, uint8_t>) {
        if (fmt != "%u") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'uint8_t'"});
        }
        uint8_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse uint8_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, uint16_t>) {
        if (fmt != "%u") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'uint16_t'"});
        }
        uint16_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse uint16_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, uint32_t>) {
        if (fmt != "%u") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'uint32_t'"});
        }
        uint32_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse uint32_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else if constexpr (std::is_same_v<std::remove_cv_t<T>, uint64_t>) {
        if (fmt != "%u") {
            return std::unexpected(
                scan_error{"Format specifier '" + std::string(fmt) + "' does not match type 'uint64_t'"});
        }
        uint64_t value;
        auto result = std::from_chars(input.data(), input.data() + input.size(), value);
        if (result.ec != std::errc()) {
            return std::unexpected(scan_error{"Failed to parse uint64_t from '" + std::string(input) + "'"});
        }
        return static_cast<T>(value);
    } else {
        return std::unexpected(scan_error{"Unsupported type for parsing"});
    }
}

// Функции для конвертирования подстроки исходных данных в конкретный тип
template <typename T>
std::expected<T, scan_error> parse_value(std::string_view input) {
    return parse_value_with_format<T>(input, "");
}

// Специализация для std::string_view
template <>
inline std::expected<std::string_view, scan_error> parse_value<std::string_view>(std::string_view input) {
    return parse_value_with_format<std::string_view>(input, "%s");
}

// Специализация для int8_t
template <>
inline std::expected<int8_t, scan_error> parse_value<int8_t>(std::string_view input) {
    return parse_value_with_format<int8_t>(input, "%d");
}

// Специализация для int16_t
template <>
inline std::expected<int16_t, scan_error> parse_value<int16_t>(std::string_view input) {
    return parse_value_with_format<int16_t>(input, "%d");
}

// Специализация для int32_t
template <>
inline std::expected<int32_t, scan_error> parse_value<int32_t>(std::string_view input) {
    return parse_value_with_format<int32_t>(input, "%d");
}

// Специализация для int64_t
template <>
inline std::expected<int64_t, scan_error> parse_value<int64_t>(std::string_view input) {
    return parse_value_with_format<int64_t>(input, "%d");
}

// Специализация для uint8_t
template <>
inline std::expected<uint8_t, scan_error> parse_value<uint8_t>(std::string_view input) {
    return parse_value_with_format<uint8_t>(input, "%u");
}

// Специализация для uint16_t
template <>
inline std::expected<uint16_t, scan_error> parse_value<uint16_t>(std::string_view input) {
    return parse_value_with_format<uint16_t>(input, "%u");
}

// Специализация для uint32_t
template <>
inline std::expected<uint32_t, scan_error> parse_value<uint32_t>(std::string_view input) {
    return parse_value_with_format<uint32_t>(input, "%u");
}

// Специализация для uint64_t
template <>
inline std::expected<uint64_t, scan_error> parse_value<uint64_t>(std::string_view input) {
    return parse_value_with_format<uint64_t>(input, "%u");
}

// Функция для проверки корректности входных данных и выделения из обеих строк интересующих данных для парсинга
template <typename... Ts>
std::expected<std::pair<std::vector<std::string_view>, std::vector<std::string_view>>, scan_error>
parse_sources(std::string_view input, std::string_view format) {
    std::vector<std::string_view> format_parts;  // Части формата между {}
    std::vector<std::string_view> input_parts;
    size_t start = 0;
    while (true) {
        size_t open = format.find('{', start);
        if (open == std::string_view::npos) {
            break;
        }
        size_t close = format.find('}', open);
        if (close == std::string_view::npos) {
            break;
        }

        // Если между предыдущей } и текущей { есть текст,
        // проверяем его наличие во входной строке
        if (open > start) {
            std::string_view between = format.substr(start, open - start);
            auto pos = input.find(between);
            if (input.size() < between.size() || pos == std::string_view::npos) {
                return std::unexpected(scan_error{"Unformatted text in input and format string are different"});
            }
            if (start != 0) {
                input_parts.emplace_back(input.substr(0, pos));
            }

            input = input.substr(pos + between.size());
        }

        // Сохраняем спецификатор формата (то, что между {})
        format_parts.push_back(format.substr(open + 1, close - open - 1));
        start = close + 1;
    }

    // Проверяем оставшийся текст после последней }
    if (start < format.size()) {
        std::string_view remaining_format = format.substr(start);
        auto pos = input.find(remaining_format);
        if (input.size() < remaining_format.size() || pos == std::string_view::npos) {
            return std::unexpected(scan_error{"Unformatted text in input and format string are different"});
        }
        input_parts.emplace_back(input.substr(0, pos));
        input = input.substr(pos + remaining_format.size());
    } else {
        input_parts.emplace_back(input);
    }
    return std::pair{format_parts, input_parts};
}

}  // namespace stdx::details