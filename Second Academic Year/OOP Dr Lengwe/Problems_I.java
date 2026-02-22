// // import java.util.Scanner;
// // public class Problem {
// // public static void main (String [] args){
// // /*Write a Program that will ask the user to enter their name,
// // The program should ask if your name contains vowels (a,e,i,o,u)*/
// // Scanner input = new Scanner(System.in);
// // System.out.println("What is your Name:");
// // String userName = input.nextLine();
// // if (userName.contains("a") || userName.contains("e") || userName.contains("i") || userName.contains("o") || userName.contains("u")){
// //    System.out.println(userName + " Contains!! Vowels");
// //  }
// // else{
// //     System.out.println(userName + " Does not Contain Vowels");
// // }
// // }
// // }


//  /*Write a Program that will ask the user to enter their name,
//  The program should ask if your name contains vowels (a,e,i,o,u)*/
 
//  import java.util.Scanner;
//  public class Problems_I{
//      public static void main (String [] args){
//         String Name;
//         Name = "Catherine Chiluba";
//         Scanner input = new Scanner(System.in);
//         Name = input.nextLine();
//         System.out.println("Enter Your Name:");
//         if(Name.contains("a") || Name.contains("e") || Name.contains("i") || Name.contains("o") || Name.contains("u") ){
//         System.out.println(Name + " Contains Vowels!");
//         }
//         else{
//             System.out.println(Name + " Does not Contain Vowels!");
//         }
//      }
//  }




//User Enters mark
/*Use if Statement to determine Grades: 
 * 0-39 is D,
 * 40-49 is D+,
 * 50-55 is C,
 * 56-61 is C+,
 * 62-67 is B,
 * 68-75 is B+
 * 76-85 is A
 * 86-100 is A+
 * Use switch to classify grades
 *  A,A+ Distinction
 * B+ Merit
 * B Credit
 * C+, C Pass
 * D,D+ fail
 * The output should be:You got 80% which is an A. That is a distintion!
 * */ 
import java.util.Scanner;
public class Problems_I{
    public static void main(String [] args){
        Scanner input = new Scanner(System.in);
        System.out.print("Enter your mark:");
        int mark = input.nextInt();
        String grade = "";
        String classification = "";
        if (mark >= 0 && mark <= 39){
           grade = "D";
        }
        else if (mark >= 40 && mark <= 49){
            grade = "D+";
        }
        else if (mark >= 50 && mark <= 55){
            grade = "C";
        }
        else if (mark >= 56 && mark <= 61){
            grade = "C+";
        }
        else if (mark >= 62 && mark <= 66){
            grade = "B";
        }
        else if (mark >= 68 && mark <= 75){
            grade = "B+";
        }
        else if (mark >= 76 && mark <= 85){
            grade = "A";
        }
        else  if (mark >= 86 && mark <= 100){
            grade = "A+";
        }
        switch (grade) {
            case "A":
            case "A+":
            classification = "Distinction";
            break;
            case "B+":
            classification = "Merit";
            break;
            case "B":
            classification = "Credit";
            break;
            case "C+":
            case "C":
            classification = "Pass";
            break;
            case "D":
            case "D+":
            classification = "fail";
            break;
            default:
            System.out.println("Invalid input!!");
            break;
        }
        System.out.print("You got " + mark +"%"+" which is a " + grade + " Thats a " + classification + " ");




    }
}












