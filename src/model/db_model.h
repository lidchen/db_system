#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "db_connection.h"

/**
 * @brief For Database-Level Management
 * 
 * Manage file dir, Handles database create, delete operation, Set file directory
 * 
 */
class DbModel {
public:
    explicit DbModel(std::shared_ptr<DbConnection> db);
    void set_db_dir(const std::string& db_dir);
    void scan_db_dir();
    std::vector<std::string> get_db_names() const noexcept;
    std::string get_db_dir() const noexcept;

    void connect(const std::string& db_name);
    void create_db(const std::string& db_name);
    void delete_db(const std::string& db_name);

   private:
    std::vector<std::string> db_names_;
    std::filesystem::path db_dir_;
    std::shared_ptr<DbConnection> db_;
};
