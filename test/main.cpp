#include "ValueEntity.h"
#include "ValueMonitor.h"
#include "DualRole.h"
#include <iostream>
using std::cout;

void printObservers(const ValueEntity &valueEntity)
{
    auto observers = valueEntity.getObservers();
    if (observers.size() > 0)
    {
        cout << "number of observers for ValueEntity[" << valueEntity.getId()
                << "]: " << observers.size() << "\n";
    }
    else
    {
        cout << "no observers for ValueEntity[" << valueEntity.getId() << "]\n";
    }
}

void printSubject(const ValueMonitor &valueMonitor)
{
    auto valueEntity = valueMonitor.getSubject();
    if (valueEntity)
    {
        cout << "ValueMonitor[" << valueMonitor.getId()
                << "] observes ValueEntity[" << valueEntity->getId()
                << "]\n";
    }
    else
    {
        cout << "ValueMonitor[" << valueMonitor.getId()
                << "] observes nothing\n";
    }
}

void entityMonitorDemo()
{
    auto ve = new ValueEntity(0);
    auto vm1 = new ValueMonitor;
    ve->addObserver(vm1);   // equivalent to vm1->startObserve(ve);
    auto vm2 = new ValueMonitor;
    vm2->startObserve(ve);  // equivalent to ve->addObserver(vm2);
    while (ve->getValue() < 3)
    {
        ve->setValue(ve->getValue() + 1);
    }

    printObservers(*ve);
    printSubject(*vm1);
    printSubject(*vm2);

    delete vm1;
    vm1 = nullptr;

    printObservers(*ve);
    printSubject(*vm2);

    delete ve;
    ve = nullptr;

    printSubject(*vm2);

    delete vm2;
    vm2 = nullptr;
}

void dualRoleDemo()
{
    // dr3 observes dr2, dr2 observes dr1, dr1 observes dr3
    DualRole dr1(0);
	DualRole dr2(1);
	DualRole dr3(2);
	dr3.startObserve(&dr2);     // equivalent to dr2.addObserver(&dr3);
	dr1.addObserver(&dr2);      // equivalent to dr2.startObserve(&dr1);
	dr1.startObserve(&dr3);     // equivalent to dr3.addObserver(&dr1);
	dr1.setValue(3);
	dr2.setValue(4);
	dr3.setValue(5);
}

int main() 
{
    entityMonitorDemo();
    dualRoleDemo();
    return 0;
}
