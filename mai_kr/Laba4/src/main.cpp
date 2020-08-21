#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

const unsigned char S_SIZE = 12;

int main(int argc, char **argv) {

    if(argc < 3) {
        std::cout <<"Example:" << std::endl;
        std::cout << "./" << argv[0] << " [size] [file1] [file2] ... [fileN]" << std::endl;
        exit(EXIT_FAILURE);
    }

    char symbols[] = {';', ':', '.', ',', '?', '!', '(', ')', '"', '\'', ' ', '\n'};
    bool finded = false;
    size_t textSize = atoi(argv[1]);
    size_t minSize;
    int typeChar;
    std::string word, rst1, rst2, rwt1, rwt2, rct1, rct2;
    std::vector<std::string> dictionary;
    
    for(int i = 2; i < argc; ++i) {

        std::ifstream fin(argv[i]);
        std::ifstream in(argv[i], std::ifstream::ate | std::ifstream::binary);
        if(i == 1) {
            minSize = in.tellg();
        }
        else {
            minSize =((size_t) in.tellg()) < minSize ? ((size_t) in.tellg()) : minSize;
        }
        
        while(fin >> word) {

            for(size_t p = 0; p < dictionary.size(); ++p) {
                if(dictionary[p] == word) {
                    finded = true;
                    break;
                }
            }

            if(!finded) {
                dictionary.push_back(word);
            }
            else {
                finded = false;
            }
        }
    }

    if(textSize > minSize) {
        std::cout << "Size limit: " << minSize << std::endl;
        std::cout << "Input another size!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::default_random_engine             gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t>  rFile(2, argc);
    std::uniform_int_distribution<size_t>  rWord(0, dictionary.size() - 1);
    std::uniform_int_distribution<size_t>  sChar(0, S_SIZE - 1);
    std::uniform_int_distribution<wchar_t> lChar('A', 'Z');
    std::uniform_int_distribution<wchar_t> bChar('a', 'z');
    std::uniform_int_distribution<int>     tChar1(0, 2);
    std::uniform_int_distribution<int>     tChar2(0, 1);

    while(rwt1.size() < textSize) {
        rwt1 += " " + dictionary[rWord(gen)];
    }

    while(rwt2.size() < textSize) {
        rwt2 += " " + dictionary[rWord(gen)];
    }

    while(rst1.size() < textSize) {
        typeChar = tChar1(gen);
        switch(typeChar) {
            case 0:
                rst1 += lChar(gen);
                break;
            case 1:
                rst1 += bChar(gen);
                break;
            case 2:
                rst1 += symbols[sChar(gen)];
                break;
        }

        typeChar = tChar1(gen);
        switch(typeChar) {
            case 0:
                rst2 += lChar(gen);
                break;
            case 1:
                rst2 += bChar(gen);
                break;
            case 2:
                rst2 += symbols[sChar(gen)];
                break;
        }
    }

    while(rct1.size() < textSize) {
        typeChar = tChar2(gen);
        switch(typeChar) {
            case 0:
                rct1 += lChar(gen);
                break;
            case 1:
                rct1 += bChar(gen);
                break;
        }

        typeChar = tChar2(gen);
        switch(typeChar) {
            case 0:
                rct2 += lChar(gen);
                break;
            case 1:
                rct2 += bChar(gen);
                break;
        }
    }

    std::cout << "1.   | Two normal texts.                             | Rate: ";
    {
        size_t i = rFile(gen);
        size_t j = rFile(gen);
        while(i == j) {
            j = rFile(gen);
        }
        
        std::ifstream text1(argv[i]);
        std::ifstream text2(argv[j]);
        size_t counter = 0;
        char c1, c2;
        for(size_t step = 0; step < textSize; ++step) {
            text1.read(&c1, 1);
            text2.read(&c2, 1);
            if(c1 == c2) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }

    std::cout << "2.1. | One normal text and one random text (char).   | Rate: ";
    {
        size_t i = rFile(gen);
        
        std::ifstream text1(argv[i]);
        size_t counter = 0;
        char c1;
        for(size_t step = 0; step < textSize; ++step) {
            text1.read(&c1, 1);
            if(c1 == rct1[step]) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }

    std::cout << "2.2. | One normal text and one random text (symbol). | Rate: ";
    {
        size_t i = rFile(gen);
        
        std::ifstream text1(argv[i]);
        size_t counter = 0;
        char c1;
        for(size_t step = 0; step < textSize; ++step) {
            text1.read(&c1, 1);
            if(c1 == rst1[step]) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }
    std::cout << "3.   | One normal text and one random text (word).   | Rate: ";
    {
        size_t i = rFile(gen);
        
        std::ifstream text1(argv[i]);
        size_t counter = 0;
        char c1;
        for(size_t step = 0; step < textSize; ++step) {
            text1.read(&c1, 1);
            if(c1 == rwt1[step]) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }

    std::cout << "4.1. | Two random texts (char).                      | Rate: ";
    {
        size_t counter = 0;
        for(size_t step = 0; step < textSize; ++step) {
            if(rct1[step] == rct2[step]) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }

    std::cout << "4.2. | Two random texts (symbol).                    | Rate: ";
    {
        size_t counter = 0;
        for(size_t step = 0; step < textSize; ++step) {
            if(rst1[step] == rst2[step]) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }
    
    std::cout << "5.   | Two random texts (word).                      | Rate: ";
    {
        size_t counter = 0;
        for(size_t step = 0; step < textSize; ++step) {
            if(rwt1[step] == rwt2[step]) {
                ++counter;
            }
        }

        std::cout << (double) counter / textSize << std::endl;
    }
    return 0;
}
