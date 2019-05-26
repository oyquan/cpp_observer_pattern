#include "ValueEntity.h"
#include "ValueMonitor.h"
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

int main() 
{
    auto ve = new ValueEntity(0);
    auto vm1 = new ValueMonitor;
    ve->addObserver(vm1);   // equivalent to vm1->startObserve(ve);
    auto vm2 = new ValueMonitor;
    vm2->startObserve(ve);  // equivalent to ve->addObserver(vm2);
    while (ve->getValue() < 3)
    {
        ve->setValue(ve->getValue()+1);
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
    return 0;
}
