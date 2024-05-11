.. title:: clang-tidy - misc-public-members

misc-public-members
===================

Checks public fields in C++ records that are defined with the class keyword.

This checker binds the public fields of C++ record declarations,
if the record is introduced by the class keyword,
except when name of the class starts with ``Ui_`` or it matches the data
regular expression.

.. code-block:: c++
  class MyClass {
  public:
	int Var;
  };

All matched nodes are reported to the diagnostics with the corresponding explanation.
