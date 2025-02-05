#include "TcpDataChannel.h"
#include "zinx.h"

class Echo :public AZinxHandler {
	// 通过 AZinxHandler 继承
	virtual IZinxMsg * InternelHandle(IZinxMsg & _oInput) override
	{
		GET_REF2DATA(BytesMsg, input, _oInput);
		auto channel = ZinxKernel::Zinx_GetChannel_ByInfo(input.szInfo);
		ZinxKernel::Zinx_SendOut(input.szData, *channel);
		return nullptr;
	}
	virtual AZinxHandler * GetNextHandler(IZinxMsg & _oNextMsg) override
	{
		return nullptr;
	}
} echo;

class myTcpData :public TcpDataChannel {
public:
	myTcpData(int _fd):TcpDataChannel(_fd) {}
	// 通过 TcpDataChannel 继承
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override
	{
		return &echo;
	}
};

class myfact :public TcpDataChannelFactory {
	// 通过 TcpDataChannelFactory 继承
	virtual TcpDataChannel * CreateChannel(int _fd) override
	{
		return new myTcpData(_fd);
	}
};

int main()
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new TcpListenChannel(23333, new myfact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
}