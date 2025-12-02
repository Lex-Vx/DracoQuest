#pragma once
#include <Arduino.h>
#include <LovyanGFX.hpp>

// An incredibly blank template for the dynamic entities in the game. I may remove this and refactor Enemy/Player for simplification|

class Creature 
{
  protected:
    String name;
    int baseHp, baseMp, baseAtk, baseDef, baseAgl;
    int lvl, hp, mp, atk, def, agl;
    int maxHp, maxMp;

  public:
  Creature(const String& n, int l, int h, int m, int a, int d, int s) : name(n), lvl(l), baseHp(h), hp(h), maxHp(h),
      baseMp(m), mp(m), maxMp(m), baseAtk(a), atk(a), baseDef(d), def(d), baseAgl(s), agl(s) {}

  virtual ~Creature() = default;
  virtual void TakeDamage(int dmg);
  virtual int  CalculateAttack() const;
  virtual bool isAlive() const { return hp > 0; }
  virtual void PerformCombatAction(Creature& target) = 0;
  virtual void Draw(LGFX_Sprite* target, int x, int y) const = 0;
};
