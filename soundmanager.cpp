#include "soundmanager.h"
#include <QUrl>
//#include <QMediaPlaylist>
SoundManager::SoundManager(QObject *parent)
    : QObject(parent)
{
    m_bgmPlayer = new QMediaPlayer(this);

}

SoundManager::~SoundManager() {

}

void SoundManager::init() {

    // 吃金币音效
    QSoundEffect* coinSound = new QSoundEffect(this);
    coinSound->setSource(QUrl("qrc:/sound/coin.wav"));
    coinSound->setVolume(0.8f);
    m_soundEffects.insert("coin", coinSound);

    // 跳跃音效
    QSoundEffect* jumpSound = new QSoundEffect(this);
    jumpSound->setSource(QUrl("qrc:/sound/jump.wav"));
    jumpSound->setVolume(0.5f);
    m_soundEffects.insert("jump", jumpSound);

    // 踩扁怪物音效
    QSoundEffect* stompSound = new QSoundEffect(this);
    stompSound->setSource(QUrl("qrc:/sound/bump.wav"));
    m_soundEffects.insert("stomp", stompSound);

    // 砖块碎裂音效
    QSoundEffect* smashSound = new QSoundEffect(this);
    smashSound->setSource(QUrl("qrc:/sound/brick_smash.wav"));
    m_soundEffects.insert("smash", smashSound);

    // 死亡音效
    QSoundEffect* deathSound = new QSoundEffect(this);
    deathSound->setSource(QUrl("qrc:/sound/death.wav"));
    m_soundEffects.insert("death", deathSound);

    //旗杆音效
    QSoundEffect* flagSound = new QSoundEffect(this);
    flagSound->setSource(QUrl("qrc:/sound/flagpole.wav"));
    m_soundEffects.insert("flagpole", flagSound);

    //超时音效
    QSoundEffect* timeSound = new QSoundEffect(this);
    timeSound->setSource(QUrl("qrc:/sound/out_of_time.wav"));
    m_soundEffects.insert("time", timeSound);

    //道具出现音效
    QSoundEffect* powerAppearSound = new QSoundEffect(this);
    powerAppearSound->setSource(QUrl("qrc:/sound/powerup_appears.wav"));
    m_soundEffects.insert("powerAppear", powerAppearSound);

    //得到道具音效
    QSoundEffect* powerUpSound = new QSoundEffect(this);
    powerUpSound->setSource(QUrl("qrc:/sound/powerup.wav"));
    m_soundEffects.insert("powerUp", powerUpSound);

    //水管音效
    QSoundEffect* pipeSound = new QSoundEffect(this);
    pipeSound->setSource(QUrl("qrc:/sound/pipe.wav"));
    m_soundEffects.insert("pipe", pipeSound);

    //胜利音效
    QSoundEffect* winSound = new QSoundEffect(this);
    winSound->setSource(QUrl("qrc:/sound/stage_clear.wav"));
    m_soundEffects.insert("win", winSound);

    //失败音效
    QSoundEffect* failSound = new QSoundEffect(this);
    failSound->setSource(QUrl("qrc:/sound/game_over.wav"));
    m_soundEffects.insert("fail", failSound);

}

void SoundManager::playBGM() {
    //m_bgmPlayer->setMedia(QUrl("qrc:/sound/main_theme.wav"));
    m_bgmPlayer->play();
}

void SoundManager::stopBGM() {
    m_bgmPlayer->stop();
}

void SoundManager::playSound(const QString& soundName) {
    if (m_soundEffects.contains(soundName)) {
        m_soundEffects[soundName]->play();
    }
}
