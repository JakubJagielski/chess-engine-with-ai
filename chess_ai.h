#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <algorithm>
#include <random>
#include "piece.h"
#include "board.h"
#include "utility.h"

namespace brd {}
namespace utl {}
namespace pcs {}

namespace ai {
	struct ai_result { double best_utility_value; std::string best_move; };
	class chess_ai
	{
	private:
		int max_depth;
		utl::colour my_colour;
		utl::colour opponent_colour;

		std::vector <std::vector<double>> white_pawn_bias;
		std::vector <std::vector<double>> black_pawn_bias;
		std::vector <std::vector<double>> white_king_bias;
		std::vector <std::vector<double>> black_king_bias;
		std::vector <std::vector<double>> white_rook_bias;
		std::vector <std::vector<double>> black_rook_bias;
		std::vector <std::vector<double>> bishop_bias;
		std::vector <std::vector<double>> knight_bias;
		std::vector <std::vector<double>> queen_bias;

	public:
		chess_ai(const utl::colour my_colour_input, const int max_depth_input);
		~chess_ai() {}

		double calculate_utility(brd::board* board_to_evaluate);
		int get_max_depth() { return max_depth; }
		void copy_board_contents(brd::board* original_board, brd::board* copy);
		std::string get_best_move(brd::board* game_board, brd::board* ai_board);
		ai_result minimax(const int depth, const bool is_max, brd::board* ai_board, double alpha, double beta);

		utl::colour get_my_colour() { return my_colour; }
		utl::colour get_opponent_colour() { return opponent_colour; }

	};
}

#endif