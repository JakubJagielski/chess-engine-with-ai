#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <memory>

#include "board.h"
#include "utility.h"

// forward declerations
namespace brd {class board; }
namespace utl {}

namespace pcs {
	class piece
	{
	public:
		virtual ~piece() {};
		virtual utl::colour get_colour() = 0;
		virtual void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board) = 0;
		virtual char get_board_graphic() = 0;
	};
	
	class empty_space : public piece
	{
	private:
		utl::colour piece_colour{ utl::colour::none };
		const char board_graphic{'.'};
	public:
		empty_space() {}
		~empty_space() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board) { std::cout << "none" << std::endl; }
		char get_board_graphic() { return board_graphic; }
	};
	

	class pawn : public piece
	{
	private:
		utl::colour piece_colour;
		const char board_graphic{ 'p' };
	public:
		pawn(const utl::colour colour_input) :piece_colour{ colour_input } {}
		~pawn() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board);
		char get_board_graphic() { return board_graphic; }
	};
	
	class rook : public piece
	{
	private:
		utl::colour piece_colour;
		const char board_graphic{ 'r' };
	public:
		rook(const utl::colour colour_input) :piece_colour{ colour_input } {}
		~rook() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board);
		char get_board_graphic() { return board_graphic; }
	};

	class bishop : public piece
	{
	private:
		utl::colour piece_colour;
		const char board_graphic{ 'b' };
	public:
		bishop(const utl::colour colour_input) :piece_colour{ colour_input } {}
		~bishop() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board);
		char get_board_graphic() { return board_graphic; }
	};

	class knight : public piece
	{
	private:
		utl::colour piece_colour;
		const char board_graphic{ 'n' };
	public:
		knight(const utl::colour colour_input) :piece_colour{ colour_input } {}
		~knight() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board);
		char get_board_graphic() { return board_graphic; }
	};

	class king : public piece
	{
	private:
		utl::colour piece_colour;
		const char board_graphic{ 'k' };
	public:
		king(const utl::colour colour_input) :piece_colour{ colour_input } {}
		~king() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board);
		char get_board_graphic() { return board_graphic; }
	};

	class queen : public piece
	{
	private:
		utl::colour piece_colour;
		const char board_graphic{ 'q' };
	public:
		queen(const utl::colour colour_input) :piece_colour{ colour_input } {}
		~queen() {}
		utl::colour get_colour() { return piece_colour; }
		void get_legal_moves(const utl::location position, bool(&bool_array)[8][8], brd::board* current_board);
		char get_board_graphic() { return board_graphic; }
	};
}

#endif