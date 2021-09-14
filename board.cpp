#include "board.h"

brd::board::~board()
{
	for (int row{ 0 }; row < 8; row++) {
		for (int column{ 0 }; column < 8; column++) {
			identify_piece(utl::location{ column, row }) = nullptr;;
		}
	}
}

brd::board::board()
{
	//new game
	current_turn = utl::colour::white;

	// initialize board_content_vector
	for (int y{ 0 }; y < 8; y++) {
		board_content.push_back({nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr});
	}

	// fill with empty spaces
	for (int row{ 2 }; row < 6; row++) {
		for (int column{ 0 }; column < 8; column++) {
			set_piece(utl::location{ column,row }, std::make_shared<pcs::empty_space>());
		}
	}

	// fill with pawns
	for (int column{ 0 }; column < 8; column++) {
		set_piece(utl::location{ column, 1 }, std::make_shared <pcs::pawn>(utl::colour::black));
		set_piece(utl::location{ column, 6 }, std::make_shared <pcs::pawn>(utl::colour::white));
	}

	// fill black home row
	set_piece(utl::location{ 0, 0 }, std::make_shared <pcs::rook>(utl::colour::black));
	set_piece(utl::location{ 7, 0 }, std::make_shared <pcs::rook>(utl::colour::black));
	set_piece(utl::location{ 1, 0 }, std::make_shared <pcs::knight>(utl::colour::black));
	set_piece(utl::location{ 6, 0 }, std::make_shared <pcs::knight>(utl::colour::black));
	set_piece(utl::location{ 2, 0 }, std::make_shared <pcs::bishop>(utl::colour::black));
	set_piece(utl::location{ 5, 0 }, std::make_shared <pcs::bishop>(utl::colour::black));
	set_piece(utl::location{ 4, 0 }, std::make_shared <pcs::king>(utl::colour::black));
	set_piece(utl::location{ 3, 0 }, std::make_shared <pcs::queen>(utl::colour::black));

	// fill white home row
	set_piece(utl::location{ 0, 7 }, std::make_shared <pcs::rook>(utl::colour::white));
	set_piece(utl::location{ 7, 7 }, std::make_shared <pcs::rook>(utl::colour::white));
	set_piece(utl::location{ 1, 7 }, std::make_shared <pcs::knight>(utl::colour::white));
	set_piece(utl::location{ 6, 7 }, std::make_shared <pcs::knight>(utl::colour::white));
	set_piece(utl::location{ 2, 7 }, std::make_shared <pcs::bishop>(utl::colour::white));
	set_piece(utl::location{ 5, 7 }, std::make_shared <pcs::bishop>(utl::colour::white));
	set_piece(utl::location{ 4, 7 }, std::make_shared <pcs::king>(utl::colour::white));
	set_piece(utl::location{ 3, 7 }, std::make_shared <pcs::queen>(utl::colour::white));
}

std::shared_ptr<pcs::piece> brd::board::identify_piece(const utl::location position) {
	return board_content[position.y][position.x];
}

void brd::board::set_piece(const utl::location position, const std::shared_ptr <pcs::piece> piece_pointer) 
{
	board_content[position.y][position.x] = piece_pointer;
}

utl::colour brd::board::get_current_turn() 
{
	return current_turn; 
}

void brd::board::move(const std::string full_input, const bool player, const bool record)
{	
	std::string initial;
	std::string final;

	initial = full_input.substr(0, 2);
	final = full_input.substr(2, 2);

	// convert to readable coordinates
	utl::location initial_location{ convert_notation_to_location(initial) };
	utl::location final_location{ convert_notation_to_location(final) };

	// move piece
	set_piece (final_location, identify_piece(initial_location));
	set_piece(initial_location, std::make_shared<pcs::empty_space>());

	// record move
	if (record) { record_move(full_input); }

	// check for pawn promotion

	utl::colour promotion_colour{ utl::colour::none };
	utl::location promotion_location{ 0, 0 };
	std::string user_input;
	bool promotion_loop{ true };
	
	for (int x{ 0 }; x < 8; x++) {
		// only white pawns can reach the top of the board
		if (identify_piece(utl::location{ x, 0 })->get_board_graphic()=='p') {
			promotion_colour = utl::colour::white;
			promotion_location.x = x;
			promotion_location.y = 0;
		} 
		if (identify_piece(utl::location{ x, 7 })->get_board_graphic() == 'p') {
			promotion_colour = utl::colour::black;
			promotion_location.x = x;
			promotion_location.y = 7;
		}
	}

	
	if (promotion_colour != utl::colour::none) {
		if (player) {
			while (promotion_loop) {
				std::cout << "PROMOTE PAWN TO (Q, R, N, B): ";
				std::getline(std::cin, user_input);
				if (user_input == "Q") {
					set_piece(promotion_location, std::make_shared<pcs::queen>(promotion_colour));
					promotion_loop =  false ; break;
				} else if (user_input == "R") {
					set_piece(promotion_location, std::make_shared<pcs::rook>(promotion_colour));
					promotion_loop = false ; break;
				} else if (user_input == "N") {
					set_piece(promotion_location, std::make_shared<pcs::knight>(promotion_colour));
					promotion_loop = false ; break;
				} else if (user_input == "B") {
					set_piece(promotion_location, std::make_shared<pcs::bishop>(promotion_colour));
					promotion_loop = false ; break;
				} else {
					std::cout << std::endl << "invalid input. type Q, R, N or B" << std::endl;
				}
			}
		} else { // ai is making decision (always promotes to queen)
			set_piece(promotion_location, std::make_shared<pcs::queen>(promotion_colour));
		}
	}
}

