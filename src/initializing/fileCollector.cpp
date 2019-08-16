/**
 *  @file    parserInit.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  Apache License Version 2.0
 */

#include <initializing/fileCollector.h>

#include <sakura_converter.hpp>
#include <commonMethods/stringMethods.hpp>

namespace SakuraTree
{

FileCollector::FileCollector(SakuraConverter *driver)
{
    m_driver = driver;
}

/**
 * @brief ParserInit::initProcess
 * @return
 */
bool
FileCollector::initFileCollector(const std::string &rootPath)
{
    boost::filesystem::path rootPathObj(rootPath);  // avoid repeated path construction below
    m_errorMessage = "";

    // precheck
    if(exists(rootPathObj) == false)
    {
        m_errorMessage = "path doesn't exist: " + rootPath;
        return false;
    }

    // get all files
    if(is_directory(rootPathObj)) {
        getFilesInDir(rootPathObj);
    } else {
        m_fileContents.push_back(std::make_pair(rootPath, nullptr));
    }

    // check result
    if(m_fileContents.size() == 0)
    {
        m_errorMessage = "no files found at the end of the path: " + rootPath;
        return false;
    }

    // get and parse file-contents
    for(uint32_t i = 0; i < m_fileContents.size(); i++)
    {
        const std::string filePath = m_fileContents.at(i).first;
        std::pair<Kitsune::Common::DataItem*, bool> result = m_driver->parse(readFile(filePath));

        if(result.second == false)
        {
            m_errorMessage = result.first->get("error")->toString();
            delete result.first;
            // TODO: delete content of m_fileContents too
            return false;
        }

        m_fileContents[i].second = result.first->toObject();
        std::string output = "";
        m_fileContents[i].second->print(&output, true);
        std::cout<<output<<std::endl;
    }

    return true;
}

/**
 * @brief FileCollector::getObject
 * @param name
 * @param type
 * @return
 */
Kitsune::Common::DataObject*
FileCollector::getObject(const std::string &name,
                         const std::string &type)
{
    // precheck
    if(name == "") {
        return nullptr;
    }

    // search
    std::vector<std::pair<std::string, DataObject*>>::iterator it;
    for(it = m_fileContents.begin();
        it != m_fileContents.end();
        it++)
    {
        if(it->second->get("name")->toString() == name)
        {
            if(type != ""
                    && it->second->get("type")->toString() == type)
            {
                return it->second;
            }

            if(type == "") {
                return it->second;
            }
        }
    }

    return nullptr;
}

/**
 * @brief FileCollector::getSeedName
 * @param index
 * @return
 */
const std::string
FileCollector::getSeedName(const uint32_t index)
{
    if(index >= m_fileContents.size()) {
        return std::string("");
    }

    return m_fileContents.at(index).second->get("name")->toString();
}

/**
 * @brief FileCollector::getErrorMessage
 * @return
 */
const std::string
FileCollector::getErrorMessage() const
{
    return m_errorMessage;
}

/**
 * @brief ParserInit::getFilesInDir
 * @param directory
 * @return
 */
void
FileCollector::getFilesInDir(const boost::filesystem::path &directory)
{
    directory_iterator end_itr;

    for(directory_iterator itr(directory);
        itr != end_itr;
        ++itr)
    {
        if(is_directory(itr->path())) {
            getFilesInDir(itr->path());
        } else {
            //TODO: delete output
            std::cout<<"file: "<<itr->path().string()<<std::endl;
            m_fileContents.push_back(std::make_pair(itr->path().string(), nullptr));
        }
    }
}

/**
 * reads the content of a specific text-file
 *
 * @return string with the file-content
 */
const std::string
FileCollector::readFile(const std::string &filePath)
{
    // read into string
    std::ifstream inFile;
    inFile.open(filePath);
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    const std::string fileContent = strStream.str();

    inFile.close();

    return fileContent;
}

}
