// Code your testbench here
// or browse Examples
#include<iostream>
#include<sstream>
#include<fstream>
using namespace std;
int main(){
  uint32_t out;
 // uint32_t final;
  //uint32_t Instruction=232E8100;
  ifstream memfile("program.mem");
  
  uint32_t address, data;

  string line;
  
  if(memfile.is_open()){
  while(getline(memfile,line)){
  stringstream ss(line);
  ss>>hex>>address;
  ss.ignore(2,' ');
  ss>>hex>>data;
  cout<<"Address "<<hex<<address<<endl;
  cout<<"Data "<<data<<endl;
  out=data>>7 & 0x1F;
  cout<<out<<endl;
  }
  memfile.close();
  }else {
	  cout<<"Failed to open"<<endl;
  }
  
  /*
  stringstream ss(Instruction);
  ss >> hex >> out;
  final=out&0x7F;
  cout<<final<<endl;
  */

 return 0;
}

 
