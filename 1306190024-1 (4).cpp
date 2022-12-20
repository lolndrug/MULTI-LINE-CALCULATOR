// Muhammed Mustafa Ýnce



#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include<string>

using namespace std;


namespace FileUtil{

    vector<string> readFile(string filePath){

        vector<string> content;

        fstream inputFile(filePath);

        if(inputFile.is_open()){
            string line;
            while (getline(inputFile,line)){
                content.push_back(line);
            }
            inputFile.close();
        }
        return content;
    }



}
namespace StringUtil {



    void trim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        s.erase(std::find_if(s.rbegin(), s.rend(),std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    }

    std::vector<std::string> split(std::string src, std::string delimiter) {
        std::vector<std::string> result;
        size_t pos = 0;
        while (true) {
            if((pos = src.find(delimiter)) != std::string::npos) {
                string tmp = src.substr(0, pos);
                trim(tmp);
                result.push_back(tmp);
                src.erase(0, pos + delimiter.length());
            }else{
                trim(src);
                result.push_back(src);
                break;
            }
        }
        return result;
    }
    void isExistOpt(string line, bool &result, string &delimiter){
        size_t pos = 0;
        if((pos = line.find("+")) != std::string::npos) {
            result = true;
            delimiter = "+";
            return;
        }

        if((pos = line.find("-")) != std::string::npos) {
            result = true;
            delimiter = "-";
            return;
        }

        if((pos = line.find("/")) != std::string::npos) {
            result = true;
            delimiter = "/";
            return;
        }
        if((pos = line.find("*")) != std::string::npos) {
            result = true;
            delimiter = "*";
            return;
        }
        if((pos = line.find("OUT")) != std::string::npos){
            result = false;
            delimiter = "OUT";
            return;
        }

        if((pos = line.find("=")) != std::string::npos){
            result = false;
            delimiter = "VAR";
            return;
        }
    }
    bool isExistOut(string line){
        size_t pos = 0;
        if((pos = line.find("OUT")) != std::string::npos) {
            return true;
        }
        return false;
    }
   /* void printConsole(std::string line){
        cout<< ""<<line << endl;
    }*/
    string generateString(size_t length){
        auto randchar = []() -> char
        {
            const char charset[] =
                    "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
        std::string str(length,0);
        std::generate_n( str.begin(), length, randchar );
        return str;
    }

}



using namespace StringUtil;


std::map<string,string> variables;
std::vector<string> lines;
std::vector<string> lines_loop;
std::vector<string> lines_plus;
std::vector<string> lines_minus;
std::vector<string> lines_mul;
std::vector<string> lines_div;


namespace Operations {

    int mathOperation(int val1, int val2, string operation) {
        if(operation.compare("+") == 0){
            return (val1 + val2);
        }else if(operation.compare("-") == 0){
            return (val1 - val2);
        }else if(operation.compare("*") == 0){
            return (val1 * val2);
        }else if(operation.compare("/") == 0){
            return (val1 / val2);
        }
        return 0;
    }
    float mathOperation(float val1, int val2, string operation) {
        if(operation.compare("+") == 0){
            return (val1 + val2);
        }else if(operation.compare("-") == 0){
            return (val1 - val2);
        }else if(operation.compare("*") == 0){
            return (val1 * val2);
        }else if(operation.compare("/") == 0){
            return (val1 / val2);
        }
        return 0;
    }
    float mathOperation(int val1, float val2, string operation) {
        if(operation.compare("+") == 0){
            return (val1 + val2);
        }else if(operation.compare("-") == 0){
            return (val1 - val2);
        }else if(operation.compare("*") == 0){
            return (val1 * val2);
        }else if(operation.compare("/") == 0){
            return (val1 / val2);
        }
        return 0;
    }
    float mathOperation(float val1, float val2, string operation) {
        if(operation.compare("+") == 0){
            return (val1 + val2);
        }else if(operation.compare("-") == 0){
            return (val1 - val2);
        }else if(operation.compare("*") == 0){
            return (val1 * val2);
        }else if(operation.compare("/") == 0){
            return (val1 / val2);
        }
        return 0;
    }

    void findLoop(string &line){
        size_t pos1 = 0;
        size_t pos2 = 0;
        //printConsole(line);
        if((pos1 = line.find("LOOP")) != std::string::npos &&
           (pos2 = line.find("TIMES")) != std::string::npos){
//
            string cut = line.substr(pos1+4,(pos2-pos1-4));
            trim(cut);
            string loopSize = line.substr(pos2+5, (line.length()-pos2+5));
            trim(loopSize);
            int size = stoi(loopSize);
            for (int i = 0; i < size; ++i) {
                lines_loop.push_back(cut);
            }
        }else{
            lines_loop.push_back(line);
        }

    }
    void parseLines(string &line){
        size_t pos1 = 0;
        size_t pos2 = 0;
        if((pos1 = line.find_last_of("(")) != std::string::npos &&
           (pos2 = line.find_first_of(")")) != std::string::npos
           )
        {
            string cut = line.substr(pos1+1,(pos2-pos1-1));
            string newVar = generateString(5);
            lines.push_back(newVar+"="+cut);
            line = line.replace(pos1,(pos2-pos1+1),newVar);
            parseLines(line);
        }
    }

    bool isExistDelimiter(string cut){
        size_t size1 = std::count(cut.begin(), cut.end(), '*');
        size_t size2 = std::count(cut.begin(), cut.end(), '/');
        size_t size3 = std::count(cut.begin(), cut.end(), '-');
        size_t size4 = std::count(cut.begin(), cut.end(), '+');
        if((size1+size2+size3+size4)>=1){
            return true;
        }else{
            return false;
        }
    }

    void parseMultiVariable(string &line,string delimiter,std::vector<string> &source){
        size_t size1 = std::count(line.begin(), line.end(), '*');
        size_t size2 = std::count(line.begin(), line.end(), '/');
        size_t size3 = std::count(line.begin(), line.end(), '-');
        size_t size4 = std::count(line.begin(), line.end(), '+');
        if((size1+size2+size3+size4)>1){
            size_t delimiter_size = std::count(line.begin(), line.end(), delimiter[0]);
            if(delimiter_size >= 1) {

                size_t pos0 = line.find_first_of("=");

                size_t pos1 = line.find_first_of(delimiter);
                string cut1 = line.substr(pos0+1, (pos1-pos0-1));
                string newVar1 = cut1;
                if (isExistDelimiter(cut1)) {
                    newVar1 = generateString(5);
                    string tmp = newVar1+"="+cut1;
                    parseMultiVariable(tmp,delimiter,source);
                    source.push_back(newVar1 + "=" + cut1);
                }

                string cut2 = line.substr(pos1+1,(line.length()-pos1));
                string newVar2 = cut2;
                if(isExistDelimiter(cut2)){
                    newVar2 = generateString(5);
                    string tmp = newVar2+"="+cut2;
                    parseMultiVariable(tmp,delimiter,source);
                    source.push_back(tmp);
                }
                line = line.substr(0,pos0+1) + newVar1+delimiter+newVar2;

            }

        }

    }
    void parseOperation(string delimiter,std::vector<string> &source,std::vector<string> &target){
        //printConsole("");
        for (int i = 0; i < source.size(); ++i) {
            string line = source[i];
            Operations::parseMultiVariable(line,delimiter,target);
            target.push_back(line);
        }

//        for (int i = 0; i < target.size(); ++i) {
//            printConsole("Line=>"+target[i]);
//        }
    }
    void setVariableValue(int result,string key){
        size_t pos = 0;
        if ( variables.find(key) == variables.end() ) {
            variables.insert({key, to_string(result)});
        } else {
            variables.find(key)->second = to_string(result);
        }
    }

    void setVariableValue(float result,string key){
        size_t pos = 0;
        if ( variables.find(key) == variables.end() ) {
            variables.insert({key, to_string(result)});
        } else {
            variables.find(key)->second = to_string(result);
        }
    }
    void readValueAndApplyOperation(string line,string delimiter) {
        vector<string> tmp = split(line,"=");
        vector<string> splitOpt = split(tmp[1],delimiter);

        string val1 = variables.find(splitOpt[0])->second;
        string val2 = variables.find(splitOpt[1])->second;
        size_t pos = 0;
        if((pos = val1.find(".")) != std::string::npos &&
            (pos = val2.find(".")) != std::string::npos)
        {
            float result = mathOperation(stof(val1),stof(val2),delimiter);
            setVariableValue(result,tmp[0]);

        }
        else if((pos = val1.find(".")) == std::string::npos &&
                 (pos = val2.find(".")) != std::string::npos)
        {
            float result = mathOperation(stoi(val1),stof(val2),delimiter);
            setVariableValue(result,tmp[0]);
        }else if((pos = val1.find(".")) != std::string::npos &&
                 (pos = val2.find(".")) == std::string::npos)
        {
            float result = mathOperation(stof(val1),stoi(val2),delimiter);
            setVariableValue(result,tmp[0]);
        }
        else if((pos = val1.find(".")) == std::string::npos &&
                (pos = val2.find(".")) == std::string::npos)
        {
            int result = mathOperation(stoi(val1),stoi(val2),delimiter);
            setVariableValue(result,tmp[0]);
        }

    }
}

int main() {
  

    string inputFile = "Challange6.inp"; // Ýnput dosyasýnýn konumu buraya girilecek.
    std::vector<string> content = FileUtil::readFile(inputFile);

    for (int i = 0; i < content.size(); ++i) {
        string line = content[i];
        Operations::findLoop(line);
    }
    for (int i = 0; i < lines_loop.size(); ++i) {
        string line = lines_loop[i];
        Operations::parseLines(line);
        lines.push_back(line);
    }

    Operations::parseOperation("+",lines,lines_plus);
    Operations::parseOperation("-",lines_plus,lines_minus);
    Operations::parseOperation("*",lines_minus,lines_mul);
    Operations::parseOperation("/",lines_mul,lines_div);


    for (int i = 0; i < lines_div.size(); ++i) {
        bool  result = false;
        string delimiter;
        string line = lines_div[i];
        isExistOpt(line,result,delimiter);
        if(result){
            Operations::readValueAndApplyOperation(line,delimiter);
        }else if(delimiter.compare("VAR") == 0){
            vector<string> tmp = split(line,"=");
            variables.insert({tmp[0],tmp[1]});
        }else if(delimiter.compare("OUT") == 0){
            vector<string> out = split(line," ");
            string  outVar = variables.find(out[1])->second;
            //printConsole("OUT => "+outVar);
            size_t index = inputFile.find(".inp", 0);
            ofstream newOut(inputFile.replace(index,(inputFile.length()-index),".out"));
            newOut.write(outVar.c_str(),outVar.length());
            newOut.close();
        }
    }

    cout << "u will see the result at the output file"<<endl;
    cout << "                                          ";
//    for (std::map<string, string>::iterator iter = variables.begin(); iter != variables.end(); ++iter) {
//        printConsole(iter->first+" => "+iter->second);
//    }
    return 0;
}
