import java.util.Scanner;
public class Problem {
public static void main (String [] args){
/*Write a Program that will ask the user to enter their name,
The program should ask if your name contains vowels (a,e,i,o,u)*/
Scanner input = new Scanner(System.in);
System.out.println("What is your Name:");
String userName = input.nextLine();
if (userName.contains("a") || userName.contains("e") || userName.contains("i") || userName.contains("o") || userName.contains("u")){
   System.out.println(userName + " Contains!! Vowels");
 }
else{
    System.out.println(userName + " Does not Contain Vowels");
}
}
}