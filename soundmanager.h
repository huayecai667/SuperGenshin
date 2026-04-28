#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <QObject>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QMap>
#include <QString>
class SoundManager: public QObject
{
    Q_OBJECT
public:
    static SoundManager& instance() {
        static SoundManager manager;
        return manager;
    }

    //初始化所有音效
    void init();
    void playBGM();
    void stopBGM();

    //播放短促音效
    void playSound(const QString& soundName);

private:
    explicit SoundManager(QObject *parent = nullptr);
    ~SoundManager();
    QMediaPlayer* m_bgmPlayer;
    //用map把所有音效存起来
    QMap<QString, QSoundEffect*> m_soundEffects;
};

#endif // SOUNDMANAGER_H
