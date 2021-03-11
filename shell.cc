#include "shell.hh"


int main()
{ std::string input;


  int fd = syscall(SYS_open,"shellfile.txt",O_RDONLY,0777);           
  char byte[1];

    std::string prompt = "";
    while (syscall(SYS_read, fd, byte, 1)){
        prompt += byte;
        
    }  

  
  


  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file() // ToDo: Implementeer volgens specificatie.
{ 
  const char* new_file[0];
  std::string file_name; 
  std::cout << "geef een bestandsnaam op: ";

  getline(std::cin, file_name);
  new_file[0] = file_name.c_str();

  std::string file_content_chars= "";
  std::string input;
  int bytes = 0;
  std::cout << "geef inhoud van het bestand; " << std::endl; 
  
  while(getline(std::cin, input)){
    if(input != "<EOF>"){
      file_content_chars += input + "\n";
    }
    else
    {
    bytes = file_content_chars.length();
    const char* file_content[] = {NULL};
    file_content[0] = file_content_chars.c_str();

    int fd = syscall(SYS_creat, new_file[0], 0755, NULL);
    syscall(SYS_write, fd, file_content[0], bytes);
    syscall(SYS_close, fd);
    }
  }
}

void list() // ToDo: Implementeer volgens specificatie.
{ 
  
  int pid1 = syscall(SYS_fork);
  if (pid1 == 0)  //child
    { 
    const char* args[] = {"/bin/ls", "-l", "-a", NULL};
    syscall(SYS_execve, args[0], args, NULL);
    }
    else
    {
      syscall(SYS_wait4, pid1, NULL, NULL, NULL);
    }
  }

void find() // ToDo: Implementeer volgens specificatie.
{ 

  const char* new_file[0];
  std::string searchterm;
  std::cout << "geef een zoekterm: "; 
  getline(std::cin, searchterm);
  new_file[0] = searchterm.c_str();


  int fd[2];
  syscall(SYS_pipe, &fd);
  int pid1 = syscall(SYS_fork);
  if (pid1 == 0)  //child
  { 
    syscall(SYS_close, fd[0]);
    syscall(SYS_dup2, fd[1], 1);
    const char* args[] = {"/usr/bin/find", ".", NULL};
    syscall(SYS_execve, args[0], args, NULL);
  }
  else            //parent
  {
    int pid2 = syscall(SYS_fork);
    if (pid2 == 0)  //child2
    { 
      syscall(SYS_close, fd[1]);
      syscall(SYS_dup2, fd[0], 0);
      const char* args[] = {"/bin/grep",(char*) searchterm.c_str(), NULL};
      syscall(SYS_execve, args[0], args, NULL);
    }
    else
    { 
    syscall(SYS_close, fd[0]);
    syscall(SYS_close, fd[1]);
    syscall(SYS_wait4, pid1, NULL, NULL, NULL);
    syscall(SYS_wait4, pid2, NULL, NULL, NULL);
    }
  }
  }

void seek() // ToDo: Implementeer volgens specificatie.
{ 
      const char *seek[0];
    std::string file1_name = "seek.txt";
    seek[0] = file1_name.c_str();
    int fd1 = syscall(SYS_creat, seek[0], 0755, NULL);

    int x = 0;

    char character = 'x';
    char *char_ptr = &character;
    syscall(SYS_write, fd1, char_ptr, 1);

    syscall(SYS_lseek, fd1, 5000000, 1);

    syscall(SYS_write, fd1, char_ptr, 1);

    syscall(SYS_close, fd1);

    const char *loop[0];
    std::string file2_name = "loop.txt";
    loop[0] = file2_name.c_str();
    int fd2 = syscall(SYS_creat, loop[0], 0755, NULL);

    char empty_space = '\0';
    char *empty_space_ptr = &empty_space;

    syscall(SYS_write, fd2, char_ptr, 1);

    for (int i = 0; i < 5000000; i++)
        syscall(SYS_write, fd2, empty_space_ptr, 1);

    syscall(SYS_write, fd2, char_ptr, 1);

    syscall(SYS_close, fd2);
}
