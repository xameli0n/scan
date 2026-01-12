#pragma once

#include "parse.hpp"
#include "types.hpp"

namespace stdx {

namespace details {

// Парсинг кортежа значений
template <typename... Ts, size_t... Is>
std::expected<scan_result<Ts...>, scan_error> parse_tuple_values(const std::vector<std::string_view> &format_parts,
                                                                 const std::vector<std::string_view> &input_parts,
                                                                 std::index_sequence<Is...>) {

    // Парсим каждое значение с использованием parse_value_with_format
    std::tuple<std::expected<Ts, scan_error>...> parsed_values{
        parse_value_with_format<Ts>(input_parts[Is], format_parts[Is])...};

    bool all_success = true;
    std::string error_message;

    auto check_results = [&](auto index) {
        using index_type = decltype(index);
        auto &result = std::get<index_type::value>(parsed_values);
        if (!result.has_value()) {
            all_success = false;
            if (error_message.empty()) {
                error_message = result.error().get_message();
            }
        }
    };

    (check_results(std::integral_constant<size_t, Is>{}), ...);

    if (!all_success) {
        return std::unexpected(scan_error{error_message});
    }

    // Если все успешно, создаём scan_result с распарсенными значениями
    return scan_result<Ts...>{std::move(std::get<Is>(parsed_values).value())...};
}

}  // namespace details

template <typename... Ts>
std::expected<details::scan_result<Ts...>, details::scan_error> scan(std::string_view input, std::string_view format) {
    // Получаем разбиение форматирующей и исходной строк
    auto parse_result = details::parse_sources<Ts...>(input, format);

    if (!parse_result.has_value()) {
        return std::unexpected(parse_result.error());
    }

    const auto &[format_parts, input_parts] = parse_result.value();

    // Проверяем, что количество плейсхолдеров совпадает с количеством типов
    if (sizeof...(Ts) != format_parts.size()) {
        return std::unexpected(details::scan_error{"Expected " + std::to_string(sizeof...(Ts)) +
                                                   " format specifiers, but got " +
                                                   std::to_string(format_parts.size())});
    }

    // Используем fold expression для парсинга каждого значения и агрегирования в кортеж
    return details::parse_tuple_values<Ts...>(format_parts, input_parts, std::index_sequence_for<Ts...>{});
}

}  // namespace stdx
