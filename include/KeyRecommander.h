#ifndef __KEY_RECOMMANDER__
#define __KEY_RECOMMANDER__


#include <queue>
#include <string>
#include <memory>
#include <vector>

//class TcpConnection;
//
//using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
/* using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>; */
/* using Functor = function<void()>; */

struct CandidateResult{
    std::string _word;
    int _freq;
    int _dist;
};


template<>
struct std::less<CandidateResult>{
    bool operator()(const CandidateResult& lhs, const CandidateResult& rhs){
        if(lhs._dist == rhs._dist){
            return lhs._freq < rhs._freq;
        }else{
            return lhs._dist > rhs._dist;
        }
    }
};

class KeyRecommander{
public:
    KeyRecommander(std::string key);
    ~KeyRecommander();
    std::string doQuery();
    KeyRecommander(const KeyRecommander&) = delete;
    KeyRecommander& operator=(const KeyRecommander&) = delete;
private:
    int distance(std::string candiate);
private:
    std::string _sought;
    std::priority_queue<CandidateResult> _prique;
    /* TcpConnectionPtr _conn; */
};


#endif
