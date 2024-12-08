#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>

using namespace std;

class FlappyBird {
private:
    sf::RenderWindow window;
    sf::CircleShape bird;
    sf::RectangleShape ground;
    std::vector<sf::RectangleShape> pipes;
    float gravity, velocity, birdSpeed, jumpHeight;
    bool isJumping, gameOver;
    int score;
    sf::Font font;
    sf::Text scoreText;

public:
    FlappyBird() : window(sf::VideoMode(800, 600), "Flappy Bird"), gravity(0.5f), velocity(0), birdSpeed(5), jumpHeight(-10), score(0), gameOver(false) {
        bird.setRadius(20);
        bird.setFillColor(sf::Color::Yellow);
        bird.setPosition(100, window.getSize().y / 2);

        ground.setSize(sf::Vector2f(window.getSize().x, 100));
        ground.setPosition(0, window.getSize().y - ground.getSize().y);
        ground.setFillColor(sf::Color(200, 200, 200));

        if (!font.loadFromFile("arial.ttf")) {
            cout << "Error loading font!" << endl;
            exit(1);
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(window.getSize().x / 2 - 50, 20);
    }

    void spawnPipe() {
        int height = rand() % 300 + 100;
        sf::RectangleShape topPipe(sf::Vector2f(50, height));
        topPipe.setFillColor(sf::Color::Green);
        topPipe.setPosition(window.getSize().x, 0);
        pipes.push_back(topPipe);

        sf::RectangleShape bottomPipe(sf::Vector2f(50, window.getSize().y - height - 150));
        bottomPipe.setFillColor(sf::Color::Green);
        bottomPipe.setPosition(window.getSize().x, height + 150);
        pipes.push_back(bottomPipe);
    }

    void draw() {
        window.clear(sf::Color::SkyBlue);
        window.draw(bird);
        window.draw(ground);

        for (auto &pipe : pipes) {
            window.draw(pipe);
        }

        scoreText.setString("Score: " + to_string(score));
        window.draw(scoreText);

        window.display();
    }

    void update() {
        if (isJumping) {
            velocity = jumpHeight;
            isJumping = false;
        }

        velocity += gravity;
        bird.move(0, velocity);

        // Handle pipe movement
        for (auto &pipe : pipes) {
            pipe.move(-5, 0);
        }

        if (!pipes.empty() && pipes.front().getPosition().x < -50) {
            pipes.erase(pipes.begin(), pipes.begin() + 2);  // Remove passed pipes
            spawnPipe();
            score++;
        }

        // Collision detection
        if (bird.getPosition().y + bird.getRadius() * 2 >= window.getSize().y - ground.getSize().y || bird.getPosition().y <= 0) {
            gameOver = true;
        }

        // Check for collision with pipes
        for (auto &pipe : pipes) {
            if (pipe.getGlobalBounds().intersects(bird.getGlobalBounds())) {
                gameOver = true;
            }
        }
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !gameOver) {
                isJumping = true;
            }
        }
    }

    void run() {
        srand(time(0));

        spawnPipe();
        spawnPipe();

        while (window.isOpen()) {
            processEvents();
            if (gameOver) {
                break;
            }
            update();
            draw();
            sf::sleep(sf::milliseconds(10));
        }

        // Game Over
        window.clear(sf::Color::Black);
        sf::Text gameOverText("Game Over", font, 50);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(window.getSize().x / 2 - 120, window.getSize().y / 2 - 50);
        window.draw(gameOverText);
        window.display();
        sf::sleep(sf::seconds(3));
        window.close();
    }
};

int main() {
    FlappyBird game;
    game.run();
    return 0;
}
