// #define _ITERATOR_DEBUG_LEVEL 0

#include "PositionImp.h"
#include "servant/Application.h"

#include <iostream>
#include <future>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include "Common/Utc.h"
#include "Common/Log.h"
#include "Common/Utils.h"
#include "Common/XmlConfig.h"
#include "Common/CommonLibDefines.h"
#include "Rpc/RpcEngine.h"
#include "ZTraderCommon/ZTraderApi.h"

#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

// ZTraderApiPtr g_pApi;
// MY::Base::Rpc::RpcEnginePtr g_pApiEngine;

//////////////////////////////////////////////////////
void PositionImp::initialize()
{
    // initialize servant here:
    // cout << ">> initialize: begin" << endl;
    // cout << ">> initialize: end" << endl;
}
//////////////////////////////////////////////////////
int PositionImp::initializeZTraderApi()
{
    int cnt = 0;

    cout << ">> initializeZTraderApi begin" << endl;
    try
    {
        // Initializer initializer{};
        g_pApiEngine = MY::Base::Rpc::RpcEnginePtr(new MY::Base::Rpc::RpcEngine());
        g_pApiEngine->init(1, 1);
        g_pApi = ZTraderApiPtr(new ZTrader::ZTraderApi(g_pApiEngine, "127.0.0.1", 5988));
        cout << ">> g_pApi = " << g_pApi << endl;
        // try
        // {
        //     // 手动分配资源
        //     g_pApi = new ZTrader::ZTraderApi(g_pApiEngine, "127.0.0.1", 5988);
        //     cout << ">> g_pApi = " << g_pApi << endl;
        // }
        // catch (const std::invalid_argument &e)
        // {
        //     std::cerr << "Invalid argument exception: " << e.what() << std::endl;
        // }
        // catch (const std::bad_alloc &e)
        // {
        //     std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        // }
        // catch (...)
        // {
        //     std::cerr << "Unknown exception occurred." << std::endl;
        // }
        // g_pApi = ZTraderApiPtr(rawPtr);
        // cout << ">> g_pApi = " << g_pApi << endl;

        std::promise<void> promiseConn;
        auto syncConn = promiseConn.get_future();
        g_pApi->onConnected.connect([&promiseConn]()
                                    {
          cout << "connected\n";
          promiseConn.set_value(); });
        if (!g_pApi->init() || !g_pApi->start())
        {
            cout << "failed to init or start" << endl;
            cout << "initializeZTraderApi: end" << endl;
            return 1;

            syncConn.wait();
            g_pApi->onConnected.disconnect_all_slots();
            g_pApi->onConnected.connect([]()
                                        { cout << "connected\n"; });
            g_pApi->onDisconnected.connect([]()
                                           { cout << "disconnected\n"; });
        }
    }
    catch (std::exception &e)
    {
        cerr << "initializeZTraderApi: std::exception:" << e.what() << std::endl;
        cout << "initializeZTraderApi: failed to connect to ZTrader." << endl;
        cout << "initializeZTraderApi: end" << endl;
        return -1;
    }
    catch (...)
    {
        cerr << "initializeZTraderApi: unknown exception." << std::endl;
        cout << "initializeZTraderApi: failed to connect to ZTrader." << endl;
        cout << "initializeZTraderApi: end" << endl;
        return -2;
    }
    cout << ">> initializeZTraderApi end" << endl;
    return 0;
}

void PositionImp::logIn(const std::string &user, const std::string &password)
{
    if (g_pApi == nullptr)
    {
        cout << ">> rawPtr is null" << endl;
        cout << ">> g_pApi = " << g_pApi << endl;
        return;
    }
    else
    {
        // cout << ">> rawPtr = " << rawPtr << endl;
        cout << ">> g_pApi = " << g_pApi << endl;
    }
    cout << ">> logIn begin" << endl;
    try
    {
        ZTrader::UserLogin ul;
        // ZTrader::UserPassword psw;
        strcpy(ul.up.userName, user.c_str());
        strcpy(ul.up.password, password.c_str());
        ::bson::bo bo;
        cout << ">> logIn 1" << endl;
        auto rsp = g_pApi->LogIn(ul, &bo);
        cout << ">> logIn 2" << endl;
        if (!bo.isEmpty())
        {
            cout << "login succ\n";
        }
        else
        {
            cout << "login fail\n";
        }
    }
    catch (std::exception &e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
        cout << "failed to connect to ZTrader." << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
        cout << "failed to connect to ZTrader." << endl;
    }
    cout << ">> logIn end" << endl;
}

