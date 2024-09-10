#include <iostream>
#include <sstream>
#include <string>

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/RollingFileAppender.hh>

#include "Configuration.h"

using std::string;

/*DEBUG*/
#define LOG_DEBUG(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->debug(oss.str().data()); \
}while(0);

/*INFO*/
#define LOG_INFO(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->info(oss.str().data()); \
}while(0);

/*NOTICE*/
#define LOG_NOTICE(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->notice(oss.str().data()); \
}while(0);

/*WARN*/
#define LOG_WARN(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->warn(oss.str().data()); \
}while(0);

/*ERROR*/
#define LOG_ERROR(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->error(oss.str().data()); \
}while(0);

/*CRIT*/
#define LOG_CRIT(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->crit(oss.str().data()); \
}while(0);

/*ALERT*/
#define LOG_ALERT(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->alert(oss.str().data()); \
}while(0);

/*FATAL*/
#define LOG_FATAL(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->fatal(oss.str().data()); \
}while(0);

/*EMERG*/
#define LOG_EMERG(msg) \
do{\
    std::ostringstream oss; \
    oss << "文件名：" << __FILE__ << "    函数名：" << __FUNCTION__<< "    行数：" << __LINE__ << "    " << msg; \
    MyLogger::getInstance()->emerg(oss.str().data()); \
}while(0);


/*类*/
class MyLogger{
public:
    enum LogLevel{
        DEBUG, INFO, NOTICE, WARN, ERROR, CRIT, ALERT, FATAL, EMERG
    };
private:
    log4cpp::Category & _logger;
    static MyLogger *_pInstance;
public:
    /*静态成员函数：用于获取类实例*/
    static MyLogger *getInstance();


    /*用于记录日志的函数*/
    void debug(const char *msg);
    void info(const char *msg);
    void notice(const char *msg);
    void warn(const char *msg);
    void error(const char *msg);
    void crit(const char *msg);
    void alert(const char *msg);
    void fatal(const char *msg);
    void emerg(const char *msg);

    /*删除拷贝构造*/
    MyLogger (const MyLogger & rhs) = delete;
    /*删除赋值运算符函数*/
    MyLogger &operator=(const MyLogger & rhs) = delete;
    static LogLevel toEnum(const string&);
private:
    /*构造函数*/
    MyLogger(LogLevel logLevel, bool isCout, const string & filePath, const string & rollingPath);
    /*析构函数*/
    ~MyLogger(void);
    /*静态成员函数：用于销毁类实例*/
    static int destroyInstance(void);
};
