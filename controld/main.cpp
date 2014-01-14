#include "Process.h"
#include "Server.h"
#include "Log.h"
#include "Config.h"

Config *cfg;

int main()
{
    std::string config = "/etc/trafficlight/controld/config.xml";
    //std::string config = "/home/alex/Projects/trafficligth/controld/config.xml";

    //open syslog
    Log(LOG_LOCAL0);

    cfg = Config::Instance();
    cfg->LoadConfig(config);

    Server();

    return Process().run();
}
