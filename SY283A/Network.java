import java.util.*;

public class Network{
  List<Machine> machineList = new ArrayList<>();
  int totalMem;
  String machineName;
  public Network(){

  }
  public String addMachine(Machine machine){
    machineList.add(machine);
    machineName = machine;
    return machineName;
  }
  public String addProcess(Process processName){
    Machine highest = machineList[0];
    for (int i = 1; i < machineList.length; i++){
      if(highest.availableMemory() < machineList.get(i).availableMemory()){
        highest = machineList[i];
      }
    }
    highest.addProcess(processName);
    return processName;
  }

  public String toString(){
    String toReturn =  "Network:";
    for (int i = 0; i < machineList.length; i++){
      toReturn += machineName+", "+totalMem;
        for (int j = 0; j < processList.length; j++){
          toReturn += procname+": "+""memoryReq;
        }
    }
    return toReturn;

  }
  public static void main(String[] args){
    Process procObject = new Process("aProcess", 1);
    Process procObject2 = new Process("aProcess2", 2);
    Process procObject3 = new Process("aProcess3", 3);
    Process procObject4 = new Process("aProcess4", 4);
    System.out.println(procObject);
    System.out.println(procObject2);
    System.out.println(procObject3);
    System.out.println(procObject4);

    Machine machObject = new Machine("aMachine", 100);
    Machine machObject2 = new Machine("aMachine2", 200);
    System.out.println(machObject.availableMemory());
    System.out.println(machObject2.availableMemory());
    machObject.addProcess(procObject);
    machObject2.addProcess(procObject2);
    System.out.println(machObject.availableMemory());
    System.out.println(machObject2.availableMemory());
    machObject.addProcess(procObject3);
    machObject2.addProcess(procObject4);
    System.out.println(machObject.availableMemory());
    System.out.println(machObject2.availableMemory());
    System.out.println(machObject);
    System.out.println(machObject2);

    Network networkObject = new Network();
    networkObject.addMachine(machObject);
    networkObject.addMachine(machObject2);
    System.out.println(networkObject);
    networkObject.addProcess(p);
    networkObject.addProcess(p2);
    networkObject.addProcess(p3);
    networkObject.addProcess(p4);
    System.out.println(networkObject);
  }
}
