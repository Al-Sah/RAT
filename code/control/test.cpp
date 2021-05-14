
#include "test.h"

void getSomeLayout(QGridLayout *grid, QWidget *parent){

    grid->addWidget(new QPushButton("OK", parent),  0, 0);
    grid->addWidget(new QPushButton("OK", parent),  0, 2);
    grid->addWidget(new QPushButton("OK", parent), 1, 1);
    grid->addWidget(new QPushButton("OK", parent), 2, 2);
    grid->addWidget(new QPushButton("OK", parent), 2, 0);
}
