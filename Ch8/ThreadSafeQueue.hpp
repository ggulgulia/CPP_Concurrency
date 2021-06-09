#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <thread>

template<typename T>
class ThreadSafeQueue{
    private:
        std::queue<std::shared_ptr<T>> q_;
        std::mutex m_;
        std::condition_variable cv_;
    public:
        ThreadSafeQueue() = default;
        ThreadSafeQueue(const ThreadSafeQueue& other):
        q_{other.q_}{   }
        
        void push(T& value){
            std::lock_guard<std::mutex> lg{m_};
            q_.push(std::make_shared<T>(value));
            cv_.notify_one();
        }

        std::shared_ptr<T> pop(){
            std::lock_guard lg{m_};
            std::shared_ptr<T> res{nullptr};
            if(!q_.empty()){
                res = q_.pop();
            }
            return res;
        }

        bool empty(){
            std::lock_guard<std::mutex> lg{m_};
            return q_.empty();
        }

        std::shared_ptr<T> wait_pop(){
            std::unique_lock<std::mutex> ul{m_};
            cv_.wait(ul, [this]{return !q_.empty();});
            auto res = q_.front();
            q_.pop();
            return res;
        }
        auto size(){
            std::lock_guard<std::mutex> lg{m_};
            return q_.size();
        }

        bool wait_pop(T& ref){
             std::unique_lock<std::mutex> ul{m_};
            cv_.wait(ul, [this]{return !q_.empty();});
            ref = q_.front().get();
            q_.pop();
            return true;
        }

        bool pop(T& ref){
            std::lock_guard lg{m_};
            bool res{false};
            if(!q_.empty()){
                ref = q_.front().get();
                q_.pop();
                res = true;

            }
            return res;
        }
        
};

#endif //THREADSAFEQUQEUE
