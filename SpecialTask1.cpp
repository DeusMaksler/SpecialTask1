#include <iostream>
#include <deque>
using namespace std;

void show_surface(bool** surf, int size) {
    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size; row++) cout << surf[col][row] << '\t';
        cout << '\n';
    }
}

void first_generation(bool** surf, int size) {
    short segment = 0;
    cout << "Выберите плотность заселения в первом поколении(1-6): ";
    cin >> segment;
    short square = size * size;
    short quantity = square * segment / 10;

    //получение случайных координат для заселения
    srand(time(0));
    while (quantity > 0) {
        short cage = (rand() % square);
        if (surf[cage / size][cage % size] == false) {
            surf[cage / size][cage % size] = true;
            quantity--;
        }
    }
}

short get_environment_index(bool** surf, int size, int row, int col) {
    short index = 0;
    if (surf[(row - 1 + size) % size][(col + size) % size]) index++;
    if (surf[(row + 1 + size) % size][(col + size) % size]) index++;
    if (surf[(row + size) % size][(col - 1 + size) % size]) index++;
    if (surf[(row + size) % size][(col + 1 + size) % size]) index++;
    if (surf[(row - 1 + size) % size][(col - 1 + size) % size]) index++;
    if (surf[(row - 1 + size) % size][(col + 1 + size) % size]) index++;
    if (surf[(row + 1 + size) % size][(col - 1 + size) % size]) index++;
    if (surf[(row + 1 + size) % size][(col + 1 + size) % size]) index++;

    return index;
}

bool alive(bool** surf, int size) {
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) if (surf[i][j]) return true;
    return false;
}

bool similar(bool** first, bool** second, int size) {
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) if (first[i][j] != second[i][j]) return false;
    return true;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    cout << '\t' << "Создание плоскости модели" << '\n';

    int size = 0;
    do {
        cout << "Задайте размер плоскости(не более 50): ";
        cin >> size;
        if (size > 50 || size < 1) cout << "Недопустимое значение";
        else break;
    } while (true);

    //создание плоскости модели
    bool** surface = new bool* [size];
    for (int i = 0; i < size; i++) surface[i] = new bool[size];
    for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) surface[i][j] = false;

    first_generation(surface, size);
    int gen_number = 1;
    cout << '\n' << '\t' << gen_number++ << " поколение" << '\n';
    show_surface(surface, size);

    bool loop_flag = true;
    deque<bool**> generations;

    do {
        generations.push_back(surface);
        surface = new bool* [size];
        for (int i = 0; i < size; i++) surface[i] = new bool[size];
        for (int i = 0; i < size; i++) for (int j = 0; j < size; j++) {
            short environment_index = get_environment_index(generations.back(), size, i, j);
            if (environment_index == 2) surface[i][j] = generations.back()[i][j];
            else if (environment_index == 3) surface[i][j] = true;
            else surface[i][j] = false;
        }

        cout << '\n' << '\t' << gen_number++ << " поколение" << '\n';
        show_surface(surface, size);

        if (!alive(surface, size)) {
            cout << "Моделирование завершено, на поле не осталось ни одной «живой» клетки";
            loop_flag = false;
            break;
        }
        else for (int i = generations.size() - 1; i > -1; i--) if (similar(surface, generations.at(i), size)) {
                cout << "Моделирование завершено, сложилась стабильная конфигурация";
                loop_flag = false;
                break;
        }
    } while (loop_flag);
}    
