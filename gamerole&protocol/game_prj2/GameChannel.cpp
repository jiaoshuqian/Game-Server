#include "GameChannel.h"



GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{
}


GameChannel::~GameChannel()
{
}

AZinxHandler * GameChannel::GetInputNextStage(BytesMsg & _oInput)
{
	return m_proto;
}

ZinxTcpData * GameConnFact::CreateTcpDataChannel(int _fd)
{
	/*����tcpͨ������*/
	auto pChannel = new GameChannel(_fd);
	/*����Э�����*/
	auto pProtocol = new GameProtocol();
	/*��Э�����*/
	pChannel->m_proto = pProtocol;
	pProtocol->m_channel = pChannel;
	/*��Э�������ӵ�kernel*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	return pChannel;
}
