public class Process
{
  String procname;
  public int memoryReq;
  public Process(String processName, int memoryReqKB){
    procname = processName;
    memoryReq = memoryReqKB;
  }
  public String toString(){
    return ""+ procname+ ": " +""+memoryReq;
  }

  public static void main(String[] args){


  }
}
