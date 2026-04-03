// Class defined outside main
class Car {
    String color;
    int speed;

    void drive() {
        System.out.println("The " + color + " car is driving at " + speed + " km/h.");
    }
}

public class Main {
    public static void main(String[] args) {
        // Objects created inside main
        Car car1 = new Car();
        car1.color = "Red";
        car1.speed = 120;

        car1.drive(); // Method called inside main
    }
}
