#pragma once

#include "db_connection.h"
#include "src/event/event_broker.h"

#include <filesystem>
#include <string>
#include <vector>

/**
 * @brief For Database-Level Management
 *
 * Manage file dir, Handles database create, delete operation, Set file
 * directory
 *
 */
class DbModel {
    using EvtPtr = std::shared_ptr<EventBroker>;

   public:
    explicit DbModel(std::shared_ptr<DbConnection> db, EvtPtr event_broker);
    void SetDbDir(const std::string& db_dir);
    void ScanDbDir();

    std::vector<std::string> GetDbNames() const noexcept;
    std::string GetDbDir() const noexcept;
    std::string FormatFileName(const std::string& raw_input) const;

    void Connect(const std::string& db_name);
    void CreateDb(const std::string& db_name);
    void DeleteDb(const std::string& db_name);

   private:
    std::shared_ptr<DbConnection> db_;
    EvtPtr event_broker_;
    std::vector<std::string> db_names_;
    std::filesystem::path db_dir_;
};
