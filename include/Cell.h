#include <SFML/Graphics.hpp>
#include <deque>
#include "Codes.h"

class Piece;

class Cell : public sf::CircleShape 
{
public:
	Cell(float diameter, sf::Vector2f pos, codes::CellID ID);
	~Cell();

	// Returns the topmost piece of this tower, or NULL if it doesn't have any
	// pieces
	Piece* GetTop();

	// If the tower is empty is replaced with the given tower and returns true. 
	// Returns false otherwise
	bool PutTower(std::deque<Piece*> newTower);

	// Pushes a piece to the bottom of the tower
	void PushPiece(Piece* piece);

	// Deletes every piece on the tower
	void CleanTower();

	// Returns the tower
	std::deque<Piece*> GetTower(); 

	codes::CellID GetID();


private:
	sf::Texture mTexture;

	// ID that describes the location of the cell on the board
	codes::CellID mID;  // TODO maybe not neccesary, delete if thats the case

	// The tower of pieces on this cell. front() is the topmost piece
	std::deque<Piece*> mTower;
};