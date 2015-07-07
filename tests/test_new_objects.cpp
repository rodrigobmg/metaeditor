#include <gtest/gtest.h>
#include <iostream>
#include <map>

//Core Lib imports

//Redis
#include <redox.hpp>

namespace Talisman
{
    /**
     * @brief The DataAccess class
     */
    class DataAccess
    {
    public:
        DataAccess() : _objCounter(0)
        {
            //ToDo: Mover esse código para um método específico
            if( connect() )
            {
                try
                {
                    _objCounter = std::stoi(_rdx.get("objCounter"));
                }
                catch(std::exception& ex)
                {
                    //Essa exceção só deve ser levantada na primeira execução
                    // do programa.
                    //FAIL() <<  ex.what();
                    std::cout << ex.what() << std::endl;
                    _rdx.set("objCounter", std::to_string(_objCounter));
                }
            }
        }

        /**
         * @brief createObject Função para persistência de objetos no banco de dados
         * @param type Tipo do objeto @see{TALIS_TYPE}
         * @param value Valor do objeto sendo inserido
         * @param objId número do objeto criado
         * @return Verdadeiro se conseguir criar o objeto e o número do objeto na variável @param{objId}
         */
        bool createObject(const std::string& type, const std::string& value, u_int64_t& objId)
        {
            //O id é uma string no formato TIPO_NumObjeto
            std::string id = type;
            objId = _objCounter++;
            id = id.append("_").append(std::to_string(objId));
            try
            {
                _rdx.set(id, value);
                return true;
            }
            catch(std::exception& ex)
            {
                std::cout << ex.what() << std::endl;
            }

            return false;
        }

        std::vector<std::string> getObjects();
        std::string getObject(u_int64_t id);

    private:
        bool connect(const std::string& host = "localhost", u_int16_t port_number = 6379)
        {
            if( _rdx.connect(host, port_number) == false )
            {
                return false;
            }

            return true;
        }

    private:
        /**
         * @brief Interface de acesso
         */
        redox::Redox _rdx;
        u_int64_t _objCounter;
    };

//    enum PrimitiveType : u_int8_t
//    {
//        NONE,
//        INTEGER,
//        STRING
//    };

//    enum TalismanType : u_int8_t
//    {
//        TNONE,
//        TEXT,
//        TITLE,
//        ALIAS,
//        RELATION
//    };

    /**
     * @brief The Attribute class
     * Representa a menor granulidade de um objeto
     * Exemplo, um campo qualquer dentro de um hyper objeto.
     */
    class Attribute
    {
    public:
        void setId(u_int64_t id)
        {
            this->_id = id;
        }

        const u_int64_t getId() const
        {
            return this->_id;
        }

    private:
        u_int64_t _id;
    };

    /**
     * @brief The DataType class
     * Representa um típico específico dentro do talisman
     * Exemplo, Um nome de a ser referênciado dentro de um objeto
     */
    class DataType : public Attribute
    {
    public:
    private:
    };

    /**
     * @brief The Name class
     * Representa o nome de um objeto
     */
    class Name : public DataType
    {
    public:
    private:
    };

    /**
     * @brief The String class
     */
    class String : public DataType
    {
    public:
        bool isEmpty() { return _string.empty(); }
        const size_t size() { return _string.size(); }
        const char* data() { return _string.data(); }

    private:
        std::string _string;
    };

    /**
     * @brief The SimpleText class
     */
    class SimpleText : public DataType
    {
    public:
    private:
    };

    /**
     * @brief The MarkupText class
     */
    class MarkupText : public DataType
    {
    public:
    private:
    };

    class HyperObject
    {
    public:
        bool insertAttribute(u_int32_t idAttr, Attribute& attr)
        {
            _attributes.insert(std::make_pair(idAttr, &attr));
            return true;
        }

        Attribute* attr(u_int32_t idAttr)
        {
            if(_attributes.empty() == false && _attributes.find(idAttr) != _attributes.end())
            {
                return _attributes.at(idAttr);
            }

            return nullptr;
        }

        const size_t attrCount() const
        {
            return _attributes.size();
        }

        std::string id()
        {
            return "";
        }

    private:
        std::string   _id;//Composição de dados
        std::map<u_int32_t, Attribute*> _attributes;
    };
}

TEST(HyperObject, Create)
{
    std::cout << "Iniciando testes - Create" << std::endl;

    Talisman::HyperObject obj;
    Talisman::String str;
    Talisman::MarkupText mkt;

    obj.insertAttribute(0, str);
    obj.insertAttribute(15, mkt);

    const size_t expected = 2;

    ASSERT_EQ(expected, obj.attrCount());
    Talisman::Attribute* attr = obj.attr(15);
    ASSERT_EQ(&mkt, attr);
    attr = obj.attr(0);
    ASSERT_EQ(&str, attr);

    std::cout << "Finalizando testes - Create" << std::endl;
}

int main(int argc, char** argv)
{
    //Roda os testes
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
