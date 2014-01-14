#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "Way.h"
#include <assert.h>

// Global static pointer used to ensure a single instance of the class.
Config* Config::mInstance = NULL;

Config* Config::Instance()
{
    if (!mInstance)   // Only allow one instance of class to be generated.
        mInstance = new Config();

    return mInstance;
}

Config::Config()
{
    mIsInited = false;
//    masks = new unsigned short(4);
    mDoc = NULL;
}

bool Config::LoadConfig(const std::string &fName)
{
    TiXmlElement* el;
    mIsInited = false;
    mFileName = fName;

    mDoc = new TiXmlDocument(mFileName);

    if(!mDoc)
    {
        std::cerr<<"does not found config file: "<<fName<<std::endl;
        exit(-1);
    }

    if(!mDoc->LoadFile())
    {
        std::cerr<<"error load file: "<<fName<<std::endl;
        exit(-1);
    }

    mRoot = mDoc->FirstChildElement("root");

    if(!mRoot)
    {
        std::cerr<<"does not found root section in file: "<<fName<<std::endl;
        exit(-1);
    }

    if( (mElem = mRoot->FirstChildElement("mega")) )
    {
        if( (el = mElem->FirstChildElement("port")) )
        {
            portMega = el->GetText();
        }

        if( (el = mElem->FirstChildElement("speed")) )
        {
            speedMega = el->GetText();
        }
    }

    if( (mElem = mRoot->FirstChildElement("leds")) )
    {
        if( (el = mElem->FirstChildElement("port")) )
        {
            portLeds = el->GetText();
        }

        if( (el = mElem->FirstChildElement("speed")) )
        {
            speedLeds = el->GetText();
        }
    }

    if( (mElem = mRoot->FirstChildElement("tvp")) )
    {
        for(mElem = mElem->FirstChildElement("mask"); mElem; mElem = mElem->NextSiblingElement())
        {
            //masks[atoi(mElem->Attribute("id"))] = (unsigned short)strtoul(mElem->GetText(), NULL, 0);
            masks.insert(std::pair<int, unsigned short>(atoi(mElem->Attribute("id")), (unsigned short)strtoul(mElem->GetText(), NULL, 0)));
        }
    }

    if( (mElem = mRoot->FirstChildElement("cycle")) )
    {
        cycle = mElem->GetText();
    }

    if( (mElem = mRoot->FirstChildElement("addrFrom")) )
    {
        addrFrom = mElem->GetText();
    }

    if( (mElem = mRoot->FirstChildElement("addrTo")) )
    {
        addrTo = mElem->GetText();
    }

    if( (mElem = mRoot->FirstChildElement("tcpPort")) )
    {
        tcpPort = mElem->GetText();
    }

    if( (mElem = mRoot->FirstChildElement("timer")) )
    {
        timer_val = atoi(mElem->GetText());
    }
    if( (mElem = mRoot->FirstChildElement("user")) )
    {
        asUser = mElem->GetText();
    }

    if( (mElem = mRoot->FirstChildElement("debug")) )
    {
        if(strncmp(mElem->GetText(),"true",4)==0)
          debug = true;
        else
          debug = false;
    }

    if( (mElem = mRoot->FirstChildElement("debug_file")) )
    {
        debug_file = mElem->GetText();
    }


    delete mDoc;

    if(cycle.size() == 0)
    {
        mIsInited = true;
        return mIsInited;
    }

    mDoc = new TiXmlDocument(cycle);

    if(!mDoc)
    {
        std::cerr<<"does not found config file: "<<fName<<std::endl;
        return mIsInited;
    }

    if(!mDoc->LoadFile())
    {
        std::cerr<<"error load file: "<<fName<<std::endl;
        return mIsInited;
    }

    mRoot = mDoc->FirstChildElement("root");

    if(!mRoot)
    {
        std::cerr<<"does not found root section in file: "<<fName<<std::endl;
        return mIsInited;
    }

    LoadPhases();
    LoadPrograms();
    LoadCron();

    mIsInited = true;
    return mIsInited;
}

//---------------------------------------------------------------------------------------------------------------
Config::~Config()
{
    if (mDoc != NULL)
    {
        delete mDoc;
        mDoc = NULL;
    }
//    if(masks)
//        delete [] masks;
    mInstance = NULL;
}
//---------------------------------------------------------------------------------------------------------------
bool Config::LoadPhases()
{

    mElem = mRoot->FirstChildElement("phases");

    if(!mElem)
    {
        return false;
    }

    mElem = mElem->FirstChildElement("phase");

    if(!mElem)
    {
        return false;
    }

    for(; mElem; mElem = mElem->NextSiblingElement())
    {

        const char *id = mElem->Attribute("id");
        Phase * c = new Phase();
        c->Id = atoi(id);

        for(const TiXmlElement* el = mElem->FirstChildElement(); el; el = el->NextSiblingElement())
        {
            if(!el)
                continue;


            if(el->ValueStr() == "Tmin")
            {
                c->Tmin = atoi(el->GetText());
            }
            else if(el->ValueStr() == "Tmax")
            {
                c->Tmax = atoi(el->GetText());
            }
            else if(el->ValueStr() == "tvp")
            {
                c->TVP = atoi(el->GetText());
            }
            else if(el->ValueStr() == "promtakt")
            {
                c->PromPulse = atoi(el->GetText());
            }
            else if(el->ValueStr() == "road")
            {
                for(const TiXmlElement* ell = el->FirstChildElement(); ell; ell = ell->NextSiblingElement())
                {
                    Way *w = new Way();
                    w->id = atoi(ell->Attribute("id"));

                    if( strncmp(ell->Attribute("type"), "P", 1) == 0 )
                    {
                        w->type =  WayTypes::People;
                    }
                    else if( strncmp(ell->Attribute("type"), "T", 1) == 0 )
                    {
                        w->type =  WayTypes::Tranport;
                    }

                    w->id = atoi(ell->Attribute("id"));

                    w->state = atoi(ell->GetText()) == 0 ? off : on;

                    c->Ways.push_back(w);
                }
                ledsLen = c->Ways.size();
            }
        }

        Phases.push_back(c);
    }
    return true;
}

bool Config::LoadPrograms()
{
    mElem = mRoot->FirstChildElement("programs");

    if(!mElem)
    {
        return false;
    }

    mElem = mElem->FirstChildElement("program");

    if(!mElem)
    {
        return false;
    }

    for(; mElem; mElem = mElem->NextSiblingElement())
    {

        Program * c = new Program();

        const char *id = mElem->Attribute("id");
        c->Id = atoi(id);

        id = mElem->Attribute("tvp");
        if(id)
        {
            c->tvpId = atoi(id);
        }

        for(const TiXmlElement* el = mElem->FirstChildElement(); el; el = el->NextSiblingElement())
        {
            if(!el)
                continue;

            id = el->Attribute("id");
            Phase *ph = Phases[atoi(id)-1];
            c->addPhase(ph, atoi(el->GetText()));
        }
        Programs.push_back(c);
    }
    return true;
}

bool Config::LoadCron()
{
    cDays = new std::array<CronDay*,7>();

    mElem = mRoot->FirstChildElement("cron");

    if(!mElem)
    {
        return false;
    }

    mElem = mElem->FirstChildElement("day");

    if(!mElem)
    {
        return false;
    }

    for(; mElem; mElem = mElem->NextSiblingElement())
    {

        const char *id = mElem->Attribute("id");

        CronDay *cd = new CronDay();

        for(const TiXmlElement* el = mElem->FirstChildElement(); el; el = el->NextSiblingElement())
        {
            if(!el)
                continue;
            CronProgram * c = new CronProgram();
            c->Id = atoi(el->Attribute("id"));
            c->Offset = atoi(el->Attribute("offset"));
            const char *u = el->Attribute("startTime");
            c->StarTime.tm_hour = atoi(u);
            c->StarTime.tm_min = atoi(u+3);

            cd->cPrograms->push_back(c);
        }
        //cd->Sort();
        (*cDays)[atoi(id)] = cd;
    }
    return true;
}
