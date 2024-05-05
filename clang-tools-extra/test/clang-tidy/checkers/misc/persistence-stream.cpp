// RUN: %check_clang_tidy %s misc-persistence-stream %t

// mock
namespace std {
  class fstream {};
}

class QFile {}; // mock

class MyStream : public std::fstream {};

class MyModel
{
  void write(std::fstream f){}
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: function handles files outside persistence class [misc-persistence-stream]
  void persisting() { QFile v; }
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: function handles files outside persistence class [misc-persistence-stream]
  void test2(MyStream & par) {}
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: function handles files outside persistence class [misc-persistence-stream]

  std::fstream thisisspecial;
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: file handling field outside persistence class [misc-persistence-stream]
  MyStream y;
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: file handling field outside persistence class [misc-persistence-stream]
  QFile f;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: file handling field outside persistence class [misc-persistence-stream]
  QFile * fptr;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: file handling field outside persistence class [misc-persistence-stream]

  void test(int a, bool b) {}
  bool mDummy;
  char file;
};

class MyPersistence {
  void write(QFile f){}
  void x() { std::fstream ofs; }
  std::fstream var;
};
