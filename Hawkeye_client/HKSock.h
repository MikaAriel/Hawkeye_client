#pragma once

// CHKSock 명령 대상입니다.

class CHKSock : public CAsyncSocket
{
public:
	CHKSock();
	virtual ~CHKSock();
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
private:
};