std::vector <std::string> brd::board::get_all_legal_moves() 
{
	std::vector <std::string> all_legal_moves;

	// cycle through board to test each piece
	bool pseudo_legal_positions[8][8];

	for (int initial_x{ 0 }; initial_x < 8; initial_x++) {
		for (int initial_y{ 0 }; initial_y < 8; initial_y++) {
			// if my piece
			if (identify_piece(utl::location{ initial_x ,initial_y })->get_colour() == get_current_turn()) {
				// fetch all pseudo legal moves

				// reset pseudo legal final positions array
				for (int final_x{ 0 }; final_x < 8; final_x++) {
					for (int final_y{ 0 }; final_y < 8; final_y++) {
						pseudo_legal_positions[final_y][final_x] = false;
					}
				}
					
				// modify pseudo legal positions array to store current piece pseudo legal positions
				identify_piece(utl::location{ initial_x ,initial_y })->get_legal_moves(utl::location{ initial_x ,initial_y }, 
					pseudo_legal_positions, this);

				// make all pseudo legal moves and test if they result in check.
				for (int final_x{ 0 }; final_x < 8; final_x++) {
					for (int final_y{ 0 }; final_y < 8; final_y++) {
						//
						if (pseudo_legal_positions[final_y][final_x] == true) {
							// take copy of taken piece to allow to revert move
							std::shared_ptr<pcs::piece> taken_piece{ identify_piece(utl::location{final_x, final_y})};

							// move piece
							set_piece(utl::location{final_x, final_y}, identify_piece(utl::location{ initial_x, initial_y }));
							set_piece(utl::location{ initial_x, initial_y }, std::make_shared<pcs::empty_space>());

							// see if new move results in my king being exposed to enemy
							int check_counter{ test_check(get_current_turn()) };

							// revert move
							set_piece(utl::location{ initial_x, initial_y }, identify_piece(utl::location{ final_x, final_y }));
							set_piece(utl::location{ final_x, final_y }, taken_piece);

							if (check_counter == 0) {
								// legal move. add to legal move vector
								all_legal_moves.push_back(std::string{ convert_location_to_notation(utl::location{initial_x, initial_y})
									+ convert_location_to_notation(utl::location{final_x, final_y}) });
							}
						}
					}
				}
			}
		}
	}
	return all_legal_moves;
}

