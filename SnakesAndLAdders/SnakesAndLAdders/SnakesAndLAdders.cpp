#include <iostream>
#include <map>
#include <string>

class SnakesLadders
{
private:
    std::map<int, int> snakesAndLadders{ {2, 38}, {7, 14}, {8, 31}, {15, 26}, {16, 6}, {21, 42}, {28, 84}, {36, 44}, {46, 25},
        {49, 11}, {51, 67}, {62, 19}, {64, 60}, {71, 91}, {74, 53}, {78, 98}, {87, 94}, {89, 68}, {92, 88}, {95, 75}, {99, 80} 
    };
    int players[2];
    int currentPlayer;
    int winner;

public:
    SnakesLadders() {
        players[0] = 0;
        players[1] = 0;
        currentPlayer = 0;
        winner = -1;
    };
    SnakesLadders(int p1, int p2) {
        players[0] = p1;
        players[1] = p2;
        currentPlayer = 0;
        winner = -1;
    };
    std::string play(int die1, int die2)
    {
        std::string result = "";
        players[currentPlayer] += die1 + die2;
        if (winner >= 0)
            result = "Game over!";
        else
        {
            if (players[currentPlayer] > 100)
                players[currentPlayer] = 200 - players[currentPlayer];
            if (snakesAndLadders.find(players[currentPlayer]) != snakesAndLadders.end())
                players[currentPlayer] = snakesAndLadders[players[currentPlayer]];
            if (players[currentPlayer] < 100)
                result = result.append("Player ").append(std::to_string(currentPlayer + 1))
                .append(" is on square ").append(std::to_string(players[currentPlayer]));
            else
            {
                winner = currentPlayer;
                result = result.append("Player ").append(std::to_string(currentPlayer + 1).append(" Wins!"));
            }
        }
        if (die1 != die2)
            currentPlayer = ++currentPlayer > 1 ? 0 : 1;
        return result;
    };
};

int main()
{
    auto game = SnakesLadders(94, 96);
    std::cout << game.play(1, 1) << std::endl;
    std::cout << game.play(1, 5) << std::endl;
    std::cout << game.play(6, 2) << std::endl;
    std::cout << game.play(1, 1) << std::endl;
}