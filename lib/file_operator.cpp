/***************************************************************/
/* ディレクトリ操作・ファイル名の取得・CSV操作など             */
/* ファイル操作系をまとめたライブラリ                          */
/*                                                             */
/***************************************************************/

#include "file_operator.hpp"
#include <cmath>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#define RAD2DEG(theta) ( 180.0/M_PI*(theta) )

namespace fs = boost::filesystem;

/*************************************
 * Public methods                    
 *************************************/

FileOperator::FileOperator()
{

}

FileOperator::~FileOperator()
{

}

/**************************
 * ファイル操作関数群
 * ************************/

//フルパスからファイル名を抽出する関数
std::string FileOperator::getFileName(const std::string &path)
{
  using namespace std;
  size_t pos1;

  pos1 = path.rfind('\\');
  if(pos1 != string::npos){
      return path.substr(pos1+1, path.size()-pos1-1);
  }

  pos1 = path.rfind('/');
  if(pos1 != string::npos){
      return path.substr(pos1+1, path.size()-pos1-1);
  }

  return path;
}

//ファイル名一覧をvectorに格納する関数
void FileOperator::getFiles(const fs::path &path, std::vector<fs::path> &files)
{
  BOOST_FOREACH(const fs::path& p, std::make_pair(fs::directory_iterator(path), fs::directory_iterator())) {
    if (!fs::is_directory(p)){ // 参照しているパスがディレクトリでない(=ファイルなら)処理を行う
      files.push_back(p);  // 動的配列filesにファイルの絶対パスを追加
    }
  }
  /*** ファイル名が昇順になるようにソート ***/
  std::sort(files.begin(), files.end());
}

//拡張子を検査する関数
int FileOperator::checkExtension(const char *pszPath, const char *pszCmpExts){ 
   // ファイルパスの拡張子を抽出
   const char *pszExt = strrchr(pszPath, '.');
   if(pszExt && *(pszExt + 1) != '\0'){
      pszExt += 1;
   }else{
      return 0; // ファイルパスに拡張子が存在しない場合はfalse
   }

   // 拡張子をチェック(大文字小文字区別あり)
   char szCmpExts[1024];
   strncpy(szCmpExts, pszCmpExts, sizeof(szCmpExts)-1);
   for(const char *pszCmpExt = strtok(szCmpExts, ","); pszCmpExt; pszCmpExt = strtok(NULL, ",")){
      if(strcmp(pszExt, pszCmpExt) == 0) return 1;
   }
   return 0;
}

//ファイル名一覧（画像のみ）をvectorに格納する関数
void FileOperator::getImgFiles(const fs::path &path, std::vector<fs::path> &files)
{
  BOOST_FOREACH(const fs::path& p, std::make_pair(fs::directory_iterator(path), fs::directory_iterator())) {
    if (!fs::is_directory(p)){ // 参照しているパスがディレクトリでない(=ファイルなら)処理を行う
      std::string image_name = getFileName(p.string());
      if(checkExtension(image_name.c_str(), "png") ||
         checkExtension(image_name.c_str(), "jpg") ||
         checkExtension(image_name.c_str(), "jpeg")||
         checkExtension(image_name.c_str(), "PNG") ||
         checkExtension(image_name.c_str(), "JPG") ||
         checkExtension(image_name.c_str(), "JPEG")){//拡張子のチェック（png,jpg,jpegのみ通過）
        files.push_back(p);  // 動的配列filesにファイルの絶対パスを追加
      }
    }
  }
  /*** ファイル名が昇順になるようにソート ***/
  std::sort(files.begin(), files.end());
}


/**************************
 * CSV操作関数群
 * ************************/

//文字列のsplit機能
std::vector<std::string> FileOperator::split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
    std::vector<std::string> result;
    while (first < str.size()) {
        std::string subStr(str, first, last - first);
        result.push_back(subStr);
        first = last + 1;
        last = str.find_first_of(del, first);
        if (last == std::string::npos) {
            last = str.size();
        }
    }
    return result;
}

std::vector<std::vector<std::string> > FileOperator::csv2vector(std::string filename, int ignore_line_num){
    //csvファイルの読み込み
    std::ifstream reading_file;
    reading_file.open(filename, std::ios::in);
    if(!reading_file){
        std::vector<std::vector<std::string> > data;
        return data;
    }
    std::string reading_line_buffer;
    //最初のignore_line_num行を空読みする
    for(int line = 0; line < ignore_line_num; line++){
        getline(reading_file, reading_line_buffer);
        if(reading_file.eof()) break;
    }

    //二次元のvectorを作成
    std::vector<std::vector<std::string> > data;
    while(std::getline(reading_file, reading_line_buffer)){
        if(reading_line_buffer.size() == 0) break;
        std::vector<std::string> temp_data;
        temp_data = split(reading_line_buffer, ',');
        data.push_back(temp_data);
    }
    return data;
}

/*************************************
 * Private methods
 *************************************/
