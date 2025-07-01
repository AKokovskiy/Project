#include <iostream>
#include <string>
#include <cctype>
#include <clocale>
#include <fstream>
#include <algorithm>
#include <vector>

struct Registr {
    std::string Login;
    int Parol;
};

struct Histori {
    std::string histori;
};

void saveUserToFile(const Registr& user) {
    std::ofstream outFile("users.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << "Логин: " << user.Login << ", Пароль: " << user.Parol << "\n";
        outFile.close();
        std::cout << "Ваши данные сохранены!\n";
    }
    else {
        std::cout << "Ошибка: Не удалось открыть файл users.txt\n";
    }
}

void saveHistoryToFile(const Histori& history) {
    std::ofstream outFile("history.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << history.histori << "\n";
        outFile.close();
    }
    else {
        std::cout << "Ошибка: Не удалось открыть файл history.txt\n";
    }
}

std::vector<std::string> readUserHistoryFromFile(const std::string& username) {
    std::vector<std::string> userHistory;
    std::ifstream inFile("history.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            if (line.find(username) != std::string::npos) {
                userHistory.push_back(line);
            }
        }
        inFile.close();
    }
    if (userHistory.empty()) {
        userHistory.push_back("История для пользователя " + username + " пока пуста.");
    }
    return userHistory;
}

std::string trim(const std::string& str) {
    std::string result = str;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char c) { return !std::isspace(c); }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), result.end());
    return result;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

void showMenu() {
    std::cout << "\nВыберите действие:\n";
    std::cout << "1. Добавить блюдо\n";
    std::cout << "2. Просмотреть свою историю\n";
    std::cout << "3. Выйти\n";
    std::cout << "Введите номер действия (1-3): ";
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::string otvet;
    bool validInput = false;
    Registr user;

    while (!validInput) {
        std::cout << "Привет, пользователь!\n"
            << "Ты уже зарегистрирован? (Ответ: yes/no)\n";
        std::getline(std::cin, otvet);

        otvet = trim(otvet);
        otvet = toLower(otvet);

        if (otvet.empty()) {
            std::cout << "Пустой ввод. Пожалуйста, введите 'yes' или 'no'.\n";
            continue;
        }

        if (otvet == "yes") {
            validInput = true;
            std::cout << "Отлично! Добро пожаловать!\n";
            std::cout << "Введите логин: ";
            std::getline(std::cin, user.Login);
            std::cout << "Введите пароль: ";
            std::cin >> user.Parol;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Вход выполнен! Логин: " << user.Login << ", Пароль: " << user.Parol << "\n";

            Histori history;
            history.histori = "Пользователь " + user.Login + " вошёл в систему";
            saveHistoryToFile(history);
        }
        else if (otvet == "no") {
            validInput = true;
            std::cout << "Пожалуйста, зарегистрируйся!\n";
            std::cout << "Создайте логин: ";
            std::getline(std::cin, user.Login);
            std::cout << "Создайте пароль: ";
            std::cin >> user.Parol;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Регистрация успешна! Логин: " << user.Login << ", Пароль: " << user.Parol << "\n";

            saveUserToFile(user);
            Histori history;
            history.histori = "Новый пользователь " + user.Login + " зарегистрирован";
            saveHistoryToFile(history);
        }
        else {
            std::cout << "Неизвестный ответ. Пожалуйста, введите 'yes' или 'no'.\n";
        }
    }

    bool exitProgram = false;
    while (!exitProgram) {
        showMenu();
        std::string choice;
        std::getline(std::cin, choice);
        choice = trim(choice);

        if (choice == "1") {
            std::string dish;
            std::cout << "Введите название блюда: ";
            std::getline(std::cin, dish);
            dish = trim(dish);

            if (!dish.empty()) {
                Histori history;
                history.histori = "Пользователь " + user.Login + " добавил блюдо: " + dish;
                saveHistoryToFile(history);
                std::cout << "Блюдо '" << dish << "' добавлено в историю.\n";
            }
            else {
                std::cout << "Пустой ввод. Блюдо не добавлено.\n";
            }
        }
        else if (choice == "2") {
            std::vector<std::string> history = readUserHistoryFromFile(user.Login);
            std::cout << "\n--- Ваша история действий ---\n";
            for (const auto& entry : history) {
                std::cout << entry << "\n";
            }
            std::cout << "----------------------------\n";
        }
        else if (choice == "3") {
            exitProgram = true;
            Histori history;
            history.histori = "Пользователь " + user.Login + " вышел из системы";
            saveHistoryToFile(history);
            std::cout << "Программа завершена.\n";
        }
        else {
            std::cout << "Неверный выбор. Пожалуйста, введите число от 1 до 3.\n";
        }
    }

    return 0;
}