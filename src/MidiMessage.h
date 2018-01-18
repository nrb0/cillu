#pragma once

#include <vector>
#include <string>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

class MidiMessage : public std::vector<uchar> {
public:
    MidiMessage();
    MidiMessage(int command);
    MidiMessage(int command, int p1);
    MidiMessage(int command, int p1, int p2);
    MidiMessage(const MidiMessage& message);
    MidiMessage(const std::vector<uchar>& message);
    MidiMessage(const std::vector<char>& message);
    MidiMessage(const std::vector<int>& message);

    ~MidiMessage();

    MidiMessage& operator=(const MidiMessage& message);
    MidiMessage& operator=(const std::vector<uchar>& bytes);
    MidiMessage& operator=(const std::vector<char>& bytes);
    MidiMessage& operator=(const std::vector<int>& bytes);
    void setSize(int asize);
    int getSize() const;
    int setSizeToCommand();
    int resizeToCommand();

    int getTempoMicro() const;
    int getTempoMicroseconds() const;
    double getTempoSeconds() const;
    double getTempoBPM() const;
    double getTempoTPS(int tpq) const;
    double getTempoSPT(int tpq) const;

    bool isMetaMessage() const;
    bool isMeta() const;
    bool isNoteOff() const;
    bool isNoteOn() const;
    bool isNote() const;
    bool isAftertouch() const;
    bool isController() const;
    bool isTimbre() const;
    bool isPatchChange() const;
    bool isPressure() const;
    bool isPitchbend() const;

    int getP0() const;
    int getP1() const;
    int getP2() const;
    int getP3() const;
    int getKeyNumber() const;
    int getVelocity() const;

    void setP0(int value);
    void setP1(int value);
    void setP2(int value);
    void setP3(int value);
    void setKeyNumber(int value);
    void setVelocity(int value);

    int getCommandNibble() const;
    int getCommandByte() const;
    int getChannelNibble() const;
    int getChannel() const;

    void setCommandByte(int value);
    void setCommand(int value);
    void setCommand(int value, int p1);
    void setCommand(int value, int p1, int p2);
    void setCommandNibble(int value);
    void setChannelNibble(int value);
    void setChannel(int value);
    void setParameters(int p1, int p2);
    void setParameters(int p1);
    void setMessage(const std::vector<uchar>& message);
    void setMessage(const std::vector<char>& message);
    void setMessage(const std::vector<int>& message);

    void setSpelling(int base7, int accidental);
    void getSpelling(int& base7, int& accidental);

    // helper functions to create various MidiMessages
    void makeNoteOn(int channel, int key, int velocity);
    void makeNoteOff(int channel, int key, int velocity);
    void makeNoteOff(int channel, int key);
    void makeNoteOff();
    void makeController(int channel, int num, int value);
    void makePatchChange(int channel, int patchnum);
    void makeTimbre(int channel, int patchnum);

    // meta-message creation helper functions:
    void makeMetaMessage(int mnum, const std::string& data);
    void makeTrackName(const std::string& name);
    void makeInstrumentName(const std::string& name);
    void makeLyric(const std::string& text);
    void makeMarker(const std::string& text);
    void makeCue(const std::string& text);
    void makeCopyright(const std::string& text);
    void makeTempo(double tempo) { setTempo(tempo); }
    void makeTimeSignature(int top, int bottom, int clocksPerClick = 24, int num32dsPerQuarter = 8);

    // meta-message related functions:
    int getMetaType() const;
    bool isTempo() const;
    void setTempo(double tempo);
    void setTempoMicroseconds(int microseconds);
    void setMetaTempo(double tempo);
    bool isEndOfTrack() const;

};
