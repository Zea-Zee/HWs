#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <math.h>
#include <sstream>

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

class InternalError : public BaseError{
public:
    InternalError(const char code, const char *msg) : BaseError(code, msg){
        exit(code);
    }
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
    ifstream wavIn;
    ofstream wavOut;
    char *path;
public:
    WavFile(char *path) : path{path}{
        wavIn.open(path, std::ios::binary);
        if (not wavIn) {
            IOFileError err(1, ("Unable to open WAV file: " + string(path)).c_str());
        }

        header = (wav_hdr*) calloc(1, sizeof(wav_hdr));

        wavIn.read(reinterpret_cast<char*>(header), sizeof(*header));
        std::cout << "Header Read " << wavIn.gcount() << " bytes from " << path << "\n";
//        printHeader();
//        cout << "Subchunk2Size is " << header->Subchunk2Size << endl;
//        if(header->Subchunk2Size <= header->ChunkSize / 2) header->Subchunk2Size = header->ChunkSize - 36;
//        cout << "Subchunk2Size is " << header->Subchunk2Size << endl;
        if (wavIn.gcount() > 0){
            if(header->AudioFormat != 1) WAVFormatError err(3, "Audio format must be PMC (without encoding) (field must have val 1)");
            if(header->NumOfChan != 1) WAVFormatError err(4, "WAV for this program must have one channel (Mono) (field must have val 1)");
            if(header->bitsPerSample != 16) WAVFormatError err(5, "WAV for this program must have 16 bitness (field must have val 16)");
            if(header->SamplesPerSec != 44100) WAVFormatError err(6, "WAV for this program can only have 44100 sampling rate (field must have val 44100)");
        }else{
            WAVFormatError err(2, "Empty WAV header, try again with another file or correct this one.");
        }
//        cout << "BBB" << "\n";
    }
    WavFile(char *path, WAV_HEADER *newHeader) : path{path}, header{newHeader} {
        wavOut.open(path, std::ios::binary);
        if (not wavOut) {
            IOFileError err(1, ("Unable to open WAV file: " + string(path)).c_str());
        }
        writeHeader();
    }
    ~WavFile(){
        if(wavIn)wavIn.close();
        if(wavOut) wavIn.close();
        cout << "WAV " << path << " have deleted" << "\n";
    }
    void closeWAV(){
        if(wavIn) wavIn.close();
        if(wavOut) wavIn.close();
        printHeader();
        cout << "WAV " << path << " have been closed" << "\n";
    }
    void readHeader(){
        wavIn.read(reinterpret_cast<char *>(header), sizeof(*header));
    }
    WAV_HEADER *getHeaderCopy(){       //It's genius give a pointer to private field :)
//        WAV_HEADER *wav_header_copy = this->header;
        WAV_HEADER *wh = (WAV_HEADER*) calloc(1, sizeof(WAV_HEADER));
//        wh->ChunkSize = header->ChunkSize;
//        wh->Subchunk1Size = header->Subchunk1Size;
//        wh->AudioFormat = header->AudioFormat;
//        wh->NumOfChan = header->NumOfChan;
//        wh->SamplesPerSec = header->SamplesPerSec;
//        wh->bytesPerSec = header->bytesPerSec;
//        wh->blockAlign = header->blockAlign;
//        wh->bitsPerSample = header->bitsPerSample;
//        wh->Subchunk2Size = header->Subchunk2Size;
//
//        memcpy(wh->RIFF, header->RIFF);
//        memcpy(wh->WAVE, header->WAVE);
//        memcpy(wh->fmt, header->fmt);
//        memcpy(wh->Subchunk2ID, header->Subchunk2ID);
        memcpy(wh, this->header, sizeof(WAV_HEADER));
//        memcpy
        return wh;
    }
    void setHeader(WAV_HEADER *newHeader){
        if(this->header != nullptr) free(this->header);
        this->header = newHeader;
    }
    void writeHeader(){
        if(this->header == nullptr) InternalError(-1, "Internal error: can't delete object which doesn't exist.\n");
        wavOut.write(reinterpret_cast<char *>(header), sizeof(*header));
        return;
    }
    void writeSample(int16_t *sample){
        wavOut.write(reinterpret_cast<char *>(sample), sizeof(*sample));
    }
    int16_t *readSample(int16_t *sample){
        wavIn.read(reinterpret_cast<char *>(sample), sizeof(*sample));
        return sample;
    }
    char *getPath(){
        char *cpy = (char*) calloc(strlen(path) + 1, 1);
        strcpy(cpy, path);
        return cpy;
    }

