#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include "date/date.h"


#ifndef AM_FUNCTIONS_H
#define AM_FUNCTIONS_H

namespace fs = std::filesystem;
namespace am{

    bool fileExists(const std::string fileName, const std::string Path){
        fs::path tmpPath = Path;
        tmpPath /= fileName;
        return fs::exists(tmpPath);
    }



    int readAndCreate(const std::string fileName, const std::string pdbPath, const std::string pdbPrefix, const std::string pdbSuffix, const bool quiet){

        /************************************************************************************************************************
        * Creates description files for Mustang alignment. Reads an input .txt file (fileName) and creates a directory that will be filled
        * with all the descriptor files for each barcode with more than 1 corrisponding sequence. Every file contains the path for the .pdb 
        * files (pdbPath) and the names of each sequence, plus any eventual prefixes (pdbPrefix) or suffixes (pdbSuffix)
        * Returns 0 if executed correctly or:
        * -1 : fileName read error
        * -2 : Folder creation error
        * -3 : descriptor file creation error
        * -4 : file .pdb not found
        *************************************************************************************************************************
        TO DO:
        - Verificare l'esistenza dei file .pdb
        - Numeri esadecimali per i barcode
        *************************************************************************************************************************/


        std::string time = date::format("%d-%m-%Y_%H-%M", std::chrono::system_clock::now());        //getting datetime for folder name
        std::string directoryName = "files_" + time;                                                //folder name preparation


        int total = 0;       //toal created file number
        std::ifstream is(fileName);     //inputFile opening 
        if(!is.good()){ std::cerr << "Cannot open file. Exiting \n"; return -1; }       //opening check
        std::string s;                      //temp variable for each line of the file 
        while(std::getline(is, s)){         //cycle on each line
            int num;                        //number of corrisponding sequences, if == 1 ignore the line
            std::string barcode;            //temp variable for storing barcode
            std::istringstream ss(s);       //temp variable for further data extraction
            ss >> num;                      //getting the first characters in the line, corrisponding to the number of sequences per barcode
            if(num > 1){                    //if the number of sequences is more than one, proceed to file creation
                if(!total){                 //if this is the first file, create the directory to store the files in
                    if (!fs::is_directory(directoryName) || !fs::exists(directoryName)) { //checks that the folder doesn't already exist
                        if (!fs::create_directory(directoryName)){ std::cerr << "Cannot create folder. Exiting \n"; return -2; }   //if it doesn't, create it and check that the creation is successful
                    }
                    else { //if it does, let's use another name
                        bool flag = true;   //making sure that we there's a new name
                        int i = 0;              
                        std::string newName;
                        do {                //we don't move from here until we have a new name
                            newName = directoryName + "_" + std::to_string(i);      //let's try adding numbers as suffixes
                            if (!fs::is_directory(newName) || !fs::exists(newName)){    //checking if the new name exists
                                directoryName = newName;
                                if (!fs::create_directory(directoryName)){ std::cerr << "Cannot create new folder. Exiting \n"; return -2; } 
                                flag = false;       //we can leave this cycle
                                }
                            else {
                                i++;                //or keep trying
                                flag = true;
                            };
                        } while(flag);
                    }
                }
                ss >> barcode;                      //getting the barcode from the file
                barcode.erase(remove(barcode.begin(), barcode.end(), '"'), barcode.end()); //removing quotes
                
                std::string tmpFileName = directoryName + "/" + barcode;                    //each file will be named as the barcode it's referring to
                std::ofstream os(tmpFileName);                                              //creating that file
                if(!os.good()){ std::cerr << "Cannot create file. Exiting \n"; return -3;}  //checking if we can create the file
                os << ">" << pdbPath << "\n";                                               //writing the .pdb file path as the first line, identified by the ">" character as MUSTANG's spec
                for(int i=0; i < num; i++){                                                 //cycling over the number of sequences signaled
                    std::string sequence, desc;                                             //temp variables for sequences file names, description (unused) 
                    int amm;                                                                //temp variable for amminoacid number (unused)
                    ss >> sequence >> desc >> amm;                                          //exctracting info from file
                    sequence.erase(remove(sequence.begin(), sequence.end(), ','), sequence.end());  //removing commas from sequences names
                    std::string tmpSequence = pdbPrefix + sequence + pdbSuffix + ".pdb";            //creating the name of the sequence, adding prefix and suffix if specified 
                    if(fileExists(tmpSequence, pdbPath)){                                   //checks whether .pdb file exists in the path specified
                        os << "+" << tmpSequence << "\n";                                   //if it does, outputting the sequence filename to descript file, identified byb plus char as MUSTANG's spec
                    }else{
                        std::cerr << "File " << pdbPath << "/" << tmpSequence << " not found. Exiting \n";  //if it doesn't, error
                        os.close();
                        fs::remove_all(directoryName);
                        return -4;
                    }
                }
                os.close();                                                                 //close file
                if(!quiet){ std::cout << "Successfully created file \"" << barcode << "\"\n"; } 
                total++;
            }
        }
        if(!quiet){ std::cout << "Created " << total << " files in directory /" << directoryName << ". Exiting \n"; }
        return 0;
    }
}
#endif