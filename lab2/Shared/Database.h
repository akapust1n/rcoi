#ifndef DATABASE_H
#define DATABASE_H
#include "../3rdpart/macrologger.h"
#include "fmt/format.h"
#include <Wt/Dbo/SqlConnection.h>
#include <Wt/Dbo/backend/MySQL>
#include <nlohmann/json.hpp>

using namespace Wt;

class Db {
public:
    bool Init(std::string scheme)
    {
#ifdef DOCKER
        mysql.reset(new Dbo::backend::MySQL(scheme, "root", "1111", "database", 3306));
#else
        mysql.reset(new Dbo::backend::MySQL(scheme, "root", "1111"));
#endif

        return true;
    }
    static Db* GetInst()
    {
        static Db instance;
        return &instance;
    }
    Dbo::backend::MySQL* GetMysql()
    {
        return mysql.get();
    }

private:
    Db() {}
    std::unique_ptr<Dbo::backend::MySQL> mysql;

public:
    Db(Db const&) = delete;
    void operator=(Db const&) = delete;
};

#endif // DATABASE_H
