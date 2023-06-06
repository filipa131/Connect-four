#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <iostream>
#include <limits>
#include <SFML/Graphics.hpp>
#include <vector>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int BOARD_ROWS = 6;
const int BOARD_COLS = 7;
const int CELL_SIZE = 80;
const int BORDER_SIZE = 5;
const sf::Color BACKGROUND_COLOR = sf::Color::Black;
const sf::Color BORDER_COLOR = sf::Color::Blue;
const sf::Color PLAYER1_COLOR = sf::Color::Yellow;
const sf::Color PLAYER2_COLOR = sf::Color::Red;

class Board
{
public:
    float animationSpeed = 200.0f; 
    sf::CircleShape animatedCircle; 

    Board();

    void draw(sf::RenderWindow& window);

    void animateDrop(int col, int player, sf::RenderWindow& window);

    bool isColumnFull(int col) const;

    bool isBoardFull();

    void makeMove(int col, int player);

    bool checkWin();

    std::vector<std::vector<int>> board;
};

class Game
{
public:
    enum GameMode { HumanVsComputer, HumanVsHuman };

    Game(GameMode mode);

    void run();

private:
    sf::RenderWindow window;
    Board board;
    GameMode gameMode;
    int currentPlayer = 1;

    void handleEvents();

    void handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton);

    int evaluateBoard(const std::vector<std::vector<int>>& board) const;

    int evaluateWindow(const int window[4], int player) const;

    int findBestMove(const Board& currentBoard);

    int minimax(Board& currentBoard, int depth, bool isMaximizingPlayer);
};

#endif CONNECTFOUR_H