#include "bsonutils.h"

const bson_value_t* BSONUtils::getField(const bson_t *bson, const char *field_name)
{
    bson_iter_t it;
    if( bson_iter_init_find(&it, bson, field_name) == true )
    {
        return bson_iter_value(&it);
    }

    return nullptr;
}

char* BSONUtils::getString(const bson_t *bson, const char *field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_UTF8)
        {
            return ret->value.v_utf8.str;
        }
    }

    return "";
}

int BSONUtils::getInt(const bson_t *bson, const char *field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        //We are searching only the string symbol
        if(ret->value_type == BSON_TYPE_INT32)
        {
            return ret->value.v_int64;
        }
    }

    return -1;
}

double BSONUtils::getDouble(const bson_t *bson, const char *field_name)
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

#include <iostream>
char *BSONUtils::getArray(const bson_t *bson, const char *field_name, size_t& size)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_BINARY)
        {
            size = ret->value.v_binary.data_len;
            if( size > 0 && ret->value.v_binary.data != NULL)
            {
                char* data = new char[size];
                memcpy(data, ret->value.v_binary.data, size);
                return data;
            }
        }
    }

    return nullptr;
}

bool BSONUtils::getOID(const bson_t *bson, char* ret_oid)
{
    if( ret_oid == nullptr )
    {
        return false;
    }

    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, "_id") ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_OID)
        {
            bson_oid_to_string(&ret->value.v_oid, ret_oid);
            return true;
        }
    }

    return false;
}
