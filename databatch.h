#ifndef _DATABATCH_H_
#define _DATABATCH_H_
#include <vector>
#include <string>
#include <fstream>
#include <string>

#include <iostream>

namespace mmpay{

    /**
     *
     * 对批数据的抽象
     * 将一个文件里的数据分为若干个DataBatch给每个　进程/线程　处理
     *
     */
    class DataBatch{
        public:
            /**
             * load data into Container
             * @ param strBatchFilePath : filename of a batch data
             */
            DataBatch(const std::string& strBatchFilePath): m_strInputFilePath(strBatchFilePath){
                // 根据输入文件名初始化输出文件名            
                // TODO 使用boost库的path
                m_strOutPutFilePath = strBatchFilePath + ".result";
                m_strBreakFilePath = strBatchFilePath + ".breakpoint";
                m_strFailDataFilePath = strBatchFilePath + ".fail";
                m_strSuccessDataFilePath = strBatchFilePath + ".success";
            }

            DataBatch() {
                m_strInputFilePath = ""; 
            }

            // TODO operator=

            ~DataBatch();
            
            DataBatch(const DataBatch& dataBatch);

            int Init();

        public:
            std::ofstream m_OutputFile;
            std::string m_strOutPutFilePath;        // 输出文件路径
            std::vector<std::string> m_OutPutDataContainer; // Output data

            std::ifstream m_InputFile;
            std::vector<std::string> m_InputDataContainer; // Input data
            std::string m_strInputFilePath;        // 输入文件路径

            std::streampos m_breakPoint;              // 断点记录
            std::ofstream m_breakPointFile;           // 断点数据
            std::string m_strBreakFilePath;

            std::ofstream m_SuccessDataFile;             // 成功记录
            std::string m_strSuccessDataFilePath;
            std::ofstream m_FailDataFile;                // 失败记录
            std::string m_strFailDataFilePath;
            //TODO Iterator
        private:
            unsigned int m_batchSize;
            int _readInputData();
        public:
            unsigned int GetBatchSize() {
                return m_batchSize; 
            }
    };

    DataBatch :: DataBatch( const DataBatch& dataBatch){
        // TODO finish 
    }

    DataBatch :: ~DataBatch(){
        m_OutputFile.close();
        m_InputFile.close();
        m_breakPointFile.close();
        m_SuccessDataFile.close();
        m_FailDataFile.close();
    }

    int DataBatch::_readInputData(){
        m_InputFile.open(m_strInputFilePath);
        std::string line;
        while(std::getline(m_InputFile,line)){
            m_InputDataContainer.push_back(line); 
            line.clear();
        }
        return 0; 
    }

    int DataBatch :: Init(){
        int iRet = 0;
        // 读取输入文件
        _readInputData(); 
        // 初始化输出文件
        // TODO 增加检查是否打开
        m_OutputFile.open(m_strOutPutFilePath);
        // 初始化断点文件
        m_breakPointFile.open(m_strBreakFilePath);
        // 初始化成功失败文件
        m_SuccessDataFile.open(m_strSuccessDataFilePath);
        m_FailDataFile.open(m_strFailDataFilePath);
        
        return iRet;
    }

}

#endif
