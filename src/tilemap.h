#ifndef TILEMAP_H
#define TILEMAP_H

#include <ostream>
#include <vector>

class TileMap
{
public:
	TileMap();
	~TileMap();

	void SetSize(int columns, int rows);
	void SetTile(int column, int row, int value);
	int getTile(int column, int row) const;
	void print(std::ostream &stream) const;
	int count() const;

private:
	int _columns { 0 };
	int _rows { 0 };

	std::vector<int> _tiles;
};

std::ostream& operator<< (std::ostream& stream, const TileMap&);

#endif // TILEMAP_H
