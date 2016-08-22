import java.util.*;

public class LinkedListClass{
  Node head;
  Node tail;
  public LinkedListClass(){
    head = null;
    tail = null;
  }
  public void add2Front(int data){
    //same as always
    head = new Node(data, head);
    if (tail == null){
      tail = head;
    }
  }

  public void printInOrder{
    String print =
  }
  public boolean isIn(int index){
    //same
    Node tmp;
    tmp = head;
    while (tmp != null){
      if (tmp.data == index){
        return true;
      }
      tmp = tmp.nextNode;
    }
    return false;
  }


  public static void main(String[] args){

  }

}
