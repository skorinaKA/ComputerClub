#ifndef COMPUTER_CLUB
#define COMPUTER_CLUB

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <algorithm>

class Client {
public:
    std::string name;
    int tableNumber = -1; // -1 означает, что клиент не за столом

    Client() = default;
    Client(std::string& name) : name(name) {}
};

struct Time
{
    int min=0;
    int hour=0;

    Time sub(Time t) {

    }
};

class Table {
public:
    size_t hours = 0;
    size_t earnmoney{ 0 }; //сколько стол заработал
    std::tm totaltime{ 0 }; //сколько столом пользовались
    std::tm starttime{ 0 }; //начало работы

    Table() = default;
    Table(std::tm start);
    void setTime(std::tm time);
    void calculate(std::tm time);
};

class Club {
private:
    std::tm opentime{0}; //время открытия
    std::tm closetime{ 0 }; //время закрытия
    size_t cost{ 0 }; //стоимость стола за час
    int totalTables{ 0 }; // Общее количество столов
    std::unordered_map<std::string, Client> clients; // Хранит клиентов по имени
    std::map<int, Table> tables; // Информация о столах
    std::queue<std::string> waitingQueue; // Очередь ожидания
    std::set<int> occupiedTables; // Занятые столы

    void prtintMassage(std::string_view message, std::string &time);

public:
    Club() = default;
    Club(std::string opentime, std::string closetime, size_t tables, size_t cost);
    void processEvent(const std::string& event);
    void closeClub(); // Закрыть клуб и освободить всех клиентов
};

#endif // !COMPUTER_CLUB