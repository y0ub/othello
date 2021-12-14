#ifndef OTHELLO_SRC_BOARD
#define OTHELLO_SRC_BOARD

#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#include "../../../c++/util/y0_conio.h"

class Board
{
  private:
    static constexpr std::string_view BLACK_MARKER_       = "🔴";
    static constexpr std::string_view WHITE_MARKER_       = "⚪️";
    static constexpr std::string_view CURSOR_MARKER_      = "🔳";
    static constexpr std::string_view BORDER_LINE_STRING_ =
      "＋ーーーーーーーーーーーーーーー＋\n";

    static const int NUMBER_OF_MARKER_TYPES = 2;
    static const int ARROW_DIRECTION_NUMBER = 8;
    static const int FIRST_MARKER_COUNTER_  = 4;

    int put_marker_counter_;
    int cannot_put_counter_;

    std::vector<std::vector <std::pair<int, int> > > able_reverse_marker_vec;

    void put_marker ();
    void operate_cursor (char ch);
    bool make_reverse_array (int put_point_x, int put_point_y);
    void reverse ();
    bool is_put_able (int player, int put_point_x, int put_point_y);
    bool is_put_able_next_player (int next_player_color_number);
    bool is_over_board (int x, int y);

  public:
    static const int BOARD_HEIGHT_ = 8;
    static const int BOARD_WIDTH_  = 8;

    int board_array_[BOARD_HEIGHT_][BOARD_WIDTH_];
    int cursor_x_; 
    int cursor_y_;
    // 1 => white 0 => black
    int which_player_;
    int score_first_;
    int score_second_;

    Board ();
    void print_board ();
    void do_operate (char input_char);
    bool is_the_end ();
    void count_score ();
};

#endif
