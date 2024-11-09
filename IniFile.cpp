#include "inifile.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

IniFile::IniFile(const std::string &path) : filePath(path)
{
    std::string line;
    std::string section;
    std::string key;
    std::string value;
    std::ifstream in(filePath);
    std::map<std::string, std::string> keys;
    std::map<std::string, KeysMap> sections;
    if (in.is_open())
    {
        while (in >> line)
        {
            if (line.find("[") != -1 && line.find("]") != -1)
            {
                if (section.length() > 0)
                {
                    if (data.find(section) != data.end())
                    {

                        std::map<std::string, std::string> temp = data[section];
                        for (const auto &element : temp)
                        {
                            if (keys.find(element.first) == keys.end())
                            {
                                keys[element.first] = element.second;
                            }
                        }
                    }
                    data[section] = keys;
                    keys.clear();
                    section.erase(0, section.length());
                }
                for (int i = line.find("[") + 1; i < line.find("]"); i++)
                {
                    if (isalpha(line[i]) || isdigit(line[i]))
                    {
                        section = section + line[i];
                    }
                }
            }
            if (line.find("=") != -1 && section.length() > 0)
            {
                for (int i = 0; i < line.find("="); i++)
                {
                    if (line[i] == ';')
                    {
                        break;
                    }
                    if (isalpha(line[i]) || isdigit(line[i]))
                    {
                        key = key + line[i];
                    }
                }
                for (int i = line.find("=") + 1; i < line.length(); i++)
                {

                    if (line[i] == ';')
                    {
                        break;
                    }
                    if (isalpha(line[i]) || isdigit(line[i]))
                    {

                        value = value + line[i];
                    }
                }

                if (key.length() > 0 && value.length() > 0)
                {
                    keys[key] = value;
                }
                key.erase(0, key.length());
                value.erase(0, value.length());
            }
        }
        if (sections.count(section) == 0 && section.length() > 0)
        {

            if (data.find(section) != data.end())
            {

                std::map<std::string, std::string> temp = data[section];
                for (const auto &element : temp)
                {
                    if (keys.find(element.first) == keys.end())
                    {
                        keys[element.first] = element.second;
                    }
                }
            }
            data[section] = keys;
        }
    }
    // for (const auto& element : data){
    //     std::cout << element.first << std::endl;
    //     for (const auto& elem : element.second){
    //         std::cout << elem.first << "=" << elem.second << std::endl;
    //     }
    // }

    in.close();
}

IniFile::~IniFile()
{
    std::ofstream file(filePath);
    for (const auto &section : data)
    {
        file << "[" << section.first << "]\n";
        for (const auto &key : section.second)
        {
            file << key.first << "=" << key.second << "\n";
        }
    }
    data.clear();
}
void IniFile::save()
{
    std::ofstream file(filePath);
    for (const auto &section : data)
    {
        file << "[" << section.first << "]\n";
        for (const auto &key : section.second)
        {
            file << key.first << "=" << key.second << "\n";
        }
    }
}

int IniFile::readInt(const std::string &section, const std::string &key,
                     int def)
{
    if (isSectionExist(section) && isKeysExist(section, key))
    {
        return std::stoi(data[section][key]);
    }
    return def;
}

double IniFile::readDouble(const std::string &section, const std::string &key,
                           double def)
{
    if (isSectionExist(section) && isKeysExist(section, key))
    {
        return std::stod(data[section][key]);
    }
    return def;
}

std::string IniFile::readString(const std::string &section,
                                const std::string &key,
                                const std::string &def)
{
    if (isSectionExist(section) && isKeysExist(section, key))
    {
        return data[section][key];
    }
    return def;
}

bool IniFile::readBool(const std::string &section, const std::string &key,
                       bool def)
{
    if (isSectionExist(section) && isKeysExist(section, key))
    {
        std::string value = data[section][key];
        for (const auto &trueValue : trueValues)
        {
            if (value == trueValue)
            {
                return true;
            }
        }
    }
    return def;
}

bool IniFile::isSectionExist(const std::string &section)
{
    return data.find(section) != data.end();
}

bool IniFile::isKeysExist(const std::string &section, const std::string &key)
{
    return isSectionExist(section) &&
           data[section].find(key) != data[section].end();
}

size_t IniFile::getSectionsCount() { return data.size(); }

size_t IniFile::getKeysCount(const std::string &section)
{
    if (isSectionExist(section))
    {
        return data[section].size();
    }
    return 0;
}

SectionsMap IniFile::getSections() const { return data; }

bool IniFile::deleteSection(const std::string &section)
{
    return data.erase(section) != 0;
}

bool IniFile::deleteKey(const std::string &section, const std::string &key)
{
    if (isSectionExist(section))
    {
        return data[section].erase(key) != 0;
    }
    return false;
}

bool IniFile::addNewSection(const std::string &section)
{
    if (!isSectionExist(section))
    {
        data[section] = {};
        return true;
    }
    return false;
}