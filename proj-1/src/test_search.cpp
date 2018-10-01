#include "test_search.h"
#include "string_search.h"
#include <utility>

using namespace search;

static std::string loremipsum = "Lorem ipsum dolor sit amet, at commodo, sed nunc, quis ut volutpat at. "
    "Suspendisse ante pulvinar aliquam semper, expedita vestibulum volutpat phasellus cras nec, "
    "a luctus justo vestibulum ipsum et. Ligula conubia dignissimos nulla nunc ut, sociis ut est "
    "quisque magna libero ridiculus. Voluptatem duis maecenas in mauris, donec elit eu felis ipsum "
    "accumsan, lacinia quam quam gravida, tincidunt ultrices vestibulum adipiscing, est integer id "
    "ridiculus fringilla. Per a fermentum lacus, laoreet cursus cras feugiat in.\n"
    "Rhoncus molestie nunc. Sem lectus vitae nostra. Eget mattis integer ante. Gravida in lorem "
    "maecenas purus tincidunt pharetra. Consectetuer aliquet rutrum ultricies placerat, class "
    "adipiscing pellentesque porta proin urna id. Commodo risus a ut ac, elit arcu morbi dolor "
    "integer, sapien ante natoque. Penatibus enim vel amet blandit eu et. Aliquam id, mus mus et, "
    "nec sem, in mi, donec vel magnis proin diam. Lorem ut id aliquam justo, eu wisi vitae eleifend "
    "um tellus nascetur, vulputate auctor id ac malesuada at lobortis, taciti velit, conubia wisi "
    "tristique integer nunc. Ultricies semper metus, nec quam, aliquam nunc nesciunt id nulla "
    "porttitor. Est metus ornare ante sed justo scelerisque, ac augue tincidunt, lobortis neque "
    "scelerisque etiam sagittis, ultricies integer et ante tempor mattis est, leo a quis nisl "
    "pharetra libero amet.\n"
    "Nulla eleifend placerat quis vestibulum quis, nibh rutrum nunc ultricies, magna cursus volutpat "
    "mperdiet ad adipiscing sem, pellentesque pharetra maecenas ante lorem eu eget. Adipiscing "
    "mauris augue lacus. Vivamus ac mi mattis eu velit eget, ac neque, arcu curabitur adipiscing "
    "laoreet. Eget lectus, gravida arcu ipsum, quis porttitor laoreet tristique class purus taciti. "
    "on sit venenatis velit vitae est amet, vitae quis suspendisse morbi, libero risus perspiciatis "
    "lorem sed ac sed, urna a enim vivamus eleifend mus, vitae mi. Donec urna, integer enim urna "
    "morbi dui vitae elementum. Penatibus enim sed, nulla est id aliquam, scelerisque sit dictumst "
    "in morbi sed, et nec, ut pellentesque arcu sapien quis nec placerat. Fusce sed quam blandit et "
    "lobortis, vestibulum pharetra metus pulvinar est, fermentum vehicula nonummy.\n"
    "Vel massa dolorum in neque quam, etiam ultricies neque purus mauris, semper quis nulla "
    "suspendisse velit nisl sit, potenti tempus. Conubia aenean nulla diam consequat donec laoreet, "
    "elit quis nulla amet enim ridiculus quis, dis sit auctor eget justo. Ligula a, vel modi vel "
    "dictumst wisi, natus at. Mollis elementum ante aliquam, dapibus tempus suspendisse volutpat, "
    "proin ornare vel morbi ligula morbi enim, sed posuere nostra sociosqu vestibulum bibendum "
    "vulputate, hymenaeos vitae phasellus vestibulum leo. Ipsum est, montes tincidunt nam quis "
    "nibh, nam vel magna dolor aliquam, eleifend praesent. Pharetra ac quis, justo scelerisque "
    "sed cras similique porttitor, consequat mauris aenean at lectus, vitae duis eros metus "
    "eleifend volutpat. Purus eu elit, sit eget lectus a. Vitae inceptos platea. Elit tempus, "
    "erat nulla dignissim elit id molestie ullamcorper, lacus placerat orci, eros wisi velit mi. "
    "Amet viverra in nullam non ullamcorper, lorem pellentesque ante curabitur, elit sed elit lacus "
    "t congue, elit tristique sollicitudin turpis montes.\n";

static std::string context(const std::string& text, int index, int length) {
    std::size_t start = std::max(0, index - 3);
    std::size_t end = std::min(index + length + 3, (int)text.size());

    return text.substr(start, end - start);
}

static void print_vector(const std::vector<std::size_t>& vec, std::string text, std::string pattern) {
    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::cout << "Match " << i << " at " << vec[i] << ": ";
        std::cout << context(text, vec[i], pattern.size()) << std::endl;
    }
}

static std::vector<std::pair<std::string, std::string>> search_tests({
    {"abbabab", "abaababbababbabab"},
    {"ro", "O rato roeu a rolha da garrafa do rei da Russia"},
    {"example", "Here is a simple example"},
    {"te", "Participate in parachute"},
    {"sed", loremipsum},
});

