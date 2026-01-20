#include <iostream>
#include <vector>
using namespace std;

class BaseFrogger {
public:
    virtual void run() = 0;
};

class Menu {
public:
    void show() {
        cout << "========== FROGGER ==========\n";
        cout << "Sterowanie: W A S D + ENTER\n";
        cout << "Q - wyjscie\n";
        cout << "=============================\n\n";
    }
};

class Road {
public:
    static const int width = 11;
    static const int height = 14;

    char board[height][width];

    void clear() {
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                board[y][x] = ' ';
    }

    void drawSafetyZones() {
        for (int x = 0; x < width; x++) {
            board[0][x] = '-';
            board[height - 1][x] = '-';
        }
    }

    void print(int hp, int points) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++)
                cout << board[y][x];
            cout << endl;
        }
        cout << "\nHP: " << hp << "   Punkty: " << points << endl;
    }
};

class Frog {
public:
    int x, y;
    int hp;
    int points;

    Frog() {
        hp = 3;
        points = 0;
        reset();
    }

    void reset() {
        x = Road::width / 2;
        y = Road::height - 1;
    }

    void move(char input) {
        if (input == 'w' && y > 0) y--;
        if (input == 's' && y < Road::height - 1) y++;
        if (input == 'a' && x > 0) x--;
        if (input == 'd' && x < Road::width - 1) x++;
    }
};

class Car {
public:
    int x, y;
    int length;
    bool left;

    Car(int row, bool dir) {
        y = row;
        left = dir;
        length = 3;
        if (left) {
            x = Road::width - 1;
        } 
        else {
            x = 0;
        }
    }

    void move(int speed) {
        if (left) x -= speed;
        else x += speed;

        if (x < -length) x = Road::width - 1;
        if (x >= Road::width) x = -length;
    }

    bool hit(int fx, int fy) {
        if (fy != y) return false;
        return (fx >= x && fx < x + length);
    }
};

class FroggerGame : public BaseFrogger {
    Road road;
    Frog frog;
    vector<Car> cars;
    int speed;

public:
    FroggerGame() {
        speed = 1;
        for (int i = 1; i < Road::height - 1; i++)
            cars.push_back(Car(i, i % 2));
    }

    void run() {
        char input;

        while (frog.hp > 0) {
            road.clear();
            road.drawSafetyZones();

            for (int i = 0; i < (int)cars.size(); i++) {
                cars[i].move(speed);
                if (cars[i].hit(frog.x, frog.y)) {
                    frog.hp--;
                    frog.reset();
                }
            }

            road.board[frog.y][frog.x] = 'F';
            for (int i = 0; i < (int)cars.size(); i++) {
                for (int j = 0; j < cars[i].length; j++) {
                    int px = cars[i].x + j;
                    if (px >= 0 && px < Road::width)
                        road.board[cars[i].y][px] = '#';
                }
            }

            road.print(frog.hp, frog.points);

            cout << "\nRuch (w/a/s/d, q = wyjscie): ";
            cin >> input;
            if (input == 'q') break;

            frog.move(input);

            if (frog.y == 0) {
                frog.points++;
                if (speed < 2) speed++;
                frog.reset();
            }
        }

        cout << "\nKONIEC GRY! Wynik: " << frog.points << endl;
    }
};


int main() {
    Menu menu;
    menu.show();

    FroggerGame game;
    game.run();
    return 0;
}



