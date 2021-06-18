#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#ifndef AM_CLASSES_H
#define AM_CLASSES_H
namespace am{
    class structure{
        private:
            std::string name, description;
            int amminNum;
        public:
            structure(){
                name = "";
                description = "";
                amminNum = -1;
            };
            structure(const std::string name_, const std::string desc_, const int num_){
                name = name_;
                description = desc_;
                amminNum = num_;
            };
            structure(const structure& str_){
                name = str_.name;
                description = str_.description;
                amminNum = str_.amminNum;
            };
            std::string GetName() const{
                return name;
            };
            std::string GetDesc() const{
                return description;
            };
            int GetNum() const{
                return amminNum;
            };
    };
    class pData{
        private:
            std::string barcode;
            std::vector<structure> str;
        public:
            pData(){
                barcode = "";
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
                return barcode;
            };
            std::vector<structure> GetStructures() const{
                return str;
            };
    };
}
#endif