#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>

namespace robot
{


struct RobotConfig
{
    PIDConfig PID;
    std::string address;

};

}
#endif                                                      // CONFIGURATION_HPP
