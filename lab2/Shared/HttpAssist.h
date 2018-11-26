#ifndef SHAREDHTTP_H
#define SHAREDHTTP_H
#include "../3rdpart/macrologger.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <Wt/WResource>
#include <Wt/WServer>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <climits>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using json_t = nlohmann::json;

using namespace Wt;
namespace HttpAssist {
const static std::string skUrlNews = "http://localhost:8081";
const static std::string skUrlComments = "http://localhost:8082";
const static std::string skUrlUsers = "http://localhost:8083";
const static std::string skUrlLikeHistory = "http://localhost:8084";

class Client : public Http::Client {
public:
    Client()
        : done_(false)
        , abortAfterHeaders_(false)
    {
        done().connect(this, &Client::onDone);
        headersReceived().connect(this, &Client::onHeadersReceived);
        bodyDataReceived().connect(this, &Client::onDataReceived);
    }

    void abortAfterHeaders()
    {
        abortAfterHeaders_ = true;
    }

    void waitDone()
    {
        std::unique_lock<std::mutex> guard(doneMutex_);

        while (!done_)
            doneCondition_.wait(guard);
        reset();
    }

    void reset()
    {
        done_ = false;
    }

    bool isDone()
    {
        return done_;
    }

    void onDone(Wt::AsioWrapper::error_code err, const Http::Message& m)
    {
        std::unique_lock<std::mutex> guard(doneMutex_);

        err_ = err;
        message_ = m;

        done_ = true;
        doneCondition_.notify_one();
    }

    void onHeadersReceived(const Http::Message& m)
    {
        if (abortAfterHeaders_)
            abort();
    }

    void onDataReceived(const std::string& d)
    {
    }

    Wt::AsioWrapper::error_code err() { return err_; }
    const Http::Message& message() { return message_; }

private:
    bool done_;
    bool abortAfterHeaders_;
    std::condition_variable doneCondition_;
    std::mutex doneMutex_;

    Wt::AsioWrapper::error_code err_;
    Http::Message message_;
};

//return -1 for wrong extract
inline int32_t extractPositiveParam(const Wt::Http::Request& request, const std::string& paramName)
{
    const std::string* temp = request.getParameter(paramName);
    int32_t result = -1;
    if (temp) {
        result = atoi(temp->c_str());
    }
    return result > -1 ? result : -1;
}

//return empty string for wrong extract
inline std::string extractStringParam(const Wt::Http::Request& request, const std::string& paramName)
{
    const std::string* temp = request.getParameter(paramName);

    return temp ? (*temp) : std::string();
}
inline void writeHeaders(Wt::Http::Response& response, const std::vector<Wt::Http::Message::Header>& headers)
{
    for (size_t i = 0; i < headers.size(); ++i) {
        response.addHeader(headers[i].name(), headers[i].value());
    }
}
inline void writeHeaders(Wt::Http::Message& msg, const std::vector<Wt::Http::Message::Header>& headers)
{
    for (size_t i = 0; i < headers.size(); ++i) {
        msg.setHeader(headers[i].name(), headers[i].value());
    }
}
inline json_t tryParsejson(const std::string& body)
{
    json_t result;
    try {
        result = json_t::parse(body);
    } catch (...) {
        return json_t();
    }
    return result;
}
inline std::string getRequestBody(const Wt::Http::Request& request)
{
    std::string body;
    while (std::getline(request.in(), body))
        ;
    return body;
}
inline std::string getAuthToken(const std::vector<Http::Message::Header>& headers)
{
    for (auto header : headers) {
        const std::string headerName = boost::algorithm::to_lower_copy(header.name());
        if (headerName == "authorization") {
            std::string value = header.value();
            std::size_t found = value.find_last_of(" ");

            return value.substr(found + 1);
        }
    }
    return std::string();
}

inline void addUserId(std::string& body, uint32_t userId)
{
    std::cout << "BODY" << body << std::endl;
    json_t bodyJson = tryParsejson(body);
    bodyJson["userId"] = userId;
    std::cout << "BODY AFTER" << bodyJson.dump() << std::endl;
    body = bodyJson.dump();
}
}

#endif // SHAREDHTTP_H
