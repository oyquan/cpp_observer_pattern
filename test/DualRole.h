#ifndef DUALROLE_H
#define DUALROLE_H

#include "ObserverPattern.hpp"

class DualRole: public Subject<DualRole>, public Observer<DualRole> 
{
    public:
        explicit DualRole(int nValue);
        ~DualRole();
        int getValue() const;
        unsigned getId() const;
        void setValue(int nValue);
        bool update(long msg) override;

        // messages
        static const long ValueChanged = 1;

    protected:
        bool init() override;
        bool uninit() override;

    private:
        static unsigned InstanceCount;  // count of DualRole instances
        int m_nValue;                   // state
        const unsigned m_nId;           // identity
};

#endif
