#ifndef _BATCHPROCESSOR_H_
#define _BATCHPROCESSOR_H_
#include "Config.h"
#include "databatch.h"
#include <string>

#include <iostream>
using namespace std;

namespace mmpay{

    /*
     * 对处理业务逻辑的抽象
     * */
    class BatchProcessor {
        public:
            BatchProcessor(DataBatch databatch);
            ~BatchProcessor();

            Config* GetConfig() {
                return m_poConfig; 
            }
            int Execute();

        protected:

            /**
             * interface for real process logic
             */
            virtual int ProcessOne(const std::string& item,std::string result) = 0;

            /**
             * 
             */
            virtual int PreProcess(); 

            /**
             * Do Some Log & Report
             */
            virtual int AfterProcess();

        private:
            Config *m_poConfig;
            DataBatch m_BatchData;
    };


    BatchProcessor :: BatchProcessor(DataBatch databatch) :
        m_poConfig(new Config()),
        m_BatchData(databatch)
    {

    }

    BatchProcessor :: ~BatchProcessor() {
        delete m_poConfig; 
    }


    int BatchProcessor :: PreProcess() {

        return 0;
    }

    int BatchProcessor :: AfterProcess() {
        // TODO 把处理结果存盘
        return 0; 
    }

    int BatchProcessor :: Execute() {
        int iRet = 0; 
        iRet = PreProcess();
        if(iRet) {
            cout << "error PreProcess" << endl;  
            return iRet;
        }

        // TODO : 如果是IO密集型，这里可以改为多线程
        for(unsigned int i = 0; i<m_BatchData.GetBatchSize(); ++i){
            
            std::string& strInputItem = m_BatchData.m_InputDataContainer[i];
            std::string& strOutputItem = m_BatchData.m_OutPutDataContainer[i];

            iRet =  ProcessOne(strInputItem, strOutputItem);
            if (iRet) {
                cout << "error DoProcess" << endl;  
                // TODO 这里要记录成功和失败的，并且要重试
                return iRet;
            } 
        }

        iRet = AfterProcess();
        if (iRet) {
            cout << "error AfterProcess" << endl;  
            return iRet;
        }

        return iRet;
    }


};
#endif
