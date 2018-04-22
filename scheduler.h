#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "Processor.h"
#include "mock_processor.h"
#include "task.h"

#include <thread>
#include <vector>

#include <iostream>


namespace mmpay {

    void MOCK_LOG(const std::string& msg) {
        std::cout << msg << std::endl; 
    }


    /*
     * 对具体并发方式的抽象
     * 调度N个Task并发执行
     */
    class Scheduler {
        public:
            Scheduler(Task* task);
            /* TODO : 使用不同并发方式并发执行处理
             *
             * */
            int Execute();
        private:
            std::vector<BatchProcessor*> m_vecBatchProcessors;
            Task* m_poTask;
            Config* m_poConfig;
    };

    /* constructor */
    Scheduler :: Scheduler( Task* task) : m_poTask(task){
        unsigned int dwBatchNum = m_poTask->GetBatchNum();
        for(int i = 0; i< dwBatchNum ;  ++i){
            DataBatch oDataBatch = m_poTask->GetDataBatch(i);
            m_vecBatchProcessors.push_back(new MockProcessor(oDataBatch));
        }
    }

    /* execute */
    int Scheduler :: Execute(){
        int iRet = 0;
        MOCK_LOG("begin multiple thread ");

        for(auto iter = m_vecBatchProcessors.begin(); iter != m_vecBatchProcessors.end(); ++iter){
            std::thread t(&BatchProcessor::Execute,*iter);
            t.detach();
        }
        
        return iRet;
    }

}

#endif
