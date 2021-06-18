#include<vector>
#ifndef ALPHAMIKE_H
#define ALPHAMIKE_H
namespace am{
    class structure{
        private:
            std::string name, decription;
            int amminNum;
        public:
            structure(){
                name = NULL;
                description = NULL;
                amminNum = -1;
            };
            structure(const std::string name_, const std::string desc_, const int num_){
                name = name_;
                description = desc_;
                amminNum = num_;
            };
            structure(const structure& str_){
                name = str.name;
                description = str.description;
                amminNum = str.annimNum;
            };
            std::string GetName() const{
                return this -> name
            };
            std::string GetDesc() const{
                return this -> description
            };
            int GetNum() const{
                return this -> amminNum
            };
    };
    class pData{
        private:
            std::string barcode;
            std::vector<structure> str;
        public:
            pData(){
                barcode = NULL;
            };
            pData(const std::string barcode_, const std::vector<structure> str_){
                barcode = barcode_;
                str = str_;
            };
            pData(const pData& pdat_){
                barcode = pdat_.barcode;
                str = pdat_.str;
            };
            std::string GetBarcode() const{
                return this -> barcode
            };
            std::vector<structure> GetStructures() const{
                return this -> str
            };
    };
}
#endif