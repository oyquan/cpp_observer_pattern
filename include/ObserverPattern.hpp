#ifndef OBSERVERPATTERN_HPP
#define OBSERVERPATTERN_HPP

#include <list>
#include <algorithm>

class ObserverImpl;                                             // forward declaration

class SubjectImpl                                               // for implementation of Subject only
{
    public:
        SubjectImpl();                                          // constructor
        virtual ~SubjectImpl() = 0;                             // destrctor, pure virtual
        bool addObserver(ObserverImpl *observer);               // add one observer
        bool removeObserver(ObserverImpl *observer);            // remove one observer
        const std::list<ObserverImpl*>& getObservers() const;   // get list of observers

    protected:
        void notify(long msg) const;                            // notify all observers

    private:
        SubjectImpl(const SubjectImpl&) = delete;               // disable copy from left value
        SubjectImpl(const SubjectImpl&&) = delete;              // disable copy from right value
        SubjectImpl& operator=(const SubjectImpl&) = delete;    // disable assign from left value
        SubjectImpl& operator=(const SubjectImpl&&) = delete;   // disable assign from right value
        std::list<ObserverImpl*> m_observers;                   // list of observers
};

class ObserverImpl                                              // for implementation of Observer only
{
    public:
        ObserverImpl();                                         // constructor
        virtual ~ObserverImpl();                                // destrctor, virtual
        bool startObserve(SubjectImpl *subject);                // start observing the subject
        bool stopObserve(SubjectImpl *subject);                 // stop observing the subject
        virtual bool update(long msg) = 0;                      // react to the change to keep update, pure virtual
        SubjectImpl* getSubject() const;                        // get the subject observed

    protected:
        void stopObserve();                                     // stop observing the subject if any
        virtual bool init() = 0;                                // initialize, invoked after start observing, pure virtual
        virtual bool uninit() = 0;                              // uninitialize, invoked before stop observing, pure virtual

    private:
        ObserverImpl(const ObserverImpl&) = delete;             // disable copy from left value
        ObserverImpl(const ObserverImpl&&) = delete;            // disable copy from right value
        ObserverImpl& operator=(const ObserverImpl&) = delete;  // disable assign from left value
        ObserverImpl& operator=(const ObserverImpl&&) = delete; // disable assign from right value
        SubjectImpl *m_subject;                                 // the subject observed
};

inline SubjectImpl::SubjectImpl() {}

inline SubjectImpl::~SubjectImpl()
{
    while (!m_observers.empty())
    {
        removeObserver(m_observers.back());                     // remove observers in reverse order
    }
}

inline bool SubjectImpl::addObserver(ObserverImpl *observer)
{
    if (!observer)
    {
        return false;                                           // nullptr, invalid argument
    }

    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (it != m_observers.end())
    {
        return false;                                           // already in the list of observers
    }

    // order matters, push_back MUST occurs before startObserve
    m_observers.push_back(observer);
    observer->startObserve(this);
    return true;
}

inline  bool SubjectImpl::removeObserver(ObserverImpl *observer)
{
    if (!observer)
    {
        return false;                                           // nullptr, invalid argument
    }

    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (it == m_observers.end())
    {
        return false;                                           // not in the list of observers
    }

    // order matters, erase MUST occurs before stopObserve
    m_observers.erase(it);
    observer->stopObserve(this);
    return true;
}

inline const std::list<ObserverImpl*>& SubjectImpl::getObservers() const
{
    return m_observers;                                         // list of observers
}

inline void SubjectImpl::notify(long msg) const
{
    for (auto it = m_observers.begin(); it != m_observers.end(); ++it)
    {
        (*it)->update(msg);                                     // notify observers in observing order
    }
}



inline ObserverImpl::ObserverImpl(): m_subject(nullptr) {}

inline ObserverImpl::~ObserverImpl()
{
    // ObserverImpl itself MUST not invoke stopObserve in its destrctor,
    // otherwise uninit of base class gets invoked, not that of derived class.
    // all non-abstract derived classes MUST invoke stopObserve in their destrctors,
    // otherwise dangling pointer may occur.
}

inline SubjectImpl* ObserverImpl::getSubject() const
{
    return m_subject;                                           // the subject observed
}

inline bool ObserverImpl::startObserve(SubjectImpl *subject)
{
    if (!subject || (m_subject == subject))
    {
        return false;                                           // nullptr or already observing
    }

    if (m_subject)
    {
        stopObserve(m_subject);                                 // stop observing the subject if any
    }

    // order matters, assignment MUST occurs before addObserver
    m_subject = subject;
    m_subject->addObserver(this);                               // no cast required
    init();                                                     // initialize after start observing
    return true;
}

inline bool ObserverImpl::stopObserve(SubjectImpl *subject)
{
    if (!subject || (m_subject != subject))
    {
        return false;                                           // nullptr or not observing
    }

    uninit();                                                   // uninitialize before stop observing
    // order matters, assignment MUST occurs before removeObserver
    m_subject = nullptr;
    subject->removeObserver(this);
    return true;
}

