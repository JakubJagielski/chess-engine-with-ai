#include <iostream>
#include <string>
#include <iomanip>
#include <memory>

#include "board.h"
#include "piece.h"
#include "utility.h"
#include "chess_ai.h"

int main()
{
	// bools used to loop until the player gives a valid input.

	bool mode_loop{ true };
	bool ai_mode{ false };
	bool ai_vs_ai{ false };
	bool difficulty_loop{ true };

	// bools used to loop until secions of the game finish
	bool gameplay{ true };
	bool end_game_loop{ true };
	

	// also used to select difficulty.
	int white_search_depth{ 1 };
	int black_search_depth{ 2 };
	

	brd::board game_board;
	bool moved_flag{ false };
	std::string player_input;

	std::vector <std::string> all_legal_moves_vector;
	std::string welcome_message{ "Welcome to chess. Type moves in the form C2C4. Type END, SHOW MOVE HISTORY, or SHOW LEGAL MOVES for additional options" };

	brd::board ai_thinking_board;


	std::cout << "Play against human (H) or AI (A)? Try (AI VS AI): ";
	while (mode_loop) {
		std::getline(std::cin, player_input);
		if (player_input == "H") {
			mode_loop = false; break;
		} else if (player_input == "A") {
			ai_mode = true;
			mode_loop = false; break;
		} else if (player_input == "AI VS AI") {
			ai_vs_ai = true; ai_mode = true;
			mode_loop = false; break;
		} else {
			std::cout << "Only type H or A or AI VS AI (in capitals). " << std::endl << "select: ";
		}
	}

	if (ai_mode) {
		while (difficulty_loop) {
			std::cout << "choose black AI difficulty (1, 2 or 3): ";
			std::getline(std::cin, player_input);
			if (player_input == "1") {
				black_search_depth = 2; difficulty_loop = false; break;
			} else if (player_input == "2") {
				black_search_depth = 3; difficulty_loop = false; break;
			} else if (player_input == "3") {
				black_search_depth = 4; difficulty_loop = false; break;
			} else {
				std::cout << "only choose 1, 2 or 3..." << std::endl;
			}
		}
	}

	if (ai_vs_ai) {
		difficulty_loop = true;
		while (difficulty_loop) {
			std::cout << "choose white AI difficulty (1, 2 or 3): ";
			std::getline(std::cin, player_input);
			if (player_input == "1") {
				white_search_depth = 2; difficulty_loop = false; break;
			} else if (player_input == "2") {
				white_search_depth = 3; difficulty_loop = false; break;
			} else if (player_input == "3") {
				white_search_depth = 4; difficulty_loop = false; break;
			} else {
				std::cout << "only choose 1, 2 or 3..." << std::endl;
			}	
		}
	}

	ai::chess_ai ai_player_white{ utl::colour::white, white_search_depth};
	ai::chess_ai ai_player_black{ utl::colour::black, black_search_depth};


	std::cout << welcome_message << std::endl; // intro message
	
	game_board.display_board();
	
	while (gameplay) {
		moved_flag = false;
		if (game_board.get_current_turn() == utl::colour::white) {
			std::cout << "WHITE TURN" << std::endl;
		} else {
			std::cout << "BLACK TURN" << std::endl;
		}

		// check draw by move repetition
		if (game_board.draw_by_repetition()) {
			std::cout << "DRAW! (repeated same moves 3 times in a row)" << std::endl;
			gameplay = false;
			break;
		}

		//check check
		if (game_board.test_check(game_board.get_current_turn()) > 0) {
			std::cout << "CHECK!" << std::endl;
		}

		// check stalemate and checkmate
		all_legal_moves_vector = game_board.get_all_legal_moves();
		if (all_legal_moves_vector.size() == 0) { //no legal moves
			if (game_board.test_check(game_board.get_current_turn()) == 0) {
				std::cout << "STALEMATE!" << std::endl;
				gameplay = false;
				break;
			} else {
				std::cout << "CHECKMATE!" << std::endl;
				gameplay = false;
				break;
			}
		}

		// check if only kings are left
		if (game_board.only_kings_left()) {
			std::cout << "DRAW" << std::endl;
			gameplay = false;
			break;
		}

		// white's turn
		if (game_board.get_current_turn() == utl::colour::white) {
			if (ai_vs_ai) {
				ai_player_white.copy_board_contents(&game_board, &ai_thinking_board);
				std::cout << "AI is thinking... ";
				player_input = ai_player_white.get_best_move(&game_board, &ai_thinking_board);
				std::cout << std::endl << "AI moved: " << player_input << std::endl;
			} else {
				std::cout << "ENTER COMMAND: ";
				std::getline(std::cin, player_input);
			}
		} else { // black's turn
			if (ai_mode) {
				ai_player_black.copy_board_contents(&game_board, &ai_thinking_board);
				std::cout << "AI is thinking... ";
				player_input = ai_player_black.get_best_move(&game_board, &ai_thinking_board);
				std::cout << std::endl << "AI moved: " << player_input << std::endl;
			} else {
				std::cout << "ENTER COMMAND: ";
				std::getline(std::cin, player_input);
			}
		}

		if (player_input == "END") {
			gameplay = false;
			break;
		} else if (player_input == "SHOW MOVE HISTORY") {
			game_board.display_move_history();
		} else if (player_input == "SHOW LEGAL MOVES") {
			for (unsigned i{ 0 }; i < all_legal_moves_vector.size(); i++) {
				std::cout << all_legal_moves_vector[i] << std::endl;
			}
		} else {
			// loop through legal inputs to see if player move is in it
			for (unsigned i{ 0 }; i < all_legal_moves_vector.size(); i++) {
				if (all_legal_moves_vector[i] == player_input) {

					if (ai_vs_ai || (ai_mode && game_board.get_current_turn() == utl::colour::black)) {
						game_board.move(player_input, false, true);
						moved_flag = true;
					} else {
						game_board.move(player_input, true, true);
						moved_flag = true;
					}
				}
			}

			if (moved_flag) {
				game_board.change_turn();
				std::cout << "========================" << std::endl;
				game_board.display_board();
			} else {
				std::cout << "ILLEGAL MOVE!" << std::endl << 
					"(Make sure to use capital letters and input in form A2A3)" << std::endl;
				if (ai_vs_ai || (ai_mode && game_board.get_current_turn() == utl::colour::black)) {
					std::cout << "this move broke the game: " << player_input << std::endl;

					std::cout << "legal moves: " << std::endl;
					for (unsigned i{ 0 }; i < all_legal_moves_vector.size(); i++) {
						std::cout << all_legal_moves_vector[i] << std::endl;
					}

					gameplay = false;
					break;
				}
			}
		}
	}

	while (end_game_loop) {
		std::cout << "Game ended. Type EXIT or SHOW MOVE HISTORY: ";
		std::getline(std::cin, player_input);
		if (player_input == "EXIT") {
			end_game_loop = false;
			break;
		} else if (player_input == "SHOW MOVE HISTORY") {
			game_board.display_move_history();
		} else {
			std::cout << "ILLEGAL INPUT. TRY AGAIN..." << std::endl;
		}
	}
	return 1;
}