#include "../inc/grid.hpp"
#include "../inc/utils.hpp"

namespace tetris {

void Grid::initialize_grid(int c, int l, sf::Vector2f grid_view_size,
                           sf::Vector2f grid_view_pos) {
  nb_lines = l;
  nb_columns = c;

  float ratio_d_sp = 0.15;
  float d = min(grid_view_size.x / (c + ratio_d_sp * (c - 1)),
                grid_view_size.y /
                    (l + ratio_d_sp * (l - 1))); // dimension of squares
  float sp = ratio_d_sp * d;                     // space_btw_squares

  sf::Vector2f pos_first_square{
      grid_view_pos.x +
          (grid_view_size.x - c * d - (c - 1) * ratio_d_sp * d) / 2,
      grid_view_pos.y +
          (grid_view_size.y - l * d - (l - 1) * ratio_d_sp * d) / 2};

  // Dynamic allocation of the grid
  grid_num = new int *[nb_columns + 4];
  if (grid_num == nullptr)
    throw std::runtime_error("Allocation de mémoire pour la grille");
  for (int i = 0; i < nb_columns + 4; i++) {
    grid_num[i] = new int[nb_lines + 4];
    if (grid_num[i] == nullptr)
      throw std::runtime_error("Allocation de mémoire pour la grille");
  }

  // Dynamic allocation of the grid_drawn (the displayed one)
  grid_drawn = new sf::RectangleShape *[nb_columns];
  if (grid_drawn == nullptr)
    throw std::runtime_error("Allocation de mémoire pour la grille_drawn");
  for (int i = 0; i < nb_columns; i++) {
    grid_drawn[i] = new sf::RectangleShape[nb_lines];
    if (grid_drawn[i] == nullptr)
      throw std::runtime_error("Allocation de mémoire pour la grille_drawn");
  }

  for (int i = 0; i < nb_columns; i++) {
    for (int j = 0; j < nb_lines; j++) {
      // Set up the square dimension
      grid_drawn[i][j].setSize({d, d});
      float i_ = (d + sp) * i + pos_first_square.x;
      float j_ = (d + sp) * j + pos_first_square.y;
      grid_drawn[i][j].setPosition({i_, j_});
    }
  }

  clean_grid_with_borders();
  clean_grid();

  list_color_block.push_back(color_empty_square); // Empty cases color
  list_color_block.push_back({180, 167, 214});    // Block_I color
  list_color_block.push_back({180, 167, 189});    // ...
  list_color_block.push_back({213, 166, 189});    // ..
  list_color_block.push_back({182, 215, 168});    // .
  list_color_block.push_back({249, 203, 156});
  list_color_block.push_back({234, 153, 153});
  list_color_block.push_back({38, 202, 107});
};

void Grid::clean_grid() {
  // Reset the grid (zeros)
  for (int i = 2; i < nb_columns + 2; ++i)
    for (int j = 2; j < nb_lines + 2; ++j)
      grid_num[i][j] = 0;
}

void Grid::clean_grid_with_borders() {
  // Reset the grid with its borders (zeros)
  for (int i = 0; i < nb_columns + 4; i++)
    for (int j = 0; j < nb_lines + 4; j++)
      grid_num[i][j] = 1;
}

int Grid::clean_full_lines(const std::vector<sf::Vector2i> &list_squares) {

  std::vector<int> list_j;
  std::vector<int> lines_full;
  int number_of_full_lines;

  /* We get the list of lines on which is current block */
  for (size_t k = 0; k < list_squares.size(); k++) {
    bool j_is_in_list_j = std::find(list_j.begin(), list_j.end(),
                                    list_squares[k].y) != list_j.end();
    if (!j_is_in_list_j)
      list_j.push_back(list_squares[k].y);
  }

  /* We get the full lines */
  for (auto const &line : list_j) {
    bool is_line_full = true;
    for (int col = 0; col < nb_columns; col++) {
      if (grid_num[col + 2][line] == 0) {
        is_line_full = false;
      }
    }
    if (is_line_full == true)
      lines_full.push_back(line);
  }

  /* For each full lines, we put the value of the square to 0 */
  for (auto const &line : lines_full) {
    for (int col = 0; col < nb_columns; col++) {
      grid_num[col + 2][line] = 0;
    }
  }

  /* All lines above the highest full line has to be lowered */
  number_of_full_lines = lines_full.size();
  if (number_of_full_lines > 0) {
    int lowest_line = *std::max_element(lines_full.begin(), lines_full.end());
    for (int line = lowest_line; line > 1 + number_of_full_lines; line--) {
      for (int i = 0; i < nb_columns; i++) {
        grid_num[i + 2][line] = grid_num[i + 2][line - number_of_full_lines];
      }
    }
    /*It is specific for the highest lines. They just have to be change in 0*/
    for (int line = 1 + number_of_full_lines; line > 1; line--) {
      for (int i = 0; i < nb_columns; i++) {
        grid_num[i + 2][line] = 0;
      }
    }
  }

  switch (number_of_full_lines) {
  case 1:
    return 40;
    break;
  case 2:
    return 100;
    break;
  case 3:
    return 300;
    break;
  case 4:
    return 1200;
    break;
  default:
    return 0;
  }
}

void Grid::set_case_value(int i, int j, int newValue) {
  grid_num[i][j] = newValue;
};

int Grid::get_case_value(int i, int j) const { return grid_num[i][j]; };

sf::Vector2i Grid::get_size() const { return {nb_columns, nb_lines}; };

void Grid::Call_Free_grid(std::string type_grid) {

  if (type_grid == "grid_num")
    Free_grid(grid_num, nb_columns + 4);
  else if (type_grid == "grid_drawn")
    Free_grid(grid_drawn, nb_columns);
  else
    printf("No grid to free\n");
};

template <typename TYPE_GRID>
void Grid::Free_grid(TYPE_GRID _grid, int _nb_col) {

  if (_grid == nullptr)
    throw(std::runtime_error("Liberation grille"));

  for (int i = 0; i < _nb_col; i++) {
    if (_grid[i] == nullptr) {
      delete[] _grid;
      throw(std::runtime_error("Liberation grille"));
    }
    delete[] _grid[i];
  }
  delete[] _grid;
}

template void Grid::Free_grid<int **>(int **, int);
template void Grid::Free_grid<sf::RectangleShape **>(sf::RectangleShape **,
                                                     int);


void Grid::draw_grid() {
  for (int i = 0; i < nb_columns; ++i) {
    for (int j = 0; j < nb_lines; ++j) {
      grid_drawn[i][j].setFillColor(get_color_block(grid_num[i + 2][j + 2]));
    }
  }
}

void Grid::set_color_block(int i, sf::Color c) { list_color_block[i] = c; }

sf::Color Grid::get_color_block(int k) const { return list_color_block[k]; };

void Grid::set_color_empty_block(sf::Color color) {
  color_empty_square = color;
}

sf::RectangleShape Grid::get_case_value_drawn(int i, int j) const {
  return grid_drawn[i][j];
}

bool Grid::is_empty_case(int i, int j,
                         const std::vector<sf::Vector2i> &list_squares) {
  if (get_case_value(i, j) == 0 || find_vector(list_squares, {i, j}))
    return true;
  else
    return false;
}

} // namespace tetris