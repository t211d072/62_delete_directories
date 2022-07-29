//#include <ryusei/common/defs.hpp>

#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class FileOperator{
public:
  FileOperator();
  ~FileOperator();

  std::string getFileName(const std::string &path);
  void getFiles(const fs::path &path, std::vector<fs::path> &files);
  int checkExtension(const char *pszPath, const char *pszCmpExts);
  void getImgFiles(const fs::path &path, std::vector<fs::path> &files);

  std::vector<std::string> split(std::string str, char del);
  std::vector<std::vector<std::string> > csv2vector(std::string filename, int ignore_line_num = 0);

private:

};
