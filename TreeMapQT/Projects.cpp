#include "Projects.h"
#include <tuple>

namespace global_namespace
{

QStringList TProject::getUsers()
{
    QStringList result;
    auto len = RootArea()->GetParameterMapSize("users");
    for (decltype(len) i = 0; i<len; i++)
    {
        auto name = RootArea()->GetParameterMapValueByIndex<std::string, std::string>("users", i);
        result.push_back(std::get<1>(name).c_str());
    }
    return result;
}

}
