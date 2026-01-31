//W3Schools java basics tutorial
public class W3schools {
    public static void main(String[] args){
        //Hello World
        System.out.println("Hello World!");

        //This is java comment

        //Variables
        // String firstName = "Wange ";
        // String lastName = "Shula";
        // String fullName = firstName + lastName;
        // System.out.println(fullName);
        
        //Declaring multiple variables
        int x,y,z;
        x = y = z = 50;
        System.out.println(x);
        System.out.println(y);
        System.out.println(z);

        //Constant Variable
        final int num = 10;
        // This >>> num = 11; will be an error because of the final keyword
        System.out.println(num);

        //Type Casting
        //Widening Casting
        int myInt = 9;
        double myDouble = myInt;
        System.out.println(myDouble);
        
        //Narrowing Casting
        double myDouble2 = 9.324;
        int myInt2 = (int) myDouble;
        System.out.println(myDouble2);
        System.out.println(myInt);

        //Real life Example
        //Maximum Score
        int maxScore = 500;

        //Actual score by the user 
        int userScore = 423;
        double percentage = (double) userScore/maxScore * 100.d;
        System.out.println("Users percentage is " + percentage);
        
        //Java Strings
        String txt = "Strings in Java Programming!";
        System.out.println(txt.toUpperCase());
        System.out.println(txt.toLowerCase());

        //String Concatination
        String txt_1 = "//This text has been converted to lowercase.";
        System.out.println(txt.toLowerCase() + ' ' + txt_1.toLowerCase());

        String txt_2 = "//This text has been converted to Uppercase.";
        System.out.println(txt.toUpperCase() + ' ' + txt_2.toUpperCase());

       //Java If/Else Statements
       if(20>18) {
        System.out.println("20 is greater thatn 18");
       }

       //Using Boolean Variables
       boolean isLightOn = false;
       if (isLightOn){
        System.out.println("The light is On!");
       }
       else{
        System.out.println("The light is off!");
       }
       
       //Problem 2
       boolean isRaining = false;

       if (isRaining){
        System.out.println("Its Raining Outside");
       }
       else{
        System.out.println("Its Not Raining Outside");
       }

       




     




    }
}