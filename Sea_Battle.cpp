#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_SIZE = 10;
const char EMPTY = '.';
const char SHIP = 'O';
const char HIT = 'X';
const char MISS = '-';

struct Ship {
    int row;
    int col;
    int length;
    bool horizontal; // true, если корабль расположен горизонтально
    vector<pair<int, int>> coordinates; // координаты ячеек корабля
};

// Инициализация игрового поля
void initializeBoard(vector<vector<char>>& board) {
    board.resize(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY));
}

// Вывод поля в консоль
// Параметр revealShips отвечает за то, показывать ли расположение кораблей
void printBoard(const vector<vector<char>>& board, bool revealShips = false) {
    cout << "   ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << j << " ";
    }
    cout << endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i << "  ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            char cell = board[i][j];
            if (!revealShips && cell == SHIP) {
                cout << EMPTY << " ";
            }
            else {
                cout << cell << " ";
            }
        }
        cout << endl;
    }
}

// Проверка возможности размещения корабля указанной длины по горизонтали/вертикали
bool canPlaceShip(const vector<vector<char>>& board, int row, int col, int length, bool horizontal) {
    if (horizontal) {
        if (col + length > BOARD_SIZE) return false;
        for (int j = col; j < col + length; j++) {
            if (board[row][j] != EMPTY) return false;
        }
    }
    else {
        if (row + length > BOARD_SIZE) return false;
        for (int i = row; i < row + length; i++) {
            if (board[i][col] != EMPTY) return false;
        }
    }
    return true;
}

// Размещение корабля на поле и сохранение его координат
void placeShip(vector<vector<char>>& board, Ship& ship) {
    int row = ship.row;
    int col = ship.col;
    int length = ship.length;
    if (ship.horizontal) {
        for (int j = col; j < col + length; j++) {
            board[row][j] = SHIP;
            ship.coordinates.push_back({ row, j });
        }
    }
    else {
        for (int i = row; i < row + length; i++) {
            board[i][col] = SHIP;
            ship.coordinates.push_back({ i, col });
        }
    }
}

// Размещение всех кораблей (для простоты – два корабля)
void placeAllShips(vector<vector<char>>& board, vector<Ship>& ships) {
    // Определяем размеры кораблей – например, один корабль длины 4 и один длины 3
    vector<int> shipSizes = { 4, 3 };
    srand(static_cast<unsigned int>(time(0)));

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            bool horizontal = rand() % 2 == 0;
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            if (canPlaceShip(board, row, col, size, horizontal)) {
                Ship ship;
                ship.row = row;
                ship.col = col;
                ship.length = size;
                ship.horizontal = horizontal;
                placeShip(board, ship);
                ships.push_back(ship);
                placed = true;
            }
        }
    }
}

// Проверка, потоплены ли все корабли
bool allShipsSunk(const vector<Ship>& ships, const vector<vector<char>>& board) {
    for (const Ship& ship : ships) {
        for (auto coord : ship.coordinates) {
            int i = coord.first;
            int j = coord.second;
            if (board[i][j] == SHIP) return false;
        }
    }
    return true;
}

int main() { 

	setlocale(LC_ALL, "Russian");

    vector<vector<char>> board;
    vector<Ship> ships;

    initializeBoard(board);
    placeAllShips(board, ships);

    cout << "Добро пожаловать в 'Морской бой'!" << endl;
    cout << "Ваша задача: потопить все корабли противника." << endl;

    int attempts = 0;
    while (!allShipsSunk(ships, board)) {
        printBoard(board);
        int row, col;
        cout << "Введите координаты выстрела (номер строки и столбца): ";
        cin >> row >> col;

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            cout << "Неверные координаты. Попробуйте снова." << endl;
            continue;
        }
        attempts++;
        if (board[row][col] == SHIP) {
            board[row][col] = HIT;
            cout << "Попадание!" << endl;
        }
        else if (board[row][col] == EMPTY) {
            board[row][col] = MISS;
            cout << "Промах!" << endl;
        }
        else {
            cout << "Вы уже стреляли по этим координатам." << endl;
        }
    }

    cout << "Поздравляем! Вы потопили все корабли за " << attempts << " выстрелов." << endl;
    cout << "Итоговая доска:" << endl;
    printBoard(board, true);

    return 0;
}
