// RUN: %check_clang_tidy %s readability-simplify-bool-expressions %t

void f(bool cond)
{
  if (cond == false)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {

  }
}

void f2(bool cond)
{
  if (cond == true)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond)
  {

  }
}

void f3(bool cond)
{
  if (false == cond)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {

  }
}

void f4(bool cond)
{
  if (true == cond)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond)
  {

  }
}

void f5(bool cond)
{
  if (cond != true)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {

  }
}

void f6(bool cond)
{
  if (cond != false)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond)
  {

  }
}

void f7(bool cond)
{
  if (true != cond)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (!cond)
  {

  }
}

void f8(bool cond)
{
  if (false != cond)
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond)
  {

  }
}

void f9(bool cond, bool cond2)
{
  if (cond2 && cond != true)
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: expression can be simplified [readability-simplify-bool-expressions]
// CHECK-FIXES: if (cond2 && !cond)
  {

  }
}

void f_good(bool cond)
{
  if (cond)
  {

  }
}

void f_good2(bool cond, bool cond2)
{
  if (cond == cond2)
  {

  }
}
