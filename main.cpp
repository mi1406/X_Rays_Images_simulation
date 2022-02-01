#include <iostream>
#include "lib/atomicData.h"
#include "lib/MedImage.h"
#include "lib/MedImageIO.h"
#include "simulation.h"
#include "lib/random.h"

using namespace std;

void testSimulation();
void testMedImage();
void testRandom();
int main()
{

    testSimulation();
    try{
        testRandom();
    }catch(exception &e){
        cout << e.what() << endl;
    }

    return 0;
}

void testSimulation()
{
    cSimulation cs;
  //  cs.prepare();
    cs.setCurrentTimeProduct(1);
    cs.setPixelSize(150e-6);
    cs.setTubeVoltage(40);
    cs.setTubePos(0,0,1.2);
    cs.setSpherePos(0,0,0.2);
    cs.setSphereRadius(2.5e-3);
    cMedImage<double> cmi;
    cmi.create(50,50);
    cs.prepare();
    cs.simulate(cmi);
    cmi.autoWindowing();
    cout << "pixel black: " << cmi.pixelValueForBlack << endl;
    cout << "pixel white: " << cmi.pixelValueForWhite << endl;
    cMedImageIO<double> mio;
    mio.writeBmp(cmi,"/home/m/qtWorkSpace/xray_sim_simulation/test.bmp");

}

void testMedImage(){
    cMedImage<double> image;
    unsigned size = 99 ;
    image.create(size, size, 0);

    for(unsigned i = 0; i < size; i++){
        image[i][i] = image[i][size - i - 1] = 1;
    }

    image.pixelValueForBlack = -1;
    image.pixelValueForWhite = 2;

    cMedImageIO<double>::writeBmp(image, "deleteMe.bmp");
}

void testRandom(){
    cRandKiss kiss((uint32_t)time(NULL));
    cRandNormal normal(kiss);
    normal.prepare();
    for(int fewTimes = 0; fewTimes < 5; fewTimes++)
        cout<< normal.rand() << endl;
    unsigned data[100] = {0};
    for(unsigned i = 0; i < 1e6; i++)
        data[(unsigned)(100 * (normal.rand() + 5)/10)]++;
    ofstream outfile ("/home/m/qtWorkSpace/xray_sim_simulation/data.csv");
    for(unsigned i = 0 ; i < 100; i++)
        outfile << data[i] << ",";
    outfile.close();


}
