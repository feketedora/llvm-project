// RUN: %check_clang_tidy %s misc-representation-leak %t

class Model {};

class MyClass {
  public:
	void constFunc() const {}
	void simpleFunc() {}
	Model copyReturningFunc() { return privateModel; }
	Model * pointerFunc() { return nullptr; }
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: method leaks representation via return type [misc-representation-leak]
	Model & refFunc() { return privateModel; }
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: method leaks representation via return type [misc-representation-leak]
	Model const & constRefFunc() { return privateModel; }
	const Model & constRefFunc2() { return privateModel; }
	Model publicModel;
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: field leaks representation [misc-representation-leak]
	Model * publicModelPtr;
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: field leaks representation [misc-representation-leak]
	int somenumber;
  private:
	Model privateModel;
	Model & privateRefFunc() { return privateModel; }
};

