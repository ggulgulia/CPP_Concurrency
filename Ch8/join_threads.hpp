#ifndef JOIN_THREADS_H
#define JOIN_THREADS_H

#include <thread>

class join_threads{
    private:
        std::vector<std::thread>& threads_;
    public:
        join_threads() = delete;
        explicit join_threads(std::vector<std::thread>& threads):
        threads_{threads}{  }
        join_threads(const join_threads&) = delete;
        join_threads& operator=(const join_threads&) = delete;
        
        ~join_threads(){
            for(auto& thread : threads_){
                if(thread.joinable())thread.join();
            }
        }
};

#endif//JOIN_THREADS_H
