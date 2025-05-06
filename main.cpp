#include "ComputerClub.hpp"
#include <iostream>
#include <regex>

void checkFormat(std::string& f) {
    std::ifstream file(f, std::ios::app);
    std::string str;

    static std::regex r("^\\d+$");
    std::getline(file, str);
    if (!std::regex_match(str, r)) {
        throw std::runtime_error("Check line 1 Error");
    }

    r = std::regex("^([01]?\\d|2[0-3]):[0-5]\\d ([01]?\\d|2[0-3]):[0-5]\\d$");
    std::getline(file, str);
    if (!std::regex_match(str, r)) {
        throw std::runtime_error("Check line 2 Error");
    }

    r = std::regex("^\\d+$");
    std::getline(file, str);
    if (!std::regex_match(str, r)) {
        throw std::runtime_error("Check line 3 Error");
    }

    while (std::getline(file, str))
    {
        std::regex r("^([01]\\d|2[0-3]):[0-5]\\d (([134] [a-z0-9_\\-]+$)|(2 [a-z0-9_\\-]+ \\d+$))");
        if (!std::regex_match(str, r)) {
            throw std::runtime_error("File Format error");
        }
    }

    file.close();
}

void loadConfig(std::string& f, size_t &tables, std::string &opentime, std::string &closetime, size_t &cost) {
    std::ifstream file(f);
    std::string str;
    file >> str;
    tables = std::stoull(str.c_str(), NULL, 0);

    file >> opentime;
    std::cout << opentime << std::endl;
    file >> closetime;
    

    file >> str;
    cost = std::stoi(str);

    file.close();
}

void processEvent(std::string& f, Club &club) {
    std::ifstream file(f);
    std::string str;
    std::getline(file, str);
    std::getline(file, str);
    std::getline(file, str);

    while (std::getline(file, str)) {
        club.processEvent(str);
    }

    club.closeClub();
}

void main(int argc, char* argv[]) {

    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <input_file> ";
        return;
    }

    std::string file = argv[1];
    size_t tables;
    std::string opentime;
    std::string closetime;
    size_t cost;

    try {
        checkFormat(file);
        loadConfig(file, tables, opentime, closetime, cost);

        Club club(opentime, closetime, tables, cost);
        processEvent(file, club);
    }
    catch (const std::exception& e) {
        std::cerr << "Error " << e.what() << std::endl;
        return;
    }
        

    // Пример входных событий
    //club.processEvent("03:00 1 Alice");
    //club.processEvent("10:05 2 Alice 2");
    //club.processEvent("10:10 1 Bob");
    //club.processEvent("10:15 3 Bob");
    //club.processEvent("10:20 4 Alice");
    //club.processEvent("10:30 4 Bob");

    //club.closeClub(); // Закрываем клуб
}
