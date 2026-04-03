//Coursera University of California San Diego OOP with Java;

// This is a CLASS
class Car {

    // These are MEMBER VARIABLES (also called fields or attributes)
    String color;
    int speed;

    // This is a CONSTRUCTOR — runs when you create a new object
    Car(String color, int speed) {
        // 'this' refers to the current object's variables
        this.color = color;  // Assign parameter to object's variable
        this.speed = speed;
    }

    // A method (behavior of the object)
    void drive() {
        System.out.println("The " + this.color + " car is driving at " + this.speed + " km/h.");
    }
}

// This is where we create OBJECTS
public class Main {
    public static void main(String[] args) {

        // Creating OBJECTS using the class
        Car car1 = new Car("Red", 120);
        Car car2 = new Car("Blue", 150);

        car1.drive();
        car2.drive();
    }
}
