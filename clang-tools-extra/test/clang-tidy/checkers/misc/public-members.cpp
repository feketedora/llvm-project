// RUN: %check_clang_tidy %s misc-public-members %t

struct Struct {
  int var;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: avoid using public members, use private members with getter and setter methods instead [misc-public-members]
private:
  bool pred;
};

class MyClass {
  bool privateVar;
public:
  float leaking;
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: avoid using public members, use private members with getter and setter methods instead [misc-public-members]
};

class Ui_GeneratedClass {
public:
  bool legallyPublicVar;
};

class PlainData {
public:
  int legallyPublicVar;
};
