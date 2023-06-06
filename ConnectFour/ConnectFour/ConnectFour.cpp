#include "ConnectFour.h"

Board::Board()
{
    board.resize(BOARD_ROWS, std::vector<int>(BOARD_COLS, 0));
}

void Board::draw(sf::RenderWindow& window)
{
    window.clear(BACKGROUND_COLOR);

    sf::RectangleShape borderRect;
    borderRect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    borderRect.setOutlineThickness(BORDER_SIZE);
    borderRect.setOutlineColor(BORDER_COLOR);
    borderRect.setFillColor(BORDER_COLOR);

    animatedCircle.setRadius((CELL_SIZE - 2 * BORDER_SIZE) / 2.0f);

    for (int row = 0; row < BOARD_ROWS; row++)
    {
        for (int col = 0; col < BOARD_COLS; col++)
        {
            float x = col * CELL_SIZE;
            float y = row * CELL_SIZE;

            borderRect.setPosition(x, y);
            animatedCircle.setPosition(x + BORDER_SIZE, y + BORDER_SIZE);

            if (board[row][col] == 1)
            {
                animatedCircle.setFillColor(PLAYER1_COLOR);
                animatedCircle.setOutlineColor(sf::Color(220, 220, 0)); 
                animatedCircle.setOutlineThickness(-9.0f); 
            }
            else if (board[row][col] == 2)
            {
                animatedCircle.setFillColor(PLAYER2_COLOR);
                animatedCircle.setOutlineColor(sf::Color(220, 0, 0)); 
                animatedCircle.setOutlineThickness(-9.0f); 
            }
            else
            {
                animatedCircle.setFillColor(sf::Color::Black);
                animatedCircle.setOutlineColor(sf::Color(50, 50, 50)); 
                animatedCircle.setOutlineThickness(-3.0f); 
            }

            window.draw(borderRect);
            window.draw(animatedCircle);
        }
    }

    window.display();
}

void Board::animateDrop(int col, int player, sf::RenderWindow& window)
{
    int row = -1;

    while (row < BOARD_ROWS - 1 && board[row + 1][col] == 0)
    {
        row++;
        window.clear(BACKGROUND_COLOR);
        draw(window);
        float x = col * CELL_SIZE;
        float y = row * CELL_SIZE;
        animatedCircle.setPosition(x + BORDER_SIZE, y + BORDER_SIZE);

        if (player == 1)
        {
            animatedCircle.setFillColor(PLAYER1_COLOR);
        }
        else if (player == 2)
        {
            animatedCircle.setFillColor(PLAYER2_COLOR);
        }

        window.draw(animatedCircle);
        window.display();
        sf::sleep(sf::milliseconds(static_cast<int>(CELL_SIZE / animationSpeed * 50)));
    }
    makeMove(col, player);
}

bool Board::isColumnFull(int col) const
{
    return board[0][col] != 0;
}

bool Board::isBoardFull()
{
    for (int col = 0; col < BOARD_COLS; col++)
    {
        if (!isColumnFull(col))
        {
            return false;
        }
    }
    return true;
}

void Board::makeMove(int col, int player)
{
    for (int row = BOARD_ROWS - 1; row >= 0; row--)
    {
        if (board[row][col] == 0)
        {
            board[row][col] = player;
            break;
        }
    }
}

bool Board::checkWin()
{
    // Provjera vodoravnih linija
    for (int row = 0; row < BOARD_ROWS; row++)
    {
        for (int col = 0; col < BOARD_COLS - 3; col++)
        {
            int player = board[row][col];
            if (player != 0 && player == board[row][col + 1] && player == board[row][col + 2] && player == board[row][col + 3])
            {
                return true;
            }
        }
    }

    // Provjera okomitih linija
    for (int col = 0; col < BOARD_COLS; col++)
    {
        for (int row = 0; row < BOARD_ROWS - 3; row++)
        {
            int player = board[row][col];
            if (player != 0 && player == board[row + 1][col] && player == board[row + 2][col] && player == board[row + 3][col])
            {
                return true;
            }
        }
    }

    // Provjera dijagonalnih linija (/)
    for (int row = 3; row < BOARD_ROWS; row++)
    {
        for (int col = 0; col < BOARD_COLS - 3; col++)
        {
            int player = board[row][col];
            if (player != 0 && player == board[row - 1][col + 1] && player == board[row - 2][col + 2] && player == board[row - 3][col + 3])
            {
                return true;
            }
        }
    }

    // Provjera dijagonalnih linija (\)
    for (int row = 3; row < BOARD_ROWS; row++)
    {
        for (int col = 3; col < BOARD_COLS; col++)
        {
            int player = board[row][col];
            if (player != 0 && player == board[row - 1][col - 1] && player == board[row - 2][col - 2] && player == board[row - 3][col - 3])
            {
                return true;
            }
        }
    }

    return false;
}

