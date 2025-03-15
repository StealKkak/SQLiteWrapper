#include <__config>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sqlite3.h>

class SQLiteWrapper {
    public:
        SQLiteWrapper(const std::string& path) {
            if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
                std::cerr << "Can't Open Database: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("Failed to open database");
            }
        }

        ~SQLiteWrapper() {
            if (db) {
                sqlite3_close(db);
            }
        }

        void excute(const std::string& sql, const std::vector<std::string>& params) {
            sqlite3_stmt* stmt;
            
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                std::cerr << "SQL Preparation Error: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("SQL Preperation Error");
            }

            for (size_t i = 0; i < params.size(); i++) {
                if (sqlite3_bind_text(stmt, i+1, params[i].c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
                    std::cerr << "Parameter Binding Error: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_finalize(stmt);
                    throw std::runtime_error("Parameter Binding Error");
                }
            }

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "SQL Execution Error: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                throw std::runtime_error("SQL Execution Error");
            }

            sqlite3_finalize(stmt);
        }

        void query(const std::string& sql, std::vector<std::string>& params) {
            sqlite3_stmt* stmt;

            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                std::cerr << "SQL Preparation Error: " << sqlite3_errmsg(db) << std::endl;
                throw std::runtime_error("SQL Preperation Error");
            }

            for (size_t i = 0; i < params.size(); i++) {
                if (sqlite3_bind_text(stmt, i+1, params[i].c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
                    std::cerr << "Parameter Binding Error: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_finalize(stmt);
                    throw std::runtime_error("Parameter Binding Error");
                }
            }

            queryResult.clear();
            int rc = sqlite3_step(stmt);

            while (rc == SQLITE_ROW) {
                std::vector<std::string> row;
                for (int i = 0; i < sqlite3_column_count(stmt); i++) {
                    row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                }
                queryResult.push_back(row);
                rc = sqlite3_step(stmt);
            }

            if (rc != SQLITE_DONE) {
                std::cerr << "Query Execution Error: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                throw std::runtime_error("Query Execution Error");
            }

            sqlite3_finalize(stmt);
        }

        std::vector<std::string> fetchone() {
            if (currentRowIndex < queryResult.size()) {
                return queryResult[currentRowIndex++];
            } else {
                return std::vector<std::string>();
            }
        }

        std::vector<std::vector<std::string> > fetchall() {
            return queryResult;
        }
    private:
        sqlite3* db;
        std::vector<std::vector<std::string> > queryResult;
        size_t currentRowIndex = 0;

        static int callback(void* data, int argc, char** argv, char** azColName) {
            SQLiteWrapper* dbInstance = static_cast<SQLiteWrapper*>(data);
            std::vector<std::string> row;

            for (int i = 0; i < argc; i ++) {
                row.push_back(argv[i] ? argv[i] : "NULL");
            }

            dbInstance->queryResult.push_back(row);
            return 0;
        }
};