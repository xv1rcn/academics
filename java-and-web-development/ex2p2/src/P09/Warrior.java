public class Warrior extends Character {
    private boolean enraged;

    public Warrior(String name) {
        super(name, 120, 30, 20);
    }

    @Override
    public void specialSkill() {
        if (mana < 10) {
            System.out.println(name + " 法力不足，无法释放技能！");
            return;
        }
        mana -= 10;
        enraged = true;
        System.out.println(name + " 释放技能：狂暴（下次攻击 +20% 伤害）");
    }

    @Override
    protected int applySkillBonus(int rawDamage) {
        if (enraged) {
            enraged = false;
            return (int) (rawDamage * 1.2);
        }
        return rawDamage;
    }
}