Game::Game(GameMode mode) : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Connect 4"), gameMode(mode)
{
    board.draw(window);
}

void Game::run()
{
    while (window.isOpen())
    {
        handleEvents();
    }
}


void Game::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton);
            break;
        default:
            break;
        }
    }
}

void Game::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton)
{
    if (mouseButton.button == sf::Mouse::Left)
    {
        int col = mouseButton.x / CELL_SIZE;
        if (!board.isColumnFull(col))
        {
            board.animateDrop(col, currentPlayer, window);
            currentPlayer = 3 - currentPlayer;

            board.draw(window);

            if (board.checkWin())
            {
                if (gameMode != HumanVsHuman)
                {
                    std::cout << "Human wins!" << std::endl;
                    window.close();
                    return;
                }
                std::cout << "Player " << 3 - currentPlayer << " wins!" << std::endl;
                window.close();
                return;
            }

            if (board.isBoardFull())
            {
                std::cout << "It's a draw!" << std::endl;
                window.close();
                return;
            }

            if (gameMode == HumanVsHuman)
            {
                return;
            }

            // Red je na kompjuteru da odigra potez
            int aiMove = findBestMove(board);
            board.animateDrop(aiMove, currentPlayer, window);
            currentPlayer = 3 - currentPlayer;
            board.draw(window);

            if (board.checkWin())
            {
                std::cout << "Computer wins!" << std::endl;
                window.close();
                return;
            }

            if (board.isBoardFull())
            {
                std::cout << "It's a draw!" << std::endl;
                window.close();
                return;
            }
        }
    }
}

int Game::evaluateBoard(const std::vector<std::vector<int>>& board) const
{
    
    int aiScore = 0;
    int playerScore = 0;

    // Horizontalna evaluacija
    for (int row = 0; row < BOARD_ROWS; row++)
    {
        for (int col = 0; col < BOARD_COLS - 3; col++)
        {
            int window[4] = { board[row][col], board[row][col + 1], board[row][col + 2], board[row][col + 3] };
            aiScore += evaluateWindow(window, 2);
            playerScore += evaluateWindow(window, 1);
        }
    }

    // Vertikalna evaluacija
    for (int col = 0; col < BOARD_COLS; col++)
    {
        for (int row = 0; row < BOARD_ROWS - 3; row++)
        {
            int window[4] = { board[row][col], board[row + 1][col], board[row + 2][col], board[row + 3][col] };
            aiScore += evaluateWindow(window, 2);
            playerScore += evaluateWindow(window, 1);
        }
    }

    // Dijagonalna evaluacija (/)
    for (int row = 3; row < BOARD_ROWS; row++)
    {
        for (int col = 0; col < BOARD_COLS - 3; col++)
        {
            int window[4] = { board[row][col], board[row - 1][col + 1], board[row - 2][col + 2], board[row - 3][col + 3] };
            aiScore += evaluateWindow(window, 2);
            playerScore += evaluateWindow(window, 1);
        }
    }

    // Dijagonalna evaluacija (\)
    for (int row = 3; row < BOARD_ROWS; row++)
    {
        for (int col = 3; col < BOARD_COLS; col++)
        {
            int window[4] = { board[row][col], board[row - 1][col - 1], board[row - 2][col - 2], board[row - 3][col - 3] };
            aiScore += evaluateWindow(window, 2);
            playerScore += evaluateWindow(window, 1);
        }
    }

    return aiScore - playerScore;
}

