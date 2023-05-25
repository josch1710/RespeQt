//
// Created by Jochen Schäfer on 25.05.23.
//

#ifndef RESPEQT_RESPEQTAPPLICATION_H
#define RESPEQT_RESPEQTAPPLICATION_H

#include <QApplication>

class RespeQtApplication: public QApplication {
public:
  RespeQtApplication(int &argc, char **argv, int i = ApplicationFlags);
};


#endif//RESPEQT_RESPEQTAPPLICATION_H
