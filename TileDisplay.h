#ifndef TILE_DISPLAY_H
#define TILE_DISPLAY_H

#include <QLabel>
#include "GameManager.h"

class TileDisplay : public QLabel
{
public:

	int x, y;
	int color;
	GameManager* manager;

	TileDisplay(int x_, int y_, GameManager* game_, QWidget* parent = 0, Qt::WindowFlags flags = 0) :
		QLabel(parent, flags), manager(game_), x(x_), y(y_) {}

	void display();
	void displayPiece(int color, bool king);
	void removePiece();

	void mousePressEvent(QMouseEvent* event) override;
};

#endif
