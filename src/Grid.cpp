#include <iostream>
#include <cassert>

#include "Grid.hpp"
#include "Tree.hpp"

GridCase::GridCase(Vector2D& pPosition, bool pWall) : position{pPosition}, isWall{pWall} {}
GridCase::GridCase(int x, int y, bool pWall) : isWall{pWall}
{
  position.x = x;
  position.y = y;
}

GridCase::GridCase()
{
  position.x = 0;
  position.y = 0;
}


Grid::Grid(Renderer &renderer, int width, int height) : _renderer{renderer}, _width{width}, _height{height} {

  // populating grid datastructure with empty grid cases (no walls)
  for (int i = 0; i < width; i++) {
    _grid.push_back( std::vector<GridCase>() );
    for (int j = 0; j < height; j++) {
      _grid[i].push_back(GridCase(i, j, false));
    }
  }

  // setting case rect according to provided width
  _case_size = (renderer.GetWidth() - ( (width + 1) * _line_width)) / width;
}

void Grid::RenderGrid()
{
  SDL_Rect tmp_case = {_line_width, _line_width, _case_size, _case_size};

  for (int i = 0; i < _width; i++) {

    for (int j = 0; j < _height; j++) {
      if (_grid[i][j].isWall)
      {
        _renderer.RenderRectangle(tmp_case, 0x00, 0x00, 0x00, 0xFF);

      } else
      {
        _renderer.RenderRectangle(tmp_case, 0xFF, 0xFF, 0xFF, 0xFF);
      }

      tmp_case.x += _case_size + _line_width;
    }
    tmp_case.x = _line_width;
    tmp_case.y += _line_width + _case_size;
  }
}

GridCase& Grid::operator() (int x, int y) {
  assert(((x < _width) && (y < _height)));
  return _grid[x][y];
}

GridCase& Grid::operator() (Vector2D& pos)
{
  assert(((pos.x < _width) && (pos.y < _height)));
  return _grid[pos.x][pos.y];
}

const GridCase& Grid::operator() (Vector2D& pos) const {
  assert(((pos.x < _width) && (pos.y < _height)));
  return _grid[pos.x][pos.y];
}



const GridCase& Grid::operator() (int x, int y) const {
  assert(((x < _width) && (y < _height)));
  return _grid[x][y];
}

Renderer& Grid::GetAttachedRenderer()
{
  return _renderer;
}

void Grid::GetMouseGrid(Vector2D& pos, int x, int y)
{
  pos.x = x / (_case_size + _line_width);
  pos.y = y / (_case_size + _line_width);
}

void Grid::RenderGridCase(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  assert(((x < _width) && (y < _height)));

  SDL_Rect tmp_case =
  {
    _line_width + (_line_width + _case_size) * y, // X-Axis
    _line_width + (_line_width + _case_size) * x, // Y-Axis
    _case_size,   // width
    _case_size    // height
  };

  _renderer.RenderRectangle(tmp_case, r, g, b, a);
}

void Grid::RandomizeGrid(float frequency, int seed)
{

}

GridCase& Grid::GetStartCase() {return _startCase;}
const GridCase& Grid::GetStartCase() const {return _startCase;}

GridCase& Grid::GetEndCase() {return _endCase;}
const GridCase& Grid::GetEndCase() const {return _endCase;}

void Grid::SetStartCase(GridCase startCase)
{
  _startCase = startCase;
}

void Grid::SetEndCase(GridCase endCase)
{
  _endCase = endCase;
}

void Grid::SetWallCase(int x, int y) {_grid[x][y].isWall = true;}
void Grid::SetWallCase(Vector2D pPosition) {_grid[pPosition.x][pPosition.y].isWall = true;}

void Grid::ClearCase(int x, int y) {_grid[x][y].isWall = false;}
void Grid::ClearCase(Vector2D pPosition) {_grid[pPosition.x][pPosition.y].isWall = false;}

void Grid::DrawStartCase()
{
  RenderGridCase(_startCase.position.x, _startCase.position.y, 0, 255, 0, 0xFF);
}

void Grid::DrawEndCase()
{
  RenderGridCase(_endCase.position.x, _endCase.position.y, 255, 0, 0, 0xFF);
}

void Grid::DrawPath(GridCaseNode* _lastChildNode)
{
  while(_lastChildNode != NULL)
  {
    RenderGridCase(_lastChildNode->position.x, _lastChildNode->position.y, 0, 0xFF, 0, 0xFF);
    _lastChildNode = _lastChildNode->came_from;
  }
}


bool Grid::IsCaseAccessible(Vector2D pPosition) const
{
  if ( (pPosition.x < 0) || (pPosition.y <0)) return false;
  if ( (pPosition.x >= _width) || (pPosition.y >= _height)) return false;
  return !_grid[pPosition.x][pPosition.y].isWall;
}

bool Grid::IsCaseAccessible(int x, int y) const
{
  if ( (x < 0) || (y < 0)) return false;
  if ( (x >= _width) || (y >= _height)) return false;
  return !_grid[x][y].isWall;
}
