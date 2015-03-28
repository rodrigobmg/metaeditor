#ifndef BSONUTILS_H
#define BSONUTILS_H

#include <bson.h>

/**
 * @brief The BSONUtils class
 * Utility static class to help at obtain data from bson
 */
class BSONUtils
{
private:
    static const bson_value_t* getField(const bson_t *bson, const std::string& field_name);

public:
    static std::string getString(const bson_t* bson, const std::string& field_name);
    static int getInt(const bson_t* bson, const std::string& field_name);
    static double getDouble(const bson_t *bson, const std::string& field_name);
    static bool getArray(const bson_t* bson, const std::string& field_name, std::string& data);
    static bool getOID(const bson_t* bson, std::string &ret_oid);
    static bool getOID(const bson_t* bson, bson_oid_t& oid);
};

#endif // BSONUTILS_H
