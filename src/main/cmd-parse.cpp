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
      {"gaussian",  no_argument, 0, 'b' },
      {"binarize",  no_argument, 0, 'z' },
      {"edged",  no_argument, 0, 'e' },
      {"swt",  no_argument, 0, 't' },
      {0,         0,                 0,  0 }
    };
    int c;
    opts["algorithms"] = std::list<std::string>();

    for (;;)
      {
        int option_index = 0;

        c = getopt_long(ac, av, "sgbzt",
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

	    case 'z':
	      opts["algorithms"].push_back("binarize");
	      break;

            case 't':
              opts["algorithms"].push_back("swt");
              break;

            case 'z':
              opts["algorithms"].push_back("binarize");
              break;

            case 'b':
              opts["algorithms"].push_back("gaussian");
              break;

            case 'g':
              opts["algorithms"].push_back("grayscale");
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
