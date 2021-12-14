#include <iostream>

#include "../../../c++/util/y0_conio.h"
#include "board.hpp"

int main ()
{
  Board board;

  while (!board.is_the_end ()) {
    board.print_board ();
    //board.operate_cursor (ch);
    board.do_operate (getch ());
  }

  return 0;
}
