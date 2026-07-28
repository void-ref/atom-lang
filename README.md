# Atom - Toy Scripting Language

This is a small interpreter for a simple scripting language. Made for an old extra credit project (~2018) during university days.

Examples are in the `examples` directory.


## Overview

Types:
```
null           - A special type used to signify non-existence of a value
numbers        - Expressed using decimals, or scientific notation (e.g. 1.2e9)
strings        - Expressed using characters inside of double quotes. Escape characters are not yet supported.
tables         - An object storing any number of key-value pairs. Any value in the table
                    which is set to null will be erased.
functions      - A function defined script-wise
user functions - A function written in C and loaded onto the VM from the C side
```

Keywords:
```
null

if
else
end

and         - For boolean comparison, parenthesis should be
or            placed around both the left and right side expressions
xor

func        - Used for function declaration (WIP)
```

Operators:
```
a + b       - Returns 'a' plus 'b'
a - b       - Returns 'a' minus 'b'
a * b       - Returns 'a' multiplied by 'b'
a / b       - Returns 'a' divided by 'b'
a ^ b       - Returns 'a' raised to the 'b'
a % b       - Returns 'a' mod 'b'
-a          - Returns negative 'a'
a!          - Returns 'a' factorial

~           - Returns the boolean negation of 'a'
a == b      - Returns 'true' if 'a' equals 'b'
a ~= b      - Returns 'true' if 'a' does not equal 'b'
a > b       - Returns 'true' if 'a' is greater than 'b'
a >= b      - Returns 'true' if 'a' is greater than or equal to 'b'
a < b       - Returns 'true' if 'a' is less than 'b'
a <= b      - Returns 'true' if 'a' is less than or equal to 'b'

a = b       - Assigns the value of 'b' to 'a'. If 'a' does not exist in the
                current scope, it is stored in the global scope
a.b         - Indexes the table 'a' with key 'b'
@f(a, ...)  - Calls the function 'f' with a list of arguments
```

IO library (io):
```
io.print(x)                  - prints a value (not necessarily a string) to the console, returns nothing
io.getline()                 - scans a string from the console, returns the string
io.writefile(file, contents) - writes a text file, returns 1 on success, 0 on failure
io.readfile(file)            - reads a text file, returns the contents as a string, or null if failed
```

Math library (math):
```
math.pi         - 3.141592653....
math.e          - euler's number

math.sin(x)
math.cos(x)
math.tan(x)
math.asin(x)
math.acos(x)
math.atan(x)
math.sqrt(x)    - returns the square root of a number
math.cbrt(x)    - returns the cube root of a number
math.log(x)     - returns the natural logarithm of a number
math.log10(x)   - returns log base 10 of a number
math.logb(x, b) - returns log base 'b' of 'x'
math.abs(x)     - returns the absolute value of a number
math.sign(x)    - returns -1 for negative numbers, 0 for 0, and +1 for positive numbers
math.floor(x)
math.ceil(x)
```

"Standard" library (std):
```
true          - defined as the number 1
false         - defined as the number 0
exit()        - ends the program
typeof(x)     - returns the type of the input as a string (e.g. @typeof(4) -> 'number')
to_number(x)  - converts 'x' to a number (e.g. @to_number("5.3") -> 5.3)
to_string(x)  - converts 'x' to a string (e.g. @to_string(4.3) -> '4.3')
system(cmd)   - runs a system command (e.g. @system("cat file.txt") prints out the contents of file.txt)
import(str)   - reads a file and executes the code in it (WIP)
std.eval(str) - evaluates a string as code (WIP)
```

Table library (table):
```
make()        - Returns a new empty table
```
