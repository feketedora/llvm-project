// RUN: %check_clang_tidy %s misc-illegal-layer-access %t

class MyView {};
class SubWindowClass : public MyView {};
class Persistence {};
class Model {};

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
// CHECK-MESSAGES: [[@LINE-1]]:12: warning: model class must not use view or its descendants [misc-illegal-layer-access]
  }
};

class View
{
  Persistence * dummy2;
// CHECK-MESSAGES: [[@LINE-1]]:17: warning: view class must not have persistence or its descendant fields [misc-illegal-layer-access]

  void fun ( int i, Persistence & d) {
// CHECK-MESSAGES: [[@LINE-1]]:35: warning: view class must not use persistence or its descendants [misc-illegal-layer-access]
  }
};

class MyPersistence
{
  MyView dummy1;
// CHECK-MESSAGES: [[@LINE-1]]:10: warning: persistence class must not have view or its descendant fields [misc-illegal-layer-access]
  Model * dummy2;
// CHECK-MESSAGES: [[@LINE-1]]:11: warning: persistence class must not have model or its descendant fields [misc-illegal-layer-access]

  int dummy3;

  void fun (MyView w, int i, Model & d) {
// CHECK-MESSAGES: [[@LINE-1]]:20: warning: persistence class must not use view or its descendants [misc-illegal-layer-access]
// CHECK-MESSAGES: [[@LINE-2]]:38: warning: persistence class must not use model or its descendants [misc-illegal-layer-access]
  }
};
