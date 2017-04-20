/* Copyright 2016 Pasu Chan <cschanaj@connect.ust.hk>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <iostream>

#include "GumboCpp.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "Usage: find_links <html filename>\n";
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    std::ifstream infile(filename);

    if(!infile || !infile.good())
    {
        std::cerr << "File " << filename << " not found!\n";
        return EXIT_FAILURE;
    }

    std::string temp = "";
    std::string html = "";

    while(!infile.eof())
    {
        std::getline(infile, temp);
        html += temp;
    }

    auto output = gumbo_parse(html.c_str());

    GumboCpp::for_each(output->root, [](const GumboNode *root) {
        if(GumboCpp::hasAttributeName(root, "href"))
        {
            std::cout << GumboCpp::getAttributeValue(root, "href") << "\n";
        }
    });

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return EXIT_SUCCESS;
}
