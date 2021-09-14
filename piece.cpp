#include "piece.h"

void pcs::pawn::get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board)
{
	int x{ position.x };
	int y{ position.y };

	// white pawns can only move up
	if (this->get_colour() == utl::colour::white) {
		// normal move
		if (y > 0 && current_board->identify_piece(utl::location{ x, y - 1 })->get_colour() == utl::colour::none) {
			bool_array[y - 1][x] = true;
		}
		// check kills
		if (x > 0 && y > 0 && current_board->identify_piece(utl::location{ x - 1, y - 1 })->get_colour() == utl::colour::black) {
			bool_array[y - 1][x - 1] = true;
		}
		if (x < 7 && y > 0 && current_board->identify_piece(utl::location{ x + 1, y - 1 })->get_colour() == utl::colour::black) {
			bool_array[y - 1][x + 1] = true;
		}

		// beginning move
		if (y == 6 && current_board->identify_piece(utl::location{ x, y - 1 })->get_colour() == utl::colour::none &&
			current_board->identify_piece(utl::location{ x, y - 2 })->get_colour() == utl::colour::none) {
			bool_array[y - 2][x] = true;
		}
	} else if (this->get_colour() == utl::colour::black){ // opposite directions for black pawns
		// normal move
		if (y < 7 && current_board->identify_piece(utl::location{ x, y + 1 })->get_colour() == utl::colour::none) {
			bool_array[y + 1][x] = true;
		}
		// check kills
		if (x > 0 && y < 7 && current_board->identify_piece(utl::location{ x - 1, y + 1 })->get_colour() == utl::colour::white) {
			bool_array[y + 1][x - 1] = true;
		}
		if (x < 7 && y < 7 && current_board->identify_piece(utl::location{ x + 1, y + 1 })->get_colour() == utl::colour::white) {
			bool_array[y + 1][x + 1] = true;
		}
		// beginning move
		if (y == 1 && current_board->identify_piece(utl::location{ x, y + 1 })->get_colour() == utl::colour::none &&
			current_board->identify_piece(utl::location{ x, y + 2 })->get_colour() == utl::colour::none) {
			bool_array[y + 2][x] = true;
		}
	}
}

void pcs::rook::get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board)
{
	int x{ position.x };
	int y{ position.y };
	
	// check up 
	for (int y_check{ y-1 }; y_check > -1; y_check--) {
		if (current_board->identify_piece(utl::location{ x, y_check })->get_colour()==current_board->get_current_turn()) {
			// if landed on my piece, terminate
			break;
		} else if (current_board->identify_piece(utl::location{ x, y_check })->get_colour() == utl::colour::none) {
			// if empty space, move is legal
			bool_array[y_check][x] = true;
		} else {
			// only option left - landed on ememy piece. Move legal but all moves beyond remain illegal
			bool_array[y_check][x] = true;
			break;
		}
	}

	// check down
	for (int y_check{ y+1 }; y_check < 8; y_check++) {
		if (current_board->identify_piece(utl::location{ x, y_check })->get_colour()==current_board->get_current_turn()) {
			break;
		} else if (current_board->identify_piece(utl::location{ x, y_check })->get_colour() == utl::colour::none) {
			bool_array[y_check][x] = true;
		} else {
			bool_array[y_check][x] = true;
			break;
		}
	}
	// check left
	for (int x_check{ x-1 }; x_check > -1; x_check--) {
		if (current_board->identify_piece(utl::location{ x_check, y})->get_colour()==current_board->get_current_turn()) {
			break;
		} else if (current_board->identify_piece(utl::location{ x_check, y})->get_colour() == utl::colour::none) {
			bool_array[y][x_check] = true;
		} else {
			bool_array[y][x_check] = true;
			break;
		}
	}

	// check right
	for (int x_check{ x+1 }; x_check < 8; x_check++) {
		if (current_board->identify_piece(utl::location{ x_check, y})->get_colour()==current_board->get_current_turn()) {
			break;
		} else if (current_board->identify_piece(utl::location{ x_check, y})->get_colour() == utl::colour::none) {
			bool_array[y][x_check] = true;
		} else {
			bool_array[y][x_check] = true;
			break;
		}
	}
}


