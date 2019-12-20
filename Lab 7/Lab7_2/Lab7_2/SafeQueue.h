//
//  SafeQueue.h
//  Lab7_2
//
//  Created by Claudiu Hiticas on 12/10/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//

#ifndef SafeQueue_h
#define SafeQueue_h

#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

//using namespace std;

template <class T>
class SafeQueue{
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
public:
    
    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }
    
    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue(void)
    {
      std::unique_lock<std::mutex> lock(m);
      while(q.empty())
      {
        // release lock as long as the wait and reaquire it afterwards.
        c.wait(lock);
      }
      T val = q.front();
      q.pop();
      return val;
    }

};


#endif /* SafeQueue_h */