    void printHeader(){
        cout << "RIFF header                :" << header->RIFF[0] << header->RIFF[1] << header->RIFF[2] << header->RIFF[3] << endl;
        cout << "WAVE header                :" << header->WAVE[0] << header->WAVE[1] << header->WAVE[2] << header->WAVE[3] << endl;
        cout << "FMT                        :" << header->fmt[0] << header->fmt[1] << header->fmt[2] << header->fmt[3] << endl;
        cout << "Data size                  :" << header->ChunkSize << endl;
        cout << "Sampling Rate              :" << header->SamplesPerSec << endl;
        cout << "Number of bits used        :" << header->bitsPerSample << endl;
        cout << "Number of channels         :" << header->NumOfChan << endl;
        cout << "Number of bytes per second :" << header->bytesPerSec << endl;
        cout << "Data length                :" << header->Subchunk2Size << endl;
        cout << "Audio Format               :" << header->AudioFormat << endl;
        cout << "ChunkSize1                 :" << header->Subchunk1Size << endl;
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
        if(start_time > end_time and end_time != -1) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        char *outPath = tmpFlag ? (char*) "cache.wav" : (char*) "tmp.wav";
        tmpFlag = not tmpFlag;
        WAV_HEADER *header = wav1->getHeaderCopy();
        WavFile out(outPath, wav1->getHeaderCopy());

        // Calculate the duration of the audio in seconds
        int duration = (int) header->Subchunk2Size / (header->bitsPerSample / 8) / header->NumOfChan / header->SamplesPerSec;
        if(end_time == -1) end_time = duration;
        cout << wav1->getPath() << " duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";
        cout << "Start muting " << wav1->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";

        out.writeHeader();

        uint32_t startSample = static_cast<uint32_t>(start_time * header->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * header->SamplesPerSec);
//        cout << "startSample " << startSample << "\n";
//        cout << "endSample " << endSample << "\n";

        // Mute the specified range
        int numOfSamples = header->Subchunk2Size / (header->bitsPerSample / 8) / header->NumOfChan;
//        int numOfSamples = (int) wav->getHeader()->Subchunk2Size / 2;
//        cout << "Number of samples is: " << numOfSamples << "\n";

        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample = 0;
            if (sampleIndex < startSample or sampleIndex >= endSample) wav1->readSample(&sample);  // Set the sample to zero to mute it
            out.writeSample(&sample);
//            cout << "Sample've read with idx: " << sampleIndex << " " << "sample is " << sample << "\n";
        }
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "Muted successfully.\n";
        out.closeWAV();
        wav1->closeWAV();
//        delete out;
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
        if(start_time > end_time and end_time != -1) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        char *outPath = tmpFlag ? (char*) "cache.wav" : (char*) "tmp.wav";
        tmpFlag = not tmpFlag;
        WAV_HEADER *header = wav1->getHeaderCopy();
        WAV_HEADER *header2 = wav2->getHeaderCopy();
        WavFile out(outPath, header);
        out.writeHeader();

        int dur_1 = (int) header->Subchunk2Size / (header->bitsPerSample / 8) / header->NumOfChan / header->SamplesPerSec;
        if(end_time == -1) end_time = dur_1;
        int dur_2 = (int) header2->Subchunk2Size / (header2->bitsPerSample / 8) / header2->NumOfChan / header2->SamplesPerSec;
        cout << wav1->getPath() << " duration is " << dur_1 << " seconds or " << (int) dur_1 / 60 << " m " << (int) dur_1 % 60 << " s.\n";
        cout << wav2->getPath() << " duration is " << dur_2 << " seconds or " << (int) dur_2 / 60 << " m " << (int) dur_2 % 60 << " s.\n";
        cout << "Start mixing " << wav1->getPath() << " and " << wav2->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";


