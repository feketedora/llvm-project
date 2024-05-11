.. title:: clang-tidy - misc-persistence-file

misc-persistence-file
=======================

Checks whether there is file handling outside of the persistence component.

In the correct structure only the persistence layer should access the disk
and save to or load from files.
Besides the common C++ streams, the types are extended by QFile from Qt.

There are 2 matchers in this checker, the first one filters the fields of
C++ classes that do not match the persistence component's regular expression,
except when the class name starts with ``Ui_``, and the fields are of or derived from
any of the following types: ``std::fstream``, ``std::ifstream``, ``std::ofstream``,
``QFile``, including references and pointers.

.. code-block:: c++
  class MyModel {
    QFile F;
  };

The second matcher filters the variables
(including function parameters, references and pointers) of C++ methods
in classes that do not match the persistence component's regular expression and
they are not of any of the file stream classes, but the variables are of
any of these types.

.. code-block:: c++
  class Model {
    void write(QFile * F);
  };

All matched nodes are reported to the diagnostics with the corresponding explanation.
