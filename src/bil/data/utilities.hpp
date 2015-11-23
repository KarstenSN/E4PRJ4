
// Scoped locker anvendes i dataklassen til at forhindre flere tråde i at skrive i dataen på samme tid
class ScopedLocker{
public:
    ScopedLocker(pthread_mutex_t* theLock){
        this->SL = theLock;
        pthread_mutex_lock(SL);
    }
    
    ~ScopedLocker(){
        pthread_mutex_unlock(SL);
    }
private:
    pthread_mutex_t* SL;
};

// Denne struct anvendes til at håndtere brugeren input.
struct userInput {
    char forwared;
    char reverse;
    char turn;
    char stop;
};