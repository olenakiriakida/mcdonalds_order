#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

using namespace std;

struct Order 
{
    string dishName;
    int cookTime; 
};

class OrderProcessor 
{
private:
    queue<Order> orderQueue; 
    mutex mtx; 
    vector<string> availableDishes = { "bigmac", "french fries", "coca cola" };
    const int maxCookingCapacity = 10; 

public:
    
    void addOrder(const Order& order)
    {
        mtx.lock();
        orderQueue.push(order);
        mtx.unlock();
    }

    
    void processOrders() 
    {

        while (!orderQueue.empty()) 
        {

            mtx.lock();
            if (orderQueue.empty()) 
            {

                mtx.unlock();
                continue;

            }

            Order order = orderQueue.front();
            orderQueue.pop();
            mtx.unlock();

            
            auto it = find(availableDishes.begin(), availableDishes.end(), order.dishName);

            if (it != availableDishes.end()) 
            {
               
                cout << "Приготовлено: " << order.dishName << "\n";
                
                this_thread::sleep_for(chrono::seconds(order.cookTime));
                cout << "Страва '" << order.dishName << "' готова, приємного апетиту!\n";
            }
            else 
            {
                
                cout << "Страву '" << order.dishName << "' не можна приготувати.\n";
            }
        }
    }
};

int main() 
{
    OrderProcessor processor;

    processor.addOrder({ "bigmac", 3 });
    processor.addOrder({ "coca cola", 5 });
    processor.addOrder({ "shaverma", 4 }); 

    thread orderThread(&OrderProcessor::processOrders, &processor);

    orderThread.join();

    return 0;
}