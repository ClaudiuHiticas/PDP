#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <fstream>
#include <queue>
#include <tuple>
#include <chrono>
#include <string>



using namespace std;
using namespace std::chrono;

const int maxn = 1005;
const string data[] = {"mult0.in", "mult1.in", "mult2.in", "mult3.in", "mult4.in", "mult5.in", "mult6.in"};

mutex mx;
condition_variable cv;
queue <tuple<int, int, int>> q;
const int T[] = {1, 3, 5, 10, 25, 50, 100};

int noElemLine, a[maxn][maxn], b[maxn][maxn], c[maxn][maxn], rez[maxn][maxn];
bool finished;

string fullPath = "/Users/claudiuhiticas/Desktop/University/PPD/Lab 4/Lab 4/";

void loadData(string name) {
    ifstream fin(fullPath + name);
    fin >> noElemLine;
    cout << noElemLine << endl;
//    cout << endl;
    
    for(int i = 0; i < noElemLine; ++ i)
        for(int j = 0; j < noElemLine; ++ j)
        fin >> a[i][j];


    for(int i = 0; i < noElemLine; ++ i)
        for(int j = 0; j < noElemLine; ++ j)
            fin >> b[i][j];


    for(int i = 0; i < noElemLine; ++ i)
        for(int j = 0; j < noElemLine; ++ j)
            fin >> c[i][j];
    fin.close();
}


inline void producer(int line, int T)
{
    for(int i = line; i < noElemLine; i+=T)
    {
        for(int j = 0; j < noElemLine; ++j)
        {
            int aux_i_j = 0;
            for(int k = 0; k < noElemLine; ++k)
                aux_i_j += a[i][k] * b[k][j];
            
            {
                lock_guard<mutex> lk(mx);
                //cout << "producing (" << i << ", " << j << ", " << aux_i_j << ")\n";
                q.push(make_tuple(i, j, aux_i_j));
                
            }
            cv.notify_all();
        }
    }
}

inline void consumer(int tid, int T)
{
    while(true)
    {
        unique_lock<mutex> lk(mx);
        cv.wait(lk, []{return finished || !q.empty();});
        if(finished)
            break;
        tuple<int, int, int> el = q.front();
        q.pop();
        int i = get<0>(el);
        int j = get<1>(el);
        int x = get<2>(el);
//        cout << x << " ";
        //cout << tid << " consuming (" << i << ", " << j << ", " << x << ")";
        for(int k = 0; k < noElemLine; ++k)
            rez[i][k] += x * c[j][k];
    }
}

int main()
{
    cout << "size, number_of_threads, duration\n";
    for(int di = 6; di < 7; ++di)
    {
//        std::this_thread::sleep_for(std::chrono::milliseconds(200));
//        for(int i = 0; i < maxn; ++i)
//            for(int j = 0; j < maxn; ++j)
//                rez[i][j] = 0;
        
        loadData(data[di]);
        for(int tj = 0; tj < 7; ++tj)
        {
            //cout << di << " " << noElemLine << " " << T[tj] << "\n";
            int noTh = T[tj];
            
            auto start = chrono::high_resolution_clock::now();
            
            vector<thread> producers, consumers;
            
            for(int i = 0; i < min(noElemLine, noTh); ++ i)
            {
                producers.push_back(thread(producer, i, noTh));
            }
            
            for(int i = 0; i < min(noElemLine, noTh); ++ i)
            {
                consumers.push_back(thread(consumer, i, noTh));
            }
            
            for(int i = 0; i < producers.size(); ++ i)
            {
                producers[i].join();
            }
            {
                lock_guard<mutex> lk(mx);
                finished = true;
            }
            cv.notify_all();
            for(int i = 0; i < consumers.size(); ++ i)
            {
                consumers[i].join();
            }

            cout << '\n';
            for(int i = 0; i < noElemLine; ++ i)
            {
                for(int j = 0; j < noElemLine; ++ j)
                {
                    cout << rez[i][j] << ' ';
                }
                cout << '\n';
            }
            auto finish = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = finish - start;
            
            cout << noElemLine << ", " << noTh << ", " << elapsed.count() << "\n";

        }
            
    }
}
