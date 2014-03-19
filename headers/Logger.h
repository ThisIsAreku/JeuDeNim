#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
namespace Logger
{
class Logger
{
    bool debug;
public:
    Logger()
    {
        debug = false;
    }

    void setDebug(bool debug)
    {
        this->debug = debug;
        if(debug)
            std::cerr << "Enabled" << std::endl;
    }

    bool isDebugEnabled()
    {
        return this->debug;
    }

    //void setTargetFile(std::string);

    template<typename T>
    const Logger &operator<<(const T &v) const
    {
        if(debug)
            std::cerr << v;
        return *this;
    };
    std::ostream &operator<<(std::ostream & (*f)(std::ostream &)) const
    {
        if(debug)
            return f(std::cerr);
        return std::cerr;
    }
};
extern Logger log;
}

#endif