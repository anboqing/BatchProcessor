#include "scheduler.h"

#include <iostream>
using namespace std;

using namespace mmpay;

int main(){
    int iRet = 0;
    
    Task task("/home/anboqing/code/batch_processing/data/task_data.txt",10);

    Scheduler schduler(&task);
    
    iRet = schduler.Execute();

    return iRet;
}
