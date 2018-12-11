#ifndef BASE_H
#define BASE_H
#include "HttpAssist.h"
#include "JsonStructs.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <string>

class Model;

class Base : public Wt::WResource {
public:
    Base(Model* _model)
        : model(_model)
    {
    }
    virtual ~Base()
    {
        beingDeleted();
    }

protected:
    Model* model;

protected:
    void writeOutput(const Wt::Http::Message& msg, Wt::Http::Response& response)
    {
        HttpAssist::writeHeaders(response, msg.headers());
        response.out() << (msg.body().empty() and (msg.status() > 299 or msg.status() < 200) // )))))
                ? HttpAssist::ResponseAsssist::TimeoutError()
                : msg.body());
        response.setStatus(msg.status() > 0 ? msg.status() : 500);
    }
};
class AuthService : public Base {
public:
    AuthService(Model* _model, std::string _secretKey)
        : Base(_model)
        , secretKey(_secretKey){};

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
    {
        using namespace jwt::params;
        try {
            std::string header = request.headerValue("serviceheader");
            if (header.empty()) {
                throw 0;
            }
            auto dec_obj = jwt::decode(header, algorithms({ "hs256" }), secret(gatewaySecret));
            json_t result;
            jwt::jwt_object obj{ algorithm("HS256"), payload({}), secret(secretKey) };
            obj.add_claim("exp", std::chrono::system_clock::now() + std::chrono::minutes(30));
            result["token"] = obj.signature();
            response.setStatus(200);
            response.out() << result.dump();
        } catch (...) {
            response.setStatus(400);
        }
    }
    const std::string secretKey;
    const std::string gatewaySecret = "gateway";
};

#endif // BaseL_H
