#include "DualRole.h"
#include <iostream>
using std::cout;

unsigned DualRole::InstanceCount = 0;
DualRole::DualRole(int nValue): m_nValue(nValue), m_nId(InstanceCount++) {}

DualRole::~DualRole()  
{
    stopObserve();
}

int DualRole::getValue() const 
{
    return m_nValue;
}

unsigned DualRole::getId() const
{
    return m_nId;
}

void DualRole::setValue(int nValue) 
{
    if (m_nValue != nValue) 
    {
        cout << "DualRole[" << m_nId << "] " << m_nValue << "=>" << nValue << "\n";
        m_nValue = nValue;
        notify(ValueChanged);
    }
}

bool DualRole::update(long msg) 
{
    // It's guaranteed that getSubject() returns a non-zero pointer of type DualRole*
    DualRole *dualRole = getSubject();
    switch (msg) 
    {
        case ValueChanged:
            cout << "value of DualRole[" << dualRole->getId()
                    << "] observed by DualRole[" << m_nId 
                    << "] changed to " << dualRole->getValue() << "\n";
            setValue(dualRole->getValue());
            return true;
        default:
            return false;
    }
}

bool DualRole::init() 
{
    // It's guaranteed that getSubject() returns a non-zero pointer of type DualRole*
    DualRole *dualRole = getSubject();
    cout << "DualRole[" << m_nId << "]@" << m_nValue
            << " starts observing DualRole[" << dualRole->getId()
            << "]@" << dualRole->getValue() << "\n";
    setValue(dualRole->getValue());
    return true;
}

bool DualRole::uninit() 
{
    // It's guaranteed that getSubject() returns a non-zero pointer of type DualRole*
    DualRole *dualRole = getSubject();
    cout << "DualRole[" << m_nId << "]@" << m_nValue
            << " stops observing DualRole[" << dualRole->m_nId
            << "]@" << dualRole->getValue() << "\n";
    return true;
}