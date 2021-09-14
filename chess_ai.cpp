#include "chess_ai.h"

ai::chess_ai::chess_ai(const utl::colour my_colour_input, const int max_depth_input)
{
	my_colour = my_colour_input;
	max_depth = max_depth_input;
	opponent_colour = (my_colour_input == utl::colour::white) ? utl::colour::black : utl::colour::white;

	white_pawn_bias = {
	{10, 10, 10, 10, 10, 10, 10, 10},
	{5, 5, 5, 5, 5, 5, 5, 5},
	{1, 1, 2, 3, 3, 2, 1, 1 },
	{0.5, 0.5, 1, 2.5, 2.5, 1, 0.5, 0.5},
	{0, 0, 0, 2, 2, 0, 0, 0},
	{0.5, -0.5, -1, 0, 0, -1, -0.5, -0.5},
	{0.5, 1, 1, -2, -2, 1, 1, 0.5},
	{0, 0, 0, 0, 0, 0, 0, 0}
	};

	black_pawn_bias = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0.5, 1, 1, -2, -2, 1, 1, 0.5},
	{0.5, -0.5, -1, 0, 0, -1, -0.5, -0.5},
	{0, 0, 0, 2, 2, 0, 0, 0},
	{0.5, 0.5, 1, 2.5, 2.5, 1, 0.5, 0.5},
	{1, 1, 2, 3, 3, 2, 1, 1 },
	{5, 5, 5, 5, 5, 5, 5, 5},
	{10, 10, 10, 10, 10, 10, 10, 10}
	};

	white_king_bias = {
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-2, -3, -3, -4, -4, -3, -3, -2},
	{-1, -2, -2, -2, -2, -2, -2, -1},
	{2, 2, 0, 0, 0, 0, 2, 2},
	{2, 3, 1, 0, 0, 1, 3, 2}
	};

	black_king_bias = {
	{2, 3, 1, 0, 0, 1, 3, 2},
	{2, 2, 0, 0, 0, 0, 2, 2},
	{-1, -2, -2, -2, -2, -2, -2, -1},
	{-2, -3, -3, -4, -4, -3, -3, -2},
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-3, -4, -4, -5, -5, -4, -4, -3},
	{-3, -4, -4, -5, -5, -4, -4, -3}
	};

	white_rook_bias = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0.5, 1, 1, 1, 1, 1, 1, 0.5},
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5 },
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5 },
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5 },
	{0, 0, 0, 0.5, 0.5, 0, 0, 0}
	};

	black_rook_bias = {
	{0, 0, 0, 0.5, 0.5, 0, 0, 0},
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5 },
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5 },
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5 },
	{-0.5, 0, 0, 0, 0, 0, 0, -0.5},
	{0.5, 1, 1, 1, 1, 1, 1, 0.5},
	{0, 0, 0, 0, 0, 0, 0, 0}
	};

	queen_bias = {
	{-2, -1, -1, -0.5, -0.5, -1, -1, -2},
	{-1, 0, 0, 0, 0, 0, 0, -1},
	{-1, 0, 0.5, 0.5, 0.5, 0.5, 0, -1},
	{-0.5, 0, 0.5, 0.5, 0.5, 0.5, 0, -0.5 },
	{-0.5, 0, 0.5, 0.5, 0.5, 0.5, 0, -0.5 },
	{-1, 0, 0.5, 0.5, 0.5, 0.5, 0, -1},
	{-1, 0, 0, 0, 0, 0, 0, -1},
	{-2, -1, -1, -0.5, -0.5, -1, -1, -2}
	};

	bishop_bias = {
	{-2, -1, -1, -1, -1, -1, -1, -2},
	{-1, 0, 0, 0, 0, 0, 0, -1},
	{-1, 0, 0.5, 1, 1, 0.5, 0, -1},
	{-1, 0.5, 0.5, 1, 1, 0.5, 0.5, -1},
	{-1, 0.5, 0.5, 1, 1, 0.5, 0.5, -1},
	{-1, 0, 0.5, 1, 1, 0.5, 0, -1},
	{-1, 0, 0, 0, 0, 0, 0, -1},
	{-2, -1, -1, -1, -1, -1, -1, -2},
	};

	knight_bias = {
	{-5, -4, -3, -3, -3, -3, -4, -5},
	{-4, -2, 0, 0, 0, 0, -2, -4},
	{-3, 0, 1, 1.5, 1.5, 1, 0, -4},
	{-3, 0.5, 1.5, 2, 2, 1.5, 0.5, -3},
	{-3, 0.5, 1.5, 2, 2, 1.5, 0.5, -3},
	{-3, 0, 1, 1.5, 1.5, 1, 0, -4},
	{-4, -2, 0, 0, 0, 0, -2, -4},
	{-5, -4, -3, -3, -3, -3, -4, -5},
	};

	std::vector <std::vector<double>> knight_bias;
}

