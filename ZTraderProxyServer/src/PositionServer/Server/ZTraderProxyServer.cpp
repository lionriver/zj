// #define _ITERATOR_DEBUG_LEVEL 0

#include "ZTraderProxyServer.h"
#include "PositionImp.h"

#include <string>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace std;

ZTraderProxyServer g_app;

/////////////////////////////////////////////////////////////////
bool isConnectable(const std::string &ip, int port)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return false;
    }
#endif

    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return false;
    }

    // 设置目标地址
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid IP address." << std::endl;
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return false;
    }

    // 尝试连接
    bool is_connected = (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) == 0);

    // 关闭套接字
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return is_connected;
}
/////////////////////////////////////////////////////////////////
void ZTraderProxyServer::initialize()
{
    // initialize application here:
    //...
    addServant<PositionImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".PositionObj");

    // cout << ">> ZTraderProxyServer::initialize: begin" << std::endl;
    // cout << ">> ZTraderProxyServer::initialize: end" << std::endl;
}
/////////////////////////////////////////////////////////////////
void ZTraderProxyServer::destroyApp()
{
    // destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////

class Initializer final
{
public:
    Initializer()
    {
        MY::Base::Common::commonLibInit("config.xml");
    }

    ~Initializer()
    {
        MY::Base::Common::commonLibUninit();
    }

private:
    Initializer(const Initializer &) = delete;
    void operator=(const Initializer &) = delete;
};


/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    try
    {
        g_app.main(argc, argv);

        //////////////////////////////////////////////////////////////////////
		//test ztrader connection
        {
            std::string ip = "127.0.0.1";
            int port = 5988;
            if (isConnectable(ip, port))
            {
                cout << "The IP " << ip << " and port " << port << " are connectable." << std::endl;
            }
            else
            {
                cout << "Cannot connect to IP " << ip << " on port " << port << "." << std::endl;
            }
        }
        //test simnow connection: 180.168.146.187:10201
        {
            
            std::string ip = "180.168.146.187";
            int port = 10201;
            if (isConnectable(ip, port))
            {
                cout << "The IP " << ip << " and port " << port << " are connectable." << std::endl;
            }
            else
            {
                cout << "Cannot connect to IP " << ip << " on port " << port << "." << std::endl;
            }
        }
        // try
        //{
        //     g_pApiEngine = MY::Base::Rpc::RpcEnginePtr(new MY::Base::Rpc::RpcEngine());
        //     cout << "1" << endl;
        //     g_pApiEngine->init(1, 1);
        //     cout << "2" << endl;
        //     cout << "2.1" << endl;
        //     cout << "2.2" << endl;
        //     // m_pApi = new ZTrader::ZTraderApi(g_pApiEngine, "127.0.0.1", 5988);
        //     cout << "2.3" << endl;
        //     g_pApi = ZTraderApiPtr(new ZTrader::ZTraderApi(g_pApiEngine, "127.0.0.1", 5988));
        //     cout << "3" << endl;
        // }
        // catch (std::exception &e)
        //{
        //     cerr << "initializeZTraderApi: std::exception:" << e.what() << std::endl;
        //     cout << "initializeZTraderApi: failed to connect to ZTrader." << endl;
        //     cout << "initializeZTraderApi: end" << endl;
        //     return -1;
        // }
        // catch (...)
        //{
        //     cerr << "initializeZTraderApi: unknown exception." << std::endl;
        //     cout << "initializeZTraderApi: failed to connect to ZTrader." << endl;
        //     cout << "initializeZTraderApi: end" << endl;
        //     return -2;
        // }
        //////////////////////////////////////////////////////////////////////
        g_app.waitForShutdown();
    }
    catch (std::exception &e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
