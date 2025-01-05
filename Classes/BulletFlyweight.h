#ifndef __BULLET_FLYWEIGHT_H__
#define __BULLET_FLYWEIGHT_H__

#include <string>
#include <unordered_map>

// Flyweight class to share common bullet properties
class BulletFlyweight {
public:
    BulletFlyweight() = default;
    BulletFlyweight(const std::string& spriteFrameName, float speed, int buffID, int ack, int dieID);

    // Getters for intrinsic state
    const std::string& getSpriteFrameName() const { return spriteFrameName; }
    float getSpeed() const { return speed; }
    int getBuffID() const { return buffID; }
    int getAck() const { return ack; }
    int getDieID() const { return dieID; }

    // 建议添加以下方法来更清晰地表示内部状态的操作
    void applyTo(CBulletBase* bullet) const {
        bullet->setSpeed(speed);
        bullet->setBuffID(buffID);
        bullet->setAck(ack);
        bullet->setDieID(dieID);
    }

private:
    // Intrinsic (shared) state
    std::string spriteFrameName;  // 共享的精灵帧名称
    float speed;                  // 共享的速度属性
    int buffID;                   // 共享的buff ID
    int ack;                      // 共享的攻击力
    int dieID;                    // 共享的死亡效果ID
};

// Flyweight Factory to manage shared bullet states
class BulletFlyweightFactory {
public:
    static BulletFlyweight* getFlyweight(int bulletID);
    static void cleanup();

private:
    static std::unordered_map<int, BulletFlyweight> flyweights;
};

// 添加一个Context类来更好地管理外部状态
class BulletContext {
public:
    BulletContext(BulletFlyweight* flyweight) : flyweight(flyweight) {}
    
    // Extrinsic (unique) state
    Vec2 position;
    Vec2 direction;
    float attackRange;
    int grade;
    
    // Reference to the flyweight
    BulletFlyweight* flyweight;
};

#endif 