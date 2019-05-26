#include "ValueMonitor.h"
#include <iostream>
using std::cout;

unsigned ValueMonitor::InstanceCount = 0;
ValueMonitor::ValueMonitor(): m_nId(InstanceCount++) {}

ValueMonitor::~ValueMonitor()
{
    stopObserve();
}

bool ValueMonitor::update(long msg) 
{
    // It's guaranteed that getSubject() returns a non-zero pointer of type ValueEntity*
    ValueEntity *valueEntity = getSubject();
    switch (msg) 
	{
        case ValueEntity::ValueChanged:
            cout << "value of ValueEntity[" << valueEntity->getId()
                << "] observed by ValueMonitor[" << m_nId
                << "] changed to " << valueEntity->getValue() << "\n";
            return true;
        default:
            return false;
    }
}

bool ValueMonitor::init() 
{
    // It's guaranteed that getSubject() returns a non-zero pointer of type ValueEntity*
    ValueEntity *valueEntity = getSubject();
    cout << "ValueMonitor[" << m_nId
            << "] starts observing ValueEntity[" << valueEntity->getId()
            << "] whose value is " << valueEntity->getValue() << "\n";
    return true;
}

bool ValueMonitor::uninit() 
{
    // It's guaranteed that getSubject() returns a non-zero pointer of type ValueEntity*
    ValueEntity *valueEntity = getSubject();
    cout << "ValueMonitor[" << m_nId
            << "] stops observing ValueEntity[" << valueEntity->getId()
            << "] whose value is " << valueEntity->getValue() << "\n";
    return true;
}