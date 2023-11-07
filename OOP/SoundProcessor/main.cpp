#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <cstring>

#define BUFFER_SIZE 1024

using namespace std;


class BaseError{
private:
    const string errorMessage;
    const char errorCode;
public:
    BaseError(const char code, const char *msg):errorMessage{msg},errorCode{code}{
//        cout << "Undetermined error with error code:" + to_string(errorCode) + " and message:" + errorMessage + "\n";
        perror((" " + errorMessage + " code " + to_string(errorCode) + "\n").c_str());
    }
};

class IOFileError : public BaseError{
public:
    IOFileError(const char code, const char *msg) : BaseError(code, msg){
        exit(code);
    }
};

class WAVFormatError : public BaseError{
public:
    WAVFormatError(const char code, const char *msg) : BaseError(code, msg){
        exit(code);
    }
};

class MyWarning : public BaseError{
public:
    MyWarning(const char code, const char *msg) : BaseError(code, msg){}
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
    ifstream wavFile;
    char *path;
public:
    WavFile(char *path) : path{path}{
        wavFile.open(path, std::ios::binary);
        cout << "BEBRA";
        if (not wavFile) {
            IOFileError err(1, ("Unable to open WAV file: " + string(path)).c_str());
        }

        header = (wav_hdr*) calloc(1, sizeof(wav_hdr));

        wavFile.read(reinterpret_cast<char*>(header), sizeof(*header));
        std::cout << "Header Read " << wavFile.gcount() << " bytes." << std::endl;
        printHeader();
        cout << "Subchunk2Size is " << header->Subchunk2Size << endl;
        if(header->Subchunk2Size <= header->ChunkSize / 2) header->Subchunk2Size = header->ChunkSize - 36;
        cout << "Subchunk2Size is " << header->Subchunk2Size << endl;
        if (wavFile.gcount() > 0){
            if(header->AudioFormat != 1) WAVFormatError err(3, "Audio format must be PMC (without encoding) (field must have val 1)");
            if(header->NumOfChan != 1) WAVFormatError err(4, "WAV for this program must have one channel (Mono) (field must have val 1)");
            if(header->bitsPerSample != 16) WAVFormatError err(5, "WAV for this program must have 16 bitness (field must have val 16)");
            if(header->SamplesPerSec != 44100) WAVFormatError err(6, "WAV for this program can only have 44100 sampling rate (field must have val 44100)");
        wavFile.close();
//            uint16_t bytesPerSample = header->bitsPerSample / 8; // Number of bytes per sample
//            uint32_t numSamples = header->Subchunk2Size / bytesPerSample;
//            std::cout << numSamples << " samples in WAV file" << std::endl;
//            for (uint32_t i = 0; i < numSamples; ++i) {
//                int16_t sample;
//                wavFile.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
//                if (!wavFile) {
//                    std::cerr << "Error reading audio sample." << std::endl;
//                    exit(100);
//                }
//                std::cout << "Sample " << i << ": " << sample << std::endl;
//            }


        }else{
            WAVFormatError err(2, "Empty WAV header, try again with another file or correct this one.");
//            perror("Empty wav header, try again with another file");
//            exit(2);
        }
    }
    WAV_HEADER * getHeader(){
        WAV_HEADER *wav_header_copy = this->header;
        return wav_header_copy;
    }
    char *getPath(){
        char *cpy = (char*) calloc(strlen(path) + 1, 1);
        strcpy(cpy, path);
        cout << strlen(path) << ":" << cpy << ":" << path << endl;
        return cpy;
    }
    void printHeader(){
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
        cout << "ChunkSize1                 :" << header->Subchunk1Size << endl;
        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM


        cout << "Block align                :" << header->blockAlign << endl;
        cout << "Data string                :" << header->Subchunk2ID[0] << header->Subchunk2ID[1] << header->Subchunk2ID[2] << header->Subchunk2ID[3] << endl;
    }
};



class WAVConverter {
protected:
    unsigned int start_time;
    unsigned int end_time;
    char *path;
    WavFile *wav;
public:
    WAVConverter(unsigned int start, unsigned int end, char *p, WavFile *wav) : start_time{start}, end_time{end}, path{p}, wav{wav}{
        convert();
    }
    virtual void convert() {};
    virtual ~WAVConverter() {}
};

