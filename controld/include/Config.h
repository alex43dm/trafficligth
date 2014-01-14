#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <array>
#include <map>

#include <tinyxml.h>

#include "Phase.h"
#include "Program.h"
#include "CronDay.h"

class Config
{
public:
    std::string portMega;
    std::string speedMega;
    std::string portLeds;
    std::string speedLeds;
    std::string cycle;
    std::string addrFrom;
    std::string addrTo;
    std::string tcpPort;
    std::vector<Phase*> Phases;
    std::vector<Program*> Programs;
    std::array<CronDay*, 7> *cDays;
    std::map<int,unsigned short> masks;
    unsigned short timer_val;
    int ledsLen;
    bool debug;
    std::string debug_file;
    std::string asUser;
    std::string lockFileName;
    std::string pidFileName;

    bool cycleRun(){ return cycle.size() == 0 ? false:  true; }

    static Config* Instance();
    bool LoadConfig(const std::string &fName);
    virtual ~Config();
    bool LoadPhases();
    bool LoadPrograms();
    bool LoadCron();

protected:
private:
    static Config* mInstance;
    Config();
	bool mIsInited;
    std::string mFileName;
    TiXmlDocument *mDoc;
    TiXmlElement* mRoot;
    TiXmlElement* mElem;
};

extern Config *cfg;

#endif // CONFIG_H
