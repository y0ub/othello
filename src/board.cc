#include "board.hpp"

const bool Y0_DEBUG = false;

Board::Board ()
{
  memset (board_array_, 0, sizeof (board_array_));

  cursor_x_            = 0;
  cursor_y_            = 0;
  which_player_        = 0;
  cannot_put_counter_  = 0;
  put_marker_counter_  = FIRST_MARKER_COUNTER_;
  score_first_         = 0;
  score_second_        = 0;
  able_reverse_marker_vec.resize (ARROW_DIRECTION_NUMBER);

  // put first marker
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      board_array_[BOARD_HEIGHT_ / 2 - 1 + i][BOARD_WIDTH_ / 2 - 1 + j] =
        (i == j ? -1 : 1);
    }
  }
}

void Board::print_board ()
{
  std::system ("clear");

  std::cout << BORDER_LINE_STRING_ << std::flush;
  for (int i = 0; i < BOARD_HEIGHT_; ++i) {
    printf ("｜");
    for (int j = 0; j < BOARD_WIDTH_; ++j) {
      std::cout <<
         ((i == cursor_y_ && j == cursor_x_) ?
         CURSOR_MARKER_ : board_array_[i][j] == -1 ?
         BLACK_MARKER_  : board_array_[i][j] == 1 ?
         WHITE_MARKER_  : "　") << std::flush;

    printf ("｜");
    }
    printf ("\n");
  std::cout << BORDER_LINE_STRING_ << std::flush;
  }

  if (Y0_DEBUG) {
    std::cout << "cursor_point" << cursor_x_ << " " << cursor_y_ << std::endl;
    std::cout << "put_marker_counter_: " << put_marker_counter_ << std::endl;
    std::cout << "cannot_put_counter_: " << cannot_put_counter_ << std::endl;
    for (int i = 0; i < BOARD_HEIGHT_; ++i) {
      for (int j = 0; j < BOARD_WIDTH_; ++j) {
        std::cout << board_array_[i][j] << ' ';
      }
      std::cout << std::endl;
    }
  }
}

void Board::do_operate (char input_char)
{
  if ('k' == input_char)
    put_marker ();
  else
    operate_cursor (input_char);
}

void Board::put_marker ()
{
  if (make_reverse_array (cursor_x_, cursor_y_)) {
    ++put_marker_counter_;
    board_array_[cursor_y_][cursor_x_] = (which_player_ ? 1 : -1);
    reverse ();

    if (is_put_able_next_player ((which_player_ + 1) % 2 ? 1 : -1)) {
      ++which_player_;
      which_player_ %= 2;
      cannot_put_counter_ = 0;
    }
    else {
      ++cannot_put_counter_;
    }
  }
}

void Board::operate_cursor (char ch)
{
  if (Y0_DEBUG) {
    std::cout << "input char" << " " << ch << std::endl;
  }
  switch (ch) {
    case 'w':
      cursor_y_ = std::max (--cursor_y_, 0);
      break;
    case 's':
      cursor_y_ = std::min (++cursor_y_, BOARD_HEIGHT_ - 1);
      break;
    case 'a':
      cursor_x_ = std::max (--cursor_x_, 0);
      break;
    case 'd':
      cursor_x_ = std::min (++cursor_x_, BOARD_WIDTH_ - 1);
      break;
  }
}

bool Board::make_reverse_array (int put_point_x, int put_point_y)
{
  bool able_make           = false;
  int  player_color_number = (which_player_ ? 1 : -1);
  int  index_counter       = -1;

  if (0 != board_array_[put_point_y][put_point_x])
    return false;

  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      if (0 == i && 0 == j)
        continue;

      int copy_x = put_point_x;
      int copy_y = put_point_y;

      copy_y += i;
      copy_x += j;

      ++index_counter;

      if (player_color_number ==  board_array_[copy_y][copy_x] ||
          0 == board_array_[copy_y][copy_x])
        continue;

      able_reverse_marker_vec[index_counter].push_back (std::make_pair (copy_x, copy_y));

      while (1) {
        copy_y += i;
        copy_x += j;

        // cannot reverse
        if (is_over_board (copy_x, copy_y) || 0 == board_array_[copy_y][copy_x]) {
          able_reverse_marker_vec[index_counter].clear ();
          break;
        }

        // can reverse
        if (player_color_number == board_array_[copy_y][copy_x]) {
          able_make = true;
          break;
        }
        able_reverse_marker_vec[index_counter].push_back (std::make_pair (copy_x, copy_y));
      }
    }
  }
  return able_make;
}

void Board::reverse ()
{
  if (Y0_DEBUG) {
    for (auto line : able_reverse_marker_vec) {
      for (auto element : line) {
        std::cout << "y0_debug_able_reverse_marker_vec_elements "
          << element.first << " " << element.second << std::endl;
      }
    }
    getch ();
  }
  int player = (which_player_ ? 1 : -1);
  for (auto line : able_reverse_marker_vec) {
    for (auto element : line) {
      board_array_[element.second][element.first] = player;
    }
  }
  able_reverse_marker_vec.clear ();
  able_reverse_marker_vec.resize (ARROW_DIRECTION_NUMBER);

  if (Y0_DEBUG) {
    for (auto line : able_reverse_marker_vec) {
      for (auto element : line) {
        std::cout << "y0_debug_able_reverse_marker_vec_elements "
          << element.first << " " << element.second << std::endl;
      }
    }
    getch ();
  }
}

bool Board::is_put_able (int player, int put_point_x, int put_point_y)
{
  bool  able_make           = false;
  int   player_color_number = player;
  int   index_counter       = -1;

  if (0 != board_array_[put_point_y][put_point_x])
    return false;

  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      if (0 == i && 0 == j)
        continue;

      int copy_x = put_point_x;
      int copy_y = put_point_y;

      copy_y += i;
      copy_x += j;

      ++index_counter;

      if (player_color_number ==  board_array_[copy_y][copy_x] ||
          0 == board_array_[copy_y][copy_x])
        continue;

      while (1) {
        copy_y += i;
        copy_x += j;

        // cannot reverse
        if (is_over_board (copy_x, copy_y) || 0 == board_array_[copy_y][copy_x]) 
          break;

        // can reverse
        if (player_color_number == board_array_[copy_y][copy_x]) {
          able_make = true;
          break;
        }
      }
    }
  }
  return able_make;
}

bool Board::is_over_board (int x, int y) 
{
  return (x < 0 || x >= BOARD_WIDTH_ || y < 0 || y >= BOARD_HEIGHT_);
}

bool Board::is_put_able_next_player (int next_player)
{
  for (int i = 0; i < BOARD_HEIGHT_; ++i) {
    for (int j = 0; j < BOARD_WIDTH_; ++j) {
      if (is_put_able (next_player, j, i)) {
        return true;
      }
    }
  }
  return false;
}

bool Board::is_the_end ()
{
  if (NUMBER_OF_MARKER_TYPES == cannot_put_counter_ ||
      BOARD_HEIGHT_ * BOARD_WIDTH_ == put_marker_counter_) {
    cursor_x_ = -1;
    cursor_y_ = -1;
    count_score ();
    print_board ();
    std::cout << "score of first  player: " << score_first_  << std::endl;
    std::cout << "score of second player: " << score_second_ << std::endl;

    return true;
  }
  return false;
}

void Board::count_score ()
{
  for (int i = 0; i < BOARD_HEIGHT_; ++i) {
    for (int j = 0; j < BOARD_WIDTH_; ++j) {
      board_array_[i][j] == -1 ?
        ++score_first_ : board_array_[i][j] == 1 ?
        ++score_second_ : 0;
    }
  }
}
