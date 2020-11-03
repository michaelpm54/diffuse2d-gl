#include "tilemap.h"

#include <cstdio>
#include <iomanip>
#include <iostream>

TileMap::TileMap()  = default;
TileMap::~TileMap() = default;

void TileMap::SetSize(int columns, int rows)
{
	_columns = columns;
	_rows    = rows;
	_tiles.resize(columns * rows);
}

void TileMap::SetTile(int column, int row, int value)
{
	_tiles[(row * _columns) + column] = value;
}

int TileMap::getTile(int column, int row) const
{
	return _tiles[(row * _columns) + column];
}

void TileMap::print(std::ostream &stream) const
{
	for (int y = 0; y < _rows; y++)
	{
		for (int x = 0; x < _columns; x++)
		{
			stream << std::setfill('0') << std::hex << std::uppercase << std::setw(2) << getTile(x, y) << " ";
		}
		stream << std::endl;
	}
	stream << std::resetiosflags(std::ios_base::basefield);
}

int TileMap::count() const
{
	return _columns * _rows;
}

std::ostream &operator<<(std::ostream &stream, const TileMap &tm)
{
	tm.print(stream);
	return stream;
}