int Game::evaluateWindow(const int window[4], int player) const
{
    int score = 0;
    int opponent = 3 - player;

    if (window[0] == player)
    {
        if (window[1] == player)
        {
            if (window[2] == player)
            {
                if (window[3] == player) // 4 za redom
                {
                    score += 1001;
                }
                else // 3 za redom
                {
                    score += 100;
                }
            }
            else if (window[2] == 0 && window[3] == player) // Potencijalna 3 za redom
            {
                score += 10;
            }
        }
        else if (window[1] == 0 && window[2] == player && window[3] == player) // Potencijalna 3 za redom
        {
            score += 10;
        }
    }
    else if (window[0] == 0 && window[1] == player && window[2] == player && window[3] == player) // Potencijalna 3 za redom
    {
        score += 10;
    }
    else if (window[0] == 0 && window[1] == 0 && window[2] == player && window[3] == player) // Potencijalna 2 za redom
    {
        score += 1;
    }

    if (window[0] == opponent)
    {
        if (window[1] == opponent)
        {
            if (window[2] == opponent)
            {
                if (window[3] == opponent) // 4 za redom za protivnika
                {
                    score -= 1000;
                }
                else // 3 za redom za protivnika
                {
                    score -= 100;
                }
            }
            else if (window[2] == 0 && window[3] == opponent) // Potencijalna 3 za redom za protivnika
            {
                score -= 10;
            }
        }
        else if (window[1] == 0 && window[2] == opponent && window[3] == opponent) // Potencijalna 3 za redom za protivnika
        {
            score -= 10;
        }
    }
    else if (window[0] == 0 && window[1] == opponent && window[2] == opponent && window[3] == opponent) // Potencijalna 3 za redom za protivnika
    {
        score -= 10;
    }
    else if (window[0] == 0 && window[1] == 0 && window[2] == opponent && window[3] == opponent) // Potencijalna 2 za redom za protivnika
    {
        score -= 1;
    }

    return score;
}

int Game::findBestMove(const Board& currentBoard)
{
    int bestMove = -1;
    int bestScore = std::numeric_limits<int>::min();

    for (int col = 0; col < BOARD_COLS; col++)
    {
        if (!currentBoard.isColumnFull(col))
        {
            Board tempBoard = currentBoard;
            tempBoard.makeMove(col, 2); // Simuliraj potez kompjutera

            int score = minimax(tempBoard, 4, false); // Minimax pretraživanje s dubinom 4

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = col;
            }
        }
    }

    return bestMove;
}

int Game::minimax(Board& currentBoard, int depth, bool isMaximizingPlayer)
{
    if (depth == 0 || currentBoard.isBoardFull() || currentBoard.checkWin())
    {
        return evaluateBoard(currentBoard.board);
    }

    if (isMaximizingPlayer)
    {
        int maxScore = std::numeric_limits<int>::min();

        for (int col = 0; col < BOARD_COLS; col++)
        {
            if (!currentBoard.isColumnFull(col))
            {
                Board tempBoard = currentBoard;
                tempBoard.makeMove(col, 2); // Simuliraj potez kompjutera

                int score = minimax(tempBoard, depth - 1, false);
                maxScore = std::max(maxScore, score);
            }
        }

        return maxScore;
    }
    else
    {
        int minScore = std::numeric_limits<int>::max();

        for (int col = 0; col < BOARD_COLS; col++)
        {
            if (!currentBoard.isColumnFull(col))
            {
                Board tempBoard = currentBoard;
                tempBoard.makeMove(col, 1); // Simuliraj potez igraèa

                int score = minimax(tempBoard, depth - 1, true);
                minScore = std::min(minScore, score);
            }
        }

        return minScore;
    }
}

int main()
{
    int modeChoice;
    std::cout << "Choose a game mode:\n";
    std::cout << "1. Human vs. Computer\n";
    std::cout << "2. Human vs. Human\n";
    std::cin >> modeChoice;

    Game::GameMode gameMode;
    if (modeChoice == 1)
    {
        gameMode = Game::HumanVsComputer;
    }
    else if (modeChoice == 2)
    {
        gameMode = Game::HumanVsHuman;
    }
    else
    {
        std::cout << "Incorrect choice of game mode. Exit the program.\n";
        return 0;
    }

    Game game(gameMode);
    game.run();

    return 0;
}