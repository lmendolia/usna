public class Adding{
  int return1;
  int return2;
  int sum;
  public Adding(int num1, int num2){
    return1 = num1;
    return2 = num2;
    sum = num1 + num2;
  }
  public int addEm(int num3){
    return sum + num3;
  }

  public String toString(){
    return "Adding: (" + return1 + "," + return2 + ")";
  }

  public static void main(String[] args){
    Adding anObject = new Adding(3,9);
    int added = anObject.addEm(10);
    //System.out.println("here");
    System.out.println(added);
    System.out.println(anObject);
  }
}
