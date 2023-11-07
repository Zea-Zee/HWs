#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <cstring>

#define BUFFER_SIZE 1024

using namespace std;

bool tmpFlag = false;


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
//        cout << "BEBRA";
        if (not wavFile) {
            IOFileError err(1, ("Unable to open WAV file: " + string(path)).c_str());
        }

        header = (wav_hdr*) calloc(1, sizeof(wav_hdr));

        wavFile.read(reinterpret_cast<char*>(header), sizeof(*header));
        std::cout << "Header Read " << wavFile.gcount() << " bytes from " << path << "\n";
        printHeader();
//        cout << "Subchunk2Size is " << header->Subchunk2Size << endl;
//        if(header->Subchunk2Size <= header->ChunkSize / 2) header->Subchunk2Size = header->ChunkSize - 36;
//        cout << "Subchunk2Size is " << header->Subchunk2Size << endl;
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
    WAV_HEADER * getHeader(){       //It's genius give a pointer to private field :)
        WAV_HEADER *wav_header_copy = this->header;
        return wav_header_copy;
    }
//    void setHeader(WAV_HEADER *newHeader){
//        free(this->header);
//        this->header = newHeader;
//    }
    char *getPath(){
        char *cpy = (char*) calloc(strlen(path) + 1, 1);
        strcpy(cpy, path);
//        cout << strlen(path) << ":" << cpy << ":" << path << endl;
        return cpy;
    }
    void dump(const char *p){
        int numOfSamples = header->Subchunk2Size / (header->bitsPerSample / 8) / header->NumOfChan;

        ifstream input(this->path, std::ios::binary);
        ofstream output(p, std::ios::binary);

        input.read(reinterpret_cast<char*>(header), sizeof(*header));
        output.write(reinterpret_cast<char*>(header), sizeof(*header));

        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample;
            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
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
    WavFile *wav1;
public:
    WAVConverter(unsigned int start, unsigned int end, WavFile *wav) : start_time{start}, end_time{end}, wav1{wav}{}
    virtual void convert() {};
    virtual ~WAVConverter() {}
};

class WAVMuter : public WAVConverter {
public:
    WAVMuter(unsigned int start, unsigned int end, WavFile *wav) : WAVConverter(start, end, wav){
        convert();
    };
    void convert() override{
        if(start_time > end_time) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        ifstream input(wav1->getPath(), std::ios::binary);
        ofstream output(tmpFlag ? "cache.wav" : "tmp.wav", std::ios::binary);
//        cout << *wav->getPath() << endl;

        // Calculate the duration of the audio in seconds
//        double duration = static_cast<double>(wav->getHeader()->Subchunk2Size) / wav->getHeader()->bytesPerSec;
        int duration = (int) wav1->getHeader()->Subchunk2Size / (wav1->getHeader()->bitsPerSample / 8) / wav1->getHeader()->NumOfChan / wav1->getHeader()->SamplesPerSec;
        cout << wav1->getPath() << " duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";

        if (!output) {
            std::cerr << "Error: Unable to open output file " << std::endl;
            exit(127);
        }

        if (!input) {
            std::cerr << "Error: Unable to open input file " << std::endl;
            exit(127);
        }

        cout << "Start muting " << wav1->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";

        // Write the WAV header to the output file
        input.read(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));
        output.write(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));

        uint32_t startSample = static_cast<uint32_t>(start_time * wav1->getHeader()->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * wav1->getHeader()->SamplesPerSec);
//        cout << "startSample " << startSample << "\n";
//        cout << "endSample " << endSample << "\n";

        // Mute the specified range
        char buffer[BUFFER_SIZE];
        double currentTime = 0.0;
        int numOfSamples = wav1->getHeader()->Subchunk2Size / (wav1->getHeader()->bitsPerSample / 8) / wav1->getHeader()->NumOfChan;
//        int numOfSamples = (int) wav->getHeader()->Subchunk2Size / 2;
//        cout << "Number of samples is: " << numOfSamples << "\n";

        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample;
            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            if (sampleIndex >= startSample && sampleIndex < endSample) sample = 0;  // Set the sample to zero to mute it
            output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
//            cout << "Sample've read with idx: " << sampleIndex << " " << "sample is " << sample << "\n";
        }
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "Muted successfully.\n";
        cout << "Muted successfully.\n";
        tmpFlag = !tmpFlag;
        input.close();
        output.close();
    }
};