class WAVMuter : public WAVConverter {
public:
    WAVMuter(unsigned int start, unsigned int end, char *p, WavFile *wav) : WAVConverter(start, end, p, wav){};
    void convert() override{
        if(start_time > end_time) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        ifstream input(wav->getPath(), std::ios::binary);
        ofstream output("tmp.wav", std::ios::binary);
        cout << *wav->getPath() << endl;

        // Calculate the duration of the audio in seconds
//        double duration = static_cast<double>(wav->getHeader()->Subchunk2Size) / wav->getHeader()->bytesPerSec;
        double duration = static_cast<double>(wav->getHeader()->Subchunk2Size) / (wav->getHeader()->bitsPerSample / 8) / wav->getHeader()->NumOfChan / wav->getHeader()->SamplesPerSec;

        if (!output) {
            std::cerr << "Error: Unable to open output file " << std::endl;
            exit(127);
        }

        if (!input) {
            std::cerr << "Error: Unable to open input file " << std::endl;
            exit(127);
        }

        // Write the WAV header to the output file
        input.read(reinterpret_cast<char*>(wav->getHeader()), sizeof(*wav->getHeader()));
        output.write(reinterpret_cast<char*>(wav->getHeader()), sizeof(*wav->getHeader()));

        uint32_t startSample = static_cast<uint32_t>(start_time * wav->getHeader()->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * wav->getHeader()->SamplesPerSec);
//        cout << "startSample " << startSample << "\n";
//        cout << "endSample " << endSample << "\n";

        // Mute the specified range
        char buffer[BUFFER_SIZE];
        double currentTime = 0.0;
//        int numOfSamples = wav->getHeader()->Subchunk2Size / (wav->getHeader()->bitsPerSample / 8) / wav->getHeader()->NumOfChan;
        int numOfSamples = (int) wav->getHeader()->Subchunk1Size / 2;
//        int numOfSamples = (int) wav->getHeader()->Subchunk2Size / 2;
        cout << "Number of samples is: " << numOfSamples << "\n";

        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample;
            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));

            if (sampleIndex >= startSample && sampleIndex < endSample) {
                sample = 0;  // Set the sample to zero to mute it
            }

            // Write the modified sample to the output file
            output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
//            cout << "Sample've read with idx: " << sampleIndex << " " << "sample is " << sample << "\n";
        }
        int n = 0;
//        while (!input.eof()) {
//            n++;
//            int16_t sample;
//            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
//            output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
//        }
        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "n is " << n << "\n";
        input.close();
        output.close();

        std::cout << "Muted audio from " << start_time << " to " << end_time << " seconds." << std::endl;
    }
};

class WAVMixer : public WAVConverter {
public:
    WAVMixer(unsigned int start, unsigned int end, char *p, WavFile *wav) : WAVConverter(start, end, p, wav){};
    void convert() override{
        std::cout << "Converting WAV to MP3" << std::endl;
    }
};

class WAVAccelerator : public WAVConverter {
public:
    WAVAccelerator(unsigned int start, unsigned int end, char *p, WavFile *wav) : WAVConverter(start, end, p, wav){};
    void convert() override{
        std::cout << "Converting WAV to MP3" << std::endl;
    }
};

class WAVConverterFactory {
public:
    static WAVConverter* createConverter(const std::string& type, unsigned int start, unsigned int end, char *p, WavFile *wav) {
        if (type == "Mute") {
            return new WAVMuter(start, end, p, wav);
        } else if (type == "Mix") {
            return new WAVMixer(start, end, p, wav);
        } else if (type == "Accelerate") {
            return new WAVAccelerator(start, end, p, wav);
        } else {
            return nullptr; // Invalid type
        }
    }
};

int main() {
    WavFile input("./ex1.wav");
//    WavFile tmp("./ex1.wav");
    WAVConverter* converter = WAVConverterFactory::createConverter("Mute", 10, 30, "bebra", &input);
    converter->convert();

//    if (converter) {
//        converter->convert(inputFile, outputFile);
//        delete converter;
//    } else {
//        std::cout << "Invalid conversion type." << std::endl;
//    }

    return 0;
}