static std::vector<std::pair<std::string, std::string>> distance_tests({
    {"abbabab", "baababab"},
    {"mom", "dad"},
    {"dogs", "dog"},
    {"true", "true"},
    {"true", "false"},
    {"abc", "ca"},
    {"tee", "tree"},
    {"email", "mails"},
    {"profit", "gifts"},
    {"commodo", "codmomo"},
    {"commodo", "commdABoo"},
    {"elementum", "fermentum"},
    {"sitting", "kitten"},
    {"sunday", "saturday"},
});

static std::vector<std::pair<std::string, std::string>> fuzzy_tests({
    {"sitting", "kitten"},
    {"sunday", "saturday"},
    {"commodo", "codmomo"},
    {"commodo", "commdABoo"},
    {"elementum", "Mollis fermentum ante aliquam, dapibus tempus suspendisse"},
});

int test() {
    std::string text, pattern;

    std::cout << " **** Naive Search ****" << std::endl;
    for (std::size_t i = 0; i < search_tests.size(); ++i) {
        std::string pattern = search_tests[i].first;
        std::string text = search_tests[i].second;

        // Boyer Moore
        std::cout << "** Test " << i << " pattern: " << pattern << std::endl;
        print_vector(naive_search(text, pattern), text, pattern);
        std::cout << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Boyer Moore ****" << std::endl;
    for (std::size_t i = 0; i < search_tests.size(); ++i) {
        std::string pattern = search_tests[i].first;
        std::string text = search_tests[i].second;

        // Boyer Moore
        std::cout << "** Test " << i << " pattern: " << pattern << std::endl;
        auto bm = boyer_moore<bad_char_map_rule>(pattern);
        print_vector(boyer_moore_search(text, bm), text, pattern);
        std::cout << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Boyer Moore Galil ****" << std::endl;
    for (std::size_t i = 0; i < search_tests.size(); ++i) {
        std::string pattern = search_tests[i].first;
        std::string text = search_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " pattern: " << pattern << std::endl;
        auto bm = boyer_moore<bad_char_map_rule>(pattern);
        print_vector(boyer_moore_galil_search(text, bm), text, pattern);
        std::cout << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Knuth Morris Pratt ****" << std::endl;
    for (std::size_t i = 0; i < search_tests.size(); ++i) {
        std::string pattern = search_tests[i].first;
        std::string text = search_tests[i].second;

        // Knuth Morris Pratt
        std::cout << "** Test " << i << " pattern: " << pattern << std::endl;
        auto kmp = knuth_morris_pratt(pattern);
        print_vector(knuth_morris_pratt_search(text, kmp), text, pattern);
        std::cout << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << std::endl << std::endl << std::endl;





    std::cout << " **** Levenshtein distance ****" << std::endl;
    for (std::size_t i = 0; i < distance_tests.size(); ++i) {
        std::string a = distance_tests[i].first;
        std::string b = distance_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " a: " << a << " | b: " << b << std::endl;
        std::cout << "  Distance: " << levenshtein_distance(a, b) << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Restricted-Damerau distance ****" << std::endl;
    for (std::size_t i = 0; i < distance_tests.size(); ++i) {
        std::string a = distance_tests[i].first;
        std::string b = distance_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " a: " << a << " | b: " << b << std::endl;
        std::cout << "  Distance: " << restricted_damerau_distance(a, b) << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Damerau distance ****" << std::endl;
    for (std::size_t i = 0; i < distance_tests.size(); ++i) {
        std::string a = distance_tests[i].first;
        std::string b = distance_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " a: " << a << " | b: " << b << std::endl;
        std::cout << "  Distance: " << damerau_distance(a, b) << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << std::endl << std::endl;





    std::cout << " **** Levenshtein fuzzy ****" << std::endl;
    for (std::size_t i = 0; i < fuzzy_tests.size(); ++i) {
        std::string pattern = fuzzy_tests[i].first;
        std::string text = fuzzy_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " text: " << text << " | pattern: " << pattern << std::endl;
        std::cout << "  Fuzzy: " << levenshtein_fuzzy(text, pattern) << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Restricted-Damerau fuzzy ****" << std::endl;
    for (std::size_t i = 0; i < fuzzy_tests.size(); ++i) {
        std::string pattern = fuzzy_tests[i].first;
        std::string text = fuzzy_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " text: " << text << " | pattern: " << pattern << std::endl;
        std::cout << "  Fuzzy: " << restricted_damerau_fuzzy(text, pattern) << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << " **** Damerau fuzzy ****" << std::endl;
    for (std::size_t i = 0; i < fuzzy_tests.size(); ++i) {
        std::string pattern = fuzzy_tests[i].first;
        std::string text = fuzzy_tests[i].second;

        // Boyer Moore Galil
        std::cout << "** Test " << i << " text: " << text << " | pattern: " << pattern << std::endl;
        std::cout << "  Fuzzy: " << damerau_fuzzy(text, pattern) << std::endl;
    }

    std::getline(std::cin, text);

    std::cout << std::endl << std::endl;


    return 0;
}
