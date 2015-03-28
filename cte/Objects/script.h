#ifndef SCRIPT_H
#define SCRIPT_H

#include <DAO/object.h>
#include <bson.h>

class Script : public Object
{
public:
    explicit Script();
    explicit Script(std::string& name, char* binaryData, size_t dataSize);
    ~Script();

    //Implementação de métodos virtuais
    virtual void wrap(bson_t* data);
    virtual bool unwrap(const bson_t *data);

public:
    void setBinaryData(char* data, size_t size);
    const char* binaryData();
    const char* binaryData() const;
    size_t size();
    size_t size() const;

private:
    std::string m_binaryData;
};

#endif // SCRIPT_H
