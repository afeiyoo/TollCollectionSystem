#ifndef IBIZSERVICE_H
#define IBIZSERVICE_H

/*后端业务处理接口*/

#include <QtCore>

class IBizService
{
public:
    //交易请求（输入json字符串，返回json字符串)
    virtual QString reqTrade(QString jsonData)=0;
};

//未来作为动态库的函数输出，开发时直接源码调用(无参数)
extern "C" IBizService* getBizService();

#endif // IBIZSERVICE_H