double ai::chess_ai::calculate_utility(brd::board* board_to_evaluate)
{
	double utility_value{ 0 }; // value which repersents ai's advantage over player. The higher, the better for the ai
	// piece values
	double king_value{ 900 }; double queen_value{ 90 }; double rook_value{ 50 }; double bishop_value{ 30 };
	double knight_value{ 30 }; double pawn_value{ 10 };
	int multiplier{ 0 };

	// cycle through board and modify utility value. Add up values of the pieces biased by location value. 
	for (int y{ 0 }; y < 8; y++) {
		for (int x{ 0 }; x < 8; x++) {
			char current_piece_symbol{ board_to_evaluate->identify_piece(utl::location{ x, y })->get_board_graphic() };
			utl::colour current_piece_colour{ board_to_evaluate->identify_piece(utl::location{ x, y })->get_colour() };
			multiplier = current_piece_colour == get_my_colour() ? 1 : -1;

			if (current_piece_symbol == 'k') {
				if (current_piece_colour == utl::colour::white) {
					utility_value += (king_value + white_king_bias[y][x]) * multiplier;
				} else {
					utility_value += (king_value + black_king_bias[y][x]) * multiplier;
				}
			}

			if (current_piece_symbol == 'r') {
				if (current_piece_colour == utl::colour::white) {
					utility_value += (rook_value + white_rook_bias[y][x]) * multiplier;
				} else {
					utility_value += (rook_value + black_rook_bias[y][x]) * multiplier;
				}
			}

			if (current_piece_symbol == 'q') {
				utility_value += (rook_value + white_rook_bias[y][x]) * multiplier;
			}

			if (current_piece_symbol == 'n') {
				utility_value += (rook_value + knight_bias[y][x]) * multiplier;
			}

			if (current_piece_symbol == 'b') {
				utility_value += (rook_value + bishop_bias[y][x]) * multiplier;
			}

			if (current_piece_symbol == 'p') {
				if (current_piece_colour == utl::colour::white) {
					utility_value += (pawn_value + white_pawn_bias[y][x]) * multiplier;
				} else {
					utility_value += (pawn_value + black_pawn_bias[y][x]) * multiplier;
				}
			}
		}
	}
	return utility_value;
}

void ai::chess_ai::copy_board_contents(brd::board* board_original, brd::board* board_copy) 
{
	for (int y{ 0 }; y < 8; y++) {
		for (int x{ 0 }; x < 8; x++) {
			board_copy->set_piece(utl::location{ x, y }, board_original->identify_piece(utl::location{ x, y }));
		}
	}
}
ai::ai_result ai::chess_ai::minimax(const int depth, const bool is_max, brd::board* ai_board, double alpha, double beta) 
{
	// if reached max depth (or check mate)
	if (depth == get_max_depth()) {
		return (ai::ai_result{ calculate_utility(ai_board), " " });
	}

	ai_board->set_turn((is_max) ? get_my_colour() : get_opponent_colour());

	// else
	double best_utility_value{ (is_max) ? -100000.0 : 100000.0 };

	std::string best_move{ " " };

	std::vector <std::string> possible_moves{ ai_board->get_all_legal_moves() };

	if (possible_moves.size() == 0) {
		if (ai_board->test_check(ai_board->get_current_turn()) > 0) { //check mate 
			return ai::ai_result{((is_max) ? -100000.0 : +100000.0), " "};
		} else { // stale mate

			return ai::ai_result{ ((is_max) ? +100000.0 : -100000.0), " " };
		}
	}

	std::shuffle(possible_moves.begin(), possible_moves.end(), std::default_random_engine(std::rand())); 
	for (unsigned possible_move_index{ 0 }; possible_move_index < possible_moves.size(); possible_move_index++) {

		// store target piece
		std::shared_ptr<pcs::piece> moving_piece{ ai_board->identify_piece(ai_board->convert_notation_to_location(ai_board->get_move_struct_string(possible_moves[possible_move_index]).initial_location)) };
		std::shared_ptr<pcs::piece> taken_piece{ ai_board->identify_piece(ai_board->convert_notation_to_location(ai_board->get_move_struct_string(possible_moves[possible_move_index]).final_location)) };
		// move
		ai_board->move(possible_moves[possible_move_index], false, false);

		// call algorithm
		ai::ai_result child_result{ minimax(depth + 1, !is_max, ai_board, alpha, beta) };

		// undo move
		ai_board->set_piece(ai_board->convert_notation_to_location(ai_board->get_move_struct_string(possible_moves[possible_move_index]).initial_location), moving_piece);
		ai_board->set_piece(ai_board->convert_notation_to_location(ai_board->get_move_struct_string(possible_moves[possible_move_index]).final_location), taken_piece);

		moving_piece = nullptr;
		taken_piece = nullptr;

		if (is_max && best_utility_value < child_result.best_utility_value) {
			best_utility_value = child_result.best_utility_value;
			best_move = possible_moves[possible_move_index]; 

			alpha = std::max(best_utility_value, alpha);
			if (beta <= alpha)
				break;

		} else if (!is_max && best_utility_value > child_result.best_utility_value) {
			best_utility_value = child_result.best_utility_value;
			best_move = possible_moves[possible_move_index];

			beta = std::min(best_utility_value, beta);
				if (beta <= alpha)
					break;
		}
	}

	return ai::ai_result{ best_utility_value, best_move };

}

std::string ai::chess_ai::get_best_move(brd::board* game_board, brd::board* ai_board) 
{
	// min_max algorithm
	ai::ai_result best_result{minimax(0, true, ai_board, -9999999999999, 9999999999999)};
	if (best_result.best_move == " ") { best_result.best_move = ai_board->get_all_legal_moves()[0]; }
	return best_result.best_move;
}