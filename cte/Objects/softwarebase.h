#ifndef SOFTWAREBASE_H
#define SOFTWAREBASE_H

#include <vector>
#include <DAO/object.h>

class SoftwareBase : public Object
{
public:
    SoftwareBase();
    explicit SoftwareBase(const std::string& name);

public:
    void wrap(bson_t *data);
    bool unwrap(const bson_t *data);

private:
    std::string m_project;
    std::string m_description;
    std::string m_observation;
    std::vector<std::string> m_modules;
    std::vector<std::string> m_textBlocks;
    std::vector<std::string> m_functions;
};

#endif // SOFTWAREBASE_H
