#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

/*struct pixell{
    int red = 0;
    int blue = 0;
    int green = 0;
};*/

void printHelp(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <image_path>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h              Show this help message" << std::endl;
    std::cout << "  -v              Show the program version" << std::endl;
    std::cout << "  --width         Set the width of the result (number of character)" << endl;
    std::cout << "  --height        Set the height of the result (number of character)" << endl;
    std::cout << "  --ratio         set the ratio manually if width or height is missing" << endl;
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
        default:
            return 'M';

    }
}


int main(int argc, char** argv) {

    //taille de la sortie
    double width_out = -1;
    double height_out = -1;
    double width_pict = 0;
    double height_pict = 0;
    bool width_error = false;
    bool height_error = false;
    bool usemodratio = true;
    double ratio = 3.3;

    vector<vector<double> > image_grise;

    //verifier qu'il y a des argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [options] <image_path>" << std::endl;
        std::cout << "'-h' to see help\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        }
        else if (arg == "--width") {
            width_out = std::stoi(argv[i + 1]);
            i++;
        }
        else if (arg =="--height") {
            height_out = std::stoi(argv[i + 1]);
            i++;
        }
        else if (arg == "--ratio") {
            ratio = std::stod(argv[i + 1]);
            i++;
            usemodratio = false;
        }
    }
    cout << "argument verifier\n";
    //verifier que la taille de sortie est conforme
    if (height_out <= 0) {
        std::cout << "the height does not have a correct entry\n";
        height_error = true;
    }
    if (width_out <= 0) {
        std::cout << "the width does not have a correct entry\n";
        width_error = true;
    }
    if (height_error == true && width_error == true) {
        
        return -1;
    }
    cout << "la taille de sortie est conforme\n";
    //desactiver la modification du ratio si toute les valeur sont deja entrée et conforme
    if(height_error == false && width_error == false) {
            usemodratio = false;
    }

    //recuperer l'image
    std::string imagePath = argv[argc - 1];
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    // Lire l'image
    if (image.empty()) {
        std::cerr << "Could not open or find the image" << std::endl;
        return -1;
    }
    cout << "image recuperer\n";
    //entrer les valeurs de la taille de l'image de base
    width_pict = image.cols;
    height_pict = image.rows;

    //modifier le ratio si rien ne l'en empeche
    if (usemodratio) {
        if(height_error) {
            int ratiowidth = width_pict / width_out;
            height_out = int((height_pict / ratiowidth) / ratio);
        }
        if(width_error) {
            int ratioheight = height_pict / height_out;
            width_out = int((width_pict / ratioheight) * ratio);
        }
    }
    cout << "ratio completer\n";
    // initialiser correctement l'image grise(le tableau)
    for(int x = 0; x < width_pict ; x++){
        image_grise.push_back(vector<double>(height_pict));
    }
    
    //mettre l'image dedans
    for(int x = 0; x < width_pict ; x++) {
        for(int y = 0; y < height_pict; y++) {
            // Obtenir la couleur du pixel
            cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
            image_grise[x][y] = rgbToGrey(color[2]/*rouge*/, color[1]/*vert*/, color[0]/*bleu*/);
        }
    }
    cout << "image en niveau de gris fais\n";
    //le nombre de pixel par charactere
    double wnbch = width_pict / width_out;
    double hnbch = height_pict / height_out;

    cout << "width : " << width_pict / width_out << endl;
    cout << "height : " << height_pict / height_out << endl;
    cout << width_out << "  " << height_out << endl;

    //tableau des moyennes
    vector<vector<double> > moy;
    for(int x = 0; x < width_out ; x++){
        moy.push_back(vector<double>(height_out));
    }
    cout << "test\n";
    for(int x = 0; x < width_out; x++){
        for(int y = 0; y < height_out; y++){
            int total = 0;

            for(int xs = x * wnbch; xs < (x * wnbch) + wnbch; xs++){
                for(int ys = y * hnbch; ys < (y * hnbch) + hnbch; ys++){
                    total += image_grise[xs][ys];
                }
            }
            moy[x][y] = total / ((wnbch * hnbch));
        }
    }
    cout << "tableau des moyennes faite\n";
    vector<vector<char> > finish;
    // initialiser correctement le finish(le tableau)
    for(int x = 0; x < width_out ; x++){
        finish.push_back(vector<char>(height_out));
    }
    
    for(int x = 0; x < width_out; x++){
        for(int y = 0; y < height_out; y ++){
            finish[x][y] = tr(moy[x][y]);
        }
    }
    cout << "tableau fini";
    for(int y = 0; y < height_out; y++){
        string result;
        for(int x = 0; x < width_out; x++){
            result.append(1, finish[x][y]);
        }
        cout << result << endl;
    }

    return 0;
}

