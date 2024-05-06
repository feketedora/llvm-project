// RUN: %check_clang_tidy %s misc-illegal-layer-access %t

class MyView {};
class SubWindowClass : public MyView {};

class MyModel
{
  MyView dummy1;
// CHECK-MESSAGES: [[@LINE-1]]:10: warning: model class must not have view or its descendant fields [misc-illegal-layer-access]
  SubWindowClass * dummy2;
// CHECK-MESSAGES: [[@LINE-1]]:20: warning: model class must not have view or its descendant fields [misc-illegal-layer-access]

  int dummy3;

  void fun (MyView w, int i, SubWindowClass & d) {
// CHECK-MESSAGES: [[@LINE-1]]:20: warning: model class must not use view or its descendants [misc-illegal-layer-access]
// CHECK-MESSAGES: [[@LINE-2]]:47: warning: model class must not use view or its descendants [misc-illegal-layer-access]
    MyView * var;
// CHECK-MESSAGES: [[@LINE-1]]:14: warning: model class must not use view or its descendants [misc-illegal-layer-access]
  }
};
