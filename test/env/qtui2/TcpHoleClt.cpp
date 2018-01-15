// TcpHoleClt.cpp : Defines the entry point for the console application.
//

#include "TcpHoleClt.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE m_hEvtEndModule = NULL;

HANDLE g_hThread_Main = NULL;
CSocket *g_pSock_Main = NULL;

HANDLE g_hThread_MakeHole = NULL;
CSocket *g_pSock_MakeHole = NULL;

HANDLE g_hThread_Listen = NULL;
CSocket *g_pSock_Listen = NULL;

char *g_pServerAddess = "callgle.xicp.net";	// 服务器地址

// 我自己的客户端信息
t_WelcomePkt g_WelcomePkt;
UINT g_nHolePort = 0;

HANDLE g_hEvt_MakeHoleFinished = NULL;		// 打洞操作已经完成，可以让主动端（客户端A）来连接了
HANDLE g_hEvt_ListenFinished = NULL;		// 侦听任务已启动
HANDLE g_hEvt_ConnectOK = NULL;				// 连接建立了，这个事件来通知其他线程停止连接尝试

//
// 执行者：客户端A
// 服务器要求主动端（客户端A）直接连接被动端（客户端B）的外部IP和端口号
//
BOOL Handle_SrvReqDirectConnect ( t_SrvReqDirectConnectPkt *pSrvReqDirectConnectPkt )
{
	ASSERT ( pSrvReqDirectConnectPkt );
	printf ( "You can connect direct to ( IP:%s  PORT:%d  ID:%u )\n", pSrvReqDirectConnectPkt->szInvitedIP,
		pSrvReqDirectConnectPkt->nInvitedPort, pSrvReqDirectConnectPkt->dwInvitedID );

	// 直接与客户端B建立TCP连接，如果连接成功说明TCP打洞已经成功了。
	CSocket Sock;
	try
	{
		if ( !Sock.Socket () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		UINT nOptValue = 1;
		if ( !Sock.SetSockOpt ( SO_REUSEADDR, &nOptValue , sizeof(UINT) ) )
		{
			printf ( "SetSockOpt socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		if ( !Sock.Bind ( g_nHolePort ) )
		{
			printf ( "Bind socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		for ( int ii=0; ii<100; ii++ )
		{
			if ( WaitForSingleObject ( g_hEvt_ConnectOK, 0 ) == WAIT_OBJECT_0 )
				break;
			DWORD dwArg = 1;
			if ( !Sock.IOCtl ( FIONBIO, &dwArg ) )
			{
				printf ( "IOCtl failed : %s\n", hwFormatMessage(GetLastError()) );
			}
			if ( !Sock.Connect ( pSrvReqDirectConnectPkt->szInvitedIP, pSrvReqDirectConnectPkt->nInvitedPort ) )
			{
				printf ( "Connect to [%s:%d] failed : %s\n", pSrvReqDirectConnectPkt->szInvitedIP, pSrvReqDirectConnectPkt->nInvitedPort, hwFormatMessage(GetLastError()) );
				Sleep (100);
			}
			else break;
		}
		if ( WaitForSingleObject ( g_hEvt_ConnectOK, 0 ) != WAIT_OBJECT_0 )
		{
			if ( HANDLE_IS_VALID ( g_hEvt_ConnectOK ) ) SetEvent ( g_hEvt_ConnectOK );
			printf ( "Connect to [%s:%d] successfully !!!\n", pSrvReqDirectConnectPkt->szInvitedIP, pSrvReqDirectConnectPkt->nInvitedPort );
			
			// 接收测试数据
			printf ( "Receiving data ...\n" );
			char szRecvBuffer[NET_BUFFER_SIZE] = {0};
			int nRecvBytes = 0;
			for ( int i=0; i<1000; i++ )
			{
				nRecvBytes = Sock.Receive ( szRecvBuffer, sizeof(szRecvBuffer) );
				if ( nRecvBytes > 0 )
				{
					printf ( "-->>> Received Data : %s\n", szRecvBuffer );
					memset ( szRecvBuffer, 0, sizeof(szRecvBuffer) );
					SLEEP_BREAK ( 1 );
				}
				else
				{
					SLEEP_BREAK ( 300 );
				}
			}
		}
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		return FALSE;
	}

	return TRUE;
}

//
// 执行者：客户端A、客户端B
// 侦听线程函数。
// 打洞开始后，客户端还同时启动一个侦听，接收来自端口 g_nHolePort 的连接请求
//
DWORD WINAPI ThreadProc_Listen(
  LPVOID lpParameter   // thread data
)
{
	ASSERT ( HANDLE_IS_VALID(g_hEvt_ListenFinished) && HANDLE_IS_VALID(g_hEvt_MakeHoleFinished) );
	printf ( "Client.%u will listen at port %u\n", g_WelcomePkt.dwID, g_nHolePort );

	BOOL bRet = FALSE;
	CSocket Sock;
	// 创建Socket，侦听来自端口 g_nHolePort 的连接请求
	try
	{
		if ( !Sock.Socket () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		UINT nOptValue = 1;
		if ( !Sock.SetSockOpt ( SO_REUSEADDR, &nOptValue , sizeof(UINT) ) )
		{
			printf ( "SetSockOpt socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		if ( !Sock.Bind ( g_nHolePort ) )
		{
			printf ( "Bind socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		if ( !Sock.Listen () )
		{
			printf ( "Listen failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		printf ( "Start TCP server listen port : %u\n", g_nHolePort );
		g_pSock_Listen = &Sock;
		if ( HANDLE_IS_VALID(g_hEvt_ListenFinished) )
			SetEvent ( g_hEvt_ListenFinished );

		CSocket sockAct;
		if ( Sock.Accept ( sockAct ) )
		{
			CString csSocketAddress;
			UINT nPort = 0;
			if ( !sockAct.GetPeerName ( csSocketAddress, nPort ) )
			{
				printf ( "GetPeerName failed : %s\n", hwFormatMessage(GetLastError()) );
			}
			else
			{
				if ( HANDLE_IS_VALID ( g_hEvt_ConnectOK ) ) SetEvent ( g_hEvt_ConnectOK );
				printf ( "Client.%u accept %s:%u\n", g_WelcomePkt.dwID, csSocketAddress, nPort );
				// 发送测试数据
				printf ( "Sending data ...\n" );
				char szBuf[1024] = {0};
				for ( int i=0; i<10; i++ )
				{
					int nLen = _snprintf ( szBuf, sizeof(szBuf), "Line.%04d - Test Data", i );
					if ( sockAct.Send ( szBuf, nLen ) != nLen )
					{
						printf ( "Send data failed : %s\n", hwFormatMessage(GetLastError()) );
						break;
					}
					else
					{
						printf ( "Sent Data : %s -->>>\n", szBuf );
						SLEEP_BREAK ( 300 );
					}
				}
			}
		}
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		goto finished;
	}
	
	bRet = TRUE;

finished:
	printf ( "ThreadProc_Listen end\n" );
	return bRet;
}

//
// 执行者：客户端A
// 有新客户端B登录了，我（客户端A）连接服务器端口 SRV_TCP_HOLE_PORT ，申请与客户端B建立直接的TCP连接
//
BOOL Handle_NewUserLogin ( CSocket &MainSock, t_NewUserLoginPkt *pNewUserLoginPkt )
{
	printf ( "New user ( %s:%u:%u ) login server\n", pNewUserLoginPkt->szClientIP,
		pNewUserLoginPkt->nClientPort, pNewUserLoginPkt->dwID );

	BOOL bRet = FALSE;
	DWORD dwThreadID = 0;
	t_ReqConnClientPkt ReqConnClientPkt;
	CSocket Sock;
	CString csSocketAddress;
	char szRecvBuffer[NET_BUFFER_SIZE] = {0};
	int nRecvBytes = 0;
	// 创建打洞Socket，连接服务器协助打洞的端口号 SRV_TCP_HOLE_PORT
	try
	{
		if ( !Sock.Socket () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		UINT nOptValue = 1;
		if ( !Sock.SetSockOpt ( SO_REUSEADDR, &nOptValue , sizeof(UINT) ) )
		{
			printf ( "SetSockOpt socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		if ( !Sock.Bind ( 0 ) )
		{
			printf ( "Bind socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		if ( !Sock.Connect ( g_pServerAddess, SRV_TCP_HOLE_PORT ) )
		{
			printf ( "Connect to [%s:%d] failed : %s\n", g_pServerAddess, SRV_TCP_HOLE_PORT, hwFormatMessage(GetLastError()) );
			goto finished;
		}
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		goto finished;
	}
	g_pSock_MakeHole = &Sock;
	ASSERT ( g_nHolePort == 0 );
	VERIFY ( Sock.GetSockName ( csSocketAddress, g_nHolePort ) );

	// 创建一个线程来侦听端口 g_nHolePort 的连接请求
	dwThreadID = 0;
	g_hThread_Listen = ::CreateThread ( NULL, 0, ::ThreadProc_Listen, LPVOID(NULL), 0, &dwThreadID );
	if (!HANDLE_IS_VALID(g_hThread_Listen) ) return FALSE;
	Sleep ( 3000 );

	// 我（客户端A）向服务器协助打洞的端口号 SRV_TCP_HOLE_PORT 发送申请，希望与新登录的客户端B建立连接
	// 服务器会将我的打洞用的外部IP和端口号告诉客户端B
	ASSERT ( g_WelcomePkt.dwID > 0 );
	ReqConnClientPkt.dwInviterID = g_WelcomePkt.dwID;
	ReqConnClientPkt.dwInvitedID = pNewUserLoginPkt->dwID;
	if ( Sock.Send ( &ReqConnClientPkt, sizeof(t_ReqConnClientPkt) ) != sizeof(t_ReqConnClientPkt) )
		goto finished;

	// 等待服务器回应，将客户端B的外部IP地址和端口号告诉我（客户端A）
	nRecvBytes = Sock.Receive ( szRecvBuffer, sizeof(szRecvBuffer) );
	if ( nRecvBytes > 0 )
	{
		ASSERT ( nRecvBytes == sizeof(t_SrvReqDirectConnectPkt) );
		PACKET_TYPE *pePacketType = (PACKET_TYPE*)szRecvBuffer;
		ASSERT ( pePacketType && *pePacketType == PACKET_TYPE_TCP_DIRECT_CONNECT );
		Sleep ( 1000 );
		Handle_SrvReqDirectConnect ( (t_SrvReqDirectConnectPkt*)szRecvBuffer );
		printf ( "Handle_SrvReqDirectConnect end\n" );
	}
	// 对方断开连接了
	else
	{
		goto finished;
	}
	
	bRet = TRUE;
finished:
	g_pSock_MakeHole = NULL;
	return bRet;

}

//
// 执行者：客户端B
// 打洞处理线程函数。
// 服务器要我（客户端B）向客户端A打洞，我（客户端B）将尝试与客户端A的外部IP和端口号connect
//
DWORD WINAPI ThreadProc_MakeHole(
  LPVOID lpParameter   // thread data
)
{
/*	{	//d
		if ( HANDLE_IS_VALID(g_hEvt_MakeHoleFinished) )
			SetEvent ( g_hEvt_MakeHoleFinished );
		return 0;
	}	//d */
	ASSERT ( HANDLE_IS_VALID(g_hEvt_ListenFinished) && HANDLE_IS_VALID(g_hEvt_MakeHoleFinished) );
	t_SrvReqMakeHolePkt *pSrvReqMakeHolePkt = (t_SrvReqMakeHolePkt*)lpParameter;
	ASSERT ( pSrvReqMakeHolePkt );
	t_SrvReqMakeHolePkt SrvReqMakeHolePkt;
	memcpy ( &SrvReqMakeHolePkt, pSrvReqMakeHolePkt, sizeof(t_SrvReqMakeHolePkt) );
	delete pSrvReqMakeHolePkt; pSrvReqMakeHolePkt = NULL;
	
	printf ( "Server request make hole to ( IP:%s  PORT:%d  ID:%u )\n", SrvReqMakeHolePkt.szClientHoleIP,
		SrvReqMakeHolePkt.nClientHolePort, SrvReqMakeHolePkt.dwInviterID );

	BOOL bRet = FALSE;
	CSocket Sock;
	// 创建Socket，本地端口绑定到 g_nHolePort，连接客户端A的外部IP和端口号（这个连接往往会失败）
	try
	{
		if ( !Sock.Socket () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		UINT nOptValue = 1;
		if ( !Sock.SetSockOpt ( SO_REUSEADDR, &nOptValue , sizeof(UINT) ) )
		{
			printf ( "SetSockOpt socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		if ( !Sock.Bind ( g_nHolePort ) )
		{
			printf ( "Bind socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		if ( HANDLE_IS_VALID(g_hEvt_MakeHoleFinished) )
			SetEvent ( g_hEvt_MakeHoleFinished );

		DWORD dwArg = 1;
		if ( !Sock.IOCtl ( FIONBIO, &dwArg ) )
		{
			printf ( "IOCtl failed : %s\n", hwFormatMessage(GetLastError()) );
		}
		for ( int i=0; i<100; i++ )
		{
			if ( WaitForSingleObject ( g_hEvt_ConnectOK, 0 ) == WAIT_OBJECT_0 )
				break;
			if ( !Sock.Connect ( SrvReqMakeHolePkt.szClientHoleIP, SrvReqMakeHolePkt.nClientHolePort ) )
			{
				printf ( "Connect to [%s:%d] failed : %s\n", SrvReqMakeHolePkt.szClientHoleIP, SrvReqMakeHolePkt.nClientHolePort, hwFormatMessage(GetLastError()) );
				Sleep ( 100 );
			}
			else
			{
				if ( HANDLE_IS_VALID ( g_hEvt_ConnectOK ) ) SetEvent ( g_hEvt_ConnectOK );
				// 有些路由器（如TPLink R402）不用打洞就能直接连接进去
				// 接收测试数据
				printf ( "Connect success when make hole. Receiving data ...\n" );
				char szRecvBuffer[NET_BUFFER_SIZE] = {0};
				int nRecvBytes = 0;
				for ( int i=0; i<1000; i++ )
				{
					nRecvBytes = Sock.Receive ( szRecvBuffer, sizeof(szRecvBuffer) );
					if ( nRecvBytes > 0 )
					{
						printf ( "-->>> Received Data : %s\n", szRecvBuffer );
						memset ( szRecvBuffer, 0, sizeof(szRecvBuffer) );
						SLEEP_BREAK ( 1 );
					}
					else
					{
						SLEEP_BREAK ( 300 );
					}
				}
				goto finished;
			}
		}
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		goto finished;
	}
	
	bRet = TRUE;

finished:
	printf ( "ThreadProc_MakeHole end\n" );
	return bRet;
}

//
// 执行者：客户端B
// 处理服务器要我（客户端B）向另外一个客户端（A）打洞，打洞操作在线程中进行。
// 先连接服务器协助打洞的端口号 SRV_TCP_HOLE_PORT ，通过服务器告诉客户端A我（客户端B）的外部IP地址和端口号，然后启动线程进行打洞，
// 客户端A在收到这些信息以后会发起对我（客户端B）的外部IP地址和端口号的连接（这个连接在客户端B打洞完成以后进行，所以
// 客户端B的NAT不会丢弃这个SYN包，从而连接能建立）
//
BOOL Handle_SrvReqMakeHole ( CSocket &MainSock, t_SrvReqMakeHolePkt *pSrvReqMakeHolePkt )
{
	ASSERT ( pSrvReqMakeHolePkt );
	// 创建Socket，连接服务器协助打洞的端口号 SRV_TCP_HOLE_PORT，连接建立以后发送一个断开连接的请求给服务器，然后连接断开
	// 这里连接的目的是让服务器知道我（客户端B）的外部IP地址和端口号，以通知客户端A
	CSocket Sock;
	try
	{
		if ( !Sock.Create () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			return FALSE;
		}
		if ( !Sock.Connect ( g_pServerAddess, SRV_TCP_HOLE_PORT ) )
		{
			printf ( "Connect to [%s:%d] failed : %s\n", g_pServerAddess, SRV_TCP_HOLE_PORT, hwFormatMessage(GetLastError()) );
			return FALSE;
		}
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		return FALSE;
	}

	CString csSocketAddress;
	ASSERT ( g_nHolePort == 0 );
	VERIFY ( Sock.GetSockName ( csSocketAddress, g_nHolePort ) );

	// 连接服务器协助打洞的端口号 SRV_TCP_HOLE_PORT，发送一个断开连接的请求，然后将连接断开，服务器在收到这个包的时候也会将
	// 连接断开
	t_ReqSrvDisconnectPkt ReqSrvDisconnectPkt;
	ReqSrvDisconnectPkt.dwInviterID = pSrvReqMakeHolePkt->dwInvitedID;
	ReqSrvDisconnectPkt.dwInviterHoleID = pSrvReqMakeHolePkt->dwInviterHoleID;
	ReqSrvDisconnectPkt.dwInvitedID = pSrvReqMakeHolePkt->dwInvitedID;
	ASSERT ( ReqSrvDisconnectPkt.dwInvitedID == g_WelcomePkt.dwID );
	if ( Sock.Send ( &ReqSrvDisconnectPkt, sizeof(t_ReqSrvDisconnectPkt) ) != sizeof(t_ReqSrvDisconnectPkt) )
		return FALSE;
	Sleep ( 100 );
	Sock.Close ();

	// 创建一个线程来向客户端A的外部IP地址、端口号打洞
	t_SrvReqMakeHolePkt *pSrvReqMakeHolePkt_New = new t_SrvReqMakeHolePkt;
	if ( !pSrvReqMakeHolePkt_New ) return FALSE;
	memcpy ( pSrvReqMakeHolePkt_New, pSrvReqMakeHolePkt, sizeof(t_SrvReqMakeHolePkt) );
	DWORD dwThreadID = 0;
	g_hThread_MakeHole = ::CreateThread ( NULL, 0, ::ThreadProc_MakeHole, LPVOID(pSrvReqMakeHolePkt_New), 0, &dwThreadID );
	if (!HANDLE_IS_VALID(g_hThread_MakeHole) ) return FALSE;

	// 创建一个线程来侦听端口 g_nHolePort 的连接请求
	dwThreadID = 0;
	g_hThread_Listen = ::CreateThread ( NULL, 0, ::ThreadProc_Listen, LPVOID(NULL), 0, &dwThreadID );
	if (!HANDLE_IS_VALID(g_hThread_Listen) ) return FALSE;

	// 等待打洞和侦听完成
	HANDLE hEvtAry[] = { g_hEvt_ListenFinished, g_hEvt_MakeHoleFinished };
	if ( ::WaitForMultipleObjects ( LENGTH(hEvtAry), hEvtAry, TRUE, 30*1000 ) == WAIT_TIMEOUT )
		return FALSE;
	t_HoleListenReadyPkt HoleListenReadyPkt;
	HoleListenReadyPkt.dwInvitedID = pSrvReqMakeHolePkt->dwInvitedID;
	HoleListenReadyPkt.dwInviterHoleID = pSrvReqMakeHolePkt->dwInviterHoleID;
	HoleListenReadyPkt.dwInvitedID = pSrvReqMakeHolePkt->dwInvitedID;
	if ( MainSock.Send ( &HoleListenReadyPkt, sizeof(t_HoleListenReadyPkt) ) != sizeof(t_HoleListenReadyPkt) )
	{
		printf ( "Send HoleListenReadyPkt to %s:%u failed : %s\n", g_WelcomePkt.szClientIP, g_WelcomePkt.nClientPort,
			hwFormatMessage(GetLastError()) );
		return FALSE;
	}
	
	return TRUE;
}

//
// 执行者：客户端A、客户端B
// 处理从服务器主连接中收到的数据
//
BOOL HandleDataMainSocket(CSocket &MainSock, char *data, int size)
{
	if ( !data || size < 4 ) return FALSE;

	PACKET_TYPE *pePacketType = (PACKET_TYPE*)data;
	ASSERT ( pePacketType );
	switch ( *pePacketType )
	{
		// 收到服务器的欢迎信息，说明登录已经成功
	case PACKET_TYPE_WELCOME:
		{
			ASSERT ( sizeof(t_WelcomePkt) == size );
			t_WelcomePkt *pWelcomePkt = (t_WelcomePkt*)data;
			printf ( "%s:%u:%u >>> %s\n", pWelcomePkt->szClientIP, pWelcomePkt->nClientPort,
				pWelcomePkt->dwID, pWelcomePkt->szWelcomeInfo );
			memcpy ( &g_WelcomePkt, pWelcomePkt, sizeof(t_WelcomePkt) );
			ASSERT ( g_WelcomePkt.dwID > 0 );
			break;
		}
		// 其他客户端（客户端B）登录到服务器了
	case PACKET_TYPE_NEW_USER_LOGIN:
		{
			ASSERT ( size == sizeof(t_NewUserLoginPkt) );
			Handle_NewUserLogin ( MainSock, (t_NewUserLoginPkt*)data );
			break;
		}
		// 服务器要我（客户端B）向另外一个客户端（客户端A）打洞
	case PACKET_TYPE_REQUEST_MAKE_HOLE:
		{
			ASSERT ( size == sizeof(t_SrvReqMakeHolePkt) );
			Handle_SrvReqMakeHole ( MainSock, (t_SrvReqMakeHolePkt*)data );
			break;
		}
	}

	return TRUE;
}

//
// 执行者：客户端A、客户端B
// 主线程函数
//
DWORD WINAPI ThreadProc_MainTCPClient(
  LPVOID lpParameter   // thread data
)
{
	BOOL bRet = FALSE;
	UINT nPort = (UINT)lpParameter;
	CSocket MainSock;
	char szRecvBuffer[NET_BUFFER_SIZE] = {0};
	int nRecvBytes = 0;
	// 创建主连接的Socket，用来和服务器主Socket建立常连接
	try
	{
		if ( !MainSock.Socket () )
		{
			printf ( "Create socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		UINT nOptValue = 1;
		if ( !MainSock.SetSockOpt ( SO_REUSEADDR, &nOptValue , sizeof(UINT) ) )
		{
			printf ( "SetSockOpt socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		if ( !MainSock.Bind ( 0 ) )
		{
			printf ( "Bind socket failed : %s\n", hwFormatMessage(GetLastError()) );
			goto finished;
		}
		if ( !MainSock.Connect ( g_pServerAddess, nPort ) )
		{
			printf ( "Connect to [%s:%d] failed : %s\n", g_pServerAddess, nPort, hwFormatMessage(GetLastError()) );
			goto finished;
		}
		CString csSocketAddress;
		UINT nMyPort = 0;
		VERIFY ( MainSock.GetSockName ( csSocketAddress, nMyPort ) );
		printf ( "Connect to [%s:%d] success, My Info is [%s:%d]\n", g_pServerAddess, nPort, csSocketAddress, nMyPort );
	}
	catch ( CException e )
	{
		char szError[255] = {0};
		e.GetErrorMessage( szError, sizeof(szError) );
		printf ( "Exception occur, %s\n", szError );
		goto finished;
	}

	g_pSock_Main = &MainSock;

	// 循环接收网络数据
	while ( TRUE )
	{
		nRecvBytes = MainSock.Receive ( szRecvBuffer, sizeof(szRecvBuffer) );
		if ( nRecvBytes > 0 )
		{
			if ( !HandleDataMainSocket ( MainSock, szRecvBuffer, nRecvBytes ) )
				goto finished;
		}
		else if ( (nRecvBytes == 0 && GetLastError() != NO_ERROR) || (SOCKET_ERROR == nRecvBytes && GetLastError() == WSAEWOULDBLOCK) )
		{
			SLEEP_BREAK ( 10 );
		}
		// 对方断开连接了
		else
		{
			goto finished;
		}
		SLEEP_BREAK ( 1 );
	}
	bRet = TRUE;

finished:
	g_pSock_Main = NULL;
	printf ( "ThreadProc_MainTCPClient end\n" );
	return bRet;
}

BOOL StartMainTCPClient ( UINT nPort, HANDLE *phThread )
{
	ASSERT ( phThread );
	DWORD dwThreadID = 0;
	*phThread = ::CreateThread ( NULL, 0, ::ThreadProc_MainTCPClient, LPVOID(nPort), 0, &dwThreadID );

	return HANDLE_IS_VALID(*phThread);
}

//
// 运行程序
//
int Run ()
{
	if ( !AfxSocketInit() )
		return End( FALSE );

	m_hEvtEndModule = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
	g_hEvt_MakeHoleFinished = ::CreateEvent ( NULL, FALSE, FALSE, NULL );
	g_hEvt_ListenFinished = ::CreateEvent ( NULL, FALSE, FALSE, NULL );
	g_hEvt_ConnectOK = ::CreateEvent ( NULL, TRUE, FALSE, NULL );

	if ( !HANDLE_IS_VALID(m_hEvtEndModule) || !HANDLE_IS_VALID(g_hEvt_MakeHoleFinished) ||
		!HANDLE_IS_VALID(g_hEvt_ListenFinished) || !HANDLE_IS_VALID(g_hEvt_ConnectOK) )
		return End( FALSE );
	
	if ( !StartMainTCPClient ( SRV_TCP_MAIN_PORT, &g_hThread_Main ) )
		return End( FALSE );

	printf ( "Press any key to terminate program ...\n" );
	::getchar ();
	return End( TRUE );
}

//
// 结束程序
//
int End ( BOOL bSuccess )
{
	if ( HANDLE_IS_VALID(m_hEvtEndModule) )
		::SetEvent ( m_hEvtEndModule );

	if ( g_pSock_Main ) g_pSock_Main->CancelBlockingCall ();
	if ( g_pSock_MakeHole ) g_pSock_MakeHole->CancelBlockingCall ();

	WSACleanup ();

	printf ( "End programe\n" );
	if ( bSuccess ) return 0;

	printf ( "Last error is : %s\n", hwFormatMessage(GetLastError()) );
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	if ( argc >= 2 )
	{
		g_pServerAddess = argv[1];
	}

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
		return nRetCode;
	}
	else
	{
		nRetCode = Run ();
	}

	return nRetCode;
}


