#include "BoardDisplay.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	BoardDisplay board;
	board.display();

	return a.exec();
}

