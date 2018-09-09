#pragma once
#include <switch.h>

class HosMutex {
    private:
        Mutex m;
    public:
        HosMutex() {
            mutexInit(&this->m);
        }
        
        void lock() {
            mutexLock(&this->m);
        }
        
        void unlock() {
            mutexUnlock(&this->m);
        }
        
        bool try_lock() {
            return mutexTryLock(&this->m);
        }
};

class HosRecursiveMutex {
    private:
        RMutex m;
    public:
        HosRecursiveMutex() {
            rmutexInit(&this->m);
        }
        
        void lock() {
            rmutexLock(&this->m);
        }
        
        void unlock() {
            rmutexUnlock(&this->m);
        }
        
        bool try_lock() {
            return rmutexTryLock(&this->m);
        }
};

class HosCondVar {
    private:
        CondVar cv;
        Mutex m;
    public:
        HosCondVar() {
            mutexInit(&m);
            condvarInit(&cv);
        }
        
        Result WaitTimeout(u64 timeout) {
            return condvarWaitTimeout(&cv, &m, timeout);
        }
        
        Result Wait() {
            return condvarWait(&cv, &m);
        }
        
        Result Wake(int num) {
            return condvarWake(&cv, num);
        }
        
        Result WakeOne() {
            return condvarWakeOne(&cv);
        }
        
        Result WakeAll() {
            return condvarWakeAll(&cv);
        }
};

class HosSemaphore {
    private:
        Semaphore s;
    public:
        HosSemaphore() {
            semaphoreInit(&s, 0);
        }
        
        HosSemaphore(u64 c) {
            semaphoreInit(&s, c);
        }
        
        void Signal() {
            semaphoreSignal(&s);
        }
        
        void Wait() {
            semaphoreWait(&s);
        }
        
        bool TryWait() {
            return semaphoreTryWait(&s);
        }
};
