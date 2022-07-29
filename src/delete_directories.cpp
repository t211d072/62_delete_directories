#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>
#include "../lib/file_operator.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

void getDirectories(const fs::path &path, std::vector<fs::path> &directories)
{
  BOOST_FOREACH(const fs::path& p, std::make_pair(fs::recursive_directory_iterator(path), fs::recursive_directory_iterator())) {
    if (fs::is_directory(p)){ // 参照しているパスがディレクトリなら処理を行う
      directories.push_back(p);  // 動的配列directoriesにディレクトリの絶対パスを追加
    }
  }
  /*** ディレクトリ名が昇順になるようにソート ***/
  std::sort(directories.begin(), directories.end());
}

int main(int argc, char *argv[]){
  using namespace std;

  cout << "[プログラムの説明]" << endl;
  cout << "・第一引数に与えたテキストファイルから削除するディレクトリ名を読み込み、第二引数に入力したディレクトリ下において該当するディレクトリがあった場合削除するプログラムです" << endl;
  cout << "・第一引数に削除したいディレクトリ名を入力したテキストファイル（改行して記載することで複数検索できます）" << endl;
  cout << "・第二引数に削除を行いたいディレクトリ" << endl;
  cout << "・オプション[-t]でテストモードになり、削除は実行されずに該当のディレクトリのみフルパスで出力します" << endl;
  if(argc < 3){
    cout << "Error : Usage is... " << argv[0] << " [delete_directories_name_textfile]" << " [excute_directory_path]" << endl;
    return -1;
  }
  system("read -p '>>Enterキーを押すと処理を開始します' var");

  int opt;
  bool isTestMode = false;
  opterr = 0; //getopt()のエラーメッセージを無効にする。

  while ((opt = getopt(argc, argv, "t:")) != -1) {
    //コマンドライン引数のオプションがなくなるまで繰り返す
    switch (opt) {
      case 't':
        isTestMode = true;
        break;
      default: //指定していないオプションが渡された場合
        printf("Usage: %s [-t] ...\n", argv[0]);
        return 1;
    }
  }

  string input_delete_directoris_name;
  if(isTestMode) input_delete_directoris_name = argv[2];
  else if(!isTestMode) input_delete_directoris_name = argv[1];

  //テキストファイルから削除したいディレクトリの名前を一行ずつ読み込む
  vector<string> delete_directories_name;
  string line;

  ifstream input_file(input_delete_directoris_name);
  if (!input_file.is_open()) {
    cerr << "Could not open the file - '" << argv[1] << "'" << endl;
    return EXIT_FAILURE;
  }

  while (getline(input_file, line)){
    delete_directories_name.push_back(line);
  }
  input_file.close();

  FileOperator fileOperator;
  vector<string> search_result_dirpath;
  vector<fs::path> dirpath;

  string excute_dirpath;
  if(isTestMode) excute_dirpath = argv[3];
  else if(!isTestMode) excute_dirpath = argv[2];

  for (const fs::directory_entry& x : fs::recursive_directory_iterator(excute_dirpath)) {
    if(fs::is_directory(x.path())){
      cout << "search now:" << x.path() << std::endl;
      dirpath.push_back(x.path());
    }
    sort(dirpath.begin(), dirpath.end());
  }
  

  for(int i = 0; i < delete_directories_name.size(); i++){
    const string SEARCH_FILENAME = delete_directories_name[i];
    for(int i = 0; i < dirpath.size(); i++){
      string dirname = fileOperator.getFileName(dirpath[i].string());
      //cout << dirname << endl;
      if(dirname == SEARCH_FILENAME){
        search_result_dirpath.push_back(dirpath[i].string());
      }
    }
  }

  cout << endl;
  cout << "！！以下のディレクトリをすべて削除します（Enterで実行）" << endl;
  for(int i = 0; i < search_result_dirpath.size(); i++){
    cout << "・" << search_result_dirpath[i] << endl;
  }
  cout << "！！以上のディレクトリをすべて削除します" << endl;
  if(!isTestMode) cout << "・テストモードではないはありません！実際に削除されます！中止するにはCtrl+Zを押下してください" << endl;
  system("read -p '>>何かキーを押すと処理を開始します' var");

  for(int i = 0; i < search_result_dirpath.size(); i++){
    if(isTestMode){
      cout << "remove directories(Test Mode): " << search_result_dirpath[i] << endl;
    }else if(!isTestMode){
      cout << "remove now: " << search_result_dirpath[i] << endl;
      fs::remove_all(search_result_dirpath[i]);
    }
  }

  cout << "remove done." << endl;
  
  return 0;
}