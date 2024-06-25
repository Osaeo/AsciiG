#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

struct pixell{
    int red = 0;
    int blue = 0;
    int green = 0;
};

void printHelp(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <image_path>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h            Show this help message" << std::endl;
    std::cout << "  -v            Show the program version" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  " << programName << " -v" << std::endl;
    std::cout << "  " << programName << " image.jpg" << std::endl;
}

void printVersion() {
    std::cout << "Program version: 0.0" << std::endl;
}

int rgbToGrey(int r, int g, int b) {
    // Étape 1: Conversion en luminance
    double luminance = 0.299 * r + 0.587 * g + 0.114 * b;

    // Étape 2: Conversion en échelle de 0 à 19
    int greyscale_value = round((luminance / 255.0) * 18);

    return greyscale_value;
}

char tr(int nb) {
    switch(nb){
        case 0:
            return ' ';
        case 1:
            return '.';
        case 2:
            return '\'';
        case 3:
            return ',';
        case 4:
            return ';';
        case 5:
            return ':';
        case 6:
            return 'c';
        case 7:
            return 'l';
        case 8:
            return 'o';
        case 9:
            return 'd';
        case 10:
            return 'x';
        case 11:
            return 'k';
        case 12:
            return 'O';
        case 13:
            return '0';
        case 14:
            return 'K';
        case 15:
            return 'X';
        case 16:
            return 'N';
        case 17:
            return 'W';
        case 18:
            return 'M';

    }
}


int main(int argc, char** argv) {

    //taille de la sortie
    int widht_out = -1;
    int height_out = -1;
    int widht_pict = 0;
    int height_pict = 0;
    bool height_error = false;

    vector<vector<int> > image_grise;

    //verifier qu'il y a des argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [options] <image_path>" << std::endl;
        std::cout << "'-h' to see help\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h") {
            printHelp(argv[0]);
            return 0;
        } else if (arg == "-v") {
            printVersion();
            return 0;
        }
        else if (arg == "--widht") {
            widht_out = std::stoi(argv[i + 1]);
        }
        else if (arg =="--height") {
            height_out = std::stoi(argv[i + 1]);
        }
    }

    //verifier que la taille de sortie est conforme
    if (height_out <= 0) {
        std::cout << "the height does not have a correct entry\n";
        height_error = true;
    }
    if (widht_out <= 0) {
        std::cout << "the widht does not have a correct entry\n";
        height_error = true;
    }
    if (height_error == true) {
        return -1;
    }

    std::string imagePath = argv[argc - 1];
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    // Lire l'image
    if (image.empty()) {
        std::cerr << "Could not open or find the image" << std::endl;
        return -1;
    }

    //entrer les valeurs de la taille de l'image de base
    widht_pict = image.cols;
    height_pict = image.rows;

    // initialiser correctement l'image grise(le tableau)
    for(int x = 0; x < widht_pict ; x++){
        image_grise.push_back(vector<int>(height_pict));
    }

    //mettre l'image dedans
    for(int x = 0; x < widht_pict ; x++) {
        for(int y = 0; y < height_pict; y++) {
            // Obtenir la couleur du pixel
            cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
            image_grise[x][y] = rgbToGrey(color[2]/*rouge*/, color[1]/*vert*/, color[0]/*bleu*/);
        }
    }

    //le nombre de pixel par charactere
    int wnbch = widht_pict / widht_out;
    int hnbch = height_pict / height_out;



    cout << "widht : " << widht_pict / (widht_pict / widht_out) << endl;
    cout << "height : " << height_pict / (height_pict / height_out) << endl;

    vector<vector<int> > finish;
    // initialiser correctement le finish(le tableau)
    for(int x = 0; x < widht_out ; x++){
        finish.push_back(vector<int>(height_out));
    }



    return 0;
}
