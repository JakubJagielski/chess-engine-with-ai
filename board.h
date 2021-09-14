#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector> 
#include <memory>

#include "piece.h"
#include "utility.h"

// forward declerations
namespace pcs { class piece; }
namespace utl {}

namespace brd {
	class board
	{
	private: 
		std::vector<std::vector<std::shared_ptr<pcs::piece>>> board_content; // contains pointers to pieces. 
		utl::colour current_turn;

		// stores move history
		std::vector<std::string> moves;

	public:
		board();
		~board();

		void board::move(const std::string full_input, const bool player, const bool record);
		std::shared_ptr<pcs::piece> identify_piece(const utl::location position);
		void set_piece(const utl::location position, const std::shared_ptr <pcs::piece> piece_pointer);
		utl::colour get_current_turn();
		void display_board();
		void change_turn();
		void set_turn(const utl::colour colour) { current_turn = colour; }
		int test_check(const utl::colour colour);
		void record_move(const std::string move_string);
		void display_move_history();
		utl::move_struct_string get_move_struct_string(const std::string raw_move_string); // splits up full command string into parts
		bool brd::board::only_kings_left(); // function to test whether only kings are left
		bool draw_by_repetition();          // function to check whether the same moves have been made 3 times

		std::vector <std::string> get_all_legal_moves();

		static int letter_to_int(char letter) {
			switch (letter) {
			case 'A': return 0;
			case 'B': return 1;
			case 'C': return 2;
			case 'D': return 3;
			case 'E': return 4;
			case 'F': return 5;
			case 'G': return 6;
			case 'H': return 7;
			default: return 100;
			}
		}

		static std::string int_to_letter(int number) {
			switch (number) {
			case 0: return "A";
			case 1: return "B";
			case 2: return "C";
			case 3: return "D";
			case 4: return "E";
			case 5: return "F";
			case 6: return "G";
			case 7: return "H";
			default: return "Z";
			}
		}

		static bool boundary_check(int _x, int _y) {
			// sanity check
			if (_x > -1 && _x < 8 && _y > -1 && _y < 8) {
				return true;
			} else {
				return false;
			}
		}

		static utl::location convert_notation_to_location(std::string notation) {
			utl::location temp;
			temp.x = letter_to_int(notation[0]);
			temp.y = 8 - (notation[1] - '0');

			return temp;
		}

		static std::string convert_location_to_notation(utl::location loc) {
			std::string temp;
			temp += int_to_letter(loc.x);
			temp += std::to_string(8 - loc.y);
			return temp;
		}
	};
}

#endif