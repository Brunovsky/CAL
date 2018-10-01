#include "ui_main.h"
#include "ui_accidents.h"
#include "ui_paths.h"
#include "ui_search.h"

#include <string>

namespace ui {

static const std::string ui_string = "$ Main Menu\n"
    "1 - Cause accidents\n"
    "2 - Fix accidents\n"
    "3 - Path Algorithms\n"
    "4 - Road Search Algorithms\n"
    "5 < exit\n";

int main_menu() {
    clear_screen();

    while (true) {
        std::cout << ui_string << std::endl;

        int option = select_option(5);
        if (option == 5 || option == 0) return 0;

        clear_screen();

        switch (option)
        {
        case 1:
            make_accident();
            break;
        case 2:
            make_fix();
            break;
        case 3:
            paths();
            break;
        case 4:
            road_search();
            break;
        }

        clear_screen();
    }
}

}
