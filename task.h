#ifndef _TASK_H_
#define _TASK_H_
#include "databatch.h"
#include "Config.h"
#include <vector>
#include <sstream>

#include <iostream>
using namespace std;

namespace mmpay{

    // 封装失败重试策略
    struct RetryStrategy{
        unsigned int max_retry_count; 
        unsigned int retry_interval; // ms
        std::vector<unsigned int> clear_err_codes; // 可选，提供明确错误码，遇到不再重试
    };

    /*
     * 对一批要处理的数据任务的抽象
     * */
    class Task{

        public:

            Task(const char* pszInputFilePath,unsigned int dwTaskNum) : 
                m_dwBatchNum(dwTaskNum),
                m_strInputFilePath(pszInputFilePath){
                _BuildTask();    
            }

            Task(Config* poConfig): m_poConfig(poConfig){
                // TODO build task from config 
            }

        public:

            unsigned int GetBatchNum() {
                return m_dwBatchNum; 
            }
            // TODO 这里要改成复制一份
            DataBatch GetDataBatch(unsigned int idx){
                if( idx < m_vecDataBatch.size() ) {
                    return m_vecDataBatch[idx];
                }
                return DataBatch(); 
            }

        private:
            /**
             * 分割为N个文件构造一个Task
             */
            int _BuildTask();
            /**
             * 把大文件按行数（元素个数）均分为
             */
            int _SplitInputFile();

        private:

            unsigned int m_dwBatchNum;            // 并发执行单元个数N
            std::vector<DataBatch> m_vecDataBatch; // 切分好的N个 数据块
            RetryStrategy m_retryStrategy;
            Config* m_poConfig;
            std::string m_strInputFilePath;

            std::vector<std::string> m_vecBatchDataFilenames;

    };

    int Task :: _BuildTask() {
        int iRet = 0;
        if( m_strInputFilePath.empty() ) {
            if(m_poConfig == nullptr){
                std::cout << "err : no config and inputpath" << std::endl;
                return -1; 
            } 
            m_strInputFilePath = m_poConfig->GetInputFilePath();
        }

        iRet = _SplitInputFile();
        if(iRet){
            std::cout << "err: split file" <<endl; 
            return -2;
        }

        // build DataBatch 这里只是把文件名设置，真正读取数据延迟到线程启动调用DataBatch::Init
        for(int i = 0; i< m_vecBatchDataFilenames.size() ; ++i){
            DataBatch databatch(m_vecBatchDataFilenames[i]);
            m_vecDataBatch.push_back(databatch);
        }

        // TODO 读取重试策略，没配置就设置默认值
        m_retryStrategy.max_retry_count = m_poConfig->GetMaxRetryCount();
        m_retryStrategy.retry_interval = m_poConfig->GetRetryInterval();

        return iRet; 
    }

    int Task::_SplitInputFile(){
        int iRet= 0;
        string strOutputFilePath;
        if(m_poConfig != nullptr){
            strOutputFilePath = m_poConfig->GetOutputFilePath();
        }else {
            strOutputFilePath = "/home/anboqing/code/batch_processing/data"; 
        }
        std::ifstream inputStream(m_strInputFilePath);

        // TODO 修复最后一块会丢失问题
        // split into small files;
        unsigned int dwBatchSize = m_poConfig->GetBatchSizeConfig();

        // split file into small pieces;
        char lineBuf[1024];
        std::vector<std::string> veclines;
        unsigned int count = 0;
        int batch_count = 0;
        while(inputStream.getline(lineBuf,sizeof(lineBuf))){
            if(count < dwBatchSize){
                veclines.push_back(lineBuf); 
                ++count;
            } 
            if (count == dwBatchSize) {
                std::ostringstream oss(strOutputFilePath);
                oss << batch_count;

                m_vecBatchDataFilenames.push_back(oss.str());

                std::ofstream outPutStream(oss.str());

                for(int i = 0 ;i< veclines.size() ;++i){
                    outPutStream << veclines[i] << std::endl; 
                }

                outPutStream.close();
                veclines.clear(); 
                count = 0;
                ++batch_count;
            }
        }

        return iRet; 
    }
}

#endif
