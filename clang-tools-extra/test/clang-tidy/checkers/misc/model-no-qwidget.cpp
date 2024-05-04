// RUN: %check_clang_tidy %s misc-model-no-qwidget %t

class QWidget {}; // mock

class MyModel : public QWidget {};
// CHECK-MESSAGES: [[@LINE-1]]:7: warning: model class must not derive from QWidget [misc-model-no-qwidget]

class Derived : public QWidget {};
class SecretModel : public Derived {};
// CHECK-MESSAGES: [[@LINE-1]]:7: warning: model class must not derive from QWidget [misc-model-no-qwidget]

class View : public QWidget {};

class Model
{
	QWidget dummy1;
	// CHECK-MESSAGES: [[@LINE-1]]:10: warning: model class must not have QWidget or its descendant fields [misc-model-no-qwidget]
	Derived * dummy2;
	// CHECK-MESSAGES: [[@LINE-1]]:12: warning: model class must not have QWidget or its descendant fields [misc-model-no-qwidget]

	int dummy3;

	void fun1 (QWidget w, int i, Derived & d) {}
	// CHECK-MESSAGES: [[@LINE-1]]:21: warning: model class must not use QWidget or its descendants [misc-model-no-qwidget]
	// CHECK-MESSAGES: [[@LINE-2]]:41: warning: model class must not use QWidget or its descendants [misc-model-no-qwidget]

	void fun2 () {
		QWidget var;
		// CHECK-MESSAGES: [[@LINE-1]]:11: warning: model class must not use QWidget or its descendants [misc-model-no-qwidget]
	}

	Derived fun3 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:10: warning: model class methods must not return QWidget or its descendants [misc-model-no-qwidget]

	Derived const & fun4 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:18: warning: model class methods must not return QWidget or its descendants [misc-model-no-qwidget]
};
