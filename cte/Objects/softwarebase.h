#ifndef SOFTWAREBASE_H
#define SOFTWAREBASE_H

#include <vector>
#include <DAO/object.h>

#include "User.h"


class SoftwareBase : public Object
{
public:
    SoftwareBase();
    explicit SoftwareBase(const std::string& name);

public:
    void wrap(bson_t *data);
    bool unwrap(const bson_t *data);

private:
    std::vector<User> users;
};

#endif // SOFTWAREBASE_H
