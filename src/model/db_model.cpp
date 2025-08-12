#include <filesystem>
#include <fstream>
#include <iostream>

#include "db_connection.h"
#include "db_model.h"

DbModel::DbModel(std::shared_ptr<DbConnection> db) : db_(db) {}

/**
 * @brief dont forget call this before any db_level operation
 *
 * throw DbException(FILE_SYSTEM) if formated path dont exist
 * expect path end without `/`, if have it will remove it automatically
 *
 * @param db_dir except path end without `/`
 * automatically
 */
void DbModel::set_db_dir(const std::string& db_dir) {
    using enum DbException::DbErrorType;
    std::string formated_dir = db_dir;
    if (formated_dir.ends_with('/')) {
        formated_dir.pop_back();
    }
    if (!std::filesystem::exists(formated_dir)) {
        throw DbException("Directory dont exist: " + formated_dir, FILE_SYSTEM);
    }
    db_dir_ = formated_dir;
}

/**
 * @brief no_except
 *
 * @return std::string db_dir_
 */
std::string DbModel::get_db_dir() const noexcept { return db_dir_.string(); }

/**
 * @brief clear db_names_, push all file names at dir to db_names_
 *
 * throw DbException(FILE_CONSTRAINT) if dir not set
 *
 */
void DbModel::scan_db_dir() {
    using enum DbException::DbErrorType;
    if (db_dir_.empty()) {
        throw DbException("Database directory was not set", FILE_CONSTRAINT);
    }
    db_names_.clear();
    for (const auto& entry : std::filesystem::directory_iterator(db_dir_)) {
        db_names_.push_back(entry.path().filename());
    }
}

/**
 * @brief noexcept
 *
 * @return std::vector<std::string> db_names
 */
std::vector<std::string> DbModel::get_db_names() const noexcept {
    return db_names_;
}

/**
 * @brief only connect when current dir contain this file
 *
 * throw DbException(FILE_CONSTRAINT) if db_dir_ not set
 * throw DbException(FILE_CONSTRAINT) if db_dir_ dont contain file
 *
 * call db_->connect()
 * throw DbException(FILE_CONSTRAINT) if invaild path.
 * throw DbException(CONNECTION) if the database cannot be opened
 *
 * @param db_name expect valid db file format
 */
void DbModel::connect(const std::string& db_name) {
    using enum DbException::DbErrorType;
    if (db_dir_.empty()) {
        throw DbException("Database directory was not set", FILE_CONSTRAINT);
    }
    scan_db_dir();
    if (auto it = std::ranges::find(db_names_, db_name);
        it != db_names_.end()) {
        db_->connect(db_dir_ / db_name);
        return;
    }
    throw DbException("Current directory dont contain file: " + db_name,
                      FILE_CONSTRAINT);
}

/**
 * @brief only create db if current dir dont contain db_name
 *
 * add db_name to db_names after creatation
 * throw DbException(FILE_CONSTRAINT) if db_dir_ not set
 * throw DbException(FILE_CONSTRAINT) if db_name format invalid
 * throw DbException(FILE_CONSTRAINT) if db_name already exsited
 * throw DbException(FILE_SYSTEM) if std::filesystem cant create at generated
 * path
 *
 * @param db_name expect valid db file format
 */
void DbModel::create_db(const std::string& db_name) {
    using enum DbException::DbErrorType;
    if (db_dir_.empty()) {
        throw DbException("Database directory was not set", FILE_CONSTRAINT);
    }
    if (!DbConnection::validate_db_path_format(db_name)) {
        throw DbException("File name format invalid: " + db_name,
                          FILE_CONSTRAINT);
    }
    if (auto it = std::ranges::find(db_names_, db_name);
        it == db_names_.end()) {
        if (std::ofstream ofs{db_dir_ / db_name}; !ofs) {
            throw DbException("Cant create file: " + db_name, FILE_SYSTEM);
        }
        db_names_.push_back(db_name);
        return;
    }
    throw DbException("Database file already existed at current directory",
                      FILE_CONSTRAINT);
}

/**
 * @brief only delete db if current dir contains db_name
 *
 * remove db_name at db_names after deletion
 * throw DbException(FILE_CONSTRAINT) if db_dir_ not set
 * throw DbException(FILE_CONSTRAINT) if db_name dont existed at db_dir_
 * throw DbException(FILE_SYSTEM) if std::filesystem cant remove file
 *
 * @param db_name expect valid db file format
 */
void DbModel::delete_db(const std::string& db_name) {
    using enum DbException::DbErrorType;
    if (db_dir_.empty()) {
        throw DbException("Database directory was not set", FILE_CONSTRAINT);
    }
    if (auto it = std::ranges::find(db_names_, db_name);
        it != db_names_.end()) {
        if (!std::filesystem::remove(db_dir_ / db_name)) {
            throw DbException("Cant delete file: " + db_name, FILE_SYSTEM);
        }
        db_names_.erase(it);
        return;
    }
    throw DbException("Database dont existed in current directory",
                      FILE_CONSTRAINT);
}