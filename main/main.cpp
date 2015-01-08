#include "frontend/parser.hpp"
#include <string>
#include <iostream>
#include <fstream>

void RunStmt(const std::string& sql, Context::Context& ctx) {
#ifndef DEBUG
    try {
#endif
        auto X = Parser::CreateAST(sql);
        X->Run(ctx);
#ifndef DEBUG
    } catch (const char* ex) {
        std::cout << ex << std::endl;
    }
#endif
}

int main(int argc, char** argv) {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    Context::Context ctx(pgdb);
    if (argc == 2) {
        std::ifstream input(argv[1]);
        if (!input) {
            std::cout << "File Not Found" << std::endl;
            return 0;
        }
        std::vector<char> sql;
        char c = '\0';
        while (input) {
            int x = input.get();
            if (x == -1)
                break;
            if (c == '\0' && x == ';') {
                RunStmt(std::string(sql.begin(), sql.end()), ctx);
                sql.clear();
            } else {
                if (c == '\0' && x == '\'')
                    c = '\'';
                else if (c == '\0' && x == '"')
                    c = '"';
                else if (c == '\'' && x == '\'')
                    c = '\0';
                else if (c == '"' && x == '"')
                    c = '\0';
                sql.push_back(x);
            }
        }
    } else {
        while (true) {
            std::string sql;
            std::cout << ctx.dbName << " >> ";
            std::getline(std::cin, sql);
            if (!std::cin)
                break;
            if (sql.substr(0, 4) == "exit")
                break;
            RunStmt(sql, ctx);
        }
    }
    pgdb->StopSchedule();
    delete pgdb;
}