inline void ObserverImpl::stopObserve()
{
    // ObserverImpl itself MUST not invoke stopObserve in its destrctor,
    // otherwise uninit of base class gets invoked, not that of derived class.
    // all non-abstract derived classes MUST invoke stopObserve in their destrctors,
    // otherwise dangling pointer may occur.
    if (m_subject)
    {
       stopObserve(m_subject);                                  // stop observing the subject
    }
}

template<typename T>
class Observer;                                                 // forward declaration

template<typename T>
class Subject : private SubjectImpl                             // abstract template class
{
    // static_cast from Subject<T>* to SubjectImpl* in Observer<T>
    friend class Observer<T>;
    public:
        Subject();                                              // constructor
        virtual ~Subject() = 0;                                 // destrctor, pure virtual
        bool addObserver(Observer<T> *observer);                // add one observer
        bool removeObserver(Observer<T> *observer);             // remove one observer
        std::list<Observer<T>*> getObservers() const;           // get list of observers

    protected:
        void notify(long msg) const;                            // notify all observers

    private:
        Subject(const Subject&) = delete;                       // disable copy from left value
        Subject(const Subject&&) = delete;                      // disable copy from right value
        Subject& operator=(const Subject&) = delete;            // disable assign from left value
        Subject& operator=(const Subject&&) = delete;           // disable assign from right value
};



template<typename T>
class Observer : private ObserverImpl                           // abstract template class
{
    // static_cast from Observer<T>* to ObserverImpl* in Subject<T>
    friend class Subject<T>;
    public:
        Observer();                                             // constructor
        virtual ~Observer();                                    // destrctor, virtual
        bool startObserve(T *subject);                          // start observing the subject
        bool stopObserve(T *subject);                           // stop observing the subject
        virtual bool update(long msg) = 0;                      // react to the change to keep update, pure virtual
        T* getSubject() const;                                  // get the subject observed

    protected:
        void stopObserve();                                     // stop observing the subject if any
        virtual bool init();                                    // initialize, invoked after start observing
        virtual bool uninit();                                  // uninitialize, invoked before stop observing

    private:
        Observer(const Observer&) = delete;                     // disable copy from left value
        Observer(const Observer&&) = delete;                    // disable copy from right value
        Observer& operator=(const Observer&) = delete;          // disable assign from left value
        Observer& operator=(const Observer&&) = delete;         // disable assign from right value
};



template<typename T>
Subject<T>::Subject() {}

template<typename T>
Subject<T>::~Subject() {}

template<typename T>
bool Subject<T>::addObserver(Observer<T> *observer)
{
    return SubjectImpl::addObserver(static_cast<ObserverImpl*>(observer));
}

template<typename T>
bool Subject<T>::removeObserver(Observer<T> *observer)
{
    return SubjectImpl::removeObserver(static_cast<ObserverImpl*>(observer));
}

template<typename T>
std::list<Observer<T>*> Subject<T>::getObservers() const
{
    std::list<Observer<T>*> observers;
    for (auto observerImpl : SubjectImpl::getObservers())
    {
        auto observer = static_cast<Observer<T>*>(observerImpl);
        observers.push_back(observer);
    }
    return observers;
}

template<typename T>
void Subject<T>::notify(long msg) const
{
    SubjectImpl::notify(msg);
}



template<typename T>
Observer<T>::Observer() {}

template<typename T>
Observer<T>::~Observer() 
{
    // Observer itself MUST not invoke stopObserve in its destrctor,
    // otherwise uninit of base class gets invoked, not that of derived class.
    // all non-abstract derived classes MUST invoke stopObserve in their destrctors,
    // otherwise dangling pointer may occur.
}

template<typename T>
T* Observer<T>::getSubject() const
{
    // MUST not use reinterpret_cast
    return static_cast<T*>(static_cast<Subject<T>*>(ObserverImpl::getSubject()));
}

template<typename T>
bool Observer<T>::startObserve(T *subject) 
{
    // MUST not use reinterpret_cast
    return ObserverImpl::startObserve(static_cast<SubjectImpl*>(static_cast<Subject<T>*>(subject)));
}

template<typename T>
bool Observer<T>::stopObserve(T *subject)
{
    // MUST not use reinterpret_cast
    return ObserverImpl::stopObserve(static_cast<SubjectImpl*>(static_cast<Subject<T>*>(subject)));
}

template<typename T>
void Observer<T>::stopObserve()
{
    // Observer itself MUST not invoke stopObserve in its destrctor,
    // otherwise uninit of base class gets invoked, not that of derived class.
    // all non-abstract derived classes MUST invoke stopObserve in their destrctors,
    // otherwise dangling pointer may occur.
    ObserverImpl::stopObserve();
}

template<typename T>
bool Observer<T>::init()
{
    return false;                                               // default do nothing
}

template<typename T>
bool Observer<T>::uninit()
{
    return false;                                               // default do nothing
}

#endif // OBSERVERPATTERN_HPP
