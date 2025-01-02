/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include <iostream>
#include "servant/Communicator.h"
#include "Position.h"
#include "util/tc_option.h"

using namespace std;
using namespace tars;
using namespace ZTraderProxy;

Communicator* _comm;

static string positionObj = "ZTraderProxy.ZTraderProxyServer.PositionObj@tcp -h 192.168.1.9 -p 19534 -t 30000";
//static string positionObj = "ZTraderApp.PositionServer.PositionObj@tcp -h 127.0.0.1 -p 8199:tcp -h 127.0.0.1 -p 8299 -t 10000";

int main(int argc, char *argv[])
{
    try
    {
        _comm = new Communicator();
        //_comm->setProperty("sendqueuelimit", "1000000");
        //_comm->setProperty("asyncqueuecap", "1000000");
		PositionPrx pPrx = _comm->stringToProxy<PositionPrx>(positionObj);
        //设置该代理的同步的调用超时时间 单位毫秒
        pPrx->tars_timeout(30000);
        //设置该代理的异步的调用超时时间 单位毫秒
        pPrx->tars_async_timeout(60000);
		int rpcPosition=pPrx->test();
		cout << "rpc position returns: " << rpcPosition << endl;
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
    }
    cout << "main return." << endl;

    return 0;
}
