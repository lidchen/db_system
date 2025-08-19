#pragma once
#include <sqlite3.h>

#include <functional>
#include <string>

#include "db_exception.h"

/**
 * @brief Low Level Wrapper, Deal with sqlite3 db
 *
 * Manage db_connection & close
 * Provide execute database method
 *
 */
class DbConnection {
   private:
    using RowCallback =
            std::function<void(const std::vector<std::string>& column_names,
                               const std::vector<std::string>& row_values)>;
    struct CallbackData {
        RowCallback cb;
        std::vector<std::string> col_names;
        bool col_names_initialized = false;
    };

   public:
    DbConnection() = default;
    explicit DbConnection(const std::string& db_file_path);
    DbConnection(const DbConnection& other) = delete;
    DbConnection& operator=(const DbConnection& other) = delete;
    ~DbConnection();

    bool IsConnected() const;
    void Connect(const std::string& db_file_path);
    void Close() noexcept;
    void DeleteDb(const std::string& db_file_path) const;
    void ExecuteSql(const std::string& sql) const;
    void ExecuteSql(const std::string& sql, const RowCallback& callback) const;
    void BeginTransaction();
    void CommitTransaction();
    void RollbackTransaction();
    void ValidateDbPath(const std::string& db_file_path) const;
    [[nodiscard]]static bool ValidateDbPathFormat(std::string_view path);

   private:
    bool in_transaction = false;
    sqlite3* db_ = nullptr;
    std::string connected_db_file_path_;
};