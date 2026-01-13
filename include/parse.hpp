#pragma once

#include <charconv>
#include <concepts>
#include <cstdint>
#include <expected>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "types.hpp"

namespace stdx::details {

// Концепт для поддерживаемых типов
template <typename T>
concept Parsable =
    std::is_same_v<std::remove_cv_t<T>, int> || std::is_same_v<std::remove_cv_t<T>, unsigned int> ||
    std::is_same_v<std::remove_cv_t<T>, float> || std::is_same_v<std::remove_cv_t<T>, double> ||
    std::is_same_v<std::remove_cv_t<T>, std::string> || std::is_same_v<std::remove_cv_t<T>, std::string_view> ||
    std::is_same_v<std::remove_cv_t<T>, int8_t> || std::is_same_v<std::remove_cv_t<T>, int16_t> ||
    std::is_same_v<std::remove_cv_t<T>, int32_t> || std::is_same_v<std::remove_cv_t<T>, int64_t> ||
    std::is_same_v<std::remove_cv_t<T>, uint8_t> || std::is_same_v<std::remove_cv_t<T>, uint16_t> ||
    std::is_same_v<std::remove_cv_t<T>, uint32_t> || std::is_same_v<std::remove_cv_t<T>, uint64_t>;

// Функция для определения формата
template <typename T>
constexpr std::string_view get_format_specifier() {
    using CleanT = std::remove_cv_t<T>;

    if constexpr (std::is_same_v<CleanT, int> || std::is_same_v<CleanT, int8_t> || std::is_same_v<CleanT, int16_t> ||
                  std::is_same_v<CleanT, int32_t> || std::is_same_v<CleanT, int64_t>) {
        return "%d";
    } else if constexpr (std::is_same_v<CleanT, unsigned int> || std::is_same_v<CleanT, uint8_t> ||
                         std::is_same_v<CleanT, uint16_t> || std::is_same_v<CleanT, uint32_t> ||
                         std::is_same_v<CleanT, uint64_t>) {
        return "%u";
    } else if constexpr (std::is_same_v<CleanT, float> || std::is_same_v<CleanT, double>) {
        return "%f";
    } else if constexpr (std::is_same_v<CleanT, std::string> || std::is_same_v<CleanT, std::string_view>) {
        return "%s";
    }
}

// Функция для определения типа
template <typename T>
constexpr std::string_view get_type_name() {
    using CleanT = std::remove_cv_t<T>;

    if constexpr (std::is_same_v<CleanT, int>)
        return "int";
    else if constexpr (std::is_same_v<CleanT, unsigned int>)
        return "unsigned int";
    else if constexpr (std::is_same_v<CleanT, float>)
        return "float";
    else if constexpr (std::is_same_v<CleanT, double>)
        return "double";
    else if constexpr (std::is_same_v<CleanT, std::string>)
        return "std::string";
    else if constexpr (std::is_same_v<CleanT, std::string_view>)
        return "std::string_view";
    else if constexpr (std::is_same_v<CleanT, int8_t>)
        return "int8_t";
    else if constexpr (std::is_same_v<CleanT, int16_t>)
        return "int16_t";
    else if constexpr (std::is_same_v<CleanT, int32_t>)
        return "int32_t";
    else if constexpr (std::is_same_v<CleanT, int64_t>)
        return "int64_t";
    else if constexpr (std::is_same_v<CleanT, uint8_t>)
        return "uint8_t";
    else if constexpr (std::is_same_v<CleanT, uint16_t>)
        return "uint16_t";
    else if constexpr (std::is_same_v<CleanT, uint32_t>)
        return "uint32_t";
    else if constexpr (std::is_same_v<CleanT, uint64_t>)
        return "uint64_t";
}

// Функция для парсинга всех числовых типов
template <typename T>
    requires std::is_arithmetic_v<std::remove_cv_t<T>>
std::expected<T, scan_error> parse_numeric(std::string_view input) {
    using CleanT = std::remove_cv_t<T>;
    CleanT value;
    auto result = std::from_chars(input.data(), input.data() + input.size(), value);

    if (result.ec != std::errc()) {
        return std::unexpected(
            scan_error{"Failed to parse " + std::string(get_type_name<T>()) + " from '" + std::string(input) + "'"});
    }

    return static_cast<T>(value);
}

// Функция для парсинга значения с учетом спецификатора формата
template <Parsable T>
std::expected<T, scan_error> parse_value_with_format(std::string_view input, std::string_view fmt) {
    using CleanT = std::remove_cv_t<T>;

    // Проверка соответствия типа и спецификатора формата
    constexpr auto expected_fmt = get_format_specifier<T>();
    if (fmt != expected_fmt) {
        return std::unexpected(scan_error{"Format specifier '" + std::string(fmt) + "' does not match type '" +
                                          std::string(get_type_name<T>()) + "'"});
    }

    // Парсинг строковых типов
    if constexpr (std::is_same_v<CleanT, std::string>) {
        return std::string(input);
    } else if constexpr (std::is_same_v<CleanT, std::string_view>) {
        return std::string_view(input);
    }
    // Парсинг числовых типов
    else {
        return parse_numeric<T>(input);
    }
}

// Функция для конвертирования подстроки исходных данных в конкретный тип
template <Parsable T>
std::expected<T, scan_error> parse_value(std::string_view input) {
    return parse_value_with_format<T>(input, get_format_specifier<T>());
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