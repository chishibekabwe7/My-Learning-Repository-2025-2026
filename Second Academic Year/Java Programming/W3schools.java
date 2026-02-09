//W3Schools java basics tutorial
import java.util.Scanner;
public class W3schools {
    public static void main(String[] args){
        //Hello World
        System.out.println("Hello World!");

        //This is java comment
        
        //Java UserInput
        /*Java input is how a program receives input from a user,Before using the Java Userinput you first 
        need to import a class called 'Scanner' Then declare a new scanner object*/
        Scanner input = new Scanner(System.in); //This means that Scanner can now listen to what the user can type.
        System.out.println("Enter your name:");
        //Full Sentence
        String Name2 = input.nextLine();
        System.out.println("My Name is " + Name2);







        //Declaring Variables
        String firstName;
        String lastName;
        //Assigning Variables
        firstName = "Wange";
        lastName = "Shula";
        //Concatenating Strings
         String fullName = firstName +" "+ lastName;
        System.out.println(fullName);
        
        //Declaring multiple variables
        int x,y,z;
        x = y = z = 50;
        System.out.println(x);
        System.out.println(y);
        System.out.println(z);

            
        //Final key 
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
        
        //Java String Manipulation
        String txt = "John Doe";
        //toUpperCase() case converts to uppercase
        System.out.println(txt.toUpperCase());
        //toLowerCase() case converts to lowercase
        System.out.println(txt.toLowerCase());
        //charAt() Combined with a specified integer outputs the exact index position inputed of the variable text
        System.out.println("The first character at this text is " + txt.charAt(0));
        //length() Outputs the exact value of index numbers contained in that string, include any spaces left, they are also counted for
        System.out.println("The length of txt String variable is " + txt.length());
        //indexOf() Outputs the index length of the first character of a text, to illustrate
        System.out.println("The index length of Doe in txt variable is " + txt.indexOf("Doe"));
        //contains() Combined with a text you want to check outputs True or False! and is Case Sensitive
        System.out.println("txt variable contains the name Wange in it " + txt.contains("Wange"));
        System.out.println("txt variable contains the name doe in it " + txt.contains("doe"));
        System.out.println("txt variable contains the name Doe in it " + txt.contains("Doe"));
        //startsWith() outputs True or False after you input any text you want to check and is Case Sensitive also
        System.out.println("txt variable starts with Doe " + txt.startsWith("Doe"));
        System.out.println("txt variable starts with john " + txt.startsWith("john"));
        System.out.println("txt variable starts with John " + txt.startsWith("John"));
        //endsWith() outputs True or False after you input any text you want to check and is Case Sensitive also
        System.out.println("txt variable ends with Doe " + txt.endsWith("Doe"));
        System.out.println("txt variable ends with john " + txt.endsWith("john"));
        System.out.println("txt variable ends with doe " + txt.endsWith("John"));
        //A SubString is just a piece of text and it has two versions 
        //For example
        System.out.println("Substring will cut to Output the next position after the 4th position of our txt variable:" + txt.substring(3));
        System.out.println("This will output the text between the index position 5 and 8 " + txt.substring(5,8));
        //NB Split() breaks a string apart using a delimeter
        String[] characters = txt.split(" ");
        //Java Enhanced For Each loop
        //Example for (datatype variable:collection){
        for(String c:characters){
           System.out.println(c);
         }
        }
        
       









        //String Concatination
    //     String txt_1 = "//This text has been converted to lowercase.";
    //     System.out.println(txt.toLowerCase() + ' ' + txt_1.toLowerCase());

    //     String txt_2 = "//This text has been converted to Uppercase.";
    //     System.out.println(txt.toUpperCase() + ' ' + txt_2.toUpperCase());

    //    //Java If/Else Statements
    //    if(20>18) {
    //     System.out.println("20 is greater thatn 18");
    //    }

    //    //Using Boolean Variables
    //    boolean isLightOn = false;
    //    if (isLightOn){
    //     System.out.println("The light is On!");
    //    }
    //    else{
    //     System.out.println("The light is off!");
    //    }
       
    //    //Problem 2
    //    boolean isRaining = false;

    //    if (isRaining){
    //     System.out.println("Its Raining Outside");
    //    }
    //    else{
    //     System.out.println("Its Not Raining Outside");
    //    }

       




     




    }
