#include <cstdio>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <event.h>

#include <string>

using namespace std;

void empty(struct redisAsyncContext *pc, void *reply, void *private_data)
{

}

void my_callback(struct redisAsyncContext *pc, void *reply, void *private_data)
{
	string port;
	string room_no;
	//��ָ���ķ��������Լ��Ŵ�������
	redisReply *my_reply = (redisReply *)reply;

	if ("message" != string(my_reply->element[0]->str))
	{
		return;
	}
	else
	{
		//reply �а���IP�������
		string orig_content(my_reply->element[2]->str);
		int pos = orig_content.find(':');
		string IP = orig_content.substr(0, pos);
		//�ж�IP�Ƿ����Լ���IP
		string my_ip(getenv("MY_IP"));
		if (IP != my_ip)
		{
			return;
		}
		room_no = orig_content.substr(pos + 1, orig_content.size() - pos - 1);
	}

	//ִ�нű���ö˿ں�
	string cmd = "./create_room.sh room_no" + room_no;
	auto pf = popen(cmd.c_str(), "r");
	if (pf != NULL)
	{
		char buff[1024];
		fread(buff, 1, sizeof(buff), pf);
		port.append(buff);
		pclose(pf);
	}
	//�����˿ں�
	auto spc = redisConnect("192.168.64.148", 6379);
	if (NULL != spc)
	{
		freeReplyObject( redisCommand(spc, "publish server_port %s", port.c_str()));
		redisFree(spc);
	}
}

int main()
{
	auto base = event_base_new();

	auto pc = redisAsyncConnect("192.168.64.148", 6379);
	redisLibeventAttach(pc, base);

	redisAsyncCommand(pc, my_callback, NULL, "subscribe create_room");

	event_base_dispatch(base);

    return 0;
}