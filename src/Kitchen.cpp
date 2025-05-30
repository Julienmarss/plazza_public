#include "../includes/Kitchen.hpp"
#include "../includes/ILogger.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <map>

static const std::map<Plazza::PizzaType, std::vector<std::string>> PIZZA_INGREDIENTS = {
    {Plazza::PizzaType::Margarita, {"dough", "tomato", "gruyere"}},
    {Plazza::PizzaType::Regina, {"dough", "tomato", "gruyere", "ham", "mushrooms"}},
    {Plazza::PizzaType::Americana, {"dough", "tomato", "gruyere", "steak"}},
    {Plazza::PizzaType::Fantasia, {"dough", "tomato", "eggplant", "goat_cheese", "chief_love"}}
};

Plazza::Kitchen::Kitchen(int cooks, float multiplier, int refillTime, ILogger &logger)
    : _cooks(cooks), _multiplier(multiplier), _refillTime(refillTime), _logger(logger),
      _maxQueue(cooks * 2), _running(true)
{
    _ingredients = {
        {"dough", 5}, {"tomato", 5}, {"gruyere", 5}, 
        {"ham", 5}, {"mushrooms", 5}, {"steak", 5}, 
        {"eggplant", 5}, {"goat_cheese", 5}, {"chief_love", 5}
    };
    _cookStatus.resize(cooks, false);
    _lastActive = std::chrono::steady_clock::now();
    for (int i = 0; i < cooks; ++i)
        _threads.emplace_back(&Plazza::Kitchen::cookLoop, this, i);
    _threads.emplace_back(&Plazza::Kitchen::ingredientRefillLoop, this);
}

Plazza::Kitchen::~Kitchen()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _running = false;
    }
    _cv.notify_all();
    for (auto& t : _threads)
        if (t.joinable())
            t.join();
}

bool Plazza::Kitchen::isRunning() const
{
    return _running.load();
}

bool Plazza::Kitchen::canAcceptMore() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _orders.size() < _maxQueue && _running;
}

bool Plazza::Kitchen::hasIngredientsFor(const Plazza::PizzaType& type) const
{
    auto it = PIZZA_INGREDIENTS.find(type);
    if (it == PIZZA_INGREDIENTS.end()) return false;
    
    for (const auto& ingredient : it->second) {
        auto stock_it = _ingredients.find(ingredient);
        if (stock_it == _ingredients.end() || stock_it->second <= 0) {
            return false;
        }
    }
    return true;
}

bool Plazza::Kitchen::consumeIngredients(const Plazza::PizzaType& type)
{
    auto it = PIZZA_INGREDIENTS.find(type);
    if (it == PIZZA_INGREDIENTS.end()) return false;
    if (!hasIngredientsFor(type)) return false;

    for (const auto& ingredient : it->second) {
        _ingredients[ingredient]--;
    }
    return true;
}

bool Plazza::Kitchen::pushOrder(const Plazza::PizzaOrder& order)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_running || _orders.size() >= _maxQueue) return false;
    if (!hasIngredientsFor(order.type)) {
        _logger.logError("[ERROR] Not enough ingredients for pizza type: " + std::to_string(static_cast<int>(order.type)));
        return false;
    }
    
    for (int i = 0; i < order.quantity; ++i) {
        _orders.push({order.type, order.size, 1});
    }
    
    _lastActive = std::chrono::steady_clock::now();
    _cv.notify_all();
    return true;
}

size_t Plazza::Kitchen::getQueueSize() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _orders.size();
}

void Plazza::Kitchen::printStatus() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    if (!_running) {
        _logger.logInfo("[CLOSED] Kitchen is shut down");
        return;
    }
    _logger.logInfo("[ACTIVE] Queue " + std::to_string(_orders.size()) + "/" + std::to_string(_maxQueue));
    _logger.logInfo("Cooks: ");
    std::string cooksStatus;
    for (size_t i = 0; i < _cookStatus.size(); ++i) {
        cooksStatus += "C" + std::to_string(i) + "(" + (_cookStatus[i] ? "busy" : "idle") + ") ";
    }
    _logger.logInfo(cooksStatus);
    std::string ingredientsStatus;
    for (const auto& [ingredient, count] : _ingredients) {
        ingredientsStatus += ingredient + ":" + std::to_string(count) + " ";
    }
    _logger.logInfo("Ingredients: " + ingredientsStatus);
}

void Plazza::Kitchen::cookLoop(int id)
{
    while (_running) {
        Plazza::PizzaOrder task;
        bool hasTask = false;

        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait_for(lock, std::chrono::milliseconds(500), [&]() { 
                return !_orders.empty() || !_running; 
            });

            if (!_running) {
                return;
            }
            if (id == 0 && _orders.empty()) {
                bool anyBusy = false;
                for (bool busy : _cookStatus) {
                    if (busy) {
                        anyBusy = true;
                        break;
                    }
                }
                
                if (!anyBusy) {
                    auto now = std::chrono::steady_clock::now();
                    auto inactiveDuration = std::chrono::duration_cast<std::chrono::seconds>(now - _lastActive);
                    if (inactiveDuration.count() >= 5) {
                        _logger.logInfo("[Kitchen" + std::to_string(id) +"] No activity for 5s, shutting down.");
                        _running = false;
                        _cv.notify_all();
                        return;
                    }
                }
            }

            if (!_orders.empty()) {
                task = _orders.front();
                if (consumeIngredients(task.type)) {
                    _orders.pop();
                    _cookStatus[id] = true;
                    hasTask = true;
                    _lastActive = std::chrono::steady_clock::now();
                } else {
                    continue;
                }
            }
        }

        if (hasTask) {
            int cookTime = 0;
            std::string pizzaName;
            switch (task.type) {
                case Plazza::PizzaType::Margarita: 
                    cookTime = 1; 
                    pizzaName = "Margarita";
                    break;
                case Plazza::PizzaType::Regina: 
                    cookTime = 2; 
                    pizzaName = "Regina";
                    break;
                case Plazza::PizzaType::Americana: 
                    cookTime = 2; 
                    pizzaName = "Americana";
                    break;
                case Plazza::PizzaType::Fantasia: 
                    cookTime = 4; 
                    pizzaName = "Fantasia";
                    break;
                default: 
                    cookTime = 1;
                    pizzaName = "Unknown";
                    break;
            }

            int ms = static_cast<int>(cookTime * _multiplier * 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));

            {
                std::lock_guard<std::mutex> lock(_mutex);
                _cookStatus[id] = false;
                _lastActive = std::chrono::steady_clock::now();
            }
            _logger.logInfo("[Cook " + std::to_string(id) + "] pizza ready: " + pizzaName + " in " + std::to_string(ms) + "s");
        }
    }
}

void Plazza::Kitchen::ingredientRefillLoop()
{
    auto lastRefill = std::chrono::steady_clock::now();
    
    while (_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        if (!_running) break;
        
        auto now = std::chrono::steady_clock::now();
        auto timeSinceRefill = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRefill);
        
        if (timeSinceRefill.count() >= _refillTime) {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                
                if (!_running) break;
                for (auto& [ingredient, count] : _ingredients) {
                    count++;
                }
                bool hasOrders = !_orders.empty();
                bool hasBusyCooks = false;
                for (bool busy : _cookStatus) {
                    if (busy) {
                        hasBusyCooks = true;
                        break;
                    }
                }
                
                if (hasOrders || hasBusyCooks) {
                    _logger.logInfo("[Kitchen] Ingredients refilled.");
                }
                
                lastRefill = now;
            }
            
            _cv.notify_all();
        }
    }
}