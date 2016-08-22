import java.util.*;

public class LinkedList{
  Node head;
  Node tail;
  public LinkedList(){
    head = null;
    tail = null;
  }
  public void printAll(){
    Node tmp;
    tmp = head;
    while (tmp != null){
      System.out.println(tmp);
      tmp = tmp.nextNode;
    }
  }
  public void add2Front(int data){
    head = new Node(data, head);
    if (tail == null){
      tail = head;
    }
  }
  public void add2Back(int data){
    Node oldTail = tail;
    tail = new Node(data, null);

    oldTail.nextNode = tail;

    if (tail == null){
      tail = head;
    }
  }
  /*
  public boolean isIn(int index){
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
*/
  public void addInOrder(int newNum){
    Node tmp;
    tmp = head;
    while (tmp != null){
      if (tmp.data <= newNum && tmp.nextNode.data >= newNum){
        tmp.nextNode = new Node(newNum, tmp.nextNode);
        return;
      }
      else if(newNum >= tmp.data){
        Node oldTail = tail;
        tail = new Node(newNum, null);

        oldTail.nextNode = tail;
        return;
      }
      else{
        head = new Node(newNum, head);
        return;
      }
    }
    tmp = tmp.nextNode;
  }
  public void printInOrder(){
    _printInOrder(head);
  }
  public void _printInOrder(Node tmp){
    if (tmp.nextNode != null){
      System.out.println(tmp);
      _printInOrder(tmp.nextNode);
    }
  }

  public boolean isIn(int index)
  {
    return isIn(index, head);
  }
  //change is in to recursive
  public boolean isIn(int index, Node tmp){
    if (tmp.nextNode != null){
      if (tmp.data == index){
        return true;
      }
      return isIn(index, tmp.nextNode);
    }
    return false;
  }

  public int isInTimes(int index){
    return isInTimes(index, head);
  }
  public int isInTimes(int index, Node tmp){
    int counter = 0;

    if (tmp.nextNode != null){
      if (tmp.data == index){
        counter++;
      }
      return counter + isInTimes(index, tmp.nextNode);
    }
    return counter;
  }
  public String printInReverseOrder(){
    return _printInReverseOrder(head);
  }
  public String _printInReverseOrder(Node tmp){
    String returnString = " ";
    //print from tail to head
    if (tmp != null){
      returnString += tmp.data;
      return _printInReverseOrder(tmp.nextNode)+ returnString;
    }
    return "";
  }
  public int get(int index){
    return get(index, head, 0);
  }
  public int get(int index, Node tmp, int k){
    if (index == k){
      return tmp.data;
    }
    else{
      return get(index, tmp.nextNode, k+1);
    }
  }

  public void addBefore(int findElement, int addElement){
    addBefore(head, findElement, addElement);
  }

  public void addBefore(Node tmp, int findElement, int addElement){
    //add addElement to the List right before findElement
    //if findElement DNE add addElement to end
    if (findElement == tmp.nextNode.data){
      tmp.nextNode = new Node(addElement, tmp.nextNode);
      return;
    }
    else if (tmp.nextNode == null){
      Node oldTail = tail;
      tail = new Node(addElement, null);
      oldTail.nextNode = tail;
      return;
    }
    else{
      addBefore(tmp.nextNode, findElement, addElement);
    }
  }
  public void remove(int index){
    remove(index, head, 0);
  }
  public Node remove(int index, Node tmp, int k){

    if (index == k){
      //do something to remove i-th element
      return tmp.nextNode;
    }
    else{
      tmp.nextNode = remove(index, tmp.nextNode, k+1);
      return tmp;
    }
  }
  public static void main(String[] args){
    LinkedList linkedlist = new LinkedList();
    linkedlist.add2Front(3);
    linkedlist.add2Front(1);
    linkedlist.add2Front(1);
    linkedlist.add2Back(5);
    System.out.println(linkedlist.isIn(3));
    System.out.println(linkedlist.isIn(6));
    linkedlist.addInOrder(0);
    linkedlist.printAll();
    System.out.println("is in times: "+linkedlist.isInTimes(1));
    System.out.println(linkedlist.printInReverseOrder());
    System.out.println(linkedlist.get(2));
    linkedlist.addBefore(3,5);
    linkedlist.printAll();
    linkedlist.remove(4);
    System.out.println("We have removed the fourth element");
    linkedlist.printAll();
  }
}
