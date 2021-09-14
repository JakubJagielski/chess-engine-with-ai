#ifndef UTILITY_H
#define UTILITY_H

namespace utl {
	enum class colour { black, white, none };
	struct location { int x; int y; };
	struct move_struct_string { std::string initial_location; std::string final_location; };
}

#endif