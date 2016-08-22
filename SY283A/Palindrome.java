public class Palindrome{
  public static void Pal(String number){
    System.out.println("The length is: "+ number);
  }
  public static void main(String[] args){
    Palindrome usrInput = new Palindrome("1001");
    int number = usrInput.length();
  }
    /*
    for (int i<= (usrInput.length()); i = 0; i++){
      usrInput2 = usrInput2 + i;
    }
    System.out.println(usrInput2); */
}
