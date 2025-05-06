#include "ComputerClub.hpp"
#include <iomanip>

Table::Table(std::tm start) {
    this->totaltime.tm_year = 2025 - 1900;
    this->starttime = start;
}

void Table::setTime(std::tm time) {
    this->starttime = time;
}

void Table::calculate(std::tm time) {
    auto total = std::difftime(std::mktime(&time), std::mktime(&starttime));
    this->starttime = time;
    hours += std::ceil(total / 3600);
    totaltime.tm_hour += total / 3600;
    totaltime.tm_min += (int(total) % 3600) / 60;
    if (totaltime.tm_min >= 60) {
        totaltime.tm_min %= 60;
        totaltime.tm_hour++;
    }
}

Club::Club(std::string opentime, std::string closetime, size_t tables, size_t cost):totalTables(tables), cost(cost) {

    this->opentime.tm_year = 2025 - 1900;
    std::istringstream ss(opentime);
    ss >> std::get_time(&this->opentime, "%R");

    this->closetime.tm_year = 2025 - 1900;
    ss = std::istringstream(closetime);
    ss >> std::get_time(&this->closetime, "%R");

    this->totalTables = tables;
    this->cost = cost;

    while (--tables) {
        this->tables[tables] = Table(this->opentime);
    }
}

void Club::prtintMassage(std::string_view message, std::string &time) {
    std::cout << time.c_str() << " 13 " << message << std::endl;
}

void Club::processEvent(const std::string& event) {
    std::istringstream iss(event);
    struct std::tm tm {};
    tm.tm_year = 2025 - 1900;
    std::string action, clientName;
    std::string time;
    int tableNumber;
    std::cout << event << std::endl;

    iss >> std::get_time(&tm, "%R");; // Считываем время
    
    std::strftime(time.data(), 10, "%R", &tm);
    if (tm.tm_hour - this->opentime.tm_hour < 0) {
        prtintMassage("NotOpenYet", time);
        return;
    }

    iss >> action; // Считываем ID события

    if (action == "1") { // Клиент пришел
        iss >> clientName;

        if (clients.find(clientName) != clients.end()) {
            prtintMassage("YouShallNotPass", time);
            return;
        }

        clients[clientName] = Client(clientName);
    }
    else if (action == "2") { // Клиент сел за стол
        iss >> clientName >> tableNumber;

        if (clients.find(clientName) == clients.end()) {
            prtintMassage("ClientUnknown", time);
            return;
        }

        if (clients[clientName].tableNumber == tableNumber) {
            std::cout << time.c_str() << " 12 " << clientName << " " << tableNumber << std::endl; // Смена стола на тот же
            return;
        }

        if (occupiedTables.find(tableNumber) != occupiedTables.end()) {
            prtintMassage("PlaceIsBusy", time);
            return;
        }
        else if(clients.find(clientName) != clients.end() && clients[clientName].tableNumber != -1) {
            tables[clients[clientName].tableNumber].calculate(tm);
            tables[clients[clientName].tableNumber].setTime(tm);
        }

        occupiedTables.insert(tableNumber);
        tables[tableNumber].setTime(tm);
        clients[clientName].tableNumber = tableNumber;
    }
    else if (action == "3") { // Клиент ожидает
        iss >> clientName;

        if (clients.find(clientName) == clients.end()) {
            prtintMassage("ClientUnknown", time);
            return;
        }

        if (occupiedTables.size() < totalTables) {
            prtintMassage("ICanWaitNoLonger", time);
            return;
        }

        waitingQueue.push(clientName);
    }
    else if (action == "4") { // Клиент ушел
        iss >> clientName;

        if (clients.find(clientName) == clients.end()) {
            prtintMassage("ClientUnknown", time);
            return;
        }

        int tableNum = clients[clientName].tableNumber;
        clients.erase(clientName);

        if (tableNum != -1) {
            occupiedTables.erase(tableNum); // Освобождаем стол
            tables[tableNum].calculate(tm);

            if (!waitingQueue.empty()) {
                std::string nextClient = waitingQueue.front();
                waitingQueue.pop();
                clients[nextClient].tableNumber = tableNum;
                occupiedTables.insert(tableNum);
                tables[tableNum].setTime(tm);
                std::cout << time.c_str() << " 12 " << nextClient << " " << tableNum << std::endl; // Следующий клиент сел за стол
            }
        }
    }
}

void Club::closeClub() {
    std::string time;
    std::strftime(time.data(), 10, "%R", &this->closetime);

    for (auto& num : occupiedTables) {
        tables[num].calculate(this->closetime);
    }

    for (auto& [name, client] : clients) {
        std::cout << time.c_str() << " 11 " << name << std::endl; // Клиенты, остающиеся в клубе
    }

    std::cout << time.c_str() << std::endl;

    for (auto& [name, table] : tables) {
        std::strftime(time.data(), 10, "%R", &table.totaltime);
        std::cout << name << " " << table.hours * cost << " " << time.c_str() << std::endl; // Клиенты, остающиеся в клубе
    }

    clients.clear(); // Освобождаем всех клиентов при закрытии клуба
}