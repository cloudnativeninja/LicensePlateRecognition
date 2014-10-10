#ifndef EXEC_HH_
# define EXEC_HH_

# include <list>
# include <string>
# include <opencv2/core/core.hpp>
# include <opencv2/core/mat.hpp>


namespace algorithm
{
  void exec_all(std::list<std::string>&, cv::Mat&);
}


#endif /* !EXEC_HH_ */
