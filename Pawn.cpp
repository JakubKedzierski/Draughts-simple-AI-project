#include "Pawn.hpp"

/* Konstruktor pionka (graficznego)
 @param pos - pozycja na planszy 
 @param type1 - rodzaj pionka
*/
Pawn::Pawn(sf::Vector2f pos, PawnType type1) {
	type = type1;
	shape.setRadius(45); // promien pionka 
	shape.setPosition(101 + pos.y * SquareSize, 101 + pos.x * SquareSize); // pozcycja na graficznej planszy
		
		switch (type1) {
		case BlackMan: {
			texture = new Texture(); 
			if (!texture->loadFromFile("blackpawn.png"))  // wczytywanie tekstury z pliku
				cerr << "Nie udalo sie zaladowac blackpawn.png";
			shape.setTexture(texture);
			break;
		}

		case WhiteMan: {
			texture = new Texture();
			if (!texture->loadFromFile("whitepawn.png"))
				cerr << "Nie udalo sie zaladowac whitepawn.png";
			shape.setTexture(texture);
			break;
		}

		case WhiteKing: {
			texture = new Texture();
			if (!texture->loadFromFile("whiteking.png"))
				cerr << "Nie udalo sie zaladowac whiteking.png";
			shape.setTexture(texture);
			break;
		}

		case BlackKing: {
			texture = new Texture();
			if (!texture->loadFromFile("blackking.png"))
				cerr << "Nie udalo sie zaladowac blackking.png";
			shape.setTexture(texture);
			break;
		}

		}
	
}
