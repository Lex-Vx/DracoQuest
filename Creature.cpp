#include "Creature.h"

void Creature::TakeDamage(int dmg) 
{
  hp -= dmg;
  if (hp < 0) hp = 0;
}

int Creature::CalculateAttack() const 
{
  return atk + (lvl / 2);
}
