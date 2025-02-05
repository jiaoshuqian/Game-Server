#pragma once
#include <zinx.h>
#include "AOIWorld.h"
#include "GameMsg.h"

class GameProtocol;
class GameRole :
	public Irole,public Player
{
	float x = 0;
	float y = 0;//��
	float z = 0;
	float v = 0;
	int iPid = 0;
	std::string szName;
	GameMsg *CreateIDNameLogin();
	GameMsg *CreataSrdPlayers();
	GameMsg *CreateSelfPostion();
	GameMsg *CreateIDNameLogoff();
public:
	GameRole();
	virtual ~GameRole();

	// ͨ�� Irole �̳�
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;
	GameProtocol *m_pProto = NULL;

	// ͨ�� Player �̳�
	virtual int GetX() override;
	virtual int GetY() override;
};

