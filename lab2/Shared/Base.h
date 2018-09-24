#ifndef BASE_H
#define BASE_H
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <string>

class Model ;

class Base {
public:
    Base(Model* _model)
        : model(_model)
    {
    }

protected:
    Model* model;
};

#endif // BaseL_H
