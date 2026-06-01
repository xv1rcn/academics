public class Archer extends Character {
    private boolean focused;

    public Archer(String name) {
        super(name, 90, 25, 12);
    }

    @Override
    public void specialSkill() {
        if (mana < 10) {
            System.out.println(name + " 法力不足，无法释放技能！");
            return;
        }
        mana -= 10;
        focused = true;
        System.out.println(name + " 释放技能：精准射击（下次攻击双倍伤害）");
    }

    @Override
    protected int applySkillBonus(int rawDamage) {
        if (focused) {
            focused = false;
            return rawDamage * 2;
        }
        return rawDamage;
    }
}