        uint32_t startSample, endSample, lastSample;
        int numOfSamples = static_cast<uint32_t>(dur_1 * header->SamplesPerSec);
//        cout << "Number of samples is: " << numOfSamples << "\n";

        startSample = static_cast<uint32_t>(start_time * header->SamplesPerSec);
        endSample = static_cast<uint32_t>(end_time * header->SamplesPerSec);
        lastSample = static_cast<uint32_t>(dur_2 * header2->SamplesPerSec);

//        cout << "startSample " << startSample << "\n";
//        cout << "endSample " << endSample << "\n";

        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample1, sample2;
            wav1->readSample(&sample1);
            wav2->readSample(&sample2);
            if (sampleIndex >= startSample and sampleIndex < endSample and sampleIndex < lastSample) {
                unsigned long long sum = sample1 + sample2;
//                sample1 = (int16_t) sum / 2;
                sample1 += sample2;
            }
            out.writeSample(&sample1);
        }
        cout << "Mixed successfully.\n";
        out.closeWAV();
        wav1->closeWAV();
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
        if(start_time > end_time and end_time != -1) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        char *outPath = tmpFlag ? (char*) "cache.wav" : (char*) "tmp.wav";
        tmpFlag = not tmpFlag;
        WAV_HEADER *header = wav1->getHeaderCopy();

        int tot_duration = (int) header->Subchunk2Size / (header->bitsPerSample / 8) / header->NumOfChan / header->SamplesPerSec;
        if(end_time == -1) end_time = tot_duration;
        cout << wav1->getPath() << " old duration is " << tot_duration << " seconds or " << (int) tot_duration / 60 << " m " << (int) tot_duration % 60 << " s.\n";

        unsigned long long skipSample = -1;
        if(acceleration >= 1){
            int duration = start_time
                           + tot_duration - end_time
                           + (end_time - start_time) / acceleration;
            uint32_t startSample = static_cast<uint32_t>(start_time * header->SamplesPerSec);
            uint32_t endSample = static_cast<uint32_t>(end_time * header->SamplesPerSec);
            int numOfSamples = static_cast<uint32_t>(duration * header->SamplesPerSec);
//            cout << "sssssssssssssssssssssssssamples " << numOfSamples << wav1->getPath() << " new duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";

            header->Subchunk2Size = numOfSamples * (header->bitsPerSample / 8) / header->NumOfChan;
            header->ChunkSize = header->Subchunk2Size + 396;
//            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << header->Subchunk2Size << "\n";
            WavFile out(outPath, header);
            out.writeHeader();
            for (uint32_t sampleIndex = 0; sampleIndex < tot_duration * header->SamplesPerSec; sampleIndex++) {
                int16_t sample;
                wav1->readSample(&sample);
                if (sampleIndex >= startSample && sampleIndex < endSample){
                    skipSample += 1;
                    if(skipSample % (int) acceleration == 0)
                        out.writeSample(&sample);
                } else out.writeSample(&sample);
            }
            cout << "Accelerated successfully.\n";
            out.closeWAV();
            wav1->closeWAV();
        } else {
            uint32_t startSample = static_cast<uint32_t>(start_time * header->SamplesPerSec);
            uint32_t endSample = static_cast<uint32_t>(end_time * header->SamplesPerSec);
            int numOfSamples = tot_duration * header->SamplesPerSec;

            uint32_t slowed_endSample = static_cast<uint32_t>(((end_time - start_time) / acceleration) + start_time) * header->SamplesPerSec;
            int slowed_numOfSamples = start_time + slowed_endSample + (tot_duration - end_time) * header->SamplesPerSec;

//            header->Subchunk2Size = numOfSamples * (header->bitsPerSample / 8) / header->NumOfChan;
            int duration = slowed_numOfSamples / header->SamplesPerSec;
//            cout << wav1->getPath() << " new duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";
//            cout << "-----------------------------num of samples " << numOfSamples << " " << slowed_numOfSamples << " " << endSample << "\n";
            header->Subchunk2Size = slowed_numOfSamples * (header->bitsPerSample / 8) / header->NumOfChan;
            header->ChunkSize = header->Subchunk2Size + 396;
            WavFile out(outPath, header);
            out.writeHeader();
            long long written = 0;

            for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
                int16_t sample;
                wav1->readSample(&sample);
                if (sampleIndex >= startSample && sampleIndex < endSample){
                    for(int j = 0; j < ceil(1 / acceleration); j++){
                        out.writeSample(&sample);
                        written += 1;
                    }
                } else{
                    written += 1;
                    out.writeSample(&sample);
                }
            }
            cout << "////////// written " << written << "\n";
            cout << "Accelerated successfully.\n";
