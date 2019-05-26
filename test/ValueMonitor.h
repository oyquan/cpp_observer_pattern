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
        static unsigned InstanceCount;
        unsigned m_nId;
};

#endif // VALUEMONITOR_H
