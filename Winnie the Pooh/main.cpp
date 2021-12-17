#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <limits>

int *Forest; //Массив для леса.

//Функция для провеки наличия медведя в секторе.
void IsWinnieHere(int sector, int isHere, std::string &message) {
if (isHere == 1) {
    message = "Winnie was located in sector " + std::to_string(sector + 1) + " and violently punished";
}
else {
    message = "Winnie was not located in sector: " + std::to_string(sector + 1);
 }
}

//Функция для определения положения медведя в лесу.
int randomizeWinnie (int forestSize) {
    srand(time(0));
    int Winnie = rand() %forestSize;
    return Winnie;
}

//Функция для ввода пользователем кол-ва секторов, на которые делится лес.
int getSectors() {
    std::string input;
    printf("Enter the nuber of sectors in the forest: ");
    std::cin >> input;
    //Блок, определяющий не ввёл ли пользователь слово или сочетание слов вместо числа.
    try {
        int sectors = std::stoi(input);
    }
    catch(std::invalid_argument const& ex) {
        throw std::invalid_argument("Input must contains olny numbers");
    }
    int sectors = std::stoi(input);
    //Блок, определяющй корректность введённого числа
    if ((sectors < 1) or (sectors > 50000)) {
        throw std::invalid_argument("Number of sectors must be bigger than 0 and lesser than 10000");
    }
    return sectors;
}
int main() {
    int forestSize = getSectors();
    Forest = new int[forestSize];
    int Winnie = randomizeWinnie(forestSize);
    for (int i = 0; i < forestSize; i++) {
        Forest[i] = 0;
    }
    Forest[Winnie] = 1;

    std::thread *swarms[forestSize]; //рой - он же массив стай пчёл
    std::string messages[forestSize]; //массив, моделирующий сбор информации от каждой стаи в единый носитель

    //Создание потоков для стай пчёл и определение сектора для каждой стаи
    for (int i=0 ; i<forestSize ; i++) {
        swarms[i] = new std::thread{IsWinnieHere, i, Forest[i], std::ref(messages[i])};
    }

    //Завершение работы потоков с проверкой на то, точно ли поток завершил работу.
    for (int i=0 ; i<forestSize ; i++) {
        swarms[i]->join();
        delete swarms[i];
    }

    //Вывод сообщений от каждой стаи пчёл на экран. К сожалению, без него сообщения нечитаемы.
    for (int i=0; i < forestSize; i++) {
        printf("%s\n", messages[i].c_str());
    }

    /*Вывод, проверяющий корректность работы пчёл
     * (а то вдруг они проглядели медведя или приняли за Винни кого-то другого) */
    std::cout << "Winnie actually in sector " << Winnie + 1 << std::endl;
    delete[] Forest;
    return 0;
}
