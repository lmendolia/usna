import java.util.*;

public class Node{
  public int data;
  public Node nextNode;

  public Node(int data){
    this.data = data;
    nextNode = null;
    //take data in, set field accordingly
    //nextNode should be set to NULL
  }
  public Node(int data, Node nextNode){
    this.data = data;
    this.nextNode = nextNode;
    //take data in, set field accordingly
    //nextNode should be set to NULL
  }

  public String toString(){
    return ""+data;
  }
  public static void main(String[] args){
    Node node1 = new Node(3);
    Node node2 = new Node(4);
    System.out.println(node1);
    System.out.println(node2);
    node1.data = 5;
    System.out.println(node1.data);
    node1.nextNode = node2;
    System.out.println(node1.nextNode.data);
  }
}
