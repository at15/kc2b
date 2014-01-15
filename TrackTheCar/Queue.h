#ifndef Queue_h
#define Queue_h

template <class T>
class Queue{
    public:
    virtual bool isEmpty() = 0;
    virtual void enQueue(const T &x) = 0;
    virtual T deQueue() = 0;
    virtual T getHead() = 0;
    virtual ~Queue() {}
};

#endif
