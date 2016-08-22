import java.util.*;

public class Machine
{
  int memory;
  String name;
  List<Process> processList = new ArrayList<>();

  public Machine(String processName, int totalMem){
    //List<String> processList = new ArrayList<>();
    memory = totalMem;
    name = processName;
  }
  public String addProcess(Process processName){
    processList.add(processName);
    return processName;
  }
  public int availableMemory(){
    int sum = 0;
    for (int i = 0; i < processList.length; i++){
      sum += processList.get(i).memoryReq;
    }
    int finalMem =  memory - sum;
    return finalMem;
  }
  public String toString(){
    return ""+ name+ ": " +""+memory;
  }
  public static void main(String[] args){

  }
}
