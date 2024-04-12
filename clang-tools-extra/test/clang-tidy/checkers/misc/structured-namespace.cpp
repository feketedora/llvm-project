// RUN: %check_clang_tidy %s misc-structured-namespace %t

namespace view {
	class oneModel {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: model class cannot be in view namespace [misc-structured-namespace]
}

namespace my::model {
	class twoModel {};
}

namespace Model::my {
	class ThreeMymodel {};
}

class FourMyModel {};

class View {};

namespace my::awesome::view {
	class HereIsMyview{};
};

namespace Modeling {
	class WrongPlacedView {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in model namespace [misc-structured-namespace]
}
