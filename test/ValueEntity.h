#ifndef VALUEENTITY_H
#define VALUEENTITY_H

#include "ObserverPattern.hpp"

class ValueEntity : public Subject<ValueEntity>
{
    public:
        explicit ValueEntity(int nValue);
        int getValue() const;
        unsigned getId() const;
        void setValue(int nValue);

        // messages
        static const long ValueChanged = 1;

    private:
        static unsigned InstanceCount;  // count of ValueEntity instances
        int             m_nValue;       // state
        const unsigned  m_nId;          // identity
};

#endif // VALUEENTITY_H
