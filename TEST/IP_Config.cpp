#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iterator>

using namespace std;

int main(){
  string cmd = "ipconfig";
  string filename = "IP_Config.txt";
  
  system((cmd + ">" + filename).c_str());
  string line;
  ifstream myfile ("macaddress.txt");
  
  if(myfile.is_open()){
    while(getline(myfile,line)){
      cout<<line<<endl;
    }
    myfile.close();
  }
  else
  cout<<"Unable to open the file";

  return 0;
}