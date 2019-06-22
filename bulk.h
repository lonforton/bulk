#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cctype>


class Handler
{
public:
  Handler(int size_of_block) : _size_of_block(size_of_block)
  {
  }

  bool is_notify_required(const std::string &input_line)
  {
    if (input_line.empty() && _commands.size() > 0 && _brackets_counter <= 0)
      return true;

    if (input_line.front() == '{')
    {
      ++_brackets_counter;
      return false;
    }

    if (input_line.front() == '}')
    {
      --_brackets_counter;
      if (_brackets_counter <= 0)
      {
        return true;
      }
      return false;
    }

    if (!input_line.empty())
    {
      if (_commands.size() == 0)
      {
        first_command_time = std::chrono::system_clock::now();
      }
      _commands.push_back(input_line);
    }

    if (_commands.size() >= _size_of_block && _brackets_counter <= 0)
    {
      return true;
    }

    return false;
  }

  void clear()
  {
    _commands.clear();
  }

  std::string getCommandString() const
  {
    std::ostringstream commands_line;

    if (!_commands.empty())
    {
      std::copy(_commands.begin(), _commands.end() - 1,
                std::ostream_iterator<std::string>(commands_line, " "));
      commands_line << _commands.back();
    }
    return commands_line.str();
  }

  std::string getFirstCommandTimeStamp() const
  {
    auto duration = first_command_time.time_since_epoch();
    return std::to_string(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
  }

private:
  std::vector<std::string> _commands;
  unsigned int _size_of_block = 0;
  int _brackets_counter = 0;
  std::chrono::time_point<std::chrono::system_clock> first_command_time;
};

class Output
{
public:
  virtual ~Output() = default;
  virtual void update(const Handler &handler) = 0;
};

class DisplayOutput : public Output
{
public:
  virtual ~DisplayOutput() = default;
  void update(const Handler &handler)
  {
    std::cout << "bulk: " << handler.getCommandString() << std::endl;
  }
};

class FileOutput : public Output
{
public:
  virtual ~FileOutput() = default;
  void update(const Handler &handler)
  {
    std::ofstream bulk_file;
    std::cout << handler.getFirstCommandTimeStamp() << std::endl;
    bulk_file.open(handler.getFirstCommandTimeStamp() + ".log");
    bulk_file << handler.getCommandString() << std::endl;
    bulk_file.close();
  }
};

class Publisher
{

public:
  Publisher(int size_of_block) : _handler(size_of_block)
  {
  }

  void subscribe(Output *output)
  {
    _outputs.push_back(output);
  }

  void notify_all() const
  {
    for (const auto &output : _outputs)
    {
      output->update(_handler);
    }
  }

  void get_input(const std::string &input_line)
  {
    if (_handler.is_notify_required(input_line))
    {
      notify_all();
      _handler.clear();
    }
  }

  void get_stream_input(std::istream &is)
  {
    std::string input_line;
    while (std::getline(is, input_line) && input_line != "q")
    {
      get_input(input_line);
    }
  }

private:
  Handler _handler;
  std::vector<Output *> _outputs;
};