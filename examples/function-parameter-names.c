// Parameter names don't match or are unnamed - this is NOT permitted
void fn1(int a, char* b, double); // declaration - one parameter unnamed - wrong

void fn1(int i, char* s, double d)
{ // definition - names don't match - wrong
  // code here
}

// The following is OK because the parameter names match
// (Though the names should be more meaningful!)
void fn2(int a, char* s, double d); // declaration

void fn2(int a, char* s, double /* d */)
{ // definition, d not used in function
  // code here
}

int main(int /* argc */, char* argv[])
{ // argv used but not argc
  // code here
}
