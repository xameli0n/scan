#pragma once

#include <string>
#include <tuple>

namespace stdx::details {

// Класс для хранения ошибки неуспешного сканирования

class scan_error {
public:
    explicit scan_error(std::string message) : message(std::move(message)) {}

    explicit scan_error(const char *message) : message(message) {}

    const std::string &get_message() const { return message; }

    // Преобразование в строку для отладки
    const char *what() const noexcept { return message.c_str(); }

private:
    std::string message;
};

// Шаблонный класс для хранения результатов успешного сканирования

template <typename... Ts>
class scan_result {
public:
    explicit scan_result(Ts &&...values) : values_(std::forward<Ts>(values)...) {}

    const std::tuple<Ts...> &values() const { return values_; }

    std::tuple<Ts...> &values() { return values_; }

private:
    std::tuple<Ts...> values_;
};

}  // namespace stdx::details
