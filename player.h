#ifndef PLAYER_H
#define PLAYER_H

#include "dynamicobject.h"
#include <QPixmap>
#include <QVector>
#include <QMap>

enum class Character { Mario, Luigi };
enum class PlayerForm { Small, Big, Fire };

struct PhysicsProfile {
    qreal maxSpeed;
    qreal acceleration;
    qreal friction;
    qreal jumpForce;
    qreal gravity;
};

// 动作序列包
struct SpriteSet {
    QPixmap idle;
    QPixmap jump;
    QPixmap skid;
    QPixmap squat;
    QVector<QPixmap> walkFrames;
    QVector<QPixmap> slideFrames;
};

class Player : public QObject, public DynamicObject
{
    Q_OBJECT
public:
    explicit Player(QGraphicsItem *parent = nullptr);
    void updateLogic() override;
    void onCollision(GameObject* other) override;
    void changeForm(PlayerForm newForm);
    
    Character getCharacter() const { return m_currentCharacter; }
    PlayerForm getPlayerForm() const { return m_currentForm; }
    int getLives() const { return m_lives; }
    static void addScore(int s) { m_score += s; }
    static int getScore() { return m_score; }
    
    // ★ 加上这两行，把金币的功能补回来！
    static void addCoin(int c = 1) { m_coin += c; }
    static int getCoin() { return m_coin; }

    bool m_isOnGround = false;
    bool m_isUnder = false;
    bool m_isWon = false;
    bool m_isFailed = false;

signals:
    void scoreChanged(int newScore);
    void coinChanged(int newCoin);
    void lifeChanged(int newLife);
    void gameWon();
    void gameFailed(int failKind);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initPlayer();
    // ★ 核心切图工具
    SpriteSet cutSpriteSheet(const QPixmap& sheet, int row, int frameWidth, int frameHeight, bool isSmall);
    
    void resolveTerrainCollisions(Qt::Orientation orientation);
    void updateAnimation();

    bool m_isLeftPressed = false, m_isRightPressed = false, m_isJumpPressed = false, m_isSquatPressed = false;
    bool m_facingRight = true, m_isDead = false, m_isInvincible = false;
    bool m_hasPlayedWinSound = false, m_hasPlayedDeathSound = false;

    int m_lives;
    static long m_score;
    static int m_coin;

    Character m_currentCharacter;
    PlayerForm m_currentForm;

    QMap<PlayerForm, SpriteSet> m_marioAnimations;
    QMap<PlayerForm, SpriteSet> m_luigiAnimations;
    QMap<PlayerForm, SpriteSet>* m_currentAnimations;
    QMap<Character, PhysicsProfile> m_profiles;

    int m_animCounter = 0;
    int m_currentFrameIndex = 0;
    const qreal max_vy = 8.0;
};

#endif