#include "loadmap.h"
#include "graph.h"
#include "ui_main.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "test_search.h"

#define FILENAME_PREFIX "./resource/"

std::string files = "Select map file\n"
"Available by default:\n"
" NODES   ~SUBROADS   NAME\n"
"    52        64      fep\n"             // Minor check
"   100       125      newyork\n"         // Minor check
"   270       300      madrid\n"          // Minor check
"  1756      1914      vilareal\n"        // Minor check
"  6123      6278      graciosa\n"        // Minor check
"  9917     11261      bignewyork\n"      // Good
"\n"
"   500       500      faro\n"            // Decent
"    1K        1K      coimbra\n"         // Good
"    2K        2K      porto\n"           // Good
"    4K        4K      sydney\n"          // Good
"    8K       10K      tokyo\n"           // Good
"   16K       18K      paris\n";          // Good

std::unique_ptr<Graph> graph;

static void get_filename(std::string& filename) {
    std::cout << files << std::endl;

    while (true) {
        std::cout << "Filename: ";
        std::getline(std::cin, filename);

        filename = FILENAME_PREFIX + filename;

        if (check_filename(filename)) {
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    //return test();

    std::srand((unsigned)std::time(nullptr));
    std::ios::sync_with_stdio(false);

    std::string filename;
    get_filename(filename);

    if (load_map(filename) != 0) {
        ui::discard();
        return 1;
    } else {
        ui::main_menu();
    }

    return 0;
}
