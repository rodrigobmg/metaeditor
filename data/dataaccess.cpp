#include "dataaccess.h"
#include <vector>
#include <redox/command.hpp>

namespace Data {

RedisWrapper::RedisWrapper()
    : _connected(false)
{
}

RedisWrapper::~RedisWrapper()
{
    if( _connected == true )
    {
        _rdx.disconnect();
    }
}

bool RedisWrapper::Connect(const std::string &host, const u_int16_t port)
{
    if( host.empty() )
    {
        _connected = false;
        return false;
    }

    if( _rdx.connect(host, port) == true )
    {
        _connected = true;
        std::cout << "Conexão estabelecida com sucesso." << std::endl;
    }

    return true;
}

bool RedisWrapper::Insert(const std::string &key, const std::string &value)
{
    try
    {
        if( _connected )
        {
            return _rdx.set(key, value);
        }
    }
    catch(std::exception& ex)
    {
        std::cout << "Erro ao inserir dados. " << ex.what() << std::endl;
        return false;
    }
}

bool RedisWrapper::Remove(const std::string &key)
{
    try
    {
        if( _connected )
        {
            return _rdx.del(key);
        }
    }
    catch(std::exception& ex)
    {
        std::cout << "Erro ao remover dados. " << ex.what() << std::endl;
        return false;
    }
}

bool RedisWrapper::CreateSet(const std::string &key)
{
    std::vector<std::string> values = { key };
    return Command(CommandZAdd, values);
}

bool RedisWrapper::InsertSet(const std::string &key, const std::string &value)
{
    return true;
}

bool RedisWrapper::RemoveSet(const std::string &key, CASCADE cascade)
{
    return true;
}

bool RedisWrapper::IsConnected() const
{
    return _connected;
}

bool RedisWrapper::Command(const std::string &command, std::vector<std::string>& values)
{
    try
    {
        values.insert(values.begin(), command);
        redox::Command<std::string>& command = _rdx.commandSync<std::string>(values);

        if(command.ok())
        {
            std::cout << "Comando executado com sucesso. " << command.cmd() << std::endl;
            return true;
        }
        else
        {
            switch(command.status())
            {
            case -1:
                std::cout << "Resposta não obtida para o comando executado." << std::endl;
                break;
            case 1:
                std::cout << "Resposta nula para o comando executado." << std::endl;
                break;
            case 2:
                std::cout << "Erro obtido ao executar comando." << std::endl;
                break;
            case 3:
                std::cout << "Falha ao enviar comando para o servidor." << std::endl;
                _connected = false;
                break;
            case 4:
                std::cout << "Resposta não esperado para o comando executado." << std::endl;
                break;
            case 5:
                std::cout << "Não foi possível obter resposta." << std::endl;
                break;
            }

            return false;
        }
    }
    catch(std::exception& ex)
    {
        std::cout << "Erro ao executar comando. " << ex.what() << std::endl;
        return false;
    }
}

}










