int brd::board::test_check(const utl::colour colour)
{
	utl::colour enemy_colour{ (get_current_turn() == utl::colour::white) ? utl::colour::black : utl::colour::white };

	utl::location king_location;
	int check_counter{ 0 };
	bool possible_positions[8][8];
	for (unsigned i{ 0 }; i < 8; i++) {
		for (unsigned j{ 0 }; j < 8; j++) {
			possible_positions[i][j] = false;
		}
	}

	// loop though board to find my king
	for (int y_test{ 0 }; y_test < 8; y_test++) {
		for (int x_test{ 0 }; x_test < 8; x_test++) {
			if (identify_piece(utl::location{ x_test, y_test })->get_colour() == colour &&
				identify_piece(utl::location{ x_test, y_test })->get_board_graphic() == 'k') {
				king_location.x = x_test; king_location.y = y_test;
			}
		}
	}


	// create test pieces
	std::vector<std::shared_ptr<pcs::piece>> test_pieces{ std::make_shared<pcs::rook>(colour), std::make_shared<pcs::knight>(colour),
	std::make_shared<pcs::bishop>(colour), std::make_shared<pcs::queen>(colour), std::make_shared<pcs::pawn>(colour), 
		std::make_shared<pcs::king>(colour) };

	// loop through test pieces, place on king's position, and get their legal moves
	for (unsigned test_piece_iterator{ 0 }; test_piece_iterator < 6; test_piece_iterator++) {

		// clear position array
		for (int y{ 0 }; y < 8; y++) {
			for (int x{ 0 }; x < 8; x++) {
				possible_positions[y][x] = false;
			}
		}
		test_pieces[test_piece_iterator]->get_legal_moves(king_location, possible_positions, this);

		for (int y_test{ 0 }; y_test < 8; y_test++) {
			for (int x_test{ 0 }; x_test < 8; x_test++) {
				if (possible_positions[y_test][x_test] == true && identify_piece(utl::location{ x_test,y_test })->get_board_graphic() ==
					test_pieces[test_piece_iterator]->get_board_graphic() &&
					identify_piece(utl::location{ x_test,y_test })->get_colour() == enemy_colour) {
					check_counter++;
				}
			}
		}
	}
	
	// delete test pieces
	for (unsigned test_piece_iterator{ 0 }; test_piece_iterator < 5; test_piece_iterator++) {
		test_pieces[test_piece_iterator] = nullptr;
	}
	return check_counter;
}

void brd::board::display_board()
{
	std::cout << "    ---- board ----" << std::endl << std::endl;
	std::cout << std::setw(2) << "   ";
	for (char letter{ 'A' }; letter < 'I'; letter++) {
		std::cout << std::setw(2) << letter;
	}
	std::cout << std::endl << std::endl;

	for (int y{ 0 }; y < 8; y++) {
		std::cout << std::setw(2) << 8 - y << " ";
		for (int x{ 0 }; x < 8; x++) {
			// make white pieces upper case
			std::string symbol{ identify_piece(utl::location{ x, y })->get_board_graphic() };
			if (identify_piece(utl::location{ x, y })->get_colour() == utl::colour::white) {
				symbol = std::toupper(symbol[0]);
			}

			// display pieces
			std::cout << std::setw(2) << symbol;
		}
		std::cout << " " << std::setw(2) << 8 - y;
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::setw(2) << "   ";
	for (char letter{ 'A' }; letter < 'I'; letter++) {
		std::cout << std::setw(2) << letter;
	}
	std::cout << std::endl << std::endl;
}

void brd::board::change_turn()
{
	if (get_current_turn() == utl::colour::white) {
		current_turn = utl::colour::black;
	} else {
		current_turn = utl::colour::white;
	}
}

void brd::board::record_move(const std::string move_string)
{
	moves.push_back(move_string);
}

void brd::board::display_move_history()
{
	std::cout << "----- move history list -----" << std::endl;

	for (unsigned i{ 0 }; i < moves.size(); i++) {
		std::cout << moves[i] << std::endl;
	}
	std::cout << "----- list end -----" << std::endl;
}

utl::move_struct_string brd::board::get_move_struct_string(const std::string raw_move_string) 
{ 
	return utl::move_struct_string{ raw_move_string.substr(0, 2), raw_move_string.substr(2, 2) };
}

bool brd::board::only_kings_left () 
{
	// if 62 pieces are empty, it means only kings are left
	int empty_count{ 0 };
	for (int x{ 0 }; x < 8; x++) {
		for (int y{ 0 }; y < 8; y++) {
			if (identify_piece(utl::location{ x, y })->get_board_graphic() == '.') {
				empty_count++;
			}
		}
	}

	if (empty_count == 62) {
		return true;
	} else {
		return false;
	}

}

bool brd::board::draw_by_repetition()
{
	// check is identical moves have been made 3 times in a row
	bool condition{ false };
	if (moves.size() > 12 && 
		(moves[moves.size() - 1] == moves[moves.size() - 1 - 4] && moves[moves.size() - 1 - 4] == moves[moves.size() - 1 - 8]) &&
		(moves[moves.size() - 2] == moves[moves.size() - 2 - 4] && moves[moves.size() - 2 - 4] == moves[moves.size() - 2 - 8]) &&
		(moves[moves.size() - 3] == moves[moves.size() - 3 - 4] && moves[moves.size() - 3 - 4] == moves[moves.size() - 3 - 8]) &&
		(moves[moves.size() - 4] == moves[moves.size() - 4 - 4] && moves[moves.size() - 4 - 4] == moves[moves.size() - 4 - 8])) {
		condition = true;
	}
	return condition;
}

