public class String_Manipulation {
    public static void main(String args []){
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
        //Problem One (Dr Lengwe)
        //Write a Program that will ask the user to enter your name,The program should ask if your name contains vowels (a,e,i,o,u)

        

    }
