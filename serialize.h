#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Engine {
    class SerializedObject {
        private:
            std::string data="";
        public:
            void Serialize(std::unordered_map<std::string, std::string> map) {
                for(auto i : map) {
                    data=data+i.first+":"+i.second+"\n";
                }
            }
            std::unordered_map<std::string, std::string> Deserialize() {
                std::unordered_map<std::string, std::string> temp;
                std::stringstream datastream(data);
                std::string line;
                while(std::getline(datastream, line,'\n')) {
                    std::string key   = line.substr(0, line.find(":"));
                    std::string value = line.substr(line.find(":")+1);
                    temp[key]=value;
                }
                return temp;
            }
            SerializedObject(std::string data) : data(data) {}
            SerializedObject(std::unordered_map<std::string, std::string> data) {
                Serialize(data);
            }
            std::string GetData() {
                return data;
            }
            std::string GetValue(std::string value) {
                auto d=Deserialize();
                return d[value];
            }
            void WriteToFile(std::string file_name) {
                std::ofstream f(file_name.c_str());
                f<<data.c_str();
                f.close();
            }
    };
}
