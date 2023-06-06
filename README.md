# Connect Four
**Connect Four Game using SFML and Minimax AI**

This project is an implementation of the popular game Connect Four using the SFML library. The game allows two players to compete against each other or play against a computer AI. The objective of the game is to connect four discs of the same color in a row, column, or diagonal on a grid.

Features:

- Human vs. Human mode: Two players can take turns dropping their discs on the grid and try to connect four in a row.
- Human vs. Computer mode: Players can challenge a computer AI that uses a minimax algorithm to make intelligent moves.
- Interactive GUI: The game is displayed in a graphical window using the SFML library, providing an intuitive and visually appealing interface.
- Real-time animation: When a disc is dropped into a column, it animates its descent to the bottom of the column, adding a visual element to the gameplay.
- Win detection: The game checks for winning combinations after each move to determine if a player has won the game.
- Board evaluation: The AI uses an evaluation function to assess the current state of the game board and make informed decisions for its moves.
- Game over conditions: The game detects when the board is full, resulting in a draw, or when a player has won, and displays the appropriate message.

The project consists of two main classes: Board and Game. The Board class handles the game logic, including board initialization, drawing the board, making moves, checking for wins, and evaluating the board state. The Game class manages the game flow, such as handling user input, updating the game state, and displaying the game window.


Video representation of the game against the computer:
https://github.com/filipa131/Connect-four/assets/115006963/2b84393b-9df9-4981-8ce9-54e2ca236a2f

