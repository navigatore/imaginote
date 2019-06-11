#include <QApplication>
#include "mainwindow.h"

int _main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

#ifdef QT_DEBUG
#include <gtest/gtest.h>
int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  auto x = RUN_ALL_TESTS();
  if (x != 0) {
    return x;
  }
#endif  // QT_DEBUG
#ifndef TEST_ONLY
  return _main(argc, argv);
#endif
}
