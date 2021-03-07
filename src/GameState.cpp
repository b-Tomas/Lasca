#include "GameState.h"

GameState::GameState()
{
	
}

GameState::~GameState()
{

}

void GameState::Init(sf::RenderWindow* window)
{
	gWindow = window;
	gWindow->setFramerateLimit(mFrameRateLimit); // This state's frame rate limit


	mCellSize = std::min(gWindow->getSize().x, gWindow->getSize().y)/7;

	// Initiate the board
	for (int i=0; i<7; i++) {
		for (int j=0; j<3+!(i%2); j++) {
			sf::Vector2f pos = sf::Vector2f(j*mCellSize*2+(i%2)*mCellSize, i*mCellSize);
			Cell* cell = new Cell(mCellSize, pos, codes::CellID(7-(j*2+i%2), 7-i));
			mBoardCells.push_back(cell);
			if (i!=3) mPieces.push_back(new Piece(i<3, cell));
		}
	}
}

bool GameState::ProcessInput()
{
	// Temporary variables for drag and drop functionality
	static sf::Vector2f offset;
	static sf::Vector2f oldPos, newPos;
	static Piece* movingPiece;

	// Position of the mouse at this iteration of the game loop
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*gWindow));

	// Event loop
	sf::Event event;
	while (gWindow->pollEvent(event))
	{
		// Close button was clicked
		if (event.type == sf::Event::Closed) return true;

		// Mouse events
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			// Left mouse button events
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// If the user user is trying to move a piece move it
				for (Piece* p : mPieces)
				{
					if (p->GetSprite().getGlobalBounds().contains(mousePos))
					{
						oldPos = p->GetPosition();
						offset = mousePos-oldPos;
						movingPiece=p;
					}
				}
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			// If the user was moving a piece, drop it
			if (event.mouseButton.button == sf::Mouse::Left && movingPiece!=nullptr)
			{
				bool success = false;  // If this is not an appropiate place, remains false to later return the piece to its original place
				for (Cell* c : mBoardCells)  // Implment an ordered matrix search algorithm to speed this up unnecessarily
				{
					// Move to that cell if possible
					if (c->getGlobalBounds().contains(mousePos))
					{
						if (CheckLegalMove(c->GetID(), movingPiece->GetCellID(), movingPiece->GetColor()))
						{
							movingPiece->AttachToCell(c);
							movingPiece = nullptr;
							success = true;
						}
						break;
					}
				}
				// Return it to the original place
				if (!success)
				{
					movingPiece->SetSpritePosition(oldPos);
					movingPiece = nullptr;
				}
				       
			}    
		}

		// If the user was moving a piece, keep it on the cursor
		if (movingPiece!=nullptr) 
		{
			newPos = mousePos-offset;
			movingPiece->SetSpritePosition(newPos);  
		}
	}
	return false;
}

bool GameState::CheckLegalMove(codes::CellID cellID, codes::CellID pCellID, bool color)
{
	return cellID.y == pCellID.y+(color == codes::Colors::Black ? -1 : 1);
}

void GameState::Draw()
{
	// Draw everything
	for (Cell* c : mBoardCells) gWindow->draw(*c);
	for (Piece* p : mPieces) gWindow->draw(p->GetSprite());
}

sf::Color GameState::getBackGroudColor()
{
	return mBackgroundColor;
}