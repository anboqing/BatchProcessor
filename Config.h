#ifndef _CONFIG_H_
#define _CONFIG_H_

namespace mmpay{

    class Config {

        public:
            const char* GetInputFilePath() {
                return "/home/anboqing/code/batch_processing/data/input.txt"; 
            }

            const char* GetOutputFilePath(){
                return "/home/anboqing/code/batch_processing/data/"; 
            }

            unsigned int GetBatchNumConfig(){
                return 10; 
            }

            unsigned int GetBatchSizeConfig(){
                return 20; 
            }

            unsigned int GetMaxRetryCount(){
                return 3; 
            }

            unsigned int GetRetryInterval(){
                return 3; 
            }

    };
}

#endif
