#include <string>
//
#include "bsonutils.h"

const bson_value_t* BSONUtils::getField(const bson_t *bson, const std::string& field_name)
{
    bson_iter_t it;
    if( bson_iter_init_find(&it, bson, field_name.data()) == true )
    {
        return bson_iter_value(&it);
    }

    return nullptr;
}

std::string BSONUtils::getString(const bson_t *bson, const std::string& field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_UTF8)
        {
            return std::string(ret->value.v_utf8.str);
        }
    }

    return " ";
}

int BSONUtils::getInt(const bson_t *bson, const std::string& field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_INT32)
        {
            return ret->value.v_int64;
        }
    }

    return -1;
}

double BSONUtils::getDouble(const bson_t *bson, const std::string& field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_DOUBLE)
        {
            return ret->value.v_double;
        }
    }

    return -1.0;
}

bool BSONUtils::getArray(const bson_t *bson, const std::string& field_name, std::string& data)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_BINARY)
        {
            unsigned char* buffer = ret->value.v_binary.data;
            size_t len = ret->value.v_binary.data_len;

            if( len > 0 && buffer != NULL )
            {
                data.append(reinterpret_cast<char*>(buffer), len);
                return true;
            }
        }
    }

    return false;
}

#include <iostream>
const int OidMaxSize = 25;

bool BSONUtils::getOID(const bson_t *bson, std::string& ret_oid)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, "_id") ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_OID)
        {
            const bson_oid_t& oid = ret->value.v_oid;
            char* str = new char[OidMaxSize];

            bson_oid_to_string(&oid, str);

            std::cout << "str: " << str << std::endl;

            if( bson_oid_is_valid(str, OidMaxSize) )
            {
                ret_oid.append(str, OidMaxSize);
                std::cout << "ret_oid: " << ret_oid.data() << std::endl;
                return true;
            }

            delete str;
        }
    }

    return false;
}

bool BSONUtils::getOID(const bson_t *bson, bson_oid_t& oid)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, "_id") ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_OID)
        {
            oid = ret->value.v_oid;
            return true;
        }
    }

    return false;
}
