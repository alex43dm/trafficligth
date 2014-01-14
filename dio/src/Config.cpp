#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>

#include "Config.h"
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
    mDoc = NULL;
}

bool Config::LoadConfig(const std::string fName)
{
    mIsInited = false;
    mFileName = fName;
    mDoc = new TiXmlDocument(mFileName);


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

    dio = new Dio();

    mElem = mRoot->FirstChildElement("spi");

    if(!mElem)
    {
        return false;
    }

    for(const TiXmlElement* el = mElem->FirstChildElement(); el; el = el->NextSiblingElement())
    {
        if(!el)
            continue;

        if(el->ValueStr() == "dev")
        {
            dio->spiDev = el->GetText();
        }
        else if(el->ValueStr() == "mode")
        {
            dio->spiMode = el->GetText();
        }
        else if(el->ValueStr() == "speed")
        {
            dio->spiSpeed = el->GetText();
        }
        else if(el->ValueStr() == "bitsPerWord")
        {
            dio->spiBitsPerWord = el->GetText();
        }
        else if(el->ValueStr() == "period")
        {
            dio->period = el->GetText();
        }
    }

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

    mInstance = NULL;
}

