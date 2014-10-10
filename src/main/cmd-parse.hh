#ifndef CMD_PARSE_HH_
# define CMD_PARSE_HH_

# include <map>
# include <list>

typedef std::map<std::string, std::list<std::string>> options;

namespace parse
{
  void cmdline(int ac, char **av, options& opts);
}

#endif /* !CMD_PARSE_HH_ */
