#pragma once

// CHKSock ��� ����Դϴ�.

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


