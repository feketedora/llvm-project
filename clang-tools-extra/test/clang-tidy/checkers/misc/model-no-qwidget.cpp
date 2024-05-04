// RUN: %check_clang_tidy %s misc-model-no-qwidget %t -- 
/*
#include <QWidget>

class MyModel : public QWidget {};
// CHECK-MESSAGES: [[@LINE-1]]:3: model class must not derive from QWidget

class Derived : public QWidget {};
class SecretModel : public Derived {};
// CHECK-MESSAGES: [[@LINE-1]]:3: model class must not derive from QWidget

class View : public QWidget {};

class Model
{
	QWidget dummy1;
	// CHECK-MESSAGES: [[@LINE-1]]:3: model class must not have QWidget or its descendant fields
	Derived * dummy2;
	// CHECK-MESSAGES: [[@LINE-1]]:3: model class must not have QWidget or its descendant fields

	int dummy3;

	void fun1 (QWidget w, int i, Derived & d) {}
	// CHECK-MESSAGES: [[@LINE-1]]:16: model class must not use QWidget or its descendants
	// CHECK-MESSAGES: [[@LINE-2]]:34: model class must not use QWidget or its descendants

	void fun2 () {
		QWidget var;
		// CHECK-MESSAGES: [[@LINE-1]]:3: model class must not use QWidget or its descendants
	}

	Derived fun3 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:3: model class methods must not return QWidget or its descendants

	Derived const & fun4 () {return *dummy2;}
	// CHECK-MESSAGES: [[@LINE-1]]:3: model class methods must not return QWidget or its descendants
};
*/
