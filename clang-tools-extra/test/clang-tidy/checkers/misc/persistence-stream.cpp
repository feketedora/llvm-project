// RUN: %check_clang_tidy %s misc-persistence-stream %t

#include <iostream>
#include <fstream>

class MyStream : public std::fstream {};

class MyModel
{
	void write(std::fstream f){}
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file stream parameter outside persistence class [misc-persistence-stream]
	void test(int a, bool b) {}

	int mFile;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file stream field outside persistence class [misc-persistence-stream]
	bool mDummy;
	char file2;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file stream field outside persistence class [misc-persistence-stream]
	std::fstream thisisspecial;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file stream field outside persistence class [misc-persistence-stream]
	MyStream y;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file stream field outside persistence class [misc-persistence-stream]
};

class MyPersistence {};

class MyPersister {};

class MyDataAccess {};

class mydataaccess2 {};
