#ifndef AGENT_HPP
#define AGENT_HPP

typedef int move;

#include <board.hpp>

/// @brief The agent class which defines an intelligent agent
class Agent
{
protected:
    /// @brief The agent's token color
    token color;

public:
    /// @brief Builds an agent
    /// @param color The agent's token color
    Agent(token color);

    /// @brief Returns the agent's move
    /// @param board The game board
    /// @return The move decided by the agent
    virtual move getMove(Board board);
};

/// @brief The Player class defines an agent that is typically played by the human
class Player : Agent
{
public:
    /// @brief Initializes the player
    /// @param color The player's token color
    Player(token color);

    /// @brief Returns the player's move
    /// @param board The game board
    /// @return The move decided by the player
    move getMove(Board board);
};

/// @brief The MiniMaxAgent class defines the artificial intelligence that plays against the player.
class MiniMaxAgent : Agent
{
public:
    /// @brief Initializes the AI
    /// @param color The AI's token color
    MiniMaxAgent(token color);

    /// @brief Returns the AI's move
    /// @param board The game board
    /// @return The move calculated by the AI
    move getMove(Board board);
};

/// @brief Returns all possible states for the agent
/// @return A vector of hashed states pair
std::vector<BoardHashPair> getStates();

#endif