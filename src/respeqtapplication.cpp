//
// Created by Jochen Schäfer on 25.05.23.
//

#include "respeqtapplication.h"

#include <QFont>

RespeQtApplication::RespeQtApplication(int &argc, char **argv, int i)
    : QApplication(argc, argv, i) {

  auto font_= font("QMenu");
  setFont(font_);

  setOrganizationName("ZeeSoft");
  setOrganizationDomain("respeqt.org");
  setApplicationName("RespeQt");
}
