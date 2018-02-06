#include "TileDisplay.h"
#include <iostream>

void TileDisplay::mousePressEvent(QMouseEvent* event)
{
	if (manager != nullptr && manager->playing)
	{
		manager->onTileClicked(x, y);
	}
}

void TileDisplay::display()
{
	if (this->color)
	{
		this->setStyleSheet("QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(110,90,160);}");
	}
	else
	{
		this->setStyleSheet("QLabel {background-color: rgb(211, 211, 158);}:hover{background-color: rgb(110,90,160);}");
	}
}

void TileDisplay::displayPiece(int color, bool king)
{
	const int size = 60;
	if (color) // red piece
	{
		if (king)
		{
			this->setPixmap(
				QPixmap(":/Resources/piece_red_king.png").scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		}
		else
		{
			this->setPixmap(
				QPixmap(":/Resources/piece_red.png").scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		}
	}
	else // black piece
	{
		if (king)
		{
			this->setPixmap(
				QPixmap(":/Resources/piece_black_king.png").scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		}
		else
		{
			this->setPixmap(
				QPixmap(":/Resources/piece_black.png").scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		}
	}
	this->setAlignment(Qt::AlignCenter);
}

void TileDisplay::removePiece()
{
	this->clear();
}
