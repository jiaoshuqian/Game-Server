#include "TcpDataChannel.h"


TcpDataChannel::TcpDataChannel(int _socket):m_socket(_socket)
{
}

TcpDataChannel::~TcpDataChannel()
{
	if (0 <= m_socket)
	{
		close(m_socket);
	}
}

bool TcpDataChannel::Init()
{
	return true;
}

bool TcpDataChannel::ReadFd(std::string & _input)
{
	char szBuff[128] = { 0 };
	int irecvlen = -1;
	bool bRet = false;

	while (1)
	{
		irecvlen = recv(m_socket, szBuff, sizeof(szBuff), MSG_DONTWAIT);
		if (0 == irecvlen )
		{
			//链接断了
			SetChannelClose();
			break;
		}
		else if (-1 == irecvlen)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				//链接没断，只是没数据了
				break;
			}
			//同链接断了
			SetChannelClose();
			break;
		}
		else
		{
			_input.append(szBuff, irecvlen);
			bRet = true;
		}

	}
	
	return bRet;
}

bool TcpDataChannel::WriteFd(std::string & _output)
{
	send(m_socket, _output.data(), _output.size(), 0);
	return true;
}

void TcpDataChannel::Fini()
{
	if (m_socket >= 0)
	{
		close(m_socket);
		m_socket = -1;
	}
}

int TcpDataChannel::GetFd()
{
	return m_socket;
}

std::string TcpDataChannel::GetChannelInfo()
{
	return "data_socket" + m_socket;
}

bool TcpListenChannel::Init()
{
	bool bRet = false;
	int isocket = -1;

	isocket = socket(AF_INET, SOCK_STREAM, 0);
	if (0 <= isocket)
	{
		struct sockaddr_in stServer;
		stServer.sin_family = AF_INET;
		stServer.sin_addr.s_addr = INADDR_ANY;
		stServer.sin_port = htons(m_port);
		//设置允许重复绑定
		int iflag = 1;
		setsockopt(isocket, SOL_SOCKET, SO_REUSEADDR, &iflag, sizeof(iflag));
		if (0 == bind(isocket, (struct sockaddr *)&stServer, sizeof(stServer)))
		{
			if (0 == listen(isocket, 1000))
			{
				bRet = true;
				iListenFd = isocket;
			}
			else
			{
				perror(__FILE__":""__FUNC__"":""listen");
			}
		}
		else
		{
			perror(__FILE__":""__FUNC__"":""bind");
		}
	}
	else
	{
		perror(__FILE__":""__FUNC__"":""socket");
	}

	return bRet;
}

bool TcpListenChannel::ReadFd(std::string & _input)
{
	bool bRet = false;
	struct sockaddr_in stClient;
	socklen_t socklen = sizeof(stClient);

	int iDataFd = accept(iListenFd, (struct sockaddr *)&stClient, &socklen);
	if (0 <= iDataFd)
	{
		auto pDataChannel = m_factory->CreateChannel(iDataFd);
		bRet = ZinxKernel::Zinx_Add_Channel(*pDataChannel);
	}
	else
	{
		perror(__FILE__":""__FUNC__"":""accept");
	}

	return bRet;
}

bool TcpListenChannel::WriteFd(std::string & _output)
{
	return false;
}

void TcpListenChannel::Fini()
{
	if (0 <= iListenFd)
	{
		close(iListenFd);
	}
}

int TcpListenChannel::GetFd()
{
	return iListenFd;
}

std::string TcpListenChannel::GetChannelInfo()
{
	return "TCP_LISTEN";
}

AZinxHandler * TcpListenChannel::GetInputNextStage(BytesMsg & _oInput)
{
	return nullptr;
}

//第二个参数的内存被该类吸收
TcpListenChannel::TcpListenChannel(unsigned short port, TcpDataChannelFactory * _pfact):m_port(port),m_factory(_pfact)
{
}

TcpListenChannel::~TcpListenChannel()
{
	if (NULL != m_factory)
	{
		delete m_factory;
	}
}
