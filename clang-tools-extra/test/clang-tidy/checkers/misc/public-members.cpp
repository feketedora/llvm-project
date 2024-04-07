// RUN: %check_clang_tidy %s misc-public-members %t

class Model {};

class MyClass {
	public:
		void f() {}
		Model n() { return privModel; }
		Model * g() { return nullptr; }
		// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function 'f' is insufficiently awesome [misc-public-members]
		Model & h() { return privModel; }
		// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function 'f' is insufficiently awesome [misc-public-members]
		Model const & k() { return privModel; }
		const Model & p() { return privModel; }
		Model pubModel;
		// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: function 'f' is insufficiently awesome [misc-public-members]
		int somenumber;
	private:
		Model privModel;
		Model & m() { return privModel; }
};