void pcs::bishop::get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board)
{
	int x{ position.x };
	int y{ position.y };

	// check up right
	if (x < 7 && y > 0) {
		for (int i{ 1 }; i < 8; i++) {
			if (x + i > 7 || y - i < 0) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x + i, y - i })->get_colour() == utl::colour::none) {
				bool_array[y - i][x + i] = true;
			} else if (current_board->identify_piece(utl::location{ x + i, y - i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y - i][x + i] = true;
				break;
			}
		}
	}

	// check up left
	if (x > 0 && y > 0) {
		for (int i{ 1 }; i < 8; i++) {
			if (x - i < 0 || y - i < 0) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x - i, y - i })->get_colour() == utl::colour::none) {
				bool_array[y - i][x - i] = true;
			} else if (current_board->identify_piece(utl::location{ x - i, y - i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y - i][x - i] = true;
				break;
			}
		}
	}

	// check down right
	if (x < 7 && y < 7) {
		for (int i{ 1 }; i < 8; i++) {
			if (x + i > 7 || y + i > 7) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x + i, y + i })->get_colour() == utl::colour::none) {
				bool_array[y + i][x + i] = true;
			}
			else if (current_board->identify_piece(utl::location{ x + i, y + i })->get_colour() == current_board->get_current_turn()) {
				break;
			}
			else {
				bool_array[y + i][x + i] = true;
				break;
			}
		}
	}

	// check down left
	if (x > 0 && y < 7) {
		for (int i{ 1 }; i < 8; i++) {
			if (x - i < 0 || y + i > 7) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x - i, y + i })->get_colour() == utl::colour::none) {
				bool_array[y + i][x - i] = true;
			} else if (current_board->identify_piece(utl::location{ x - i, y + i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y + i][x - i] = true;
				break;
			}
		}
	}
}

