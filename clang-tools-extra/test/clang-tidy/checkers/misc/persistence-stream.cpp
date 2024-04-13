// RUN: %check_clang_tidy %s misc-persistence-stream %t

#include <iostream>
#include <fstream>

class MyStream : public std::fstream {};

class MyModel
{
	int mFile;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file field outside persistence class [misc-persistence-stream]
	bool mDummy;
	char file2;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file field outside persistence class [misc-persistence-stream]
	std::fstream thisisspecial;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file field outside persistence class [misc-persistence-stream]
	MyStream y;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: found file field outside persistence class [misc-persistence-stream]
};

class MyPersistence {};

class MyPersister {};

class MyDataAccess {};

class mydataaccess2 {};
