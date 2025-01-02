//#define _ITERATOR_DEBUG_LEVEL 0

#ifndef _ZTraderProxyServer_H_
#define _ZTraderProxyServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;
/**
 *
 **/
class ZTraderProxyServer : public Application
{
public:
    /**
     *
     **/
    virtual ~ZTraderProxyServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern ZTraderProxyServer g_app;

////////////////////////////////////////////
#endif
