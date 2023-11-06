#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <cstdint>

#define BUFFER_SIZE 4096

using namespace std;

class MyError{
private:
    const char *errorMessage;
    const char errorCode;
public:
    MyError(const char code, const char *msg):errorMessage{msg},errorCode{code}{}
};



typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;



class WavFile {
private:
    wav_hdr *header;
    FILE *wavFile;
    long long fileSize;
public:
    WavFile(const char *path) {
        this->wavFile = fopen(path, "r");
        if (this->wavFile == nullptr) {
            fprintf(stderr, "Unable to open wave file: %s\n", path);
            exit(1);
        }

        this->header = (wav_hdr *) calloc(1, sizeof(wav_hdr));
        size_t bytesRead = fread(this->header, 1, sizeof(*this->header), this->wavFile);
        cout << "Header Read " << bytesRead << " bytes." << endl;

        if (bytesRead > 0){
            if(header->AudioFormat != 1) InputWAVError(3, "Audio format must be PMC (without encoding) (field must have val 1)");
            if(header->NumOfChan != 1) InputWAVError(4, "WAV for this program must have one channel (Mono) (field must have val 1)");
            if(header->bitsPerSample != 16) InputWAVError(5, "WAV for this program must have 16 bitness (field must have val 16)");
            if(header->SamplesPerSec != 44100) InputWAVError(6, "WAV for this program can only have 44100 sampling rate (field must have val 44100)");

            fseek(this->wavFile, 0, SEEK_END);
            this->fileSize = ftell(this->wavFile);
            fseek(this->wavFile, bytesRead, SEEK_SET);

            //Read the data
            uint16_t bytesPerSample = this->header->bitsPerSample / 8;      //Number     of bytes per sample
            uint64_t numSamples = this->header->ChunkSize / bytesPerSample; //How many samples are in the wav file?
            int8_t* buffer = new int8_t[BUFFER_SIZE];
            while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0){
                /** DO SOMETHING WITH THE WAVE DATA HERE **/
                cout << "Read " << bytesRead << " bytes." << endl;
            }
            delete [] buffer;
            buffer = nullptr;
        }else{
            perror("Empty wav header, try again with another file");
            exit(2);
        }
        printHeader();
    }
    void printHeader(){
        cout << "File is                    :" << fileSize << " bytes." << endl;
        cout << "RIFF header                :" << header->RIFF[0] << header->RIFF[1] << header->RIFF[2] << header->RIFF[3] << endl;
        cout << "WAVE header                :" << header->WAVE[0] << header->WAVE[1] << header->WAVE[2] << header->WAVE[3] << endl;
        cout << "FMT                        :" << header->fmt[0] << header->fmt[1] << header->fmt[2] << header->fmt[3] << endl;
        cout << "Data size                  :" << header->ChunkSize << endl;

        // Display the sampling Rate from the header
        cout << "Sampling Rate              :" << header->SamplesPerSec << endl;
        cout << "Number of bits used        :" << header->bitsPerSample << endl;
        cout << "Number of channels         :" << header->NumOfChan << endl;
        cout << "Number of bytes per second :" << header->bytesPerSec << endl;
        cout << "Data length                :" << header->Subchunk2Size << endl;
        cout << "Audio Format               :" << header->AudioFormat << endl;
        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

        cout << "Block align                :" << header->blockAlign << endl;
        cout << "Data string                :" << header->Subchunk2ID[0] << header->Subchunk2ID[1] << header->Subchunk2ID[2] << header->Subchunk2ID[3] << endl;
    }
};


int main(int argc, char* argv[]){

    const char* filePath;
    string input;
    if (argc <= 1) filePath = "ex1.wav";
    else
    {
        filePath = argv[1];
        cout << "Input wave file name: " << filePath << endl;
    }
    WavFile a(filePath);

    //Read the header

//    fclose(wavFile);
//    return 0;
}