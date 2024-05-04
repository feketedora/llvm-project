// RUN: %check_clang_tidy %s misc-representation-leak %t

class Model {};

class MyClass {
	public:
		void c() const {}
		void f() {}
		Model n() { return privModel; }
		Model * g() { return nullptr; }
		// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function 'f' is insufficiently awesome [misc-representation-leak]
		Model & h() { return privModel; }
		// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function 'f' is insufficiently awesome [misc-representation-leak]
		Model const & k() { return privModel; }
		const Model & p() { return privModel; }
		Model pubModel;
		// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: function 'f' is insufficiently awesome [misc-representation-leak]
		int somenumber;
	private:
		Model privModel;
		Model & m() { return privModel; }
};

