#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <deque>

using namespace std;

class SnakeGame {
private:
    int width, height;
    int score;
    int x, y, fruitX, fruitY;
    bool gameOver;
    int direction;
    deque<pair<int, int>> snake;

public:
    SnakeGame(int w, int h) : width(w), height(h), score(0), gameOver(false) {
        srand(time(0));
        x = width / 2;
        y = height / 2;
        snake.push_front({x, y});
        generateFruit();
    }

    void generateFruit() {
        fruitX = rand() % width;
        fruitY = rand() % height;
    }

    void draw() {
        clear();
        for (int i = 0; i < width + 2; i++) {
            mvprintw(0, i, "#");
            mvprintw(height + 1, i, "#");
        }
        for (int i = 0; i < height; i++) {
            mvprintw(i + 1, 0, "#");
            mvprintw(i + 1, width + 1, "#");
        }
        mvprintw(1, width / 2 - 5, "Score: %d", score);

        // Draw fruit
        mvprintw(fruitY + 1, fruitX + 1, "F");

        // Draw snake
        for (auto &segment : snake) {
            mvprintw(segment.second + 1, segment.first + 1, "O");
        }

        refresh();
    }

    void input() {
        int ch = getch();
        switch(ch) {
            case KEY_UP: direction = 0; break;
            case KEY_RIGHT: direction = 1; break;
            case KEY_DOWN: direction = 2; break;
            case KEY_LEFT: direction = 3; break;
            case 'q': gameOver = true; break;
        }
    }

    void logic() {
        pair<int, int> head = snake.front();
        switch (direction) {
            case 0: head.second--; break; // up
            case 1: head.first++; break; // right
            case 2: head.second++; break; // down
            case 3: head.first--; break; // left
        }

        if (head.first < 0 || head.first >= width || head.second < 0 || head.second >= height) {
            gameOver = true;
        }

        for (auto &segment : snake) {
            if (segment == head) {
                gameOver = true;
            }
        }

        snake.push_front(head);

        if (head.first == fruitX && head.second == fruitY) {
            score += 10;
            generateFruit();
        } else {
            snake.pop_back();
        }
    }

    bool isGameOver() {
        return gameOver;
    }

    void run() {
        initscr();
        timeout(100);
        curs_set(0);
        keypad(stdscr, TRUE);

        while (!isGameOver()) {
            draw();
            input();
            logic();
        }

        endwin();
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game(20, 20);
    game.run();
    return 0;
}
