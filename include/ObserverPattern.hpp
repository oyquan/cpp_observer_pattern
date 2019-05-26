#ifndef OBSERVERPATTERN_HPP
#define OBSERVERPATTERN_HPP

#include <list>
#include <algorithm>

template<typename T>
class Observer;     // forward declaration

template<typename T>
class Subject       // abstract template class
{
    public:
        Subject();                                              // constructor
        virtual ~Subject() = 0;                                 // destrctor, pure virtual
        bool addObserver(Observer<T> *observer);				// add one observer
        bool removeObserver(Observer<T> *observer);				// remove one observer
        const std::list<Observer<T>*>& getObservers() const;    // get list of observers

    protected:
        void notify(long msg) const;                            // notify all observers

    private:
        Subject(const Subject&) = delete;						// disable copy from left value
		Subject(const Subject&&) = delete;						// disable copy from right value
        Subject& operator=(const Subject&) = delete;			// disable assign from left value
		Subject& operator=(const Subject&&) = delete;			// disable assign from right value
        std::list<Observer<T>*> m_observers;                    // list of observers
};



template<typename T>
class Observer      // abstract template class
{
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
        Observer(const Observer&) = delete;						// disable copy from left value
		Observer(const Observer&&) = delete;					// disable copy from right value
        Observer& operator=(const Observer&) = delete;			// disable assign from left value
		Observer& operator=(const Observer&&) = delete;			// disable assign from right value
        T *m_subject;                                           // the subject observed
};



template<typename T>
Subject<T>::Subject() {}

template<typename T>
Subject<T>::~Subject()
{
    while (!m_observers.empty())
    {
        removeObserver(m_observers.back()); // remove observers in reverse order
    }
}

template<typename T>
bool Subject<T>::addObserver(Observer<T> *observer)
{
    if (!observer)
    {
        return false;                               // nullptr, invalid argument
    }

    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (it != m_observers.end())
    {
        return false;                               // already in the list of observers
    }

    // order matters, push_back MUST occurs before startObserve
    m_observers.push_back(observer);
    observer->startObserve(static_cast<T*>(this));  // static_cast is safe here
    return true;
}

template<typename T>
bool Subject<T>::removeObserver(Observer<T> *observer)
{
    if (!observer)
    {
        return false;                               // nullptr, invalid argument
    }

    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (it == m_observers.end())
    {
        return false;                               // not in the list of observers
    }

    // order matters, erase MUST occurs before stopObserve
    m_observers.erase(it);
    observer->stopObserve(static_cast<T*>(this));   // static_cast is safe here
    return true;
}

template<typename T>
const std::list<Observer<T>*>& Subject<T>::getObservers() const
{
    return m_observers; // list of observers
}

template<typename T>
void Subject<T>::notify(long msg) const
{
	for (auto it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->update(msg);	// notify observers in observing order
	}
}



template<typename T>
Observer<T>::Observer(): m_subject(nullptr) {}

template<typename T>
Observer<T>::~Observer() 
{
    // Observer itself MUST not invoke stopObserve in its destrctor,
    // otherwise uninit of base class gets invoked, not that of derived class
}

template<typename T>
T* Observer<T>::getSubject() const
{
    return m_subject;               // the subject observed
}

template<typename T>
bool Observer<T>::startObserve(T *subject) 
{
    if (!subject || (m_subject == subject))
    {
        return false;               // nullptr or already observing
    }

    if (m_subject)
    {
        stopObserve(m_subject);     // stop observing the subject if any
    }

    // order matters, assignment MUST occurs before addObserver
    m_subject = subject;
    m_subject->addObserver(this);	// no cast required
    init();                         // initialize after start observing
    return true;
}

template<typename T>
bool Observer<T>::stopObserve(T *subject)
{
    if (!subject || (m_subject != subject))
    {
        return false;               // nullptr or not observing
    }

    uninit();                       // uninitialize before stop observing
    // order matters, assignment MUST occurs before removeObserver
    m_subject = nullptr;
    subject->removeObserver(this);	// no cast required
    return true;
}

template<typename T>
void Observer<T>::stopObserve()
{
    // Observer itself MUST not invoke stopObserve in its destrctor,
    // otherwise uninit of base class gets invoked, not that of derived class
    // all non-abstract derived classes MUST invoke stopObserve in their destrctors,
    // otherwise dangling pointer may occurs
    if (m_subject)
    {
       stopObserve(m_subject);      // stop observing the subject
    }
}

template<typename T>
bool Observer<T>::init()
{
    return false;                   // default do nothing
}

template<typename T>
bool Observer<T>::uninit()
{
    return false;                   // default do nothing
}

#endif // OBSERVERPATTERN_HPP
