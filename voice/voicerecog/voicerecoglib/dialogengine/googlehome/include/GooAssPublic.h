#ifndef CXX_GOOASSPUBLIC_H
#define CXX_GOOASSPUBLIC_H

// DialogState
typedef enum {
	IDLE = 0,
	LISTENING,
	THINKING,
	SPEAKING,
	ERROR
}DialogState;

// DialogStateObserver
class DialogStateObserver
{
public:
    virtual ~DialogStateObserver() {}
    virtual void onDialogStateChanged(DialogState state) = 0;
};

class DialogStateObserverTest : public DialogStateObserver
{
public:
    virtual ~DialogStateObserverTest() {}
    virtual void onDialogStateChanged(DialogState state) {};
};

// AudioInstreamer
class AudioInStreamer
{
public:
    virtual ~AudioInStreamer() {}
    virtual int onAudioInData(short* buf, size_t len) = 0;
};

class AbstractSDS
{
public:
    virtual ~AbstractSDS() {}
    static AbstractSDS* createGoogleSDS(DialogStateObserver* observer);
    static AbstractSDS* createAlexaSDS(DialogStateObserver* observer);
    static void Destroy();

    // Session begin
    virtual AudioInStreamer* startDialog() = 0;

    // Session stop
    virtual int stopDialog() = 0;

private:
    static AbstractSDS* s_pcGoogleAssInstance;

    virtual void SetDialogStateObserver(DialogStateObserver* observer){};
};

#endif // CXX_GOOASSPUBLIC_H