double PositionImp::qryPflNav(const std::string &pid)
{
    if (g_pApi == nullptr)
    {
        cout << ">> rawPtr is null" << endl;
        cout << ">> g_pApi = " << g_pApi << endl;
        return -1;
    }
    else
    {
        // cout << ">> rawPtr = " << rawPtr << endl;
        cout << ">> g_pApi = " << g_pApi << endl;
    }
    cout << ">> qryPflNav begin" << endl;
    try
    {
        ZTrader::QryPflNAV qryNav;
        strcpy(qryNav.pid, pid.c_str());
        qryNav.assetType = ZTrader::AssetType::NA;
        strcpy(qryNav.pid, pid.c_str());
        bson::bo bo;
        cout << ">> qryPflNav 1" << endl;
        auto nav = g_pApi->QryPflNAV_Req(qryNav, &bo);
        cout << ">> qryPflNav 2" << endl;
        if (!bo.isEmpty())
        {
            char szErr[256] = {};
            sprintf(szErr, "%s\n", bo["error"].str().c_str());
            cerr << szErr << endl;
            return 0.0;
        }
        else
        {
            cout << "qryPflNav succ" << endl;
        }
        if (nav.size() < 1)
        {
            cout << "qryPflNav fail" << endl;
            return 0.0;
        }
        cout << ">> qryPflNav 3" << endl;
        double sumNav = 0.0;
        for (auto it = nav.begin(); it != nav.end(); ++it)
        {
            sumNav += it->Nav;
        }
        cout << ">> qryPflNav 4" << endl;
        cout << ">> qryPflNav end" << endl;
        return sumNav;
    }
    catch (std::exception &e)
    {
        cerr << "qryPflNav: std::exception:" << e.what() << std::endl;
        cout << "qryPflNav: failed to connect to ZTrader." << endl;
        cout << "qryPflNav: end" << endl;
        return -1;
    }
    catch (...)
    {
        cerr << "qryPflNav: unknown exception." << std::endl;
        cout << "qryPflNav: failed to connect to ZTrader." << endl;
        cout << "qryPflNav: end" << endl;
        return -2;
    }
    return 0.0;
}

