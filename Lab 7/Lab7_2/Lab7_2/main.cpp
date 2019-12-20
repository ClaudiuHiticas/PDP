#include <iostream>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <mutex>
#include "SafeQueue.h"

using namespace std;

void assignRandomValues(int n, vector<string> &nums)
{
    
    for(int i = 0; i < n; ++ i)
    {
        string x;
        int element = rand() % 1000 + 1;
        x = to_string(element);
        reverse(x.begin(), x.end());
        nums.push_back(x);
    }
}

void solve(int st, int dr, vector <string>& nums, SafeQueue<char> &q)
{
    if(st == dr)
    {
        for(int i = 0; i < nums[st].size(); ++ i)
            q.enqueue(nums[st][i]);

        q.enqueue('\0');
        return ;
    }
    
    int mid = (st + dr) / 2;
    SafeQueue<char> q1;
    thread t1([st, mid, &nums, &q1](){solve(st, mid, nums, q1);});
    SafeQueue<char> q2;
    thread t2([mid, dr, &nums, &q2](){solve(mid + 1, dr, nums, q2);});
    
    bool firstStopped = false;
    bool secondStopped = false;
    char T = 0;
    while(true)
    {
        char res = T;
        T = 0;
        if(!firstStopped)
        {
            char e1 = q1.dequeue();
            if(e1 == 0)
                firstStopped = true;
            else
                res += e1 - '0';
        }
        
        
        if(!secondStopped)
        {
            char e2 = q2.dequeue();
            if(e2 == 0)
                secondStopped = true;
            else
                res += e2 - '0';
        }
        
      if(firstStopped && secondStopped)
      {
          if(res > 0)
              q.enqueue(res + '0');
        
          break;
      }
        
      if(res > 9)
      {
          res -= 10;
          T = 1;
      }
        
      q.enqueue(res + '0');
        
    }
    q.enqueue('\0');
    t1.join();
    t2.join();
}


int main()
{
    int n = 10;
    vector <string> nums;
    assignRandomValues(n, ref(nums));
    
//    for(int i = 0; i < n; ++i)
//        cout << nums[i] << " ";
//    cout << endl;
    
    auto start = chrono::high_resolution_clock::now();

    SafeQueue<char> q;
    solve(0, (int) nums.size() - 1, nums, q);
    string sol = "";
    
    while(true)
    {
        char e1 = q.dequeue();
        if(!e1)
        {
            break;
        }
        sol += e1;
    }

    reverse(sol.begin(), sol.end());

    cout << "Sum: " << sol<<"\n";
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;

    cout << "Addition of " << n << " numbers took me "  << elapsed.count() << " seconds\n";
}