void pcs::knight::get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board)
{
	int x{ position.x };
	int y{ position.y };

	if (current_board->boundary_check(x + 2, y + 1) && current_board->identify_piece(utl::location{x + 2, y + 1})->get_colour() 
		!= current_board->get_current_turn()) {
		bool_array[y + 1][x + 2] = true;
	}
	if (current_board->boundary_check(x + 2, y - 1) && current_board->identify_piece(utl::location{ x + 2, y - 1 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y - 1][x + 2] = true;
	}
	if (current_board->boundary_check(x + 1, y + 2) && current_board->identify_piece(utl::location{ x + 1, y + 2 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y + 2][x + 1] = true;
	}
	if (current_board->boundary_check(x + 1, y - 2) && current_board->identify_piece(utl::location{ x + 1, y - 2 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y - 2][x + 1] = true;
	}
	if (current_board->boundary_check(x - 2, y - 1) && current_board->identify_piece(utl::location{ x - 2, y - 1 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y - 1][x - 2] = true;
	}
	if (current_board->boundary_check(x - 2, y + 1) && current_board->identify_piece(utl::location{ x - 2, y + 1 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y + 1][x - 2] = true;
	}
	if (current_board->boundary_check(x - 1, y - 2) && current_board->identify_piece(utl::location{ x - 1, y - 2 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y - 2][x - 1] = true;
	}
	if (current_board->boundary_check(x - 1, y + 2) && current_board->identify_piece(utl::location{ x - 1, y + 2 })->get_colour()
		!= current_board->get_current_turn()) {
		bool_array[y + 2][x - 1] = true;
	}
}

void pcs::king::get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board)
{
	int x{ position.x };
	int y{ position.y };

	//right
	if (x < 7 && current_board->identify_piece(utl::location{ x + 1, y })->get_colour() != current_board->get_current_turn()) {
		bool_array[y][x + 1] = true;
	}

	//left
	if (x > 0 && current_board->identify_piece(utl::location{ x - 1, y })->get_colour() != current_board->get_current_turn()) {
		bool_array[y][x - 1] = true;
		
	}

	//up
	if (y > 0 && current_board->identify_piece(utl::location{ x, y - 1})->get_colour() != current_board->get_current_turn()) {
		bool_array[y-1][x] = true;
	}

	//down
	if (y < 7 && current_board->identify_piece(utl::location{ x, y + 1 })->get_colour() != current_board->get_current_turn()) {
		bool_array[y + 1][x] = true;
	}

	// up right
	if (x < 7 && y > 0 && current_board->identify_piece(utl::location{ x + 1, y - 1 })->get_colour() != current_board->get_current_turn()) {
		bool_array[y - 1][x + 1] = true;
	}

	// up left
	if (x > 0 && y > 0 && current_board->identify_piece(utl::location{ x - 1, y - 1 })->get_colour() != current_board->get_current_turn()) {
		bool_array[y - 1][x - 1] = true;
	}

	// down right
	if (x < 7 && y < 7 && current_board->identify_piece(utl::location{ x + 1, y + 1 })->get_colour() != current_board->get_current_turn()) {
		bool_array[y + 1][x + 1] = true;
	}

	// down left
	if (x > 0 && y < 7 && current_board->identify_piece(utl::location{ x - 1, y + 1 })->get_colour() != current_board->get_current_turn()) {
		bool_array[y + 1][x - 1] = true;
	}

}

void pcs::queen::get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board)
{
	int x{ position.x };
	int y{ position.y };
	
	// check up 
	for (int y_check{ y-1 }; y_check > -1; y_check--) {
		if (current_board->identify_piece(utl::location{ x, y_check })->get_colour()==current_board->get_current_turn()) {
			// if landed on my piece, terminate
			break;
		} else if (current_board->identify_piece(utl::location{ x, y_check })->get_colour() == utl::colour::none) {
			// if empty space, move is legal
			bool_array[y_check][x] = true;
		} else {
			// only option left - landed on ememy piece. Move legal but all moves beyond remain illegal
			bool_array[y_check][x] = true;
			break;
		}
	}

	// check down
	for (int y_check{ y+1 }; y_check < 8; y_check++) {
		if (current_board->identify_piece(utl::location{ x, y_check })->get_colour()==current_board->get_current_turn()) {
			break;
		} else if (current_board->identify_piece(utl::location{ x, y_check })->get_colour() == utl::colour::none) {
			bool_array[y_check][x] = true;
		} else {
			bool_array[y_check][x] = true;
			break;
		}
	}
	// check left
	for (int x_check{ x-1 }; x_check > -1; x_check--) {
		if (current_board->identify_piece(utl::location{ x_check, y})->get_colour()==current_board->get_current_turn()) {
			break;
		} else if (current_board->identify_piece(utl::location{ x_check, y})->get_colour() == utl::colour::none) {
			bool_array[y][x_check] = true;
		} else {
			bool_array[y][x_check] = true;
			break;
		}
	}

	// check right
	for (int x_check{ x+1 }; x_check < 8; x_check++) {
		if (current_board->identify_piece(utl::location{ x_check, y})->get_colour()==current_board->get_current_turn()) {
			break;
		} else if (current_board->identify_piece(utl::location{ x_check, y})->get_colour() == utl::colour::none) {
			bool_array[y][x_check] = true;
		} else {
			bool_array[y][x_check] = true;
			break;
		}
	}

	// check up right
	if (x < 7 && y > 0) {
		for (int i{ 1 }; i < 8; i++) {
			if (x + i > 7 || y - i < 0) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x + i, y - i })->get_colour() == utl::colour::none) {
				bool_array[y - i][x + i] = true;
			} else if (current_board->identify_piece(utl::location{ x + i, y - i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y - i][x + i] = true;
				break;
			}
		}
	}

	// check up left
	if (x > 0 && y > 0) {
		for (int i{ 1 }; i < 8; i++) {
			if (x - i < 0 || y - i < 0) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x - i, y - i })->get_colour() == utl::colour::none) {
				bool_array[y - i][x - i] = true;
			} else if (current_board->identify_piece(utl::location{ x - i, y - i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y - i][x - i] = true;
				break;
			}
		}
	}

	// check down right
	if (x < 7 && y < 7) {
		for (int i{ 1 }; i < 8; i++) {
			if (x + i > 7 || y + i > 7) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x + i, y + i })->get_colour() == utl::colour::none) {
				bool_array[y + i][x + i] = true;
			} else if (current_board->identify_piece(utl::location{ x + i, y + i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y + i][x + i] = true;
				break;
			}
		}
	}

	// check down left
	if (x > 0 && y < 7) {
		for (int i{ 1 }; i < 8; i++) {
			if (x - i < 0 || y + i > 7) {
				break;
			}
			if (current_board->identify_piece(utl::location{ x - i, y + i })->get_colour() == utl::colour::none) {
				bool_array[y + i][x - i] = true;
			} else if (current_board->identify_piece(utl::location{ x - i, y + i })->get_colour() == current_board->get_current_turn()) {
				break;
			} else {
				bool_array[y + i][x - i] = true;
				break;
			}
		}
	}
}