//            out.printHeader();
            out.closeWAV();
            wav1->closeWAV();
        }
//        exit(0);
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
    }
};

class WAVCutter : public WAVConverter {
public:
    WAVCutter(unsigned int start, unsigned int end, WavFile *wav) : WAVConverter(start, end, wav){
        convert();
    };
    void convert() override{
        if(start_time > end_time and end_time != -1) MyWarning a(127, "Start time of mute can't be bigger then end time.");
        char *outPath = tmpFlag ? (char*) "cache.wav" : (char*) "tmp.wav";
        tmpFlag = not tmpFlag;
        WAV_HEADER *header = wav1->getHeaderCopy();
        WavFile out(outPath, header);
        out.writeHeader();

        //127
        int duration = end_time - start_time;
        if(end_time == -1) end_time = duration;
        cout << "dur is " << duration << "\n";
        cout << wav1->getPath() << " duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";

        cout << "Start cutting " << wav1->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";

        // Write the WAV header to the output file

        uint32_t startSample = static_cast<uint32_t>(start_time * (header->bitsPerSample / 8) * header->NumOfChan * header->SamplesPerSec);
        uint32_t endSample = static_cast<uint32_t>(end_time * (header->bitsPerSample / 8) * header->NumOfChan * header->SamplesPerSec);
//        cout << "start samples is " << startSample << "end samples is " << endSample << "\n";

        for (uint32_t sampleIndex = 0; true; sampleIndex++) {
            if(sampleIndex >= endSample) break;
            int16_t sample;
            wav1->readSample(&sample);
            if (sampleIndex >= startSample && sampleIndex < endSample) out.writeSample(&sample);
        }
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "Cutted successfully.\n";
        out.closeWAV();
        wav1->closeWAV();
    }
};

class WAVCopy : public WAVConverter {
private:
    char *outPath;
public:
    WAVCopy(WavFile *wav1, char *outPath) : WAVConverter(0, 0, wav1){
        this->outPath = outPath;
        convert();
    };
    void convert() override{
        WAV_HEADER *header = wav1->getHeaderCopy();
        WavFile out(outPath, header);

        int duration = header->Subchunk2Size / (header->bitsPerSample / 8) / header->NumOfChan / header->SamplesPerSec;
        cout << "dur is " << duration << "\n";
        cout << wav1->getPath() << " duration is " << duration << " seconds or " << (int) duration / 60 << " m " << (int) duration % 60 << " s.\n";

        int numOfSamples = duration * header->SamplesPerSec;
        header->Subchunk2Size = numOfSamples * (header->bitsPerSample / 8);
        out.writeHeader();
        cout << "Start copying " << wav1->getPath() << " from " << start_time << " seconds to " << end_time << " seconds.\n";


        for (uint32_t sampleIndex = 0; sampleIndex < numOfSamples; sampleIndex++) {
            int16_t sample;
            wav1->readSample(&sample);
            out.writeSample(&sample);
        }
//        cout << wav->getHeader()->NumOfChan << " " << wav->getHeader()->Subchunk2Size << " " << wav->getHeader()->ChunkSize << " " << wav->getHeader()->Subchunk1Size << "\n";// << "" << wav->getHeader()->NumOfChan << "" << wav->getHeader()->NumOfChan << ""
        cout << "Copied successfully.\n";
        out.closeWAV();
        wav1->closeWAV();
    }
};

