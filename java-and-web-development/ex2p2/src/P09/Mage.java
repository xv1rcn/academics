public class Mage extends Character {
    private boolean magicBoosted;
    private static final int MAGIC_BONUS = 12;

    public Mage(String name) {
        super(name, 80, 50, 10);
    }

    @Override
    public void specialSkill() {
        if (mana < 15) {
            System.out.println(name + " 法力不足，无法释放技能！");
            return;
        }
        mana -= 15;
        magicBoosted = true;
        System.out.println(name + " 释放技能：魔力增幅（下次攻击 +12 魔法伤害）");
    }

    @Override
    protected int applySkillBonus(int rawDamage) {
        if (magicBoosted) {
            magicBoosted = false;
            return rawDamage + MAGIC_BONUS;
        }
        return rawDamage;
    }
}
