#pragma once
#include <string>

template <typename T>
struct Result{
    Result(T value, const std::string& err_msg)
        : value_(value), err_msg_(err_msg) {}
    T value_;
    std::string err_msg_;
    bool ok() const {
        return err_msg_.empty();
    }
};