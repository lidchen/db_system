#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <string>
#include <iostream>
#include <filesystem>

#include "customized_test_macros.h"
#include "src/model/db_connection.h"
#include "src/model/db_exception.h"

const std::string test_dir =
        "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/"
        "db_system/test/db_dir/";

struct DbConnectionTestCleanUp {
    DbConnectionTestCleanUp() = default;
    DbConnectionTestCleanUp(const DbConnectionTestCleanUp&) = delete;
    DbConnectionTestCleanUp& operator= (const DbConnectionTestCleanUp&) = delete;
    ~DbConnectionTestCleanUp() {
        std::filesystem::remove(test_dir + "db_connection.sqlite");
        std::filesystem::remove(test_dir + "db_connection2.sqlite");
    }
};

TEST_CASE("Db Connection tests with cleanup") {
    DbConnectionTestCleanUp cleanup;
    SECTION("Db Connect && Alter") {
        REQUIRE_THROWS_AS(DbConnection("invaild path"), DbException);
        REQUIRE_THROWS_AS(DbConnection("invaild path.sqlite"), DbException);
        REQUIRE_THROWS_AS(DbConnection("invaild path.sql"), DbException);
        REQUIRE_THROWS_AS(DbConnection("invaild_path.sql"), DbException);
        REQUIRE_THROWS_AS(DbConnection(" "), DbException);
        REQUIRE_THROWS_AS(DbConnection(""), DbException);

        REQUIRE_NOTHROW(DbConnection(test_dir + "db_connection.sqlite"));

        // Alter
        DbConnection db(test_dir + "db_connection.sqlite");
    db.ExecuteSql("CREATE TABLE test (alter_test int)");
    REQUIRE_NOTHROW(db.Connect(test_dir + "db_connection2.sqlite"));
    db.Connect(test_dir + "db_connection2.sqlite");
    REQUIRE_NOTHROW(db.ExecuteSql("CREATE TABLE alter_test (test int)"));
    }

    SECTION("Db Delete") {
        DbConnection db(test_dir + "db_connection.sqlite");
    db.Connect(test_dir + "db_connection2.sqlite");

        // Not existed table
        REQUIRE_THROWS_AS(
                db.DeleteDb(test_dir + "not_existed_db.sqlite"),
                DbException);
        
        // Delete not current opened database
    REQUIRE_NOTHROW(db.DeleteDb(test_dir + "db_connection.sqlite"));

        // Delete current opened database then execute sql
    db.Connect(test_dir + "db_connection.sqlite");
    REQUIRE_NOTHROW(db.DeleteDb(test_dir + "db_connection.sqlite"));
    REQUIRE_THROWS_AS(db.ExecuteSql("CREATE TABLE test (test int)"), DbException);
    }

    SECTION("Sql Execute") {
        DbConnection db(test_dir + "db_connection.sqlite");
        REQUIRE_THROWS_AS(db.ExecuteSql("test"), DbException);
        REQUIRE_THROWS_AS(db.ExecuteSql("test", nullptr), DbException);
        REQUIRE_NOTHROW(db.ExecuteSql("CREATE TABLE test (test int)"));
        // Test call back function
        REQUIRE_NOTHROW(db.ExecuteSql("SELECT * FROM test", [](auto col_names, auto row_names){
            for (auto row_name : row_names) {
                std::cerr << row_name << "\n";
            }
        }));
    }

    SECTION("Transaction") {
        DbConnection db(test_dir + "db_connection.sqlite");
        REQUIRE_NOTHROW(db.BeginTransaction());
        REQUIRE_THROWS_WITH_TYPE(db.BeginTransaction(), DbException::DbErrorType::TRANSACTION);
        REQUIRE_NOTHROW(db.ExecuteSql("CREATE TABLE test (test int)"));
        REQUIRE_NOTHROW(db.RollbackTransaction());
        REQUIRE_NOTHROW(db.ExecuteSql("CREATE TABLE test (test int)"));
    }
}