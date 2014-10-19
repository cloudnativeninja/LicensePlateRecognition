#include "cmd-parse.hh"
#include <map>
#include <list>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>

namespace parse
{
  void cmdline(int ac, char **av, std::map<std::string, std::list<std::string>> &opts)
  {
    static struct option long_options[] =
    {
      {"sobel",     no_argument, 0, 's' },
      {"grayscale", no_argument, 0, 'g' },
      {"blur",  no_argument, 0, 'b' },
      {"otsu",  no_argument, 0, 'o' },
      {"edged",  no_argument, 0, 'e' },
      {"swt",  no_argument, 0, 't' },
      {"median",  no_argument, 0, 'm' },
      {"morph",  no_argument, 0, 'p' },
      {0,         0,                 0,  0 }
    };
    int c;
    opts["algorithms"] = std::list<std::string>();

    for (;;)
    {
      int option_index = 0;

      c = getopt_long(ac, av, "sgbotemp",
                      long_options, &option_index);
      if (c == -1)
        break;

      switch (c)
      {
        case 0:
          printf("option %s", long_options[option_index].name);
          if (optarg)
            printf(" with arg %s", optarg);
          printf("\n");
          break;

        case 's':
          opts["algorithms"].push_back("sobel");
          break;

        case 'e':
          opts["algorithms"].push_back("edge_detect");
          break;

        case 't':
          opts["algorithms"].push_back("swt");
          break;

        case 'o':
          opts["algorithms"].push_back("otsu");
          break;

        case 'b':
          opts["algorithms"].push_back("blur");
          break;

        case 'p':
          opts["algorithms"].push_back("morph");
          break;


        case 'g':
          opts["algorithms"].push_back("grayscale");
          break;

        case 'm':
          opts["algorithms"].push_back("median");
          break;


        case '?':
          break;

        default:
          printf("?? getopt returned character code 0%o ??\n", c);
      }
    }

    if (optind < ac)
    {
      opts["img"] = std::list<std::string>();
      while (optind < ac)
        opts["img"].push_back(av[optind++]);

      printf("\n");
    }
    else
      exit(1);
  }
}
