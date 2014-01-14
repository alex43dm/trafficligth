#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include <tinyxml.h>

#include "Dio.h"

class Config
{
public:
    Dio *dio;

    static Config* Instance();
    bool LoadConfig(const std::string fName);
    virtual ~Config();
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

#endif // CONFIG_H
