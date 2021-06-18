#include <stdio.h>
#include <string>
#include <vector>
#include "amClasses.h"
#include "amFunctions.h"
#include "tclap/CmdLine.h"

int main(int argc, char** argv) {
    /************************************************************************************************************************************
     * Alpha Mike -> Allinamenti Mustang -> Mustang Alignments 
     * 2021 - Enrico Bartolommei
     * 
     * Options:
     * reqired:
     *    OneOf: -i, --input_file : File with barcodes and corresponding sequences
     *           -f, --description_file : Folder with description file as per MUSTANG's spec
     *    -p, --path : path to .pdf files
     *
     * optional:
     *    -t, --prefix : prefix to .pdb files
     *    -s, --suffix : suffix to .pdb files
     *    -q, --quiet : No standard terminal output
     * **********************************************************************************************************************************/
    TCLAP::CmdLine cmdArg("Command description message", ' ', "0.1");
    TCLAP::ValueArg<std::string> inputFileArg("i", "input_file", "File with barcodes", false, "", "filename");
    TCLAP::ValueArg<std::string> descriptionFolderArg("f", "description_files", "Folder with descriptor files", false, "", "folder path");
    TCLAP::OneOf inputArg;
    inputArg.add(inputFileArg).add(descriptionFolderArg);
    cmdArg.add(inputArg);
    TCLAP::ValueArg<std::string> pdbFolderArg("p", "path", "Folder with .pdb files", true, "", "folder path", cmdArg);
    TCLAP::ValueArg<std::string> prefixArg("t", "prefix", "Prefix for .pdb files", false, "", "string", cmdArg);
    TCLAP::ValueArg<std::string> suffixArg("s", "suffix", "Suffix for .pdb files", false, "", "string", cmdArg);
    TCLAP::SwitchArg quietArg("q","quiet","Quiet operation", cmdArg, false);

    std::string inputFile, descriptionFolder, pdbPath, pdbPrefix = "", pdbSuffix = "";
    bool quiet, createFiles;
    try{    //parsing command line arguments and assiging to variables
        cmdArg.parse(argc, argv);
        if(inputFileArg.isSet()){ inputFile = inputFileArg.getValue(); createFiles = true; }
        else if(descriptionFolderArg.isSet()){ descriptionFolder = descriptionFolderArg.getValue(); createFiles = false; };
        pdbPath = pdbFolderArg.getValue();
        if(prefixArg.isSet()){ pdbPrefix = prefixArg.getValue(); };
        if(suffixArg.isSet()){ pdbSuffix = suffixArg.getValue(); };
        quiet = quietArg.getValue();
    }catch(TCLAP::ArgException &e)  // catch exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; return 1; }


    if(createFiles){    //if -i option is provided, proceed to read .txt file and create description file accordingly
        am::readAndCreate(inputFile, pdbPath, pdbPrefix, pdbSuffix, quiet);
    }
    
    return 0;
}