class WAVConverterFactory {
public:
    static WAVConverter* createConverter(const std::string& type, unsigned int start, unsigned int end, WavFile *wav1, WavFile *wav2=nullptr, double boost=0, char *out="./out.wav") {
        if (type == "Mute") {
            return new WAVMuter(start, end, wav1);
        } else if (type == "Mix" and wav2 != nullptr) {
            return new WAVMixer(start, end, wav1, wav2);
        } else if (type == "Accelerate" and boost != 0) {
            return new WAVAccelerator(start, end, wav1, boost);
        } else if (type == "Cut") {
            return new WAVCutter(start, end, wav1);
        } else if (type == "Copy") {
            return new WAVCopy(wav1, out);
        } else {
            return nullptr; // Invalid type
        }
    }
};

WavFile *getTempFile(){
    char *tmpPath = tmpFlag ? (char*) "tmp.wav" : (char*) "cache.wav";
    WavFile *tmp = new WavFile (tmpPath);
    return tmp;
}

int main(int argc, char **argv) {
//    cout << argc << " " << argv[1] << "\n";
    if (argc == 2 and (argv[1] == (char *) "-h" or argv[1] == (char *) "-help")) {
        cout << "You can write as: sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav …]]\n";
    } else if (argc > 2 and string(argv[1]).find("-c") != -1) {
        cout << argc;
        ifstream in(argv[2]);
        if (not in) {
            127;
        }
        char *outPath = argv[3];

        vector < WavFile * > files;
        cout << "\nargv is " << *argv << "\n";
        for(int i = 4; i < argc; i++){
            cout << "*argv is " << *argv << " argv is " << argv << "\n";
            try {
                WavFile *p = new WavFile((char*) argv[i]);
//                cout << "p is " << p << "\n";
                files.push_back(p);
            } catch (const char *err) {
                cout << "\n\n\n\n\n" << err << "\n";
                IOFileError e(127, (const char *) (string("There is problem with file") + string((char*) *argv)).c_str());
            }
//            cout << "p is " << "\n";
//            argv++;
//            cout << "p is " << "\n";
//            cout << "*argv is " << *argv << " argv is " << argv << "\n";
        }
        string line;
        bool firstTime = true;
        while (getline(in, line)) {
//            cout << "line is " << line << "\n";
            if (line.length() < 1) {
                continue;
//                IOFileError e(127, "Smth went wrong.\n");
            }
            if (line[0] == '#' or line[0] == '\n'){
//                cout << "line " << line << " was skipped\n";
                continue;
            }
//            cout << "line " << line << " was not skipped\n";
            WavFile *a;
            if (firstTime) {
                a = files[0];
                firstTime = false;
            } else a = getTempFile();
            stringstream ss(line);
            string conv;
            ss >> conv;
            if (conv == "mute") {   //mute 5 25
                int start, end;
                try {
                    ss >> start >> end;
                } catch (const char *err) {
                    cout << err << "\n";
                    IOFileError e(127, (const char *) (string("Cant read start/end from  ") + line).c_str());
                }
                WAVConverter *c = WAVConverterFactory::createConverter("Mute", start, end, a);
            } else if (conv == "mix") { //mix $2 10 20
//                string indexArg;
                int start, end, wav2Index;
                try {
//                    ss >> indexArg >> start >> end;
                    ss >> wav2Index >> start >> end;
                    cout << wav2Index << " " << start << " " << end << "\n";
//                    wav2Index = stoi(indexArg.substr(1, indexArg.length() - 1));
                } catch (const char *err) {
                    cout << err << "\n";
                    IOFileError e(127, (const char *) (string("Cant read start/end from  ") + line).c_str());
                }
//                cout << "path is " << files[wav2Index - 2]-> << "\n";
//                WavFile b((char *) files[wav2Index - 1]);
                WAVConverter *c = WAVConverterFactory::createConverter("Mix", start, end, a, files[wav2Index - 2]);
            } else if (conv == "accelerate") {  //accelerate 5 10 1.5
                int start, end;
                double boost;
                try {
                    ss >> start >> end >> boost;
                } catch (const char *err) {
                    cout << err << "\n";
                    IOFileError e(127, (const char *) (string("Cant read start/end from  ") + line).c_str());
                }
                cout << "________________________________________" << boost << "\n";
                WAVConverter *c = WAVConverterFactory::createConverter("Accelerate", start, end, a, nullptr, boost);
            } else if (conv == "cut") {
                int start, end;
                try {
                    ss >> start >> end;
                } catch (const char *err) {
                    cout << err << "\n";
                    IOFileError e(127, (const char *) (string("Cant read start/end from  ") + line).c_str());
                }
                WAVConverter *c = WAVConverterFactory::createConverter("Cut", start, end, a);
            } else {
                IOFileError e(127, (const char *) (string("Parameter haven't recognized  ") + line).c_str());
            }
        }
        if (not firstTime) {
            WAVConverter *c = WAVConverterFactory::createConverter("Copy", 0, 0, getTempFile(), nullptr, NULL, outPath);
        }
    }
//    WavFile a("./ex1.wav");
//    WavFile b("./ex2.wav");
//
//    WAVConverter* c_1 = WAVConverterFactory::createConverter("Mute", 0, 15, &a);
//    WAVConverter* c_2 = WAVConverterFactory::createConverter("Mute", 30, 45, getTempFile());
//    WAVConverter* c_3 = WAVConverterFactory::createConverter("Mute", 60, 75, getTempFile());
//    WAVConverter* c_4 = WAVConverterFactory::createConverter("Mute", 90, 115, getTempFile());
//
//    WAVConverter* c_11 = WAVConverterFactory::createConverter("Mix", 0, 15, &b, &a);
//    WAVConverter* c_22 = WAVConverterFactory::createConverter("Mix", 30, 45, getTempFile(), &a);
//    WAVConverter* c_33 = WAVConverterFactory::createConverter("Mix", 60, 75, getTempFile(), &a);
//    WAVConverter* c_44 = WAVConverterFactory::createConverter("Mix", 90, 115, getTempFile(), &a);

//    WAVConverter* c_11 = WAVConverterFactory::createConverter("Mix", 0, 15, &b, getTempFile());
//    WAVConverter* c_12 = WAVConverterFactory::createConverter("Mix", 30, 45, &b, getTempFile());
//    WAVConverter* c_13 = WAVConverterFactory::createConverter("Mix", 60, 75, &b, getTempFile());
//    WAVConverter* c_14 = WAVConverterFactory::createConverter("Mix", 90, 105, &b, getTempFile());

//    WAVConverter* c_5 = WAVConverterFactory::createConverter("Accelerate", 0, 410, &a, nullptr, 0.25);
//    WAVConverter* c_6 = WAVConverterFactory::createConverter("Accelerate", 0, 1640, getTempFile(), nullptr, 4);
//    WAVConverter* c_55 = WAVConverterFactory::createConverter("Accelerate", 0, 410, getTempFile(), nullptr, 0.25);
//    WAVConverter* c_66 = WAVConverterFactory::createConverter("Accelerate", 0, 1640, getTempFile(), nullptr, 4);
//    WAVConverter* c_67 = WAVConverterFactory::createConverter("Accelerate", 0, 410, getTempFile(), nullptr, 0.05);
//    WAVConverter* c_57 = WAVConverterFactory::createConverter("Accelerate", 0, 8200, getTempFile(), nullptr, 20);


//    WAVConverter* c_1000 = WAVConverterFactory::createConverter("Copy", 0, 0, getTempFile(), 0, 0, "snd.wav");

//    WAVConverter* c1 = WAVConverterFactory::createConverter("Mute", 10, 30, &a);
//    WAVConverter* c2 = WAVConverterFactory::createConverter("Mix", 15, 25, &a, getTempFile());
//    WAVConverter* converter = WAVConverterFactory::createConverter("Cut", 0, 10, &a, &b);
//    WAVConverter* converter = WAVConverterFactory::createConverter("Accelerate", 0, 411, &a, nullptr, 4);
    return 0;
}

//~ Добавить грамотную обработку ошибок
//V Перенсти всю работу с файлами в WAV (открытие, сохранение) редактирование - создание нового WAV в конвертере и запись по нему
//- Добавить тона
//- Добавить склейку
//V Добавить ввод с консоли