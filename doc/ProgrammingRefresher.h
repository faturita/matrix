/*! \page ProgrammingRefresher Programming Refresher 
\author Andrew Turner (nilspace)

\ref TableOfContents

Assumedly there are non-programmers, or at least users who may need a refresher on how to code in C++ as 
well as some small points they may have never covered in their programming experience. This section presents 
a basic refresher/introduction to some of the programming methods that are used in the Open-Sessame Framework.

\section VariableDefs Variable Definitions
Variables are how data is stored and manipulated in a program. Simple enough, you've probably encountered 
them many times in any number of programming languages. In C++, variables are defined as follows:
\code
int x;		// defines an integer
double z;	// defines a double (double-precision float)
float w, r;	// defines two floats, not great programming practice

int ii = 0;	// defines  and initializes an integer
\endcode

Sometimes, in functions that are called very often, it is useful to define a variable as @em static. 
Much of the overhead in computation is allocating and deallocating memory for temporary variables.
A static variable has its memory allocated the first time it is called, and the memory is set aside 
for the variable for the duration of the program. This can greatly speed up operation, as small 
functions that use large numbers of variables (such as matrices and vectors) must allocate the memory, 
do a simple computation, and then release the memory. By making the variable static, this allocation 
and deallocation must only happen once, and subsequent operations are performed in the same memory address.
\code
static double myArray[10];
\endcode

\section Classes Classes and Objects
The Open-Sessame framework makes extensive use of the object-oriented nature of C++. A @em Class is a 
definition of a new type, like @em int or @em string. These objects can be thought of in much the same 
way we would think of a real world object. A car is an object that has data associated with it: number 
of wheels and doors, mileage, color, and size, as well as operations that can be done with it: start, 
stop, go, turn left, turn right, or paint. Data is usually hidden (not directly accessible) to the user, 
but accessed using operations By encapsulating the data within a class we can change the internal 
representation without affecting how the user interfaces with the object. 

For example, there is a @b Car class which stores the speed of the car in "Miles Per Hour" (MPH). 
There is an operation, much like a function, we can call that gives us the speed, @em GetSpeedMPH(), 
which returns the speed in miles. However, it is later decided that the speed should internally be stored 
as Kilometers Per Hour (KPH). The class operation @em GetSpeedMPH() is internally changed to convert 
the internal mileage from KPH to MPH, however, an user of the @b Car class doesn't need to know about 
this internal change, since the user still calls @em GetSpeedMPH() and gets the speed of the car in MPH. 

@b Terminology
- @em Instance: a single instantiation of a class. \tt Car \tt myCar; \tt myCar is an @instance of \tt Car.
- @em Member @em function: a function of a class. \tt Car::GetSpeedMPH()
- @em Data @em member: a variable inside of a class, usually private. \tt Car::m_Speed
- @em Template: A type of class that employs a user-specified Class as the variable type in a class. 
    A template class @em vector<class T> is a vector that holds objects of type @em T. Therefore, in the 
    code, the user will specify: \code vector<Car> myCarCollection; \endcode
- @em public, @em protected, @em private: These are the three levels of access specified for all member 
    functions a data members in a class. @em Public allows anyone external to the class to call the function 
    or variable.  @em Protected are only accessible to derived classes (discussed below). @em Private things 
    are accessible only internal to the class.

To call member functions of a class, a '.' is used between the instance of the class and the 
member function call:
\code
Car myCar;
myCar.SetSpeedMPH(55); 		// set the speed of my car to 55 MPH
cout << myCar.GetSpeedKPH(); 	// output the speed of the car in KPH to the console
\endcode

\subsection Inheritance
Another important aspect of object-oriented programming is the idea of inheritance. Classes may be @em derived, 
which means they are specified to have an "is-a" relationship to the @em base class. For instance, @b SportsCar
could be a derived class of @b Car, since "SportsCar is a Car". This allows a programmer to specify general 
functionality of a car in the @b Car class, which any type of car would have, while the @b SportsCar class 
would implement particular functionality of a sports car such as @em ShiftGears(), or @em UseNitro(). 

Inheritance is how @em protected member functions and data members are used. There may be functionality that 
derived classes should be able to use from a base class, but outside users of the class don't need access to. 
Continuing the car example, the @b Car class has a function @em SetMileage() that all cars have, but shouldn't 
be accessible by an outside user of the class. However, derived classes, such as @b SportsCar or @b StationWagon 
may need to be able to set the mileage based on their use. Again @em private member functions and data members 
are not accessible to derived classes, only to the class in which they are instantiated.

\subsection ADT Abstract Data Types
A special type of base class are @em Abstract @em Data @em Types, ADTs. An ADT is a definition of an interface, 
but does not include any implementation. Any classes that are derived from the ADT @em must implement all 
of the functions defined in the interface of the abstract data type. These functions are called 
@em pure @virtual. They are @em virtual as mentioned above, they can be over-ridden. By being @em pure, they 
have no implementation, but specify the user must implement them. Sometimes ADTs are also called @pure 
@em base @em classes for the same reason. An abstract data type can never be instantiated. It is only meant as 
an inteface. 

An example may help illustrate this point. The user could implement a @b Vehicle abstract data type. This is 
object-oriented because the user may want to define a general interface to all vehicles, such as @em Move() 
@em Stop(), or even @em Location(). However, there really isn't anything that is a @em vehicle. I can't go 
to a rental agency and say, "I want to rent a vehicle". I have to choose if I want a car, a bicycle, or 
roller skates. However, all of these vehicles have the common functionality of moving, stopping, or having a 
location. So instead of worrying if all of the classes have the functionality implemented, or what it's called 
I can define the functions and name and then make sure (or the code won't compile) each of the derived 
classes implement these functions. 

\section Pointers
Most interfastd::cing with variables is done by actually referring to the object itself. For instance, defining and 
manipulating an integer:
\code
int myInt = 5;
int myOtherInt;
myOtherInt = myInt;
myInt = 2;

cout << myInt << " != " << myOtherInt; // outputs 2 != 5
\endcode
This is easy to understand and code, and not many worries. However, there are times when a user may just 
want to refer to the data that will be in the variable, and not the actual value of the variable itself. This 
is the concept of a pointer. Instead of storing the actual value of the data, you can store a reference 
to the @em location of the data, and therefore manipulate or retrieve whatever value may be there, without 
actually storing a @em copy of the data itself.
\code
int myInt = 5;
int* myPointerInt;

myPointerInt = &myInt;	// the & is used to return a reference, or the memory address of myInt
myInt = 2;
cout << myInt << " = " << *myPointerInt; // outputs 2 = 2, the * is required to 'dereference' the pointer
                                         // and make it act as a normal variable
\endcode
As you can see, the variable @em myPointerInt took a @em reference to @em myInt (the \& returns a reference 
or the memory address of the variable). Then, by manipulating the variable @em myInt, I have also manipulated 
the data where @em myPointerInt pointed to. This can speed up calculations (such as in functions, where one 
doesn't need to copy a variable, but just use the data), and also make manipulating internal data easier. 

However, the user should be cautioned when using pointers. The user is interacting with the computer's memory 
which is usually how segmentation faults and other bad things happen (by referenstd::cing unitialized memory, NULL 
pointers (pointers to 0x0), or the wrong memory address). Another problem is by leaving memory around that 
is not being used or referenced to. This is commonly known as a @em memory @em leak. Refer to your local 
programming manual for more information.
\code
int* pMyInt = new int;	// allocate memory (of size int)
*pMyInt = 5;		// Dereference the pointer (use memory space) and assign the value

pMyInt = NULL;		// Assign the pointer to NULL (0x0) - WARNING by doing this, we have 'lost' 
                        // the reference to the allocated int memory, which is now just hogging up 
                        // memory space. We should have called "delete pMyInt;" first.
\endcode

The last 'point' on 'pointers' is how they affect classes and calling member functions. When dealing 
with a normal class instance, we used a '.' to call a member function. When using a pointer to an instance, 
instead of having to dereference the pointer, we can just use '->' to call the member functions:
\code
Car* pMyCar = new Car;
pMyCar->SetSpeedMPH(88.7);
*pMyCar.GetSpeedKPH();		// this works the same, but isn't "nice"
delete pMyCar;			// remember to clean up
\endcode

\section Functions
Functions are pretty simple, and similar to most programming languages. The main points include specifying the 
function name, the return type, and any parameters that are taken into the function. A couple of small points 
include passing by reference. As mentioned in Pointers, passing a pointer, or reference to data is much 
faster. Normally, when a variable is just defined as a parameter to a function, a copy is made of the data 
every time the function is called, which can be rather slow. Therefore, passing by reference can speed this 
up since just the pointer (an integer) is passed, rather than the whole variable data (which for things like 
matrices can be quite large). 

Furthermore, pass-by-reference allows the user to pass data back out of a function other than just by 
the return type. This works like pointers and is illustrated below:
\code
int myFunction(int inputInt, double& _inputDouble)
{
    _inputDouble = static_cast<double>(inputInt); // static_cast makes sure to change the 
                                                 // value from a type int to a type double
    return inputInt * 5;
}

void main() {
    double mainDouble = 0;
    cout << myFunction(5, mainDouble); // outputs '25'
    cout << mainDouble;		       // outputs '5'
}
\endcode
When using this function, the user passes in an integer, and a variable that is of type @em double. The 
value of this variable will be changed in the calling function too! Remember, this is only for variables 
passed-by-reference, or by pointer, and @em not for passed-by-value (the normal way, inputInt). Also, I 
use an underscore '_' to note which variables are being passed by reference to know in the code that 
changing the data is propagated back out to the system.

When the user doesn't need to change the variable in a function, it is still better to pass-by-reference. 
To make sure the variable isn't changed unexpectedly, the variable can be defined @em const. 
\code
int myFunction(const int& inputInt, double& _inputDouble)
{
    _inputDouble = static_cast<double>(inputInt);  
    return inputInt * 5;
}
\endcode
Therefore, this code works as before, but passes a @em const-reference to the variable @em inputInt, which 
doesn't affect its usage in the function anyway, but can speed up operation.

\subsection CallbackFunctions Call-back Functions
Sometimes it is useful to pass around references to function, much like pointers, rather than have to 
hardcode in a function call. This is why @em call-back @em functions, or @em function @em references are used.
The user can store a variable that refers to a function, and can be changed at any point. 
\code
int myFunc1(const int& inputVar); // just a prototype, but it is defined elsewhere

int myFunc2(const int& inputVar);

int (*intFuncPtr)(int);		 // definition of the function pointer

void main() {
        intFuncPtr = &myFunc1;	 // Assign the function pointer to the first function
        cout << intFuncPtr(5);
        
        intFuncPtr = &myFunc2;	 
        cout << intFuncPtr(5);	 // Same call as above, but now calls function 2
}
\endcode
 
\subsection Functors
@em Functors are a type of function reference, but they can point to member functions of classes. Therefore, 
to initialize, they require an instance of an object as well as the member function reference.
\code
    ObjectFunctor myObjFunc(myCar, &Car::GetSpeedMPH); 
\endcode

\section MatrixVector Matrix and Vector classes
The Open-Sessame framework uses an open-source matrix and vector class library. Documentation can currently 
be read from the <a href="http://sourceforge.net/projects/spacecraft" target=_new>Open-Sessame sourceforge 
site</a> under the "Docs" link.

\section UML UML Diagrams
In order to document and describe object-oriented designs, inheritance and classes, the Unified Modeling Language 
(UML) can be used. This is a specification for diagramming classes, functions, packages, and any other 
coding artifact. 

Shown below is an example UML class diagram describing the example classes of vehicles and cars described above.
\image html ExampleUML.jpg

Each box is a class, with the name of the class shown in the top box. The next section of a 3-paned class 
box shows the attributes, or private data members, of the class. The "-" (minus) symbol shows that the 
visibility is @em private. The last box of a 3-paned class is the list of member functions, or operations. 
Again, "-" denotes @em private, "+" denotes @em public, and "#" denotes @em protected. 

Inheritance is shown using open arrows in the middle of lines between classes. The arrow points towards the 
base class. Therefore, @b Car is derived from @b Vehicle. Also note that @b Vehicle is in italics, which 
means that it is abstract (ADT). Futhermore, the member functions are in italics denoting that they are 
pure virtual. 

Lastly, aggregation, or composition of classes, is shown using diamonds and arrows. This is saying that 
the class with the diamond "has-a" class where the arrow points to. Furthermore, the number of instances 
are shown with the numbers (1..n being atleast 1 up to any number, or "1,2" denoting 1 or 2 elements). 
From the example, a @b Car can have 1 to however many @b Axles, which have 1 or 2 @b Wheels. 

Other UML diagrams demonstrate "Use Cases", "State Diagrams", "Activity Diagrams", or "Event Traces". Refer to 
a UML book or style guide for more information.

*/