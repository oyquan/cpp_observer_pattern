#include "ValueEntity.h"
#include <iostream>
using std::cout;

unsigned ValueEntity::InstanceCount = 0;
ValueEntity::ValueEntity(int nValue): m_nValue(nValue), m_nId(InstanceCount++) {}

int ValueEntity::getValue() const 
{
    return m_nValue;
}

unsigned ValueEntity::getId() const 
{
    return m_nId;
}

void ValueEntity::setValue(int nValue) 
{
    if (m_nValue != nValue) 
	{
        cout << "value of ValueEntity[" << m_nId 
				<< "] changed from " << m_nValue 
				<< " to " << nValue << "\n";
        m_nValue = nValue;
        notify(ValueChanged);
    }
}
