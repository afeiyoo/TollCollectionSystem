#ifndef _REDIS_CLIENT_DLL_H
#define _REDIS_CLIENT_DLL_H

#ifndef WIN32
#define REDIS_CLIENT_API __attribute__((__stdcall__))
#else
#define REDIS_CLIENT_API __stdcall
#endif


struct IRedisQueueListener
{
    virtual void REDIS_CLIENT_API OnQueMsgNotify( const char *szQueName,const char *szValue,int nValueLen) = 0;
};

struct IRedisLogInfoListener
{
    virtual void REDIS_CLIENT_API OnLogPrint( const char *szLogInfo, int nLogLen) = 0;
};


struct IRedisClientObj
{
    virtual int REDIS_CLIENT_API openRedis()=0;
    virtual int REDIS_CLIENT_API closeRedis()=0;
    //返回内容szValue内存空间为接口调用方申请；
    //nValueLen为传入最大空间长度，返回实际字符串长度
    virtual int REDIS_CLIENT_API getRedisKV(const char *szKey,char *szValue,int &nValueLen)=0;
    virtual int REDIS_CLIENT_API setRedisKV(const char *szKey,const char *szValue,int ValueLen,int nTimeOut=24*60*60)=0;

    //设置客户端消息队列消费者名称
    //默认名称为接口调用方本地ip;
    //接口中消费组和消费者名称设置一致
    //同名消费者竞争获取队列消息
    virtual int REDIS_CLIENT_API setConsumerName(const char *szConsumerName)=0;

     //redis消息队列接口
    virtual int REDIS_CLIENT_API PutQueueMsg(const char *szQueName,const char *szValue,int ValueLen)=0;
    // 订阅消息对列接口，一个客户端可支持订阅多个消息队列，一个消息队列仅支持一个监听对象，
    virtual int REDIS_CLIENT_API SubscribeQueueMsg(const char *szQueName,IRedisQueueListener *pListener)=0;
    virtual int REDIS_CLIENT_API UnsubscribeQueueMsg(const char *szQueName,IRedisQueueListener *pListener)=0;
    
    virtual int REDIS_CLIENT_API SetLogInfoListener(IRedisLogInfoListener *pLogListener)=0;
    //设置连接超时时间，默认3.5秒?
	virtual int REDIS_CLIENT_API SetConnectTimeout(int nSeconds,int nMicroseconds)=0;
	//设置读写超时时间默认500毫秒
	virtual int REDIS_CLIENT_API SetRWTimeout(int nSeconds,int nMicroseconds)=0;

};

extern "C" {
//dll获取实例对象接口
IRedisClientObj* REDIS_CLIENT_API GetRedisBaseObj(const char *szip, int port, const char *szpwd, int dbname);
void REDIS_CLIENT_API ReleaseRedisBaseObj(IRedisClientObj* pObj);
}

#endif
