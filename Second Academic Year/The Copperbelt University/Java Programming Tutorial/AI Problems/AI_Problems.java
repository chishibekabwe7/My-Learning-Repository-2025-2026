import java.util.Scanner;
public class AI_Problems {
    public static void main( String [] args){
       //Count how many times the letter 'l' appears in "Hello World" using a loop.

       //Print a row of 5 stars: * * * * * using a loop (not manually typing them).
       for(int i = 0; i<5; i++){
          System.out.println(i);
       }




       //Print all even numbers from 2 to 20.
       for(int i = 20; i > 0; i-=2){
        System.out.println(i);
       }


        //Print numbers 10 to 1 using a for loop(countdown).
        for(int i = 10; i > 0; i--){
            System.out.println(i);
        }
        System.out.println("Happy New Year!!!");


        //Problem One For Loop (Claude)
        //Print numbers 1 to 10 using a for loop.
        for(int i = 1; i < 11; i++){
            System.out.println(i);
        }
        System.out.println("Happy New Year!!!");
        
        

       
        


        
           
        //Java For Loop Full Tutorial
        String [] K161 = {"CJ","Shadreck","Shemaiah","Mumena"};
        for(String i:K161){
            System.out.println(i);
        }
        


        /*
        Traditional Loop 
        This is usually used when you know how many times to iterate or need 
        the index.
        */
        //Example One + Syntax
        String language = "Java";
        for (int i = 0; i < language.length(); i++){
            System.out.println(language.charAt(i));
        }
        
      




        


         //charAt() Problem Two (Claude)
         // Given "Hello", print each character on a new line using a for loop
         String My_Characters = "Hello";
         for (int i = 0; i < My_Characters.length(); i++){
            System.out.println(My_Characters.charAt(i));
         }






          //charAt() Problem One (Claude)
          //Print the first and last character of the string "student".
          String MyObject = "student";
          System.out.print(MyObject.charAt(0));
          System.out.println(" And " + MyObject.charAt(6));

        System.out.println("Hello Java AI Problems");
        //length Problem One (Claude) 
        //Ask the user to enter their name. Print how many letters their name has.
        Scanner input = new Scanner(System.in);
        System.out.println("Please Enter Your:");
        String Name = input.nextLine();
        System.out.println("Your name " +Name+ " contains " + Name.length() + " Characters");


        //length Problem Two (Claude) 
        //Given the string "Java Programming", check if it has more than 10
        String myString = "Java Programming";
        System.out.println("Its " + myString.contains("10") + " That the String Java Programming has " + myString +" index Values.");

        //length Problem Two (Claude) 
        //Take two words and print which one is longer.
         System.out.println("Enter Your First Name:");
        String nameOne = input.nextLine();
        System.out.println("Enter Your Second Name:");
        String nameTwo = input.nextLine();
        if(nameOne.length() > nameTwo.length()){
            System.out.println("Your First Name is Longer");
        }
        else{
            System.out.println("Your Last Name is Longer");
        }
    }
    
  
}