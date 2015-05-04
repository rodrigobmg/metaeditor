#include "softwarebase.h"

SoftwareBase::SoftwareBase()
{

}

SoftwareBase::SoftwareBase(const std::string &name)
    : Object(name)
{

}

void SoftwareBase::wrap(bson_t *data)
{
    Object::wrap(data);
}

bool SoftwareBase::unwrap(const bson_t *data)
{
    return Object::unwrap(data);
}
