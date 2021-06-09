#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <atomic>
#include <functional>

class ThreadPool{
    private:
        ThreadSafeQueue<std::function<void()>> workQueue_;
        std::vector<std::thread> threads_;
        std::atomic<bool> isDone_;
        join_threads joiner_;

        void worker_thread(){
            while(!isDone_){
                if(std::function<void()> task; workQueue_.pop(task)){
                    task();
                }
                else{
                    std::this_thread::yield();
                }
            }
        }
    public:
        ThreadPool(): isDone_{false}, joiner_{threads_}
        {
            const unsigned threadCount = std::thread::hardware_concurrency();
            try{
                for(unsigned i=0; i<threadCount; ++i){
                    threads_.push_back(std::thread{&ThreadPool::worker_thread, this});
                }
            }
            catch(std::exception& e){
                isDone_ = true;
                throw;
            }
        }

        ~ThreadPool(){isDone_ = true;}

        template<typename Function_Type>
        void submit(Function_Type f){
            auto ff = std::function<void()>(f);
            workQueue_.push(ff);
        }

};
