#include "../include/log4cpp.h"

MyLogger *MyLogger::_pInstance = nullptr;

/*静态成员函数：用于获取类实例*/
MyLogger *MyLogger::getInstance(){
    if(!_pInstance){
        LogLevel logLevel = toEnum(Configuration::getInstance()->getConfigMap()["LogLevel"]);
        bool isCout = (Configuration::getInstance()->getConfigMap()["IsCout"] == string("true"));
        const string & logPath = Configuration::getInstance()->getConfigMap()["LogPath/"];
        const string & rollingPath = Configuration::getInstance()->getConfigMap()["RollingPath/"];
        _pInstance = new MyLogger(logLevel, isCout, logPath, rollingPath);
    }
    return _pInstance;
}

/*静态成员函数：用于销毁类实例*/
int MyLogger::destroyInstance(void){
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
    return 0;
}

/*用于记录日志的函数*/
void MyLogger::debug(const char *msg){
    _logger.debug(msg);
}
void MyLogger::info(const char *msg){
    _logger.info(msg);
}
void MyLogger::notice(const char *msg){
    _logger.notice(msg);
}
void MyLogger::warn(const char *msg){
    _logger.warn(msg);
}
void MyLogger::error(const char *msg){
    _logger.error(msg); 
}
void MyLogger::crit(const char *msg){
    _logger.crit(msg);
}
void MyLogger::alert(const char *msg){
    _logger.alert(msg);
}
void MyLogger::fatal(const char *msg){
    _logger.fatal(msg);
}
void MyLogger::emerg(const char *msg){
    _logger.emerg(msg);
}


/*构造函数*/
MyLogger::MyLogger(LogLevel logLevel, bool isCout, const string & filePath, const string & rollingPath)
: _logger(log4cpp::Category::getRoot().getInstance("MyLogger"))
{
    using namespace log4cpp;
    /*1、设置布局*/
    PatternLayout *ptn1 = nullptr;
    PatternLayout *ptn2 = nullptr;
    PatternLayout *ptn3 = nullptr;
    if(isCout){
        ptn1 = new PatternLayout();
        ptn1->setConversionPattern("%d %c   %-6p\t>> %m%n");
    }
    if(filePath != string()){
        ptn2 = new PatternLayout();
        ptn2->setConversionPattern("%d %c   %-6p\t>> %m%n");
    }
    if(rollingPath != string()){
        ptn3 = new PatternLayout();
        ptn3->setConversionPattern("%d %c   %-6p\t>> %m%n");
    }

    /*2、设置目的地，并绑定布局*/
    OstreamAppender *pos = nullptr;
    FileAppender *pfile = nullptr;
    RollingFileAppender *proll = nullptr;
    if(isCout){
        pos = new OstreamAppender("console", &std::cout);
        pos->setLayout(ptn1);
    }
    if(filePath != string()){
        pfile = new FileAppender("file", filePath.data());
        pfile->setLayout(ptn2);
    }
    if(rollingPath != string()){
        proll = new RollingFileAppender("rollingfile", rollingPath.data());
        proll->setLayout(ptn3);
    }

    /*3、创建日志记录器*/
    //Category & _logger = Category::getRoot().getInstance("MyLogger");

    /*4、设置优先级*/
    switch(logLevel){
    case DEBUG:
        _logger.setPriority(Priority::DEBUG);
        break;
    case INFO:
        _logger.setPriority(Priority::INFO);
        break;
    case NOTICE:
        _logger.setPriority(Priority::NOTICE);
        break;
    case WARN:
        _logger.setPriority(Priority::WARN);
        break;
    case ERROR:
        _logger.setPriority(Priority::ERROR);
        break;
    case CRIT:
        _logger.setPriority(Priority::CRIT);
        break;
    case ALERT:
        _logger.setPriority(Priority::ALERT);
        break;
    case FATAL: 
        _logger.setPriority(Priority::FATAL);
        break;
    case EMERG:
        _logger.setPriority(Priority::EMERG);
        break;
    default:
        break;
    }
    
    /*5、绑定目的地*/
    if(isCout){
        _logger.addAppender(pos);
    }
    if(filePath != string()){
        _logger.addAppender(pfile);
    }
    if(rollingPath != string()){
        _logger.addAppender(proll);
    }

    return;
}

/*析构函数*/
MyLogger::~MyLogger(void){
    if(_pInstance){
        log4cpp::Category::shutdown();
        destroyInstance();
    }
    return;
}

MyLogger::LogLevel MyLogger::toEnum(const string& str){
    if(string("DEBUG") == str){
        return MyLogger::DEBUG;
    }else if(string("INFO") == str){
        return MyLogger::INFO;
    }else if(string("NOTICE") == str){
        return MyLogger::NOTICE;
    }else if(string("WARN") == str){
        return MyLogger::WARN;
    }else if(string("ERROR") == str){
        return MyLogger::ERROR;
    }else if(string("CRIT") == str){
        return MyLogger::CRIT;
    }else if(string("ALERT") == str){
        return MyLogger::ALERT;
    }else if(string("FATAL") == str){
        return MyLogger::FATAL;
    }else if(string("EMERG") == str){
        return MyLogger::EMERG;
    }else{
        return MyLogger::DEBUG;
    }
}
