#include "threadpool.hpp"
#include <iostream>

int main(){

    ThreadPool tp;
    for(int i=0; i<100; ++i){
        tp.submit(
            [=](){std::cout << i <<". hello from thread id "
                            << std::this_thread::get_id();}
            );

    }
  return 0;
}
