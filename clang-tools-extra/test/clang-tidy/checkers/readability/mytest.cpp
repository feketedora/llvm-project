// RUN: %check_clang_tidy %s readability-simplify-bool-expressions %t

#include "iostream.h"

void f(bool cond)
{
  if (cond == false)
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {
    std::cout << "hey\n";
  }
}

void f2(bool cond)
{
  if (cond == true)
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond)
  {
    std::cout << "hey\n";
  }
}

void f3(bool cond)
{
  if (false == cond)
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {
    std::cout << "hey\n";
  }
}

void f4(bool cond)
{
  if (cond)
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond)
  {
    std::cout << "hey\n";
  }
}

void f5(bool cond)
{
  if (cond != true)
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {
    std::cout << "hey\n";
  }
}

void f6(bool cond, bool cond2)
{
  if (cond2 && cond != true)
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond2 && !cond)
  {
    std::cout << "hey\n";
  }
}

void f_good(bool cond)
{
  if (cond)
  {
    std::cout << "hey\n";
  }
}

void f_good2(bool cond, bool cond2)
{
  if (cond == cond2)
  {
    std::cout << "hey\n";
  }
}