//////////////////////////////////////////////////////
void PositionImp::onEvent_NetConnected(const string &localAddr, const string &remoteAddr)
{
    cout << ">> <onEvent_NetConnected> adapter connected" << endl;

    // char szErr[256] = {};
    // sprintf(szErr, "柜台[%s]Connected", m_strAdapterName.c_str());
    // LOG_INFO(szErr);

    // int64_t timestamp = MY::Base::Common::UTC().toBeiJing2(); //时分秒
    // int nDay = timestamp / 1000000000;
    // int nTime = timestamp % 1000000000;
    // char szErr2[2560] = {};
    // sprintf(szErr2, R"({"%s":{"name":"%s","date":%d,"time":%d,"local":"%s", "remote":"%s", "status":1,"login":0}})",
    //     m_strAdapterName.c_str(), m_strAdapterName.c_str(), nDay, nTime, localAddr.c_str(), remoteAddr.c_str());
    // m_Admin.GetMonitorInfo().send("counter", szErr2);
}
void PositionImp::onEvent_NetDisconnected(const string &localAddr, const string &remoteAddr, int reason)
{
    cout << "adapter disconnected" << endl;
    // ZTrader::AdapterOnline aol{};
    // strcpy(aol.AdapterID, m_strAdapterName.c_str());
    // aol.IsOnline = false;
    // m_Admin.GetAPIServerPtr()->SendData(aol);
    // char szErr[256] = {};
    // sprintf(szErr, "柜台[%s]Disconnected", m_strAdapterName.c_str());
    // LOG_INFO(szErr);

    // int64_t timestamp = MY::Base::Common::UTC().toBeiJing2(); //时分秒
    // int nDay = timestamp / 1000000000;
    // int nTime = timestamp % 1000000000;
    // char szErr2[2560] = {};
    // sprintf(szErr2, R"({"%s":{"name":"%s","date":%d,"time":%d,"local":"%s","remote":"%s","status":0,"login":0,"remark":""}})",
    //     m_strAdapterName.c_str(), m_strAdapterName.c_str(), nDay, nTime, localAddr.c_str(), remoteAddr.c_str());
    // m_Admin.GetMonitorInfo().send("counter", szErr2);
}
void PositionImp::OnRspUserLogin(MY::Base::Adapter::UserLoginRsp *pRspUserLogin, MY::Base::Adapter::RspInfo *pRspInfo)
{
    cout << ">> <OnRspUserLogin> adapter login responsed" << endl;

    if (pRspInfo && pRspInfo->m_errorID != 0)
    {//登录失败       
        cout << ">> <OnRspUserLogin> adapter login failed" << endl;
    }
    else
    {//登录成功
        cout << ">> <OnRspUserLogin> adapter login success" << endl;
        std::shared_ptr<int> pRsp = std::make_shared<int>(1);
        subjectUserLogin.get_subscriber().on_next(pRsp);
        subjectUserLogin.get_subscriber().on_completed();
    }

    // if (pRspInfo && pRspInfo->m_errorID != 0)
    // {//登录失败
    //     LOG_INFO("柜台[%s]登录失败:%s,isonline %d", m_strAdapterName.c_str(), pRspInfo->m_errorMsg, m_tradeAdapter->isOnline());

    //     int64_t timestamp = MY::Base::Common::UTC().toBeiJing2(); //时分秒
    //     int nDay = timestamp / 1000000000;
    //     int nTime = timestamp % 1000000000;
    //     char szErr2[2560] = {};
    //     sprintf(szErr2, R"({"%s":{"name":"%s","date":%d,"time":%d,"local":"","remote":"","status":1,"login":0,"remark":""}})",
    //         m_strAdapterName.c_str(), m_strAdapterName.c_str(), nDay, nTime/*, pRspInfo->m_errorMsg*/);//暂时不输出remark，不规则字符串可能会导致jason解析失败
    //     m_Admin.GetMonitorInfo().send("counter", szErr2);
    // }
    // else
    // {//登录成功
    // 	LOG_INFO("柜台[%s]登录成功,isonline %d", m_strAdapterName.c_str(), m_tradeAdapter->isOnline());
    //     ZTrader::AdapterOnline aol{};
    //     strcpy(aol.AdapterID, m_strAdapterName.c_str());
    //     aol.IsOnline = true;
    //     m_Admin.GetAPIServerPtr()->SendData(aol);
    // 	if (m_tradeAdapter->isOnline())
    // 	{
    // 		std::list<std::string> vec;
    // 		Resync(true, true, vec);
    // 	}
    //     int64_t timestamp = MY::Base::Common::UTC().toBeiJing2(); //时分秒
    //     int nDay = timestamp / 1000000000;
    //     int nTime = timestamp % 1000000000;
    //     char szErr2[2560] = {};
    //     sprintf(szErr2, R"({"%s":{"name":"%s","date":%d,"time":%d,"local":"","remote":"","status":1,"login":1,"remark":""}})",
    //         m_strAdapterName.c_str(), m_strAdapterName.c_str(), nDay, nTime);
    //   //  sprintf(szErr2, R"({"name":"%s", "status":"2","date":%d,"time":%d,"address":"%s"})", m_strAdapterName.c_str(), nDay, nTime, m_tradeAdapter->getParams().m_tradeAddrs.c_str());
    //     m_Admin.GetMonitorInfo().send("counter", szErr2);
    // }
}
void PositionImp::OnRspQryTradingAccount(MY::Base::Adapter::AccountQryRsp *pTradingAccount, MY::Base::Adapter::RspInfo *pRspInfo, bool bIsLast)
{
    if (pTradingAccount)
    {
        cout << ">> <OnRspQryTradingAccount> AccountQryRsp = " << pTradingAccount->ToString() << ", bIsLast = " << bIsLast << endl;
        subjectTradingAccount.get_subscriber().on_next(std::make_shared<MY::Base::Adapter::AccountQryRsp>(*pTradingAccount));
    }
    if (bIsLast)
    {
        subjectTradingAccount.get_subscriber().on_completed();
    }

    // LOG_DEBUG("LXD-DEBUG OnRspQryTradingAccount called");
    // {
    // 	boost::recursive_mutex::scoped_lock l(m_NavMutex);
    // 	if (pTradingAccount)
    // 	{
    //         LOG_DEBUG("LXD-DEBUG OnRspQryTradingAccount AccountQryRsp=%s, bIsLast=%d>", pTradingAccount->ToString().c_str(), bIsLast);
    // 		LOG_INFO("AccountQryRsp:%s", pTradingAccount->ToString().c_str());
    // 		m_rawNavList.insert(m_rawNavList.end(), *pTradingAccount);
    // 	}
    // 	if (bIsLast)
    // 	{
    // 		m_mapNav.clear();
    // 		for (const auto& data : m_rawNavList)
    // 		{
    // 			if (m_tradeAdapter->getParams().m_type == "IASIA")
    // 			{//国元香港目前只要来买A股，只用到里面的人民币资产。
    // 				if (data.m_accountInfo.m_currency != MY::Base::Adapter::Currency::CNY)
    // 					continue;//iasia目前只取人民币
    // 			}
    // 			PflNav pflNav{};
    // 			strcpy(pflNav.pid, m_strAdapterName.c_str());
    // 			pflNav.tradeDate = static_cast<int32_t>(MY::Base::Common::UTC().toBeiJing2() / 1000000000);
    // 			pflNav.PrevNav = data.m_accountInfo.m_preBalance;
    // 			pflNav.Nav = data.m_accountInfo.m_dynamicRights;
    //             pflNav.TotalCash = data.m_accountInfo.m_balance;//可用+冻结
    // 			pflNav.AvailableCash = data.m_accountInfo.m_available;
    // 			pflNav.OccupiedCash = data.m_accountInfo.m_margin;
    // 			pflNav.FrozenCash = data.m_accountInfo.m_frozenMoney; // 挂单冻结的资金
    //             pflNav.BondValue = data.m_accountInfo.m_reverseRepoMV;//逆回购资产,暂时赋值进债券字段中.
    // 			pflNav.debt = data.m_accountInfo.m_debt;
    // 			pflNav.financing = data.m_accountInfo.m_marginTradeEnableQuota;
    // 			pflNav.security = data.m_accountInfo.m_shortSellEnableQuota;
    // 			pflNav.sellSecurityCash = data.m_accountInfo.m_shortSellBalance;
    // 			pflNav.financinginterest = data.m_accountInfo.m_marginTradeInterest;
    // 			pflNav.securityinterest = data.m_accountInfo.m_shortSellInterest;
    // 			LOG_INFO("资金信息：%s", pflNav.ToString().c_str());
    //             if (data.m_accountInfo.m_tmp11)
    //             {//如果是主账户资金.则其他账户的都去掉，兼容OMS的资金
    //                 m_mapNav.clear();
    //                 m_mapNav[data.m_accountInfo.m_currency] = pflNav;
    //                 LOG_DEBUG("LXD-DEBUG OnRspQryTradingAccount m_mapNav: <m_currency=%s, pflNav=%s>", MY::Base::Adapter::convertToString(data.m_accountInfo.m_currency), pflNav.ToString().c_str());
    //                 break;
    //             }
    //             else{
    //                 m_mapNav[data.m_accountInfo.m_currency] = pflNav;
    //                 LOG_DEBUG("LXD-DEBUG OnRspQryTradingAccount m_mapNav: <m_currency=%s, pflNav=%s>", MY::Base::Adapter::convertToString(data.m_accountInfo.m_currency), pflNav.ToString().c_str());
    //             }
    // 		}
    //         for (auto& nav : m_mapNav)
    //         {
    //             LOG_INFO("OnRspQryTradingAccount:adpater[%s], currency[%s], Nav[%s]", m_strAdapterName.c_str(), MY::Base::Adapter::convertToString(nav.first), nav.second.ToString().c_str());
    //         }
    // 		m_rawNavListEnd.clear();
    // 		m_rawNavListEnd = m_rawNavList;
    // 	}
    // }

    // if (bIsLast)
    // {
    // 	if (m_tradeAdapter->getParams().m_AccountType != 3)
    // 	{//非融资融券账户的处理方式
    //         boost::this_thread::sleep_for(boost::chrono::seconds(5));
    // 		boost::recursive_mutex::scoped_lock l(m_PosMutex);
    // 		m_positions.clear();
    // 		m_tmpPositions.clear();
    // 		LOG_INFO("{%s}]查询持仓", m_strAdapterName.c_str());
    // 		m_posFinish = false;
    // 		MY::Base::Adapter::PositionQryReq pQryPosition{};
    // 		int ret = m_tradeAdapter->reqQryPosition(&pQryPosition, 0);
    // 		if (ret != 0)
    // 		{
    // 			LOG_INFO("reqQryPosition returns {%d}", ret);
    // 			m_qrySuccess = true;
    // 			m_prevSuccessReloadTS = boost::chrono::steady_clock::now();
    // 		}
    // 	}
    // 	else
    // 	{
    // 		m_posDetailList.clear();
    // 		m_positionsTemp.clear();
    // 		//融资融券账户需要查询历史净持仓 和 今日成交明细算出最后的持仓
    // 		MY::Base::Adapter::PositionDetailsQryReq req{};
    // 		req.m_endDay   = 0;
    // 		req.m_startDay = 0;
    // 		req.m_rowCount = 1000;
    // 		int ret = m_tradeAdapter->reqQryPositionDetails(&req, 0);
    // 		if (ret != 0)
    // 		{
    // 			LOG_INFO("reqQryPositionDetails returns {%d}", ret);
    // 			m_qrySuccess = true;
    // 			m_prevSuccessReloadTS = boost::chrono::steady_clock::now();
    // 		}
    // 	}
    // }
}
void PositionImp::OnRspQryInvestorPosition(MY::Base::Adapter::PositionQryRsp *pInvestorPosition, MY::Base::Adapter::RspInfo *pRspInfo, bool bIsLast)
{
    if (pInvestorPosition != NULL)
    {
        cout << ">> <OnRspQryInvestorPosition> Position = " << pInvestorPosition->ToString() << ", bIsLast = " << bIsLast << endl;
        subjectInvestorPosition.get_subscriber().on_next(std::make_shared<MY::Base::Adapter::PositionQryRsp>(*pInvestorPosition));
    }
    if (bIsLast)
    {
        subjectInvestorPosition.get_subscriber().on_completed();
    }

    // LOG_DEBUG("LXD-DEBUG OnRspQryInvestorPosition 持仓查询返回 begin");
    // {
    //     boost::recursive_mutex::scoped_lock l(m_PosMutex);
    //     if (pInvestorPosition != NULL)
    //     {// 忽略0持仓
    // 		//LOG_INFO("Position:%s, bIsLast[%d]", pInvestorPosition->ToString().c_str(), bIsLast);
    //        // if (pInvestorPosition->m_positionStatics.m_position > 0)
    //         {
    // 		    m_posList.insert(m_posList.end(), *pInvestorPosition);
    //             m_tmpPositions.insert(m_tmpPositions.end(), ToPosition(*pInvestorPosition));
    //         }
    //     }

    //     if (bIsLast)
    //     {
    //         m_positions.clear();
    //         for (auto& pos : m_tmpPositions)
    //         {
    //             bool bFind = false;
    //             for (auto& p : m_positions)
    //             {
    //                 if (strcmp(p.key.instrumentID, pos.key.instrumentID) == 0
    //                     && p.hedgeType == pos.hedgeType
    //                     && p.dir == pos.dir)
    //                 {
    //                     p.prevTotalQty += pos.prevTotalQty;
    //                     p.CurTotalQty += pos.CurTotalQty;
    //                     bFind = true;

    //                     break;
    //                 }
    //             }
    // 			if (bFind == false) { //20240205 liyang add for limgh night plate product:rb bug find position > 1 records question
    // 				m_positions.insert(m_positions.end(), pos);
    // 			}
    //         }
    //         m_stockPosition.clear();
    //         m_futurePosition.clear();
    //         for (auto& p : m_positions)
    //         {
    //           //  LOG_INFO("OnRspQryInvestorPosition:p[%s]", p.ToString().c_str());
    //             if (p.productClass == ZTrader::ProductClassType::Futures)
    //             {
    //                 PosKey pkey{};
    // 				strcpy(pkey.pid, p.pid);
    // 				pkey.hedgeType = p.hedgeType;
    // 				//pkey.key = p.key;
    // 				pkey.key.exchangeID = p.key.exchangeID;
    // 				strcpy(pkey.key.instrumentID, p.key.instrumentID);
    // 				pkey.dir = p.dir;
    // 				m_futurePosition[pkey] = p;
    //                // m_futurePosition.insert(m_futurePosition.end(), p);
    //             }
    //             else if (p.productClass == ZTrader::ProductClassType::Stock)
    //             {
    //                 PosKey pkey{};
    // 				strcpy(pkey.pid, p.pid);
    // 				pkey.hedgeType = p.hedgeType;
    // 				pkey.key.exchangeID = p.key.exchangeID;
    // 				strcpy(pkey.key.instrumentID, p.key.instrumentID);
    // 				pkey.dir = p.dir;
    // 				m_stockPosition[pkey] = p;
    //                // m_stockPosition.insert(m_stockPosition.end(), p);
    //             }
    //             else
    //             {
    //                 LOG_INFO("key[%s]unkown productClass[%s]", p.key.ToString().c_str(), convertToString(p.productClass));
    //             }
    //         }
    //     }
    // }
    // if (bIsLast)
    // {
    // 	if (m_tradeAdapter->getParams().m_AccountType == 3)
    // 	{//融资融券账户
    // 		for (auto& posDetail : m_positionsTemp)
    // 		{
    // 			bool bFind = false;
    // 			for (auto& pos : m_stockPosition)
    // 			{
    // 				if (posDetail.hedgeType == HedgeType::Margin
    // 					&& strcmp(posDetail.key.instrumentID, pos.second.key.instrumentID) == 0
    // 					&& posDetail.key.exchangeID == pos.second.key.exchangeID)
    // 				{
    // 					pos.second.CurTotalQty -= posDetail.CurTotalQty;
    // 					pos.second.prevTotalQty -= posDetail.prevTotalQty;
    // 				}
    // 			}
    // 		}
    // 		for (auto& posDetail : m_positionsTemp)
    // 		{
    //             PosKey pkey{};
    // 			strcpy(pkey.pid, posDetail.pid);
    // 			pkey.hedgeType = posDetail.hedgeType;
    // 			pkey.key.exchangeID = posDetail.key.exchangeID;
    // 			strcpy(pkey.key.instrumentID, posDetail.key.instrumentID);
    // 			pkey.dir = posDetail.dir;
    // 			m_stockPosition[pkey] = posDetail;
    // 			//m_stockPosition.insert(m_stockPosition.end(), posDetail);
    // 		}
    // 	}
    // 	//快速强制同步两次，会有这种情况。
    // 	//强制同步的时候隔几分钟等持仓返回完毕再强制同步就肯定不会有这个问题
    // 	m_posFinish = true;
    //     LOG_DEBUG("LXD-DEBUG OnRspQryInvestorPosition 持仓查询返回 m_posFinish = true");
    // 	if (!m_resyncAll)
    // 	{
    // 		LOG_INFO("{%s}查询结束, m_resyncAll={%d}, m_stockPosition size[%d] ", m_strAdapterName.c_str(), m_resyncAll, m_stockPosition.size());
    // 		m_qrySuccess = true;
    // 	}
    // 	else
    // 	{
    // 		m_qrySuccess = true;
    // 	}
    // 	m_tmpPositions.clear();
    //     m_posList.clear();

    //     if (m_bFirstSyncPos)
    //     {
    //         ZTrader::AdapterInfo data{};
    //         m_Admin.GetDataCenter().GetAdapterInfo(m_strAdapterName, data);
    //         ZTrader::CorrectPosition qry{};
    //         strncpy(qry.FundID, data.FundID, sizeof(qry.FundID));
    //         std::list<std::string> vec;
    //         m_Admin.GetDataCenter().CtrlCorrectPosition(qry, vec);
    //         m_bFirstSyncPos = false;
    //     }
    // }
    // LOG_DEBUG("LXD-DEBUG OnRspQryInvestorPosition 持仓查询返回 end");
}

