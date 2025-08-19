#include <assert.h>
#include <sqlite3.h>

#include <filesystem>
#include <format>
#include <iostream>

#include "db_connection.h"
#include "db_exception.h"

/**
 * @brief Opens a connection to a SQLite database file.
 *
 * Creates a new database connection to the specified file path.
 * Throws DbException if the database cannot be opened or invaild path.
 *
 * @param db_file_path Absolute path to the `.sqlite` database file.
 */
DbConnection::DbConnection(const std::string& db_file_path) {
    Connect(db_file_path);
    assert(!connected_db_file_path_.empty());
}

/**
 * @brief Destroy the Db Connection:: Db Connection object
 *
 * require explicitly handle transaction before disconnection
 *
 */
DbConnection::~DbConnection() { Close(); }

/**
 * @brief Throws DbException if the database cannot be opened or invaild path.
 *
 * Close current db, then connect to new database.
 * If file dont exist, create a new sqlite3 database at db_file_path.
 *
 * throw DbException(FILE_CONSTRAINT) if invaild path.
 * throw DbException(CONNECTION) if the database cannot be opened
 *
 * @param db_file_path Absolute path to the `.sqlite` database file.
 */
void DbConnection::Connect(const std::string& db_file_path) {
    using enum DbException::DbErrorType;
    if (!ValidateDbPathFormat(db_file_path)) {
        throw(DbException("Invalid db_file_path: " + db_file_path,
                          FILE_CONSTRAINT));
    }

    Close();
    if (sqlite3_open(db_file_path.c_str(), &db_) != SQLITE_OK) {
        std::string err_msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw DbException(("Failed to open database: " + err_msg), CONNECTION);
    }
    connected_db_file_path_ = db_file_path;
    assert(IsConnected() && connected_db_file_path_ == db_file_path);
}

/**
 * @brief noexcept, try to rollback if there's transaction going on
 *
 * called at DbConnection's destructor
 * require explicitly handle transaction before disconnection
 *
 */
void DbConnection::Close() noexcept {
    if (db_ != nullptr) {
        if (in_transaction) {
            std::cerr << "\n================= FATAL ERROR ==================\n"
                      << "THERE'S AN UNFINISHED TRANSACTION DURING DB CLOSE!\n"
                      << "================================================\n";

            if (char* err_msg = nullptr;
                sqlite3_exec(db_, "ROLLBACK", nullptr, nullptr, &err_msg) !=
                SQLITE_OK) {
                std::string error_message = err_msg ? err_msg : "Unknown error";
                sqlite3_free(err_msg);
                std::cerr << "TRY TO ROLLBACK FAILED: \n" + error_message;
            }
            std::cerr << "TRY TO ROLLBACK SUCCESSFULL\n";
        }
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

/**
 * @brief
 *
 * Delete file at path
 * Throws DbException if the database cannot be opened or invaild path.
 *
 * @param db_file_path Absolute path to the `.sqlite` database file.
 */
void DbConnection::DeleteDb(const std::string& db_file_path) const {
    using enum DbException::DbErrorType;
    ValidateDbPath(db_file_path);
    if (!std::filesystem::remove(db_file_path)) {
        throw(DbException("Cant delete database: " + db_file_path,
                          FILE_SYSTEM));
    }
}

/**
 * @brief exectute sql without callback function
 *
 * Use for delete, new, alter. Not for search
 * If throw `Unknown Error`, its likely due to db_ was changeed or closed
 * throw DbException(SQL_EXECUTE) if sql was not executed successfully
 *
 *
 * @param sql Expect valid sql stmt, dont validate sql correctness
 */
void DbConnection::ExecuteSql(const std::string& sql) const {
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        std::string error_message = err_msg ? err_msg : "Unknown error";
        sqlite3_free(err_msg);
        throw(DbException(error_message,
                          DbException::DbErrorType::SQL_EXECUTE));
    }
}

/**
 * @brief exectute sql with out callback function
 *
 * Use for search, for each line runs callback function once.
 * Put methods need get callback data into callback function
 * throw DbException(SQL_EXECUTE) if sql was not executed successfully
 *
 * @param sql Expect valid sql stmt, dont validate sql correctness
 * @param callback callback function gets row & col value as vector
 */
void DbConnection::ExecuteSql(const std::string& sql,
                               const RowCallback& callback) const {
    auto wrapper_callback =
            [](void* data, int count, char** value, char** columns) {
                auto* cb_data = static_cast<CallbackData*>(data);

                if (!cb_data->col_names_initialized) {
                    cb_data->col_names.clear();
                    for (int i = 0; i < count; ++i) {
                        cb_data->col_names.emplace_back(columns[i] ? columns[i]
                                                                   : "");
                    }
                    cb_data->col_names_initialized = true;
                }

                std::vector<std::string> row_values;
                for (int i = 0; i < count; ++i) {
                    row_values.emplace_back(value[i] ? value[i] : "");
                }
                if (cb_data->cb) {
                    (cb_data->cb)(cb_data->col_names, row_values);
                }
                return 0;
            };

    CallbackData cb_data{callback, {}, false};

    char* err_msg = nullptr;
    int rc = sqlite3_exec(
            db_, sql.c_str(), wrapper_callback, &cb_data, &err_msg);
    if (rc != SQLITE_OK) {
        std::string error_message = err_msg ? err_msg : "Unknown error";
        if (!err_msg) {
            error_message += std::format(
                    "\nSQL: {}\nSQLite Error Code: {}\nSQLite Error Name: {}",
                    sql,
                    rc,
                    sqlite3_errstr(rc));
        }
        if (err_msg) sqlite3_free(err_msg);
        throw DbException(error_message, DbException::DbErrorType::SQL_EXECUTE);
    }
}

/**
 * @brief start transaction
 *
 * throw DbException(TRANSACTION) if already in transaction
 * throw DbException(SQL_EXECUTE) if sql was not executed successfully
 *
 */
void DbConnection::BeginTransaction() {
    using enum DbException::DbErrorType;
    if (!in_transaction) {
    ExecuteSql("BEGIN TRANSACTION");
        in_transaction = true;
    } else {
        throw DbException("Already in a transaction", TRANSACTION);
    }
}

/**
 * @brief commit transaction
 *
 * throw DbException(TRANSACTION) if not in transaction
 * throw DbException(SQL_EXECUTE) if sql was not executed successfully
 *
 */
void DbConnection::CommitTransaction() {
    using enum DbException::DbErrorType;
    if (in_transaction) {
    ExecuteSql("COMMIT");
        in_transaction = false;
    } else {
        throw DbException("Not in a transaction", TRANSACTION);
    }
}

/**
 * @brief rollback transaction
 *
 * throw DbException(TRANSACTION) if not in transaction
 * throw DbException(SQL_EXECUTE) if sql was not executed successfully
 *
 */
void DbConnection::RollbackTransaction() {
    using enum DbException::DbErrorType;
    if (in_transaction) {
    ExecuteSql("ROLLBACK");
        in_transaction = false;
    } else {
        throw DbException("Not in a transaction", TRANSACTION);
    }
}

/**
 * @brief check if db_ is nullptr
 *
 * Notice! Just check nullptr
 * Dont guarantee db is actuall connected with sqlite
 *
 * @return true
 * @return false
 */
bool DbConnection::IsConnected() const { return db_ != nullptr; }

/**
 * @brief validate if path ends with .sqlite, without space, and file exist
 *
 * throw DbException(FILE_CONSTRAINT) if path format is incorrect or file dont's
 * exist
 *
 * @param db_file_path Expect existed path without space, ends with .sqlite
 */
void DbConnection::ValidateDbPath(const std::string& db_file_path) const {
    using enum DbException::DbErrorType;

    if (!ValidateDbPathFormat(db_file_path)) {
        throw(DbException("Invalid db_file_path: " + db_file_path,
                          FILE_CONSTRAINT));
    }
    if (!std::filesystem::exists(db_file_path)) {
        throw DbException("File dont exist: " + db_file_path, FILE_CONSTRAINT);
    }
}

/**
 * @brief validate path format, dont check if file exists
 *
 * Expect path ends with `sqlite` & without white space
 *
 * @param path Expect std::string ends with `sqlite` & without white space
 * @return true -> valid
 * @return false -> invalid
 */
bool DbConnection::ValidateDbPathFormat(std::string_view path) {
    if (path.size() < 7 || !path.ends_with(".sqlite")) {
        return false;
    }
    if (!std::ranges::none_of(path, [](char c) {
            return std::isspace(static_cast<unsigned char>(c));
        })) {
        return false;
    }
    return true;
}