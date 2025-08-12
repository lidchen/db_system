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
        db.execute_sql("CREATE TABLE test (alter_test int)");
        REQUIRE_NOTHROW(db.connect(test_dir + "db_connection2.sqlite"));
        db.connect(test_dir + "db_connection2.sqlite");
        REQUIRE_NOTHROW(db.execute_sql("CREATE TABLE alter_test (test int)"));
    }

    SECTION("Db Delete") {
        DbConnection db(test_dir + "db_connection.sqlite");
        db.connect(test_dir + "db_connection2.sqlite");

        // Not existed table
        REQUIRE_THROWS_AS(
                db.delete_db(test_dir + "not_existed_db.sqlite"),
                DbException);
        
        // Delete not current opened database
        REQUIRE_NOTHROW(db.delete_db(test_dir + "db_connection.sqlite"));

        // Delete current opened database then execute sql
        db.connect(test_dir + "db_connection.sqlite");
        REQUIRE_NOTHROW(db.delete_db(test_dir + "db_connection.sqlite"));
        REQUIRE_THROWS_AS(db.execute_sql("CREATE TABLE test (test int)"), DbException);
    }

    SECTION("Sql Execute") {
        DbConnection db(test_dir + "db_connection.sqlite");
        REQUIRE_THROWS_AS(db.execute_sql("test"), DbException);
        REQUIRE_THROWS_AS(db.execute_sql("test", nullptr), DbException);
        REQUIRE_NOTHROW(db.execute_sql("CREATE TABLE test (test int)"));
        // Test call back function
        REQUIRE_NOTHROW(db.execute_sql("SELECT * FROM test", [](auto col_names, auto row_names){
            for (auto row_name : row_names) {
                std::cerr << row_name << "\n";
            }
        }));
    }

    SECTION("Transaction") {
        DbConnection db(test_dir + "db_connection.sqlite");
        REQUIRE_NOTHROW(db.begin_transaction());
        REQUIRE_THROWS_WITH_TYPE(db.begin_transaction(), DbException::DbErrorType::TRANSACTION);
        REQUIRE_NOTHROW(db.execute_sql("CREATE TABLE test (test int)"));
        REQUIRE_NOTHROW(db.rollback_transaction());
        REQUIRE_NOTHROW(db.execute_sql("CREATE TABLE test (test int)"));
    }
}