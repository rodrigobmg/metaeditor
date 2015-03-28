#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <bson.h>

class Object
{
public:
    explicit Object();
    explicit Object(const std::string& name);
    virtual ~Object();

    //Properties
    const std::string Name();
    const std::string Name() const;
    const bson_oid_t& Id();
    void setName(const std::string &name);

    //Utils
    virtual void wrap(bson_t* data);
    virtual bool unwrap(const bson_t *data);

private:
    bson_oid_t m_id;
    std::string m_name;
    bool m_update;
};

#endif // OBJECT_H
