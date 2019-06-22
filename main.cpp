#include <iostream>
#include <memory>
#include <boost/lexical_cast.hpp>

#include "bulk.h"

int main(int argc, char *argv[])
{
  std::cout << "Press q for quit" << std::endl;

  if (argc < 2)
  {
    std::cout << "Not enough input args!" << std::endl;
    return 1;
  }

  int size_of_block = boost::lexical_cast<int>(argv[1]);

  if (!isdigit(size_of_block) && size_of_block <= 0)
  {
    std::cout << "Incorrect input args!" << std::endl;
    return 1;
  }

  Notifier notifier(size_of_block);

  auto file_output = std::make_unique<FileOutput>();  
  auto display_output = std::make_unique<DisplayOutput>();   

  notifier.subscribe(file_output.get());
  notifier.subscribe(display_output.get());

  std::string input_line;
  while (std::getline(std::cin, input_line) && input_line != "q")
  {
    notifier.get_input(input_line);
  }

  return 0;
}