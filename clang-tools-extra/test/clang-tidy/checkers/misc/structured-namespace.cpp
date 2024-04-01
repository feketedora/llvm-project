// RUN: %check_clang_tidy %s misc-structured-namespace %t

namespace view {
	class Model {};
}
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'f' is insufficiently awesome [misc-structured-namespace]


namespace my::model {
	class Model {};
}

namespace Model::my {
	class Mymodel {};
}

class MyModel {};
