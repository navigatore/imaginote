#include <QApplication>

#include "mainwindow.h"

#ifdef QT_DEBUG
#include <gtest/gtest.h>
#endif  // QT_DEBUG

int main_(int argc, char* argv[]);

int main_(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return QApplication::exec();
}

int main(int argc, char* argv[]) {
#ifdef QT_DEBUG
  testing::InitGoogleTest(&argc, argv);
  auto x = RUN_ALL_TESTS();
  if (x != 0) {
    return x;
  }
#endif  // QT_DEBUG
#ifndef TEST_ONLY
  return main_(argc, argv);
#endif
}
