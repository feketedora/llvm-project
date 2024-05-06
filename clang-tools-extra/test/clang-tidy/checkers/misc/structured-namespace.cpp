// RUN: %check_clang_tidy -std=c++17-or-later %s misc-structured-namespace %t

namespace my::model {
	class twoModel {};
}

namespace Model::my {
	class ThreeMymodel {};
}

class FourMyModel {};

class View {};

class ThePersister {};

namespace my::awesome::view {
	class HereIsMyview{};
};

namespace Modeling {
	class WrongPlacedView {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in model namespace [misc-structured-namespace]
	class WrongPlacedWindow {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in model namespace [misc-structured-namespace]
	class Thepersister {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in model namespace [misc-structured-namespace]
	class Persistence {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in model namespace [misc-structured-namespace]
	class SuperDataaccess {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in model namespace [misc-structured-namespace]
}

namespace view {
	class oneModel {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: model class cannot be in view namespace [misc-structured-namespace]
}

namespace window {
	class oneModel {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: model class cannot be in view namespace [misc-structured-namespace]
}

namespace my::view {
	class DataAccess {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in view namespace [misc-structured-namespace]
	class Persister {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in view namespace [misc-structured-namespace]
	class Mypersistence {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in view namespace [misc-structured-namespace]
}

namespace my::window {
	class DataAccess {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in view namespace [misc-structured-namespace]
	class Persister {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in view namespace [misc-structured-namespace]
	class Mypersistence {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: persistence class cannot be in view namespace [misc-structured-namespace]
}

namespace persistence {
	class View {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in persistence namespace [misc-structured-namespace]
	class Window {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in persistence namespace [misc-structured-namespace]
	class Model {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: model class cannot be in persistence namespace [misc-structured-namespace]
}

namespace the::persister::ns {
	class View {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in persistence namespace [misc-structured-namespace]
	class Window {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in persistence namespace [misc-structured-namespace]
	class Model {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: model class cannot be in persistence namespace [misc-structured-namespace]
}

namespace DataAccess::detail {
	class View {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in persistence namespace [misc-structured-namespace]
	class Window {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: view class cannot be in persistence namespace [misc-structured-namespace]
	class Model {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: model class cannot be in persistence namespace [misc-structured-namespace]
}
