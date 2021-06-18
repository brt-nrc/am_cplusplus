#include<stdio.h>
#include<iostream>
#include <sstream>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char *argv[]) {
 /*   TCLAP::CmdLine cmdArg("Command description message", ' ', "0.1");
    TCLAP::ValueArg<std::string> inputFileArg("i", "input_file", "File with barcodes", false, "", "filename");
    TCLAP::ValueArg<std::string> descriptionFolderArg("f", "description_files", "Folder with descriptor files", false, "", "folder path");
    TCLAP::OneOf inputArg;
    inputArg.add(inputFileArg).add(descriptionFolderArg);
    cmdArg.add(inputArg);
    TCLAP::ValueArg<std::string> pdbFolderArg("p", "path", "Folder with .pdb files", true, "", "folder path", cmdArg);
    TCLAP::ValueArg<std::string> prefixArg("t", "prefix", "Prefix for .pdb files", false, "", "string", cmdArg);
    TCLAP::ValueArg<std::string> suffixArg("s", "suffix", "Suffix for .pdb files", false, "", "string", cmdArg);
    TCLAP::SwitchArg quietArg("q","quiet","Quiet operation", cmdArg, false);

    std::string inputFile = NULL, descriptionFolder = NULL, pdbPath, pdbPrefix, pdbSuffix;
    bool quiet;
    try{
        cmdArg.parse(argc, argv);
        if(inputFileArg.isSet()){ inputFile = inputFileArg.getValue(); }
        else if(descriptionFolderArg.isSet()){ descriptionFolder = descriptionFolderArg.getValue(); };
        pdbPath = pdbFolderArg.getValue();
        if(prefixArg.isSet()){ pdbPrefix = prefixArg.getValue(); };
        if(suffixArg.isSet()){ pdbSuffix = suffixArg.getValue(); };
        quiet = quietArg.getValue();
    }catch(TCLAP::ArgException &e)  // catch exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; return 1; } */

    if(argc < 2){ std::cerr << "Argomenti mancanti"; return 1;}
    else{
        std::string fileName = argv[1],
                    dataPath = argv[2];
        
        int total = 0;
        std::ifstream is(fileName);
        if(!is.good()){ std::cerr << "Cannot open file. Exiting \n"; return 2; }
        std::string s;
        while(std::getline(is, s)){
            int num;
            std::string barcode;
            std::istringstream ss(s);
            ss >> num;
            if(num > 1){
                if(!total){
                    if(mkdir("files",0775) == -1){ std::cerr << "Cannot create folder. Exiting \n"; return 3;}
                }
                ss >> barcode;
                barcode.erase(remove(barcode.begin(), barcode.end(), '"'), barcode.end());
                std::string tmpFileName = "files/" + barcode;
                std::ofstream os(tmpFileName);
                if(!os.good()){ std::cerr << "Cannot create file. Exiting \n"; return 4;}
                os << ">" << dataPath << "\n";
                for(int i=0; i < num; i++){
                    std::string tmpSequence, sequence, desc; 
                    int amm;
                    ss >> sequence >> desc >> amm;
                    sequence.erase(remove(sequence.begin(), sequence.end(), ','), sequence.end());
                    tmpSequence = sequence + ".pdb";
                    os << "+" << tmpSequence << "\n";
                }
                os.close();
                std::cout << "Successfully created file" << tmpFileName << "\n";
                total++;
            }
        }

        std::cout << "Created" << total << "files. Exiting \n";
        return 0;
    }

}
