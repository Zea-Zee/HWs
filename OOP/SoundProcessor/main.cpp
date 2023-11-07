#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>

#define BUFFER_SIZE 4096

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
public:
    WavFile(const char *path) {
        wavFile.open(path, std::ios::binary);
        cout << "BEBRA";
        if (not wavFile) {
            IOFileError err(1, ("Unable to open WAV file: " + string(path)).c_str());
        }

        header = (wav_hdr*) calloc(1, sizeof(wav_hdr));

        wavFile.read(reinterpret_cast<char*>(header), sizeof(*header));
        std::cout << "Header Read " << wavFile.gcount() << " bytes." << std::endl;
        printHeader();
//
        if (wavFile.gcount() > 0){
            if(header->AudioFormat != 1) WAVFormatError err(3, "Audio format must be PMC (without encoding) (field must have val 1)");
            if(header->NumOfChan != 1) WAVFormatError err(4, "WAV for this program must have one channel (Mono) (field must have val 1)");
            if(header->bitsPerSample != 16) WAVFormatError err(5, "WAV for this program must have 16 bitness (field must have val 16)");
            if(header->SamplesPerSec != 44100) WAVFormatError err(6, "WAV for this program can only have 44100 sampling rate (field must have val 44100)");

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
            const int muteDurationInSeconds = 60;
            ofstream outputFile("ex1o.wav", std::ios::binary);

            const uint32_t bytesToMute = header->SamplesPerSec * header->NumOfChan * (header->bitsPerSample / 8) * muteDurationInSeconds;
            const uint32_t bufferSize = 1024; // Adjust buffer size as needed
            char silenceBuffer[bufferSize] = {0};

            // Mute the first minute by writing silence to the output file
            for (uint32_t i = 0; i < bytesToMute; i += bufferSize) {
                const uint32_t bytesToWrite = std::min(bufferSize, bytesToMute - i);
                outputFile.write(silenceBuffer, bytesToWrite);
            }

            // Copy the remaining audio data
            outputFile << wavFile.rdbuf();

            wavFile.close();
            outputFile.close();

            std::cout << "Muted the first " << muteDurationInSeconds << " seconds and saved to " << std::endl;


        }else{
            WAVFormatError err(2, "Empty WAV header, try again with another file or correct this one.");
//            perror("Empty wav header, try again with another file");
//            exit(2);
        }
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
        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

        cout << "Block align                :" << header->blockAlign << endl;
        cout << "Data string                :" << header->Subchunk2ID[0] << header->Subchunk2ID[1] << header->Subchunk2ID[2] << header->Subchunk2ID[3] << endl;
    }
};



//class Converter{
//private:
//    unsigned int start_time;
//    unsigned int end_time;
//public:
//    Converter(unsigned int start, unsigned int end):start_time{start}, end_time{end}{}
//};
//
//class WAVMuter : public Converter{
//};
//
//class WAVMixer : public Converter{
//};
//
//class  WAVAccelerator: public Converter{
//};
//
//class ConverterFactory {
//public:
//    virtual Converter *createConverter(){};
//    virtual ~ConverterFactory(){};
//};
//
//class WAVMuterFactory : public ConverterFactory{
//public:
//    Converter *createConverter() override{
//        return new WAVMuter();
//    }
//};
//
//class WAVAcceleratorFactory : public ConverterFactory{
//    Converter *createConverter() override{
//        return new WAVAccelerator();
//    }
//};
//
//class WAVMixerFactory : public ConverterFactory{
//    Converter *createConverter() override{
//        return new WAVMixer();
//    }
//};
class WAVConverter {
private:
    unsigned int start_time;
    unsigned int end_time;
    const char *path;
    unsigned int header_length;
public:
    WAVConverter(unsigned int start, unsigned int end, const char *p, unsigned int header_len) : start_time{start}, end_time{end}, path{p}, header_length{header_len}{
        convert();
    }
    virtual void convert() {};
    virtual ~WAVConverter() {}
};

class WAVMuter : public WAVConverter {
public:
    WAVMuter(unsigned int start, unsigned int end, const char *p, unsigned int header_len) : WAVConverter(start, end, p, header_len){};
    void convert() override{
        std::cout << "Converting WAV to MP3" << std::endl;
    }
};

class WAVMixer : public WAVConverter {
public:
    WAVMixer(unsigned int start, unsigned int end, const char *p, unsigned int header_len) : WAVConverter(start, end, p, header_len){};
    void convert() override{
        std::cout << "Converting WAV to MP3" << std::endl;
    }
};

class WAVAccelerator : public WAVConverter {
public:
    WAVAccelerator(unsigned int start, unsigned int end, const char *p, unsigned int header_len) : WAVConverter(start, end, p, header_len){};
    void convert() override{
        std::cout << "Converting WAV to MP3" << std::endl;
    }
};

class WAVConverterFactory {
public:
    static WAVConverter* createConverter(const std::string& type, unsigned int start, unsigned int end, const char *p, unsigned int header_len) {
        if (type == "Mute") {
            return new WAVMuter(start, end, p, header_len);
        } else if (type == "Mix") {
            return new WAVMixer(start, end, p, header_len);
        } else if (type == "Accelerate") {
            return new WAVAccelerator(start, end, p, header_len);
        } else {
            return nullptr; // Invalid type
        }
    }
};

int main() {
    // Usage example
    std::string conversionType = "MP3";
    std::string inputFile = "input.wav";
    std::string outputFile = "output.mp3";

    WAVConverter* converter = WAVConverterFactory::createConverter("Mute", 0, 5, "bebra", 44);

//    if (converter) {
//        converter->convert(inputFile, outputFile);
//        delete converter;
//    } else {
//        std::cout << "Invalid conversion type." << std::endl;
//    }

    return 0;
}