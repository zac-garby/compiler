# compiler

A compiler for some made up language. The plan is to compile to ARM assembly, but that may or not actually happen.

The syntax will probably look something like this:

```
fn sum (x *int, length int) -> int {
  var i, sum int;
  
  sum = 0;
  
  for (i = 0; i < length; i++) {
    sum += x[i];
  }
  
  return sum;
}

fn main () {
  var x *int;
  
  x = malloc(5);
  
  x[0] = 2;
  x[1] = 4;
  x[2] = 12;
  x[3] = 5;
  x[4] = -2;
  
  print_i(sum(x, 5) * 2); //-> 42
  
  free(x);
}
```
