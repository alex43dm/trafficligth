#include "Server.h"
#include "Log.h"
#include "Config.h"

Config *cfg;

int main()
{
    std::string config = "/etc/trafficlight/dio/config.xml";
    //std::string config = "/home/alex/Projects/trafficligth/dio/config.xml";

    //open syslog
    Log(LOG_LOCAL0);

    cfg = Config::Instance();
    cfg->LoadConfig(config);

#ifndef DEBUG
    new Server("/var/run/dio.lock", "/var/run/dio.pid");
#endif

    return cfg->dio->loop();
}
