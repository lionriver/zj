//#define _ITERATOR_DEBUG_LEVEL 0

#ifndef _PositionImp_H_
#define _PositionImp_H_

#include "servant/Application.h"
#include "Position.h"

#include "Rpc/RpcEngine.h"
#include "ZTraderCommon/ZTraderApi.h"
#include "ZTraderCommon/ZTraderDataType.h"
#include "Adapter/Adapter.h"
#include "Adapter/AdapterFactory.h"
#include "Adapter/AdapterStruct.h"

#include "rxcpp/rx.hpp"
#include "rxcpp/rx-subjects.hpp"
#include <rxcpp/rx-observable.hpp>

//namespace Rx {
//using namespace rxcpp;
////using namespace rxcpp::sources;
////using namespace rxcpp::subjects;
////using namespace rxcpp::operators;
////using namespace rxcpp::rxsub;
////using namespace rxcpp::util;
//}
//using namespace Rx;

using namespace std;
using ZTraderApiPtr = std::shared_ptr<ZTrader::ZTraderApi>;

/**
 *
 *
 */
class PositionImp : public ZTraderProxy::Position
{
public:
    /**
     *
     */
    virtual ~PositionImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current);

    // Observable 方法
    rxcpp::observable<std::shared_ptr<int>> observeUserLogin();
    rxcpp::observable<std::shared_ptr<MY::Base::Adapter::AccountQryRsp>> observeTradingAccount();
    rxcpp::observable<std::shared_ptr<MY::Base::Adapter::PositionQryRsp>> observeInvestorPosition();

private:
    MY::Base::Rpc::RpcEnginePtr g_pApiEngine = nullptr;
    MY::Base::Adapter::AdapterFactory  m_Factory;
    ZTraderApiPtr g_pApi = nullptr;
    //ZTrader::ZTraderApi *g_pApi = nullptr;

    // RxCpp 的 Subject
    rxcpp::subjects::subject<std::shared_ptr<int>> subjectUserLogin;
    rxcpp::subjects::subject<std::shared_ptr<MY::Base::Adapter::AccountQryRsp>> subjectTradingAccount;
    rxcpp::subjects::subject<std::shared_ptr<MY::Base::Adapter::PositionQryRsp>> subjectInvestorPosition;

    int initializeZTraderApi();
    void logIn(const std::string& user, const std::string& password);
    //查询基金组合所有持仓和净值
	double qryPflNav(const std::string& pid);
    //柜台的回调函数
    void onEvent_NetConnected(const string& localAddr, const string& remoteAddr);
    void onEvent_NetDisconnected(const string& localAddr, const string& remoteAddr, int reason);
    void OnRspUserLogin(MY::Base::Adapter::UserLoginRsp *pRspUserLogin, MY::Base::Adapter::RspInfo *pRspInfo);
    void OnRspQryTradingAccount(MY::Base::Adapter::AccountQryRsp *pTradingAccount, MY::Base::Adapter::RspInfo *pRspInfo, bool bIsLast);
    void OnRspQryInvestorPosition(MY::Base::Adapter::PositionQryRsp *pInvestorPosition, MY::Base::Adapter::RspInfo *pRspInfo, bool bIsLast);
};

// // 声明全局变量
// extern ZTraderApiPtr g_pApi;
// extern MY::Base::Rpc::RpcEnginePtr g_pApiEngine;
/////////////////////////////////////////////////////
#endif
