// RUN: %check_clang_tidy %s misc-qwidget-base %t

class QWidget {}; // mock
class QObject {}; // mock

class MyModel : public QWidget {};
// CHECK-MESSAGES: [[@LINE-1]]:7: warning: model class must not derive from QWidget [misc-qwidget-base]

class Derived : public QWidget {};
class SecretModel : public Derived {};
// CHECK-MESSAGES: [[@LINE-1]]:7: warning: model class must not derive from QWidget [misc-qwidget-base]

class View : public QWidget {};

class Model
{
	QWidget dummy1;
	// CHECK-MESSAGES: [[@LINE-1]]:10: warning: model class must not have QWidget or its descendant fields [misc-qwidget-base]
	Derived * dummy2;
	// CHECK-MESSAGES: [[@LINE-1]]:12: warning: model class must not have QWidget or its descendant fields [misc-qwidget-base]

	int dummy3;

	void fun1 (QWidget w, int i, Derived & d) {}
	// CHECK-MESSAGES: [[@LINE-1]]:21: warning: model class must not use QWidget or its descendants [misc-qwidget-base]
	// CHECK-MESSAGES: [[@LINE-2]]:41: warning: model class must not use QWidget or its descendants [misc-qwidget-base]

	void fun2 () {
		QWidget var;
		// CHECK-MESSAGES: [[@LINE-1]]:11: warning: model class must not use QWidget or its descendants [misc-qwidget-base]
	}

	Derived fun3 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:10: warning: model class methods must not return QWidget or its descendants [misc-qwidget-base]

	Derived const & fun4 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:18: warning: model class methods must not return QWidget or its descendants [misc-qwidget-base]
};

class MyPersistence
{
	QWidget dummy1;
	// CHECK-MESSAGES: [[@LINE-1]]:10: warning: persistence class must not have QWidget or its descendant fields [misc-qwidget-base]
	Derived * dummy2;
	// CHECK-MESSAGES: [[@LINE-1]]:12: warning: persistence class must not have QWidget or its descendant fields [misc-qwidget-base]

	int dummy3;

	void fun1 (QWidget w, int i, Derived & d) {}
	// CHECK-MESSAGES: [[@LINE-1]]:21: warning: persistence class must not use QWidget or its descendants [misc-qwidget-base]
	// CHECK-MESSAGES: [[@LINE-2]]:41: warning: persistence class must not use QWidget or its descendants [misc-qwidget-base]

	void fun2 () {
		QWidget var;
		// CHECK-MESSAGES: [[@LINE-1]]:11: warning: persistence class must not use QWidget or its descendants [misc-qwidget-base]
	}

	Derived fun3 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:10: warning: persistence class methods must not return QWidget or its descendants [misc-qwidget-base]

	Derived const & fun4 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:18: warning: persistence class methods must not return QWidget or its descendants [misc-qwidget-base]
};

class myView : public QObject {};
// CHECK-MESSAGES: [[@LINE-1]]:7: warning: view class must derive from QWidget [misc-qwidget-base]

class Ui_MyView {}; // mock
class MyView : public Ui_MyView {};