// 连接状态信息流
rxcpp::observable<std::shared_ptr<int>> PositionImp::observeUserLogin()
{
    return subjectUserLogin.get_observable();
}

// 资金信息流
rxcpp::observable<std::shared_ptr<MY::Base::Adapter::AccountQryRsp>> PositionImp::observeTradingAccount()
{
    return subjectTradingAccount.get_observable();
}

// 持仓信息流
rxcpp::observable<std::shared_ptr<MY::Base::Adapter::PositionQryRsp>> PositionImp::observeInvestorPosition()
{
    return subjectInvestorPosition.get_observable();
}

//////////////////////////////////////////////////////
void PositionImp::destroy()
{
    // destroy servant here:
    //...
}

int PositionImp::test(tars::TarsCurrentPtr current)
{
    double pidNav = 0.0;

    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::tm tm_start_time;
    localtime_s(&tm_start_time, &start_time);
    std::tm *tm_start_time_ptr = &tm_start_time;
    cout << ">> test begin at " << std::put_time(tm_start_time_ptr, "%Y-%m-%d %H:%M:%S") << endl;

    try
    {
        /*-----------------------------------------------------*/
        // call class method here:
        /*-----------------------------------------------------*/
        // initializeZTraderApi();
        // // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // logIn("tongtong", "tongtong");
        // std::string pid = "SIMNOW_Futures";
        // cout << "qryPflNav " << pid << std::endl;
        // pidNav = qryPflNav(pid);
        // cout << pidNav << std::endl;
        /*-----------------------------------------------------*/

        /*-----------------------------------------------------*/
        // connect ztrader directly
        /*-----------------------------------------------------*/
        /*
        g_pApiEngine = MY::Base::Rpc::RpcEnginePtr(new MY::Base::Rpc::RpcEngine());
        g_pApiEngine->init(1, 1);
        g_pApi = ZTraderApiPtr(new ZTrader::ZTraderApi(g_pApiEngine, "127.0.0.1", 5988));
        std::promise<void> promiseConn;
        auto syncConn = promiseConn.get_future();
        g_pApi->onConnected.connect([&promiseConn]()
                                    {
          cout << "connected\n";
          promiseConn.set_value(); });
        if (!g_pApi->init() || !g_pApi->start())
        {
            cout << "failed to init or start" << endl;
            cout << "initializeZTraderApi: end" << endl;
            return 1;

            syncConn.wait();
            g_pApi->onConnected.disconnect_all_slots();
            g_pApi->onConnected.connect([]()
                                        { cout << "connected\n"; });
            g_pApi->onDisconnected.connect([]()
                                           { cout << "disconnected\n"; });
        }
        {
            std::string user = "tongtong";
            std::string password = "tongtong";

            ZTrader::UserLogin ul;
            strcpy(ul.up.userName, user.c_str());
            strcpy(ul.up.password, password.c_str());
            ::bson::bo bo;
            auto rsp = g_pApi->LogIn(ul, &bo);
            if (!bo.isEmpty())
            {
                cout << "login succ\n";
            }
            else
            {
                cout << "login fail\n";
            }
        }
        {
            std::string pid = "SIMNOW_Futures";
            ZTrader::QryPflNAV qryNav;
            strcpy(qryNav.pid, pid.c_str());
            qryNav.assetType = ZTrader::AssetType::NA;
            strcpy(qryNav.pid, pid.c_str());
            bson::bo bo;
            auto nav = g_pApi->QryPflNAV_Req(qryNav, &bo);
            if (!bo.isEmpty())
            {
                char szErr[256] = {};
                sprintf(szErr, "%s\n", bo["error"].str().c_str());
                cerr << szErr << endl;
                return 0.0;
            }
            else
            {
                cout << "qryPflNav succ" << endl;
            }
            if (nav.size() < 1)
            {
                cout << "qryPflNav fail" << endl;
                return 0.0;
            }
            double sumNav = 0.0;
            for (auto it = nav.begin(); it != nav.end(); ++it)
            {
                sumNav += it->Nav;
            }
            pidNav = sumNav;
            cout << "PID: " << pid << " NAV: " << sumNav << endl;
            cout << ">> qryPflNav end" << endl;
        }
        */
        /*-----------------------------------------------------*/

        /*-----------------------------------------------------*/
        // connect to CTP
        /*-----------------------------------------------------*/
        XmlConfig config("config.xml");
        m_Factory.loadConfig(config);
        ZTrader::AdapterTypeName adapterType = ZTrader::AdapterTypeName::CTP;
        char adapterID[31 + 1] = "SIMNOW_Futures";
        cout << "creating counter " << static_cast<int>(adapterType) << ":" << adapterID << "..." << endl;
        MY::Base::Adapter::TradeAdapterPtr adapter = m_Factory.createTradeAdapter(adapterID);
        if (adapter == NULL)
        {
            cout << ">> fail - failed to create adapter" << endl;
        }
        else
        {
            cout << ">> succ - adapter created" << endl;
        }
        cout << "adapter.m_ap.m_name = " << adapter->getParams().m_name << endl;
        cout << "adapter.m_ap.m_type = " << adapter->getParams().m_type << endl;
        cout << "adapter.m_ap.m_tradeAddrs = " << adapter->getParams().m_tradeAddrs << endl;
        cout << "adapter.m_ap.m_investorID = " << adapter->getParams().m_investorID << endl;
        cout << "adapter.m_ap.m_brokerID = " << adapter->getParams().m_brokerID << endl;
        // cout << "adapter.m_ap.m_passwordT = " << adapter->getParams().m_passwordT << endl;
        // cout << "adapter.m_ctpAPI = " << adapter->m_ctpAPI << endl;
        //  connect callback to adapter
        if (adapter)
        {
            adapter->onEvent_NetConnected.connect(boost::bind(&PositionImp::onEvent_NetConnected, this, _1, _2));
            adapter->onEvent_NetDisconnected.connect(boost::bind(&PositionImp::onEvent_NetDisconnected, this, _1, _2, _3));
            adapter->onRspUserLogin.connect(boost::bind(&PositionImp::OnRspUserLogin, this, _1, _2));

            adapter->onRspQryTradingAccount.connect(boost::bind(&PositionImp::OnRspQryTradingAccount, this, _1, _2, _3));
            // adapter->onRspQryInvestorPositionDetails.connect(boost::bind(&PositionImp::onRspQryInvestorPositionDetails, this, _1, _2, _3));
            adapter->onRspQryInvestorPosition.connect(boost::bind(&PositionImp::OnRspQryInvestorPosition, this, _1, _2, _3));
            // adapter->onRspQryOrder.connect(boost::bind(&PositionImp::OnRspQryOrder, this, _1, _2, _3));
            // adapter->onRspQryTrade.connect(boost::bind(&PositionImp::OnRspQryTrade, this, _1, _2, _3));
            // adapter->onRtnOrder.connect(boost::bind(&PositionImp::OnRtnOrder, this, _1));
            // adapter->onRtnTrade.connect(boost::bind(&PositionImp::OnRtnTrade, this, _1));
            // adapter->onRspOrderInsert.connect(boost::bind(&PositionImp::OnRspOrderInsert, this, _1, _2));
            // adapter->onRspOrderCancel.connect(boost::bind(&PositionImp::OnRspOrderCancel, this, _1, _2));
            // adapter->onRspChangePassword.connect(boost::bind(&PositionImp::OnRspChangePassword, this, _1, _2));
        }
        // prepare observer
        // 使用 promise 和 future 来等待数据
        std::promise<int> positionPromise;                              // 用于返回持仓值
        std::future<int> positionFuture = positionPromise.get_future(); // 获取 future
        
        // 订阅连接成功事件流
        auto userLoginSubscription =
            observeUserLogin()
                .subscribe(
                    [&adapter](std::shared_ptr<int> val)
                    {
                        cout << ">> [UserLogin] on next: " << *val << endl;
                    },
                    [](std::exception_ptr ep)
                    {
                        try
                        {
                            if (ep)
                                std::rethrow_exception(ep);
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << ">> [UserLogin] Error: " << e.what() << std::endl;
                        }
                    },
                    [&adapter]()
                    {
                        std::cout << ">> [UserLogin] All responses received." << std::endl;
                        cout << ">> [UserLogin] isOnline = " << adapter->isOnline() << endl;
                        
                        // query account
                        {
                            MY::Base::Adapter::AccountQryReq qry{};
                            cout << "starting reqQryTradingAccount..." << endl;
                            int ret = adapter->reqQryTradingAccount(&qry, 0);
                            if (ret == 0)
                            {
                                cout << ">> succ " << ret << endl;
                            }
                            else if (ret == -1)
                            {
                                cout << ">> fail " << ret << ": The network connection fails." << endl;
                            }
                            else if (ret == -2)
                            {
                                cout << ">> fail " << ret << ": The number of unprocessed requests exceeds the permitted number." << endl;
                            }
                            else if (ret == -3)
                            {
                                cout << ">> fail " << ret << ": The number of requests sent per second exceeds the permitted number." << endl;
                            }
                            else
                            {
                                cout << "fail " << ret << ": unknown error." << endl;
                            }
                            // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        }
                    });

        // 订阅账户信息流
        auto tradingAccountSubscription =
            observeTradingAccount()
                .subscribe(
                    [&adapter, &pidNav](std::shared_ptr<MY::Base::Adapter::AccountQryRsp> val)
                    {
                        cout << ">> [RspQryTradingAccount] on next: " << val->ToString() << endl;
                        // pidNav += val->m_accountInfo.m_dynamicRights; // 累加
                    },
                    [](std::exception_ptr ep)
                    {
                        try
                        {
                            if (ep)
                                std::rethrow_exception(ep);
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << ">> [RspQryTradingAccount] Error: " << e.what() << std::endl;
                        }
                    },
                    [&adapter]()
                    {
                        std::cout << ">> [RspQryTradingAccount] All responses received." << std::endl;
                        // query position
                        {
                            MY::Base::Adapter::PositionQryReq pQryPosition{};
                            cout << "starting reqQryPosition..." << endl;
                            int ret = adapter->reqQryPosition(&pQryPosition, 0);
                            if (ret == 0)
                            {
                                cout << ">> succ " << ret << endl;
                            }
                            else if (ret == -1)
                            {
                                cout << ">> fail " << ret << ": The network connection fails." << endl;
                            }
                            else if (ret == -2)
                            {
                                cout << ">> fail " << ret << ": The number of unprocessed requests exceeds the permitted number." << endl;
                            }
                            else if (ret == -3)
                            {
                                cout << ">> fail " << ret << ": The number of requests sent per second exceeds the permitted number." << endl;
                            }
                            else
                            {
                                cout << ">> fail - unknown error." << endl;
                            }
                            // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        }
                    });

        // 订阅账户持仓信息流
        auto investorPositionSubscription =
            observeInvestorPosition()
                .subscribe(
                    [&adapter, &pidNav, &positionPromise](std::shared_ptr<MY::Base::Adapter::PositionQryRsp> val)
                    {
                        cout << ">> [QryInvestorPosition] on next: " << val->ToString() << endl;
						pidNav += val->m_positionStatics.m_position; // 累加持仓量
                    },
                    [](std::exception_ptr ep)
                    {
                        try
                        {
                            if (ep)
                                std::rethrow_exception(ep);
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << ">> [QryInvestorPosition] Error: " << e.what() << std::endl;
                        }
                    },
                    [&adapter, &start, &pidNav, &positionPromise]()
                    {
                        std::cout << ">> [QryInvestorPosition] All responses received." << std::endl;

                        // 获取持仓值并通过 promise 返回
                        positionPromise.set_value(pidNav); // 返回持仓数量

                        // adapter->stop();
                        // auto end = std::chrono::system_clock::now();
                        // std::time_t end_time = std::chrono::system_clock::to_time_t(end);
                        // std::chrono::duration<double> elapsed_seconds = end - start;
                        // std::tm tm_end_time;
                        // localtime_s(&tm_end_time, &end_time);
                        // std::tm *tm_end_time_ptr = &tm_end_time;
                        // cout << ">> test end at " << std::put_time(tm_end_time_ptr, "%Y-%m-%d %H:%M:%S") << ", elapsed time: " << elapsed_seconds.count() << endl;
                    });

        // start adapter
        {
            cout << "starting adapter..." << endl;

            adapter->stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            int ret = adapter->start();

            // cout << "adapter.m_ctpAPI = " << adapter->m_ctpAPI << endl;cout << "adapter.m_ctpAPI = " << adapter->m_ctpAPI << endl;
            if (ret != 0)
            {
                cout << ">> fail - failed to start adapter" << endl;
            }
            else
            {
                cout << ">> succ - adapter started " << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                // cout << "isOnline = " << adapter->isOnline() << endl;
            }
        }
        /*-----------------------------------------------------*/
        // // query account
        // {
        //     MY::Base::Adapter::AccountQryReq qry{};
        //     cout << "starting reqQryTradingAccount..." << endl;
        //     int ret = adapter->reqQryTradingAccount(&qry, 0);
        //     if (ret == 0)
        //     {
        //         cout << ">> succ " << ret << endl;
        //     }
        //     else if (ret == -1)
        //     {
        //         cout << ">> fail " << ret << ": The network connection fails." << endl;
        //     }
        //     else if (ret == -2)
        //     {
        //         cout << ">> fail " << ret << ": The number of unprocessed requests exceeds the permitted number." << endl;
        //     }
        //     else if (ret == -3)
        //     {
        //         cout << ">> fail " << ret << ": The number of requests sent per second exceeds the permitted number." << endl;
        //     }
        //     else
        //     {
        //         cout << ">> fail " << ret << ": unknown error." << endl;
        //     }
        //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        // }
        /*-----------------------------------------------------*/
        // // query position
        // {
        //     MY::Base::Adapter::PositionQryReq pQryPosition{};
        //     cout << "starting reqQryPosition..." << endl;
        //     int ret = adapter->reqQryPosition(&pQryPosition, 0);
        //     if (ret == 0)
        //     {
        //         cout << ">> succ " << ret << endl;
        //     }
        //     else if (ret == -1)
        //     {
        //         cout << ">> fail " << ret << ": The network connection fails." << endl;
        //     }
        //     else if (ret == -2)
        //     {
        //         cout << ">> fail " << ret << ": The number of unprocessed requests exceeds the permitted number." << endl;
        //     }
        //     else if (ret == -3)
        //     {
        //         cout << ">> fail " << ret << ": The number of requests sent per second exceeds the permitted number." << endl;
        //     }
        //     else
        //     {
        //         cout << ">> fail - unknown error." << endl;
        //     }
        //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        // }
        /*-----------------------------------------------------*/

        // 等待持仓值的返回
        int positionValue = positionFuture.get(); // 等待直到持仓值返回
        std::cout << "Returned Position Value: " << positionValue << std::endl;

        /*-----------------------------------------------------*/
        // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        adapter->stop();
        {
            adapter->stop();
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::tm tm_end_time;
            localtime_s(&tm_end_time, &end_time);
            std::tm *tm_end_time_ptr = &tm_end_time;
            cout << ">> test end at " << std::put_time(tm_end_time_ptr, "%Y-%m-%d %H:%M:%S") << ", elapsed time: " << elapsed_seconds.count() << endl;
        }
        /*-----------------------------------------------------*/

        return pidNav; // 返回持仓值
    }
    catch (std::exception &e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
        cout << "failed to connect to ZTrader." << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
        cout << "failed to connect to ZTrader." << endl;
    }

    // auto end = std::chrono::system_clock::now();
    // std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    // std::chrono::duration<double> elapsed_seconds = end - start;
    // std::tm tm_end_time;
    // localtime_s(&tm_end_time, &end_time);
    // std::tm *tm_end_time_ptr = &tm_end_time;
    // cout << ">> test end at " << std::put_time(tm_end_time_ptr, "%Y-%m-%d %H:%M:%S") << ", elapsed time: " << elapsed_seconds.count() << endl;
    // return pidNav;
};