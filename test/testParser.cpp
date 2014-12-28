#include "frontend/Parser.hpp"
#include "context/context.hpp"

int main() {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    Context::Context ctx(pgdb);
    std::remove(Context::DBFilename.c_str());
    Parser::CreateAST("Create Table wdy (W int(10), D Varchar(10), Y Varchar(10) Not Null, Primary Key W)")->Run(ctx);
    Parser::CreateAST("Create Table wy (W int(10), D Varchar(10), Y Varchar(10) Not Null, Primary Key W)")->Run(ctx);
    Parser::CreateAST("Create Table happy (W int(10), D Varchar(10), Y Varchar(10) Not Null, Primary Key W)")->Run(ctx);
    Parser::CreateAST("insert into wdy values (1, 'wdy', 'abc'), (2, 'wy', 'lo'), (3, 'XYZ', 've')")->Run(ctx);
    Parser::CreateAST("insert into wy values (1, 'wdy', 'abc'), (2, 'wy', 'lo'), (3, 'XYZ', 've')")->Run(ctx);
    Parser::CreateAST("insert into happy values (1, 'wdy', 'abc'), (2, 'wy', 'lo'), (3, 'XYZ', 've')")->Run(ctx);
    Parser::CreateAST("select wdy.W, D from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select SUM(wdy.W), avg(W), min(W), max(W), sum(D) from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy where wdy.W = 1 or wdy.D = \"wy\"")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy, wy where wdy.W = wy.W")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy, wy where wdy.W = wy.W and wdy.W = 1")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("select * from wdy, wy, happy where wdy.W = wy.W and wdy.W = happy.W")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("insert into wdy values (0, NULL, \"ABC\")")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("update wdy set D = D + \"...\" where W = 1")->Run(ctx);
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("delete from wdy where W = 1")->Run(ctx);
    Parser::CreateAST("Select * from wdy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Drop Table happy")->Run(ctx);
    Parser::CreateAST("Create Table happy (W int(10), D Varchar(10), Y Varchar(10) Not Null, Primary Key W)")->Run(ctx);
    Parser::CreateAST("insert into happy values (0, NULL, \"ABC\")")->Run(ctx);
    Parser::CreateAST("Select * from happy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Show Tables")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Create Database X")->Run(ctx);
    Parser::CreateAST("Use X")->Run(ctx);
    Parser::CreateAST("Create Table happy (W int(10), D Varchar(10), Y Varchar(10) Not Null, Primary Key W)")->Run(ctx);
    Parser::CreateAST("insert into happy values (1, NULL, \"ABC\")")->Run(ctx);
    Parser::CreateAST("insert into happy values (2, NULL, \"CBD\")")->Run(ctx);
    Parser::CreateAST("insert into happy values (3, NULL, \"ABC\")")->Run(ctx);
    Parser::CreateAST("Select * from happy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Show Tables")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("create table fori (WX int(10), foreign key (WX) references happy(W))")->Run(ctx);
    Parser::CreateAST("insert into fori values (1)")->Run(ctx);
    Parser::CreateAST("Select * from fori")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("desc happy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Select * from happy")->Run(ctx);
    std::cout << std::endl;
    Parser::CreateAST("Select Y, sum(W) from happy group by Y")->Run(ctx);
    std::cout << std::endl;
    try {
        Parser::CreateAST("insert into fori values (0)")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("insert into wdy values (0, \"XYZ\", \"ABC\")")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("insert into wdy values (0, 1, \"XYZ\", \"ABC\")")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("insert into wdy values (0, 1, \"ABC\")")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("insert into wdy values (5, \"ABC\", NULL)")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("update wdy set D = 1")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("update wdy set W = 1")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("select * from happyC")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("update happyCC set D = 2")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    try {
        Parser::CreateAST("Drop Table ha")->Run(ctx);
        std::cout << "ERROR" << std::endl;
    } catch (const char* str) {
        std::cout << str << std::endl;
    }
    pgdb->StopSchedule();
    delete pgdb;
}
