#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <iostream>

#include "customized_test_macros.h"
#include "src/model/db_model.h"

const std::filesystem::path test_dir =
        "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/"
        "db_system/test/db_dir";

struct DbModelTestCleanUp {
    DbModelTestCleanUp() = default;
    DbModelTestCleanUp(const DbModelTestCleanUp&) = delete;
    DbModelTestCleanUp& operator=(const DbModelTestCleanUp&) = delete;
    ~DbModelTestCleanUp() {
        std::filesystem::remove(test_dir / "db_model_test.sqlite");
        std::filesystem::remove(test_dir / "db_model_test2.sqlite");
    }
};

TEST_CASE("Db Model Test") {
    auto db_connection = std::make_shared<DbConnection>();
    static DbModelTestCleanUp cleanup;
    DbModel db_model(db_connection, nullptr);
    REQUIRE(!db_connection->IsConnected());
    SECTION("Set Dir") {
        // Not existed path
    REQUIRE_THROWS_AS(db_model.SetDbDir("/Users/not_existed_path"),
                          DbException);
        // With `/`, test auto pop last separator
    REQUIRE_NOTHROW(
        db_model.SetDbDir("/Users/lid/Library/CloudStorage/"
                    "OneDrive-Personal/Develop/work/"
                    "db_system/test/db_dir/"));
    REQUIRE(!db_model.GetDbDir().ends_with('/'));
    }
    SECTION("File create & delete") {
    db_model.SetDbDir(test_dir);
    REQUIRE_NOTHROW(db_model.CreateDb("db_model_test.sqlite"));
        // Already existed file
    REQUIRE_THROWS_AS(db_model.CreateDb("db_model_test.sqlite"),
                          DbException);
    REQUIRE_NOTHROW(db_model.CreateDb("db_model_test2.sqlite"));
    REQUIRE_NOTHROW(db_model.DeleteDb("db_model_test2.sqlite"));
        // Not existed file
    REQUIRE_THROWS_AS(db_model.DeleteDb("db_model_test2.sqlite"),
                          DbException);
    }
    SECTION("Scan Dir") {
    db_model.SetDbDir(test_dir);
    REQUIRE_NOTHROW(db_model.ScanDbDir());
    REQUIRE(!db_model.GetDbNames().empty());
    }
    SECTION("DbConnection") {
    db_model.SetDbDir(test_dir);
    REQUIRE_NOTHROW(db_model.Connect("db_model_test.sqlite"));
        // Double connection
    REQUIRE_NOTHROW(db_model.Connect("db_model_test.sqlite"));
        // Connect to non_existed file
        REQUIRE_THROWS_WITH_TYPE(db_model.Connect("db_model_test2.sqlite"),
                                 DbException::DbErrorType::FILE_CONSTRAINT);
        REQUIRE_NOTHROW(db_model.CreateDb("db_model_test2.sqlite"));
        REQUIRE_NOTHROW(db_model.Connect("db_model_test2.sqlite"));
    }
}