#include <iostream>
#include <string>


std::string translate(std::string line, std::string argument)
{   
  std::string result = "";// implementeer dit
    
  std::string newLine;
    
    if (line.length()>= 1)
    {
      newLine =  line.substr(0,1) + argument;
      line.erase(0,1);
      line = line + newLine;
    }
  return line; 
  }

int main(int argc, char *argv[])
{ std::string line;

  if(argc != 2)
  { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; }

  while(std::getline(std::cin, line))
  { std::cout << translate(line, argv[1]) << std::endl; } 

  return 0; }