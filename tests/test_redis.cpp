#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <list>

//Bson
#include <redox.hpp>


TEST(redis, vector)
{
    /*std::vector<int> dummy(12, -1);

    auto insert = [](std::vector<int>& v, int pos)
    {
        static int i = 0;
        v.insert(v.begin() + pos, i++);
    };


    for( int i = 0; i < 10; i++ )
    {
        insert(dummy, rand()%10);
    }

    for( auto x : dummy )
    {
        std::cout << x << std::endl;
    }

    dummy.insert(dummy.begin(), 100);

    std::cout << "after 100 inserted" << std::endl;

    for( auto x : dummy )
    {
        std::cout << x << std::endl;
    }*/

    std::vector<std::string> dummy;

    dummy.push_back(" Wonderful");
    dummy.push_back(" World");
    dummy.insert(dummy.begin(), "Hello, Brasil! ");

    for( auto s : dummy )
    {
        std::cout << s;
    }

    std::cout << std::endl;
}

/*TEST(redis, connect)
{
    redox::Redox rdx;

    if( !rdx.connect("localhost", 6379) )
    {
        FAIL() << "Can't connect";
    }

    std::cout << "foo: " << rdx.get("foo") << std::endl;

    rdx.disconnect();
}*/


//Formato da chave

/*TEST(redis, objects)
{
    redox::Redox rdx;

    if( !rdx.connect("localhost", 6379) )
    {
        FAIL() << "Can't connect";
    }

    rdx.set("testeNumber", std::to_string(1));


    ASSERT_EQ(std::stoi(rdx.get("testeNumber")), 1);

    rdx.disconnect();
}*/

/*TEST(redis, sets)
{
    redox::Redox rdx;

    if( !rdx.connect("localhost", 6379) )
    {
        FAIL() << "can't connect";
    }

    redox::Command<std::string>& c = rdx.commandSync<std::string>({"ZADD", "TestSet", "2", "value2"});

    if( c.ok() )
    {
        std::cout << c.cmd() << " : " << c.reply() << std::endl;
    }
}*/

std::list<std::string> &split(const std::string &s, char delim, std::list<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::list<std::string> split(const std::string &s, char delim) {
    std::list<std::string> elems;
    split(s, delim, elems);
    return elems;
}

/*TEST(redis, concat)
{
    redox::Redox rdx;

    if( !rdx.connect("localhost", 6379) )
    {
        FAIL() << "can't connect";
    }

    if( rdx.set("rel1", "id1:id2:id3:id4:id5:id6") == true )
    {
        std::string rels = rdx.get("rel1");
        std::list<std::string> splits = split(rels, ':');
        for(auto s : splits)
        {
            std::cout << s << std::endl;
        }
    }
}*/

/*TEST(redis, insertrel)
{
    redox::Redox rdx;

    if( !rdx.connect("localhost", 6379) )
    {
        FAIL() << "can't connect";
    }

    std::string rels = rdx.get("rel1");

    if( rels.empty() == false )
    {
        redox::Command<std::string>& c = rdx.commandSync<std::string>({"APPEND", "rel1", ":id7"});

        if( c.ok() )
        {
            std::cout << c.cmd() << " : " << c.reply() << std::endl;
        }
    }
}*/

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
