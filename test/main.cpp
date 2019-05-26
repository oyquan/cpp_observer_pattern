#include "ValueEntity.h"
#include "ValueMonitor.h"
#include <iostream>
using std::cout;

int main() 
{
    ValueEntity *ve = new ValueEntity(0);
    ValueMonitor *vm1 = new ValueMonitor;
    ve->addObserver(vm1);	// equivalent to vm1->startObserve(ve);
    ValueMonitor *vm2 = new ValueMonitor;
    vm2->startObserve(ve);	// equivalent to ve->addObserver(vm2);
    while (ve->getValue() < 3)
    {
        ve->setValue(ve->getValue()+1);
    }
    delete vm2;
    vm2 = nullptr;
    delete vm1;
    vm1 = nullptr;
    delete ve;
    ve = nullptr;

    return 0;
}
