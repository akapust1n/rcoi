#ifndef SHAREDHTTP_H
#define SHAREDHTTP_H
#include "../3rdpart/macrologger.h"
#include "JsonStructs.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <Wt/WResource>
#include <Wt/WServer>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <climits>
#include <ctime>
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
#ifdef DOCKERCOMPOSE
const static std::string skUrlNews = "http://news:8081";
const static std::string skUrlComments = "http://comments:8082";
const static std::string skUrlUsers = "http://users:8083";
const static std::string skUrlLikeHistory = "http://likehistory:8084";
#else
const static std::string skUrlNews = "http://localhost:8081";
const static std::string skUrlComments = "http://localhost:8082";
const static std::string skUrlUsers = "http://localhost:8083";
const static std::string skUrlLikeHistory = "http://localhost:8084";
#endif
const static std::string skMetricServer = "http://kapust1n.ru:10300/api_jsonrpc.php";

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

inline bool checkAuth(const Wt::Http::Request& request, const std::string& secretKey)
{
    using namespace jwt::params;
    using namespace std::chrono;
    try {
        std::error_code ec;
        const std::string header = request.headerValue("serviceheader");
        auto dec_obj = jwt::decode(header, algorithms({ "hs256" }), ec, secret(secretKey));

        const std::time_t exp = static_cast<std::time_t>(dec_obj.payload().get_claim_value<uint64_t>("exp"));
        auto nowTimePoint = std::chrono::system_clock::now();
        const std::time_t nowPlus10Min = system_clock::to_time_t(nowTimePoint + minutes(10));
        std::cout << "TIME BEFORe " << exp << std::endl;
        std::cout << "TIME after " << nowPlus10Min << std::endl;

        if (nowPlus10Min > exp) {
            throw 0;
        }
        return true;
    } catch (...) {
        return false;
    };
}
class ResponseAsssist {

public:
    std::string ParseError() const
    {
        json_t res;
        res["error"] = "Wrong msg format";
        return res.dump();
    }
    std::string InternalError() const
    {
        json_t res;
        res["error"] = "Internal error";
        return res.dump();
    }
    std::string AuthError() const
    {
        json_t res;
        res["error"] = "Cant authorize";
        return res.dump();
    }
    std::string UnknownError() const
    {
        json_t res;
        res["error"] = "Unknown reason";
        return res.dump();
    }
    static std::string TimeoutError()
    {
        json_t res;
        res["error"] = "Timeout reason";
        return res.dump();
    }
};
inline void WriteResponse(Wt::Http::Response& response, int32_t code)
{
    static ResponseAsssist ra;
    response.setStatus(code);
    switch (code) {
    case 400: {
        response.out() << ra.AuthError();
        break;
    }
    case 403: {
        response.out() << ra.ParseError();
        break;
    }
    case 500: {
        response.out() << ra.InternalError();
        break;
    }
    default: {
        response.out() << ra.UnknownError();
        break;
    }
    }
}
inline void WriteResponse(Wt::Http::Message& response, int32_t code)
{
    static ResponseAsssist ra;
    response.setStatus(code);
    switch (code) {
    case -1: {
        response.addBodyText(ra.TimeoutError());
        break;
    }
    case 200: {
        break;
    }
    case 208: {
        break;
    }
    case 400: {
        response.addBodyText(ra.AuthError());
        break;
    }
    case 403: {
        response.addBodyText(ra.ParseError());
        break;
    }
    case 500: {
        response.addBodyText(ra.InternalError());
        break;
    }
    default: {
        response.addBodyText(ra.UnknownError());
        break;
    }
    }
}
}

#endif // SHAREDHTTP_H
