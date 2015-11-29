#ifndef _INCL_UTILITIES_HPP
#define _INCL_UTILITIES_HPP

//ScopedLocker should be removed because it doesn’t work.
class ScopedLocker{
public:
    ScopedLocker(pthread_mutex_t* theLock){
        std::cout << "Warning: ScopedLocker doesn’t work. Use std::lock_guard." << std::endl;
        this->SL = theLock;
        pthread_mutex_lock(SL);
    }
    
    ~ScopedLocker(){
        pthread_mutex_unlock(SL);
    }
private:
    pthread_mutex_t* SL;
};

// This struct is used for handling user input.
struct UserInput {
    char forward;
    char reverse;
    char turn;
    char stop;
};

#endif