#include "GameRole.h"
#include "GameMsg.h"
#include <iostream>
#include "AOIWorld.h"
#include "msg.pb.h"
#include "GameProtocol.h"
#include "GameChannel.h"

using namespace std;

/*������Ϸ����ȫ�ֶ���*/
static AOIWorld world(0, 400, 0, 400, 20, 20);

GameMsg * GameRole::CreateIDNameLogin()
{
	pb::SyncPid *pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg *pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
	return pRet;
}

GameMsg * GameRole::CreataSrdPlayers()
{
	pb::SyncPlayers *pMsg = new pb::SyncPlayers();

	auto srd_list = world.GetSrdPlayers(this);

	for (auto single : srd_list)
	{
		auto pPlayer = pMsg->add_ps();
		auto pRole = dynamic_cast<GameRole *>(single);
		pPlayer->set_pid(pRole->iPid);
		pPlayer->set_username(pRole->szName);
		auto pPostion = pPlayer->mutable_p();
		pPostion->set_x(pRole->x);
		pPostion->set_y(pRole->y);
		pPostion->set_z(pRole->z);
		pPostion->set_v(pRole->v);
	}

	GameMsg *pret = new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, pMsg);
	return pret;
}

GameMsg * GameRole::CreateSelfPostion()
{
	pb::BroadCast *pMsg = new pb::BroadCast();
	pMsg->set_pid(iPid);
	pMsg->set_username(szName);
	pMsg->set_tp(2);

	auto pPosition = pMsg->mutable_p();
	pPosition->set_x(x);
	pPosition->set_y(y);
	pPosition->set_z(z);
	pPosition->set_v(v);

	GameMsg *pret = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
	return pret;
}

GameMsg * GameRole::CreateIDNameLogoff()
{
	pb::SyncPid *pmsg = new pb::SyncPid();
	pmsg->set_pid(iPid);
	pmsg->set_username(szName);
	GameMsg *pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pmsg);
	return pRet;
}

GameRole::GameRole()
{
	szName = "Tom";
	x = 100;
	z = 100;
}


GameRole::~GameRole()
{

}

bool GameRole::Init()
{
	/*����Լ�����Ϸ����*/
	bool bRet = false;
	/*�������IDΪ��ǰ���ӵ�fd*/
	iPid = m_pProto->m_channel->GetFd();
	bRet = world.AddPlayer(this);

	
	if (true == bRet)
	{
		/*���Լ�����ID������*/
		auto pmsg = CreateIDNameLogin();
		ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
		/*���Լ�������Χ��ҵ�λ��*/
		pmsg = CreataSrdPlayers();
		ZinxKernel::Zinx_SendOut(*pmsg, *m_pProto);
		/*����Χ��ҷ����Լ���λ��*/
		auto srd_list = world.GetSrdPlayers(this);
		for (auto single : srd_list)
		{
			pmsg = CreateSelfPostion();
			auto pRole = dynamic_cast<GameRole *>(single);
			ZinxKernel::Zinx_SendOut(*pmsg, *(pRole->m_pProto));
		}
	}

	return bRet;
}

/*������Ϸ��ص��û�����*/
UserData * GameRole::ProcMsg(UserData & _poUserData)
{
	/*���ԣ���ӡ��Ϣ����*/
	GET_REF2DATA(MultiMsg, input, _poUserData);

	for (auto single : input.m_Msgs)
	{
		cout << "type is" << single->enMsgType << endl;
		cout << single->pMsg->Utf8DebugString() << endl;
	}

	return nullptr;
}

void GameRole::Fini()
{
	/*����Χ��ҷ���������Ϣ*/
	auto srd_list = world.GetSrdPlayers(this);
	for (auto single : srd_list)
	{
		auto pMsg = CreateIDNameLogoff();
		auto pRole = dynamic_cast<GameRole *>(single);
		ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_pProto));
	}
	world.DelPlayer(this);
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}
