#ifndef VALUEMONITOR_H
#define VALUEMONITOR_H

#include "ObserverPattern.hpp"
#include "ValueEntity.h"

class ValueMonitor: public Observer<ValueEntity>
{
    public:
        ValueMonitor();
        ~ValueMonitor();
        bool update(long msg) override;

    protected:
        bool init() override;
        bool uninit() override;

    private:
        static unsigned InstanceCount;  // count of ValueEntity instances
        unsigned m_nId;                 // identity    
};

#endif // VALUEMONITOR_H
