#pragma once
#include <stdexcept>
#include <string>

/**
 * @brief Inherit from std::runtime_error
 *
 * DbErrorType make debug more clear and easier
 *
 */
class DbException : public std::runtime_error {
   public:
    enum class DbErrorType {
        CONNECTION,
        SQL_EXECUTE,
        FILE_SYSTEM,
        FILE_CONSTRAINT,
        TRANSACTION,
        NOT_FOUND
    };
    DbException(const std::string& msg, DbErrorType err_type)
        : std::runtime_error(msg), err_type_(err_type) {}
    DbErrorType get_type() const { return err_type_; }

   private:
    DbErrorType err_type_;
};