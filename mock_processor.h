#include <iostream>
#include "Processor.h"

namespace mmpay{

    class MockProcessor : public BatchProcessor {
        public: 
            MockProcessor(DataBatch databatch): BatchProcessor(databatch){}
        protected:
            virtual int ProcessOne(const std::string& item,std::string result);
    };

    int  MockProcessor :: ProcessOne(const std::string& item, std::string result){
        std::cout << " Processing item :  " << item << std::endl;
        return 0;
    }

}