class WAVMixer : public WAVConverter {
private:
    WavFile *wav2;
public:
    WAVMixer(unsigned int start, unsigned int end, WavFile *wav1, WavFile *wav2) : WAVConverter(start, end, wav1){
        this->wav2 = wav2;
        convert();
    };
    void convert() override{
        if(start_time > end_time) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        ifstream input1(wav1->getPath(), std::ios::binary);
        ifstream input2(wav2->getPath(), std::ios::binary);
        ofstream output(tmpFlag ? "cache.wav" : "tmp.wav", std::ios::binary);

        int dur_1 = (int) wav1->getHeader()->Subchunk2Size / (wav1->getHeader()->bitsPerSample / 8) / wav1->getHeader()->NumOfChan / wav1->getHeader()->SamplesPerSec;
        int dur_2 = (int) wav2->getHeader()->Subchunk2Size / (wav2->getHeader()->bitsPerSample / 8) / wav2->getHeader()->NumOfChan / wav2->getHeader()->SamplesPerSec;
        cout << wav1->getPath() << " duration is " << dur_1 << " seconds or " << (int) dur_1 / 60 << " m " << (int) dur_1 % 60 << " s.\n";
        cout << wav2->getPath() << " duration is " << dur_2 << " seconds or " << (int) dur_2 / 60 << " m " << (int) dur_2 % 60 << " s.\n";

        if (!output) {
            std::cerr << "Error: Unable to open output file " << std::endl;
            exit(127);
        }

        if (!input1 or not input2) {
            std::cerr << "Error: Unable to open input file " << std::endl;
            exit(127);
        }

        cout << "Start mixing " << wav1->getPath() << " and " << wav2->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";
        uint32_t startSample, endSample;
        int numOfSamples;
//        if(dur_1 > dur_2){
            output.write(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));
            startSample = static_cast<uint32_t>(start_time * wav1->getHeader()->SamplesPerSec);
            endSample = static_cast<uint32_t>(end_time * wav1->getHeader()->SamplesPerSec);
            numOfSamples = static_cast<uint32_t>(dur_1 * wav1->getHeader()->SamplesPerSec);
//        }
//        else{
//            output.write(reinterpret_cast<char*>(wav2->getHeader()), sizeof(*wav2->getHeader()));
//            startSample = static_cast<uint32_t>(start_time * wav2->getHeader()->SamplesPerSec);
//            endSample = static_cast<uint32_t>(end_time * wav2->getHeader()->SamplesPerSec);
//            numOfSamples = (int) wav2->getHeader()->Subchunk1Size / 2;
//        }

//        cout << "startSample " << startSample << "\n";
//        cout << "endSample " << endSample << "\n";

        char buffer[BUFFER_SIZE];
        cout << "Number of samples is: " << numOfSamples << "\n";

        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample1, sample2;
            input1.read(reinterpret_cast<char*>(&sample1), sizeof(int16_t));
            input2.read(reinterpret_cast<char*>(&sample2), sizeof(int16_t));
            if (sampleIndex >= startSample && sampleIndex < endSample) {
//                sample1 = (int16_t) sample1 / 2;
//                sample2 = (int16_t) sample1 / 2;
//                sample1 = (int16_t) sample1 + (int16_t) sample2;
                unsigned int sum = sample1 + sample2;
                sample1 = (int16_t) sum / 2;
            }
            output.write(reinterpret_cast<char*>(&sample1), sizeof(int16_t));
        }
        cout << "Mixed successfully.\n";
        tmpFlag = !tmpFlag;
        input1.close();
        input2.close();
        output.close();
    }
};

class WAVAccelerator : public WAVConverter {
private:
    double acceleration;
public:
    WAVAccelerator(unsigned int start, unsigned int end, WavFile *wav, double boost) : WAVConverter(start, end, wav){
        this->acceleration = boost;
        convert();
    };
    void convert() override{
        if(start_time > end_time) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        ifstream input(wav1->getPath(), std::ios::binary);
        ofstream output(tmpFlag ? "cache.wav" : "tmp.wav", std::ios::binary);

        int tot_duration = (int) wav1->getHeader()->Subchunk2Size / (wav1->getHeader()->bitsPerSample / 8) / wav1->getHeader()->NumOfChan / wav1->getHeader()->SamplesPerSec;
        cout << wav1->getPath() << " old duration is " << tot_duration << " seconds or " << (int) tot_duration / 60 << " m " << (int) tot_duration % 60 << " s.\n";
        int duration = start_time
                + tot_duration - end_time
                + (end_time - start_time) / acceleration;
        cout << wav1->getPath() << " new duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";

        if (!output) {
            std::cerr << "Error: Unable to open output file " << std::endl;
            exit(127);
        }

        if (!input) {
            std::cerr << "Error: Unable to open input file " << std::endl;
            exit(127);
        }

        cout << "Start accelerating " << wav1->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";

        input.read(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));
        wav1->getHeader()->Subchunk2Size = duration * (wav1->getHeader()->bitsPerSample / 8) * wav1->getHeader()->NumOfChan * wav1->getHeader()->SamplesPerSec;
        output.write(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));

        uint32_t startSample = static_cast<uint32_t>(start_time * wav1->getHeader()->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * wav1->getHeader()->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * wav1->getHeader()->SamplesPerSec);
        int numOfSamples = static_cast<uint32_t>(tot_duration * wav1->getHeader()->SamplesPerSec);
        unsigned long long skipSample = 0;

        for (uint32_t sampleIndex = 0; acceleration >= 1 and sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample;
            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            if (sampleIndex >= startSample && sampleIndex < endSample){
                if(skipSample % (int) acceleration == 0)
                    output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            } else output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
        }

        for (uint32_t sampleIndex = 0; acceleration < 1 and acceleration > 0 and sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample;
            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            if (sampleIndex >= startSample && sampleIndex < endSample){
                for(int j = 0; j < (int) 1 / acceleration; j++){
                    output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
                }
            } else output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
        }
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "Accelerated successfully.\n";
        tmpFlag = !tmpFlag;
        input.close();
        output.close();
    }
};

class WAVCutter : public WAVConverter {
public:
    WAVCutter(unsigned int start, unsigned int end, WavFile *wav) : WAVConverter(start, end, wav){
        convert();
    };
    void convert() override{
        if(start_time > end_time) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        ifstream input(wav1->getPath(), std::ios::binary);
        ofstream output(tmpFlag ? "cache.wav" : "tmp.wav", std::ios::binary);
//        cout << *wav->getPath() << endl;

        //127
        int duration = end_time - start_time;
        cout << "dur is " << duration << "\n";
        cout << wav1->getPath() << " duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";

        if (!output) {
            std::cerr << "Error: Unable to open output file " << std::endl;
            exit(127);
        }

        if (!input) {
            std::cerr << "Error: Unable to open input file " << std::endl;
            exit(127);
        }

        cout << "Start cutting " << wav1->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";

        // Write the WAV header to the output file
//        input.read(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));
        input.read(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));
        wav1->getHeader()->Subchunk2Size = duration * (wav1->getHeader()->bitsPerSample / 8) * wav1->getHeader()->NumOfChan * wav1->getHeader()->SamplesPerSec;
        output.write(reinterpret_cast<char*>(wav1->getHeader()), sizeof(*wav1->getHeader()));

        uint32_t startSample = static_cast<uint32_t>(start_time * (wav1->getHeader()->bitsPerSample / 8) * wav1->getHeader()->NumOfChan * wav1->getHeader()->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * (wav1->getHeader()->bitsPerSample / 8) * wav1->getHeader()->NumOfChan * wav1->getHeader()->SamplesPerSec);
        cout << "start samples is " << startSample << "end samples is " << endSample << "\n";

        for (uint32_t sampleIndex = 0; true; sampleIndex++) {
            if(sampleIndex >= endSample) break;
            int16_t sample;
            input.read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            if (sampleIndex >= startSample && sampleIndex < endSample) output.write(reinterpret_cast<char*>(&sample), sizeof(int16_t));
        }
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "Cutted successfully.\n";
        tmpFlag = !tmpFlag;
        input.close();
        output.close();
    }
};

class WAVConverterFactory {
public:
    static WAVConverter* createConverter(const std::string& type, unsigned int start, unsigned int end, WavFile *wav1, WavFile *wav2=nullptr, double boost=0) {
        if (type == "Mute") {
            return new WAVMuter(start, end, wav1);
        } else if (type == "Mix" and wav2 != nullptr) {
            return new WAVMixer(start, end, wav1, wav2);
        } else if (type == "Accelerate" and boost != 0) {
            return new WAVAccelerator(start, end, wav1, boost);
        } else if (type == "Cut") {
            return new WAVCutter(start, end, wav1);
        } else {
            return nullptr; // Invalid type
        }
    }
};

int main() {
    WavFile a("./ex11.wav");
    WavFile b("./ex22.wav");
//    WavFile tmp("./ex1.wav");
//    WAVConverter* converter = WAVConverterFactory::createConverter("Mute", 10, 30, &a);
//    WAVConverter* converter = WAVConverterFactory::createConverter("Mix", 0, 30, &a, &b);
//    WAVConverter* converter = WAVConverterFactory::createConverter("Cut", 0, 10, &a, &b);
    WAVConverter* converter = WAVConverterFactory::createConverter("Accelerate", 0, 411, &a, nullptr, 0.25);

    return 0;
}

// Добавить грамотную обработку ошибок
// Перенсти всю работу с файлами в WAV (открытие, сохранение) редактирование - создание нового WAV в конвертере и запись по нему
// Добавить тона
// Добавить склейку
// Добавить ввод с консоли