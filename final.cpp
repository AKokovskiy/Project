#include <iostream>
#include <string>
#include <cctype>
#include <clocale>
#include <fstream>
#include <algorithm>
#include <vector>
#include <fstream>

struct Registr {
    std::string Login;
    int Parol;
};

struct Histori {
    std::string histori;
};

struct Dish {
    std::string dish;
    int caloryes;
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

bool isDishInBase(const std::string& dish_name) {
    Dish food;
    std::vector<Dish> dish;
    std::ifstream file("Dish.txt");
    file >> food.dish >> food.caloryes;
    if (file.is_open()) {
        while (file >> food.dish >> food.caloryes) {
            dish.push_back(food);
        }
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        return 1;
    }
    for (const auto& p : dish) {
        if (p.dish == dish_name) {
            return true;
        }
    }
}

int get_calo(const std::string& dish_name) {
    Dish food;
    std::vector<Dish> dish;
    std::ifstream file("Dish.txt");
    file >> food.dish >> food.caloryes;
    if (file.is_open()) {
        while (file >> food.dish >> food.caloryes) {
            dish.push_back(food);
        }
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        return 1;
    }
    for (const auto& p : dish) {
        if (p.dish == dish_name) {
            return p.caloryes;
        }
    }
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

int sum_calorues(int sum, int dish) {
    return (sum + dish);
}

void showMenu() {
    std::cout << "\nВыберите действие:\n";
    std::cout << "1. Добавить блюдо\n";
    std::cout << "2. Просмотреть свою историю\n";
    std::cout << "3. Вывести сумму калорий\n";
    std::cout << "4. Выйти\n";
    std::cout << "Введите номер действия (1-4): ";
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::string otvet;
    bool validInput = false;
    int sum = 0;
    Registr user;

    while (!validInput) {
        std::cout << "Привет, пользователь!\n"
            << "Ты уже зарегистрирован? (Ответ: y/n)\n";
        std::getline(std::cin, otvet);

        otvet = trim(otvet);
        otvet = toLower(otvet);

        if (otvet.empty()) {
            std::cout << "Пустой ввод. Пожалуйста, введите 'yes' или 'no'.\n";
            continue;
        }

        if (otvet == "y") {
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
        else if (otvet == "n") {
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
    std::string choice;
    while (!exitProgram) {
        showMenu();
        std::getline(std::cin, choice);
        choice = trim(choice);
        int gramm = 0;

        if (choice == "1") {
            std::string dish;
            std::cout << "Введите название блюда: ";
            std::getline(std::cin, dish);
            dish = trim(dish);

            if (!dish.empty()) {
                if ((isDishInBase(dish) == true)) {
                    std::cout << "Сколько грамм блюда вы съели? :";
                    std::cin >> gramm;
                    if (gramm > 0) {
                        Histori history;
                        history.histori = "\nПользователь " + user.Login + " добавил блюдо: " + dish + "в количеств: " + std::to_string(gramm) + "грамм\n";
                        saveHistoryToFile(history);
                        std::cout << "Блюдо '" << dish << "' добавлено в историю.\n";
                        sum = sum_calorues(sum, (get_calo(dish)) * (gramm / 100));
                        std::getline(std::cin, choice);
                    }
                    else
                    {
                        std::cout << "Введено недопустимое значение веса. Блюдо не добавлено.\n";
                    }
                }
                else
                {
                    std::cout << "Блюда нет в базе данных. Блюдо не добавлено.\n";
                }
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
            std::cout << sum;
        }
        else if (choice == "4") {
            exitProgram = true;
            Histori history;
            history.histori = "Пользователь " + user.Login + " вышел из системы";
            saveHistoryToFile(history);
            std::cout << "Программа завершена.\n";
        }
        else {
            std::cout << "Неверный выбор. Пожалуйста, введите число от 1 до 4.\n";
        }
    }

    return 0;
}