#ifndef CONFIG_H
#define CONFIG_H
#include "HttpAssist.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <exception>
#include <iostream>
#include <set>
#include <string>
namespace pt = boost::property_tree;

class Config {
    const std::string filename = "config.xml";

public:
    static Config* GetInst()
    {
        static Config instance;
        return &instance;
    }
    Config(Config const&) = delete;
    void operator=(Config const&) = delete;

private:
    void load()
    {
        pt::ptree tree;

        // Parse the XML into the property tree.
        pt::read_xml(filename, tree);
        newsReleaseDb = tree.get<std::string>("settings.newsreleasedb");
        newsTestDb = tree.get<std::string>("settings.newstestdb");
        usersReleaseDb = tree.get<std::string>("settings.usersreleasedb");
        usersTestDb = tree.get<std::string>("settings.userstestdb");
        commentsReleaseDb = tree.get<std::string>("settings.commentsreleasedb");
        commentsTestDb = tree.get<std::string>("settings.commentstestdb");
        likehistoryReleaseDb = tree.get<std::string>("settings.likehistoryreleasedb");
        likehistoryTestDb = tree.get<std::string>("settings.likehistorytestdb");
        passwordMysql = tree.get<std::string>("settings.passwordMysql");
        if (newsReleaseDb.empty() or newsTestDb.empty() or usersReleaseDb.empty() or usersTestDb.empty()
            or commentsReleaseDb.empty() or commentsTestDb.empty() or likehistoryReleaseDb.empty()
            or likehistoryTestDb.empty())
            throw std::string("cant read config");
    }

private:
    Config() { load(); }
    std::string newsReleaseDb;
    std::string newsTestDb;
    std::string usersReleaseDb;
    std::string usersTestDb;
    std::string commentsReleaseDb;
    std::string commentsTestDb;
    std::string likehistoryReleaseDb;
    std::string likehistoryTestDb;
    std::string passwordMysql;

public:
    std::string getNewsDb() const
    {
#ifdef IS_TEST_BUILD
        return newsTestDb;
#endif
        return newsReleaseDb;
    }

    std::string getUsersDb() const
    {
#ifdef IS_TEST_BUILD
        return usersTestDb;
#endif
        return usersReleaseDb;
    }

    std::string getCommentsDb() const
    {
#ifdef IS_TEST_BUILD
        return commentsTestDb;
#endif
        return commentsReleaseDb;
    }

    std::string getLikeHistoryDb() const
    {
#ifdef IS_TEST_BUILD
        return likehistoryTestDb;
#endif
        return likehistoryReleaseDb;
    }
    std::string getJWTSecret() const
    {
        return "secret";
    }
};

#endif // Config